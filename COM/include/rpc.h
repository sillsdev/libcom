/*
 *	$Id$
 *
 *	This file is for compatibility with the naming and use of Windows header files.
 *
 *	MarkS - 2007-02-05
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef rpc_h_included
#define rpc_h_included

#include "COMInterfaces.h"
#ifndef NO_FW
#include <COMInterfacesMore.h>
#endif

#define __RPC_USER
#define __RPC_STUB
#define __RPC_FAR
typedef int PRPC_MESSAGE;
typedef void* RPC_IF_HANDLE;

#endif //rpc_h_included
