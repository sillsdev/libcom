/*
 *      $Id$
 *
 *      Definitions
 *
 *      MarkS - 2008-07-03
 *
 * Win32 Compatibility Library
 * Copyright (C) 2008 SIL International
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

#ifndef win32base_winnt_h
#define win32base_winnt_h

static const int KEY_WRITE = ((0x00020000| 0x00000002| 0x00000004) & (~0x00100000) )
static const int KEY_READ  = ((0x00020000| 0x00000001| 0x00000008| 0x00000010) & (~0x00100000) )

#endif // !win32base_winnt_h
