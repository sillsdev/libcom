/*
 *	$Id$
 *
 *	Main COM Support Library code
 *
 *	Graeme Costin - 2001-05
 *
 * COM Support Library
 * Copyright (C) 2001, 2007 SIL International
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

#include "BasicTypes.h"
#include "COMError.h"
#include "COMInterfaces.h"

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cctype>

#include "COMSupportInternals.h"
#include "COMLibrary.h"

#include <map>
#include <dlfcn.h>
#include <stdio.h>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>

#include <stdexcept>

#include "ComRegistry.h"

#define DLLEXPORT // Define properly if ever needed

/**
 * @brief Register a class factory by class ID in the internal registry.
 *
 * "Components should call this function to register their class factory
 * with the ComRegistry map. This should be done in the constructor of the
 * server's class factory and the instance of the class factory should be
 * created as a static pointer to the class factory is initialised." Or
 * we will register components we load.
 * @param classID class ID to register
 * @param classFactory class factory to register that can create objects of class ID classID.
 */
DLLEXPORT
void RegisterFactory(const CLSID& classID, LPCLASSFACTORY classFactory)
{
	ComRegistry::getInstance()->registerFactory(classID, classFactory);
}

/**
 * @brief Write class information to stdout. Still needs processed before it's valid components.map syntax.
 */
DLLEXPORT
void CoRegisterClassInfo(const CLSID* classId, const char* progId, const char* descr)
{
	std::cout << "# " << progId << ": " << descr << "\n"
		<< classId->str() << std::endl;
}
