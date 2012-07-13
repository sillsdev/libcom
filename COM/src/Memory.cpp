/*
 *	$Id$
 *
 *	COM Support Library Memory Allocation
 *
 *	MarkS - 2007-02-26
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
#include <cstdlib>

#define DLLEXPORT // Define properly if ever needed

/**
 * Allocate memory.
 * Because there is no guarantee that memory will actually be allocated, you should always check that the return value is not NULL.
 * http://msdn2.microsoft.com/en-us/library/ms692727.aspx
 * @param size number of bytes to allocate
 * @return pointer to allocated memory, or NULL if the memory allocation request failed.
 */
DLLEXPORT
void* CoTaskMemAlloc(SIZE_T size)
{
	// This is defined as just malloc() until a reason is found
	// for a more complex implementation.

	return (std::malloc(size));
}

/**
 * Change size of a previously allocated chunk of memory.
 * If previousAllocation is NULL, allocates memory like CoTaskMemAlloc.
 * If previousAllocation is not NULL and newSize is 0, then previousAllocation is freed.
 * http://msdn2.microsoft.com/en-us/library/ms687280.aspx
 * @param previousAllocation pointer returned from a previous CoTaskMemAlloc to previous allocation of memory to be reallocated or freed, or NULL
 * @param newSize number of bytes for the new allocation, or 0 (see description).
 * @return pointer to reallocated memory (which may be in a different memory location than previousAllocation), or NULL if newSize is 0 and previousAllocation is not NULL, or NULL if we fail to allocate memory for the new allocation
 */
DLLEXPORT
void* CoTaskMemRealloc(LPVOID previousAllocation, SIZE_T newSize)
{
	// This is defined as just realloc() until a reason is found
	// for a more complex implementation.

	return (std::realloc(previousAllocation, newSize));
}

/**
 * Free previously allocated memory. If allocatedMemory is NULL, nothing happens.
 * http://msdn2.microsoft.com/en-us/library/ms680722.aspx
 * @param allocatedMemory pointer to memory previously allocated by CoTaskMemAlloc or CoTaskMemRealloc, or NULL
 */
DLLEXPORT
void CoTaskMemFree(LPVOID allocatedMemory)
{
	// This is defined as just free() until a reason is found
	// for a more complex implementation.

	std::free(allocatedMemory);
}
