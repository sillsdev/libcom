//
//	$Id$
//
//	OutputDebugString test
//
//	Neil Mayhew - 2007-01-11
//
// Win32 Compatibility Library
// Copyright (C) 2007 SIL International
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// http://www.gnu.org/licenses/lgpl.html
//

#include "Hacks.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

bool equal(std::string name, FILE* f, std::string expected);

int main(int argc, char** argv)
{
	int failures = 0;

	// Capture stdout/stderr

	FILE* outfile = tmpfile();
	FILE* errfile = tmpfile();
	int saved_stdout = dup(STDOUT_FILENO);
	int saved_stderr = dup(STDERR_FILENO);

	std::cout.flush();
	std::cerr.flush();
	fflush(stdout);
	fflush(stderr);

	dup2(fileno(outfile), STDOUT_FILENO); // Redirect stdout
	dup2(fileno(errfile), STDERR_FILENO); // Redirect stderr

	// Run tests

	// \u2022 == \xE2\x80\xA2 == bullet, \u00E9 == e acute
	OutputDebugString(L"Wide\u2022Wide\u2022Wide\n");
	OutputDebugString("Narrow\xE2\x80\xA2Narrow\xe2\x80\xa2Narrow\n");
	OLECHAR buf[] = {'O', 'l', 0xE9, 0x2022, 'O', 'l', 0xE9, '\n', 0};
	OutputDebugString(buf);

	// Restore stdout/stderr

	std::cout.flush();
	std::cerr.flush();
	fflush(stdout);
	fflush(stderr);

	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);

	// Check results

	std::string expected_out;
	std::string expected_err(
		"Wide\xe2\x80\xa2Wide\xe2\x80\xa2Wide\n"
		"Narrow\xe2\x80\xa2Narrow\xe2\x80\xa2Narrow\n"
		"Ol\xc3\xa9\xe2\x80\xa2Ol\xc3\xa9\n");

	try
	{
		equal("stdout", outfile, expected_out) || ++failures;
		equal("stderr", errfile, expected_err) || ++failures;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		++failures;
	}

	return failures;
}

inline void throw_syserr(std::string call)
{
	throw std::runtime_error(call + std::string(" failed: ") + std::to_string(errno));
}

std::string read_all(FILE* f)
{
	rewind(f);

	struct stat st;
	if (fstat((fileno(f)), &st) != 0)
		throw_syserr("fstat");

	std::vector<char> buf(st.st_size);
	if (fread(&buf[0], 1, buf.size(), f) != buf.size())
		throw_syserr("fread");

	return std::string(buf.begin(), buf.end());
}

std::string escape(std::string s)
{
	std::ostringstream result;
	result << std::setbase(16) << std::setfill('0');
	for (std::string::iterator p = s.begin(); p != s.end(); ++p)
		if (*p == '\\')
			result << '\\' << *p;
		else if (std::isprint(*p) || *p == '\n')
			result << *p;
		else
			result << "\\x" << std::setw(2) << int(static_cast<unsigned char>(*p));
	return result.str();
}

bool equal(std::string name, FILE* f, std::string expected)
{
	std::string actual = read_all(f);

	if (actual != expected)
		std::cerr << name << " differs from expected result:\n\n"
			<< escape(actual) << "\nshould be\n\n" << escape(expected) << "\n";

	return actual == expected;
}
