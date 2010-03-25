//
//	MessageBox implementation based on xmessage
//
//	Neil Mayhew - 2010-03-19
//
// Win32 Compatibility Library
// Copyright (C) 2010 SIL International
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

#undef INCLUDE_MAIN
//#define INCLUDE_MAIN

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <iostream>

#include "Hacks.h"
#include "MessageBox.h"

// Temporary hacks
static std::string convert(const wchar_t* text)
{
	std::vector<char> chars(text ? wcslen(text) : 0);
	std::copy(text, text + chars.size(), &chars[0]);
	return std::string(&chars[0], &chars[chars.size()]);
}
//static std::wstring convert(const char* text)
//{
//	std::vector<wchar_t> chars(text ? strlen(text) : 0);
//	std::copy(text, text + chars.size(), &chars[0]);
//	return std::wstring(&chars[0], &chars[chars.size()]);
//}

// Return a single-quoted string
static std::string quote(std::string text)
{
	std::string result;

	result.reserve(1 + text.size() + std::count(text.begin(), text.end(), '\'') * 3 + 1);

	result += '\'';

	std::string::size_type pos = 0;
	std::string::size_type next;

	while ((next = text.find('\'', pos)) != std::string::npos)
	{
		result.append(text, pos, next - pos);
		result.append("'\\''");
		pos = next + 1;
	}

	result.append(text, pos, next);
	result += '\'';

	return result;
}

int MessageBox(HWND /*hWnd*/, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	const char* xmessage = getenv("XMESSAGE");

	if (!xmessage)
		xmessage = "xmessage";

	std::string caption = quote(convert(lpCaption));
	std::string text = quote(convert(lpText));

	std::ostringstream buttons;
	switch (uType & MB_BUTTONMASK)
	{
		case MB_OK:
			buttons << "-buttons _OK:0 -default _OK";
			break;
		case MB_OKCANCEL:
			buttons << "-buttons _OK:0,_Cancel:2 -default _OK";
			break;
		case MB_ABORTRETRYIGNORE:
			buttons << "-buttons _Abort:3,_Retry:4,_Ignore:5 -default _Ignore";
			break;
		case MB_YESNOCANCEL:
			buttons << "-buttons _Yes:6,_No:7,_Cancel:2 -default _Yes";
			break;
		case MB_YESNO:
			buttons << "-buttons _Yes:6,_No:7 -default _Yes";
			break;
		case MB_RETRYCANCEL:
			buttons << "-buttons _Retry:4,_Cancel:2 -default _Retry";
			break;
		case MB_CANCELTRYCONTINUE:
			buttons << "-buttons _Cancel:2,_Try:10,_Continue:11 -default _Cancel";
			break;
	}
	std::ostringstream command;

	command << xmessage << " "
		<< "-center "
		<< buttons.str() << " "
		<< "-name " << caption << " "
		<< "-title " << caption << " "
		<< text;

	int result = system(command.str().c_str());

	// Shell returns 0x7f if the command can't be executed
	if (result == -1 || !WIFEXITED(result) || WEXITSTATUS(result) == 0x7f)
	{
		std::cerr
			<< "MessageBox: " << xmessage << " terminated abnormally\n"
			<< "   Caption: " << caption << "\n"
			<< "   Message: " << text << "\n"
			<< std::flush;
		return 1; // Same as xmessage error status
	}

	return WEXITSTATUS(result);
}

#ifdef INCLUDE_MAIN
int main(int argc char** argv)
{
	return MessageBox(0, convert(argv[2]).c_str(), convert(argv[1]).c_str(), MB_ABORTRETRYIGNORE);
}
#endif
