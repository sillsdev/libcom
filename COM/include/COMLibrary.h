/*
 *	$Id$
 *
 *	Functions unique to this COM Support Library
 *
 *	MarkS - 2006-12
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

#ifndef COMLibrary_h
#define COMLibrary_h

#include "COM.h"

void RegisterFactory(const CLSID& classID, class IClassFactory* classFactory);
void CoRegisterClassInfo(const CLSID*, const char*, const char*);

#endif /* !COMLibrary_h */
