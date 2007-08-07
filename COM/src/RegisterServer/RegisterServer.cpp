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

int main(int argc, char const* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		void* handle = dlopen(argv[i], RTLD_LAZY);
		if (!handle)
		{
			std::cerr << argv[i] << ": " << dlerror() << "\n";
			exit(EXIT_FAILURE);
		}

		dlerror();	/* Clear any existing error */

		long (*DllMain)(void* hmod, unsigned long dwReason, void* pvReserved);
		*(void **)(&DllMain) = dlsym(handle, "DllMain");
		if (const char* error = dlerror()) 
		{
			std::cerr << "DllMain: " << error << "\n";
			exit(EXIT_FAILURE);
		}
		//std::cerr << "DllMain = " << (void*)DllMain << "\n";

		long (*DllRegisterServer)();
		*(void **)(&DllRegisterServer) = dlsym(handle, "DllRegisterServer");
		if (const char* error = dlerror()) 
		{
			std::cerr << "DllRegisterServer: " << error << "\n";
			exit(EXIT_FAILURE);
		}
		//std::cerr << "DllRegisterServer = " << (void*)DllRegisterServer << "\n";

		/*std::cerr << "DllMain(0, 0, 0): returns " <<*/ DllMain(0, 0, 0) /*<< "\n"*/;
		/*std::cerr << "DllRegisterServer(): returns " <<*/ DllRegisterServer() /*<< "\n"*/;

		dlclose(handle);
	}
}

