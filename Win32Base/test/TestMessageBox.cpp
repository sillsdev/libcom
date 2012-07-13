//
//	MessageBox tests
//
//	Eberhard Beilharz - 2010-03-19
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
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include "Hacks.h"
#include "TestUtils.h"
#include "winuser.h"

int nErr;

void AssertMessageBox(UINT type, UINT nRetExpected, const char* expectedStr,
	const wchar_t* captionStr = L"caption", const wchar_t* textStr = L"text")
{

	std::ostringstream filename;
	filename << "/tmp/testMessageBox_" << getpid();
	std::ostringstream expectedRetValue;
	expectedRetValue << nRetExpected;

	setenv("XMESSAGE", "./messageboxdouble", true);
	setenv("MB_RESULTFILE", filename.str().c_str(), true);
	setenv("MB_EXPECTED", expectedStr, true);
	setenv("MB_RESULT", expectedRetValue.str().c_str(), true);

	UINT nRet = MessageBox(NULL, textStr, captionStr, type);

	std::ifstream resultFile(filename.str().c_str());
	if (resultFile.is_open())
	{
		std::string line;
		nRet++;
		while (!resultFile.eof())
		{
			std::getline(resultFile, line);
			std::cerr << line << std::endl;
		}
		resultFile.close();
	}

	Assert_eq(nRetExpected, nRet, "MessageBox returned wrong value");

	remove(filename.str().c_str());
}

int main(int argc, char** argv)
{
	nErr = 0;

	AssertMessageBox(MB_ABORTRETRYIGNORE, IDABORT, "_Abort:3,_Retry:4,_Ignore:5 _Ignore caption text");
	AssertMessageBox(MB_ABORTRETRYIGNORE, IDRETRY, "_Abort:3,_Retry:4,_Ignore:5 _Ignore caption text");
	AssertMessageBox(MB_ABORTRETRYIGNORE, IDIGNORE, "_Abort:3,_Retry:4,_Ignore:5 _Ignore caption text");
	AssertMessageBox(MB_OK, IDOK, "_OK:0 _OK caption text");
	AssertMessageBox(MB_OKCANCEL, IDCANCEL, "_OK:0,_Cancel:2 _OK caption text");
	AssertMessageBox(MB_OK, IDOK, "_OK:0 _OK caption text");
	AssertMessageBox(MB_YESNOCANCEL, IDYES, "_Yes:6,_No:7,_Cancel:2 _Yes caption text");
	AssertMessageBox(MB_YESNO, IDNO, "_Yes:6,_No:7 _Yes caption text");
	AssertMessageBox(MB_RETRYCANCEL, IDRETRY, "_Retry:4,_Cancel:2 _Retry caption text");
	AssertMessageBox(MB_CANCELTRYCONTINUE, IDCANCEL, "_Cancel:2,_Try:10,_Continue:11 _Cancel caption text");
	AssertMessageBox(MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL, IDABORT,
		"_Abort:3,_Retry:4,_Ignore:5 _Ignore caption text");
	AssertMessageBox(MB_ABORTRETRYIGNORE, IDABORT,
		"_Abort:3,_Retry:4,_Ignore:5 _Ignore A  \"caption\" It's text", L"A  \"caption\"", L"It's text");
	return nErr;
}
