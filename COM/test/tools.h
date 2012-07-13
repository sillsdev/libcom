/*
 *	$Id$
 *
 *	Debugging tools
 *
 *	MarkS - 2006-12-15
 *
 *	This tools.h file should probably be removed and its contents
 *	reorganized into another location if wanted.
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

#ifndef tools_h
#define tools_h

#include <stdio.h>

#define db printf("%s:%d\n",__FILE__,__LINE__)
#define dbm(message) printf("%s:%d %s\n",__FILE__,__LINE__,message)
#define dbn(message) printf("%s:%d %s",__FILE__,__LINE__,message)

/** Print a GUID in little endian.
 */
static void printGuid(const IID& iid) {
	const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&iid);
	for (int i=0; i<sizeof(iid); i++) {
		fprintf(stderr,"%02x", ptr[i]);
	}
	printf("\n");
}

#endif /* !tools_h */
