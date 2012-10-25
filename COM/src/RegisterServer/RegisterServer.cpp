/*
 *	$Id$
 *
 *	Extract GUIDs that a COM server supports
 *
 *	Neil Mayhew - 2007-07-31
 *
 * COM Support Library
 * Copyright (C) 2007 SIL International
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */
#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>

/**
 * @brief Open DLL files and call DllRegisterServer to obtain data for components.map.
 * @param argc
 * @param argv
 */
int main(int argc, char const* argv[])
{

	// For each DLL file, call DllMain and DllRegisterServer
	for (int i = 1; i < argc; ++i)
	{
		const char * dllfilename = argv[i];

		void* handle = dlopen(dllfilename, RTLD_LAZY);
		if (!handle)
		{
			std::cerr << "RegisterServer: error dlopening file '" << dllfilename << "': " << dlerror() << "\n";
			exit(EXIT_FAILURE);
		}

		dlerror();	/* Clear any existing error */

		typedef long (*DllMain)(void* hmod, unsigned long dwReason, void* pvReserved);
		DllMain dllMain = reinterpret_cast<DllMain>(dlsym(handle, "DllMain"));
		if (const char* error = dlerror())
		{
			std::cerr << "RegisterServer: error finding symbol 'DllMain' in library '" << dllfilename << "': " << error << "\n";
			exit(EXIT_FAILURE);
		}

		typedef long (*DllRegisterServer)();
		DllRegisterServer dllRegisterServer = reinterpret_cast<DllRegisterServer>(dlsym(handle, "DllRegisterServer"));
		if (const char* error = dlerror())
		{
			std::cerr << "RegisterServer: error finding symbol 'DllRegisterServer' in library '" << dllfilename << "': " << error << "\n";
			exit(EXIT_FAILURE);
		}

		dllMain(0, 0, 0);
		dllRegisterServer();

		dlclose(handle);
	}
}
