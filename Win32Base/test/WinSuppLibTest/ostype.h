/*
 *	ostype.h
 *
 *	Utility class for converting OSTypes to strings
 *
 *	Neil Mayhew - 6 Nov 2002
 */

#pragma once

#include <cstring>

class ostype
{
	char data[sizeof(OSType) + 1];
public:
	ostype(OSType t)
	{
		std::memcpy(data, &t, sizeof(t));
		data[sizeof(t)] = 0;
	}
	operator const char* () const
	{
		return data;
	}
	const char* operator & () const
	{
		return data;
	}
};

/*
	Usage:

	OSType t = 'wxyz';

	std::printf("OSType: '%s'\n", static_cast<const char*>(ostype(t)));
	std::printf("OSType: '%s'\n", &ostype(t));
	std::cout << "OSType: '" << ostype(t) << "'" << std::endl;
*/