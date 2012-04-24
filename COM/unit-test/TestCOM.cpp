/*
 * TestCOM.cpp
 *
 * Unit tests for COM Support Library
 *
 * Original author: Neil Mayhew <neil_mayhew@sil.org>
 * Creation date: 2010-05-12
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

#include "COM.h"
#include "COMSupportInternals.h"

#include <vector>
#include <algorithm>
#include <ostream>
#include <iomanip>
#include <iterator>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE COM
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

namespace std
{
	// Stream a vector up to the terminating 0 or end()
	template<class T>
	std::ostream& operator << (std::ostream& stream, const std::vector<T>& v)
	{
		std::copy(v.begin(), std::find(v.begin(), v.end(), T(0)),
			std::ostream_iterator<char>(stream));
		return stream;
	}
}

// Stream a PlainGUID
std::ostream& operator << (std::ostream& out, const PlainGUID& g)
{
	using std::setw;

	std::ios::fmtflags previous = out.flags();

	out << std::hex << std::setfill('0') << std::setiosflags(std::ios::uppercase);

	out << "{";

	out << setw(8) << g.Data1 << "-"
	    << setw(4) << g.Data2 << "-"
	    << setw(4) << g.Data3 << "-";

	for (int i = 0; i != sizeof(g.Data4); ++i)
		out << std::setw(2) << int(g.Data4[i]);

	out << "}";

	out.flags(previous);

	return out;
}

// Helper functions for iterating C-style arrays

template<class T, size_t N>
inline const T* begin(const T (&array)[N])
{
	return &array[0];
}

template<class T, size_t N>
inline const T* end(const T (&array)[N])
{
	return &array[N];
}

// Helper function for checking equality

template<typename Left, typename Right>
bool bytewise_equal(const Left& l, const Right& r)
{
	return sizeof(l) == sizeof(r) && memcmp(&l, &r, sizeof(r)) == 0;
}

#define CHECK_EQUAL_GUID(A, B) BOOST_CHECK_PREDICATE( (bytewise_equal<PlainGUID,PlainGUID>), (A) (B) )

// Base values

static const PlainGUID zero_guid = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
static const PlainGUID base_guid = {0x0c733a30,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}};
static const char      base_text[] = "0c733a30-2a1c-11ce-ade5-00aa0044773d";
static const PlainGUID copy_guid = {0x0c733a30,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}};

BOOST_AUTO_TEST_CASE( test_PlainGUID )
{
	// Copy constructor
	PlainGUID plain(base_guid);
	CHECK_EQUAL_GUID( plain, base_guid );

	// Equality
	BOOST_CHECK_EQUAL( copy_guid, base_guid );
	BOOST_CHECK_EQUAL( copy_guid.compare(base_guid), 0 );
	BOOST_CHECK_EQUAL( plain, base_guid );
	BOOST_CHECK_EQUAL( plain.compare(base_guid), 0 );
	BOOST_CHECK_NE   ( plain, zero_guid );
	BOOST_CHECK_NE   ( plain.compare(zero_guid), 0 );

	// Null-checking
	BOOST_CHECK( !plain.isNull() );
	BOOST_CHECK( zero_guid.isNull() );

	// Clearing
	plain.clear();
	BOOST_CHECK_EQUAL( plain, zero_guid );

	// Assignment
	plain = base_guid;
	BOOST_CHECK_EQUAL( plain, base_guid );

	// String out
	BOOST_CHECK_EQUAL( plain.str(), base_text );

	// String in
	plain.clear();
	plain.initialize(base_text);
	BOOST_CHECK_EQUAL( plain, base_guid );
	BOOST_CHECK_THROW( plain.initialize("garbage"), std::runtime_error );
	BOOST_CHECK_EQUAL( plain, base_guid );

	// Comparison
	plain.Data4[7] += 1;
	BOOST_CHECK( plain > base_guid );
	BOOST_CHECK( plain.compare(base_guid) > 0 );
	plain.Data4[7] -= 2;
	BOOST_CHECK( plain < base_guid );
	BOOST_CHECK( plain.compare(base_guid) < 0 );

	// Generating new values
	plain.create();
	BOOST_CHECK_NE( plain, zero_guid );
	BOOST_CHECK_NE( plain, base_guid );

	// Null value
	BOOST_CHECK_EQUAL( PlainGUID::null, zero_guid );
}

BOOST_AUTO_TEST_CASE( test_SmartGUID )
{
	PlainGUID plain(base_guid);

	// Construct default value
	SmartGUID null;
	CHECK_EQUAL_GUID( null, zero_guid );

	// Mixed equality
	BOOST_CHECK_EQUAL( null, zero_guid );
	BOOST_CHECK_EQUAL( zero_guid, null );

	// Construct Smart from Plain
	SmartGUID smart(plain);
	BOOST_CHECK_EQUAL( smart, plain );

	// Construct Plain from Smart
	SmartGUID plain2(smart);
	BOOST_CHECK_EQUAL( plain2, plain );

	// Construct Smart from Smart
	SmartGUID smart2(smart);
	BOOST_CHECK_EQUAL( smart2, plain );

	// Assign Smart from Plain
	SmartGUID smart3;
	smart3 = plain;
	BOOST_CHECK_EQUAL( smart3, plain );

	// Assign Plain from Smart
	PlainGUID plain4;
	plain4 = smart;
	BOOST_CHECK_EQUAL( plain4, plain );

	// Assign Smart from Smart
	SmartGUID smart4;
	smart4 = smart;
	CHECK_EQUAL_GUID( smart4, smart );
	BOOST_CHECK_EQUAL( smart4, smart );

	// Null-checking
	BOOST_CHECK( null.isNull() );
	BOOST_CHECK( !smart.isNull() );

	// Construct new value (in cleared memory)
	char raw_storage[sizeof(SmartGUID)];
	std::fill(raw_storage, raw_storage + sizeof(SmartGUID), 0);
	SmartGUID& smart5 = *new (raw_storage) SmartGUID(true);
	BOOST_CHECK_NE( smart5, base_guid );
	BOOST_CHECK_NE( smart5, zero_guid );
	smart3.~SmartGUID();

	// Construct from string
	SmartGUID smart6(base_text);
	BOOST_CHECK_EQUAL( smart6, base_guid );
	BOOST_CHECK_THROW( SmartGUID("garbage"), std::runtime_error );
}

BOOST_AUTO_TEST_CASE( test_StringFromGUID2 )
{
	// Params to be passed
	GUID guid = {0xf350127b,0xde04,0x44c5,{0xad,0x9c,0xe1,0xbd,0x72,0xe6,0x80,0x86}};
	std::vector<OLECHAR> output;

	// Compute expected text data
	std::string text = guid.str();
	const int NCHARS = text.size();

	// Set up possible outputs
	const OLECHAR random_pattern = ('y' << 8) | 'x';
	std::vector<OLECHAR>  initial(NCHARS*3, random_pattern);
	std::vector<OLECHAR>& unchanged = initial; // Synonym for initial
	std::vector<OLECHAR>  expected = initial;
	std::copy(text.begin(), text.end(), expected.begin()); // Converts to OLECHAR
	expected[NCHARS] = 0; // Zero-terminate

	// Negative buffer size
	output = initial;
	BOOST_CHECK_EQUAL( StringFromGUID2(guid, &output[0], -1), 0 );
	BOOST_CHECK_EQUAL( output, unchanged );

	// Zero buffer size
	output = initial;
	BOOST_CHECK_EQUAL( StringFromGUID2(guid, &output[0], 0), 0 );
	BOOST_CHECK_EQUAL( output, unchanged );

	// One buffer size
	output = initial;
	BOOST_CHECK_EQUAL( StringFromGUID2(guid, &output[0], 1), 0 );
	BOOST_CHECK_EQUAL( output, unchanged );

	// Small buffer size
	output = initial;
	BOOST_CHECK_EQUAL( StringFromGUID2(guid, &output[0], NCHARS/2), 0 );
	BOOST_CHECK_EQUAL( output, unchanged );

	// Just too small buffer size
	output = initial;
	BOOST_CHECK_EQUAL( StringFromGUID2(guid, &output[0], NCHARS), 0 );
	BOOST_CHECK_EQUAL( output, unchanged );

	// Just big enough buffer size
	output = initial;
	BOOST_CHECK_EQUAL( StringFromGUID2(guid, &output[0], NCHARS+1), NCHARS+1 );
	BOOST_CHECK_EQUAL( output, expected );

	// Large buffer size (full length of output)
	output = initial;
	BOOST_CHECK_EQUAL( StringFromGUID2(guid, &output[0], output.size()), NCHARS+1 );
	BOOST_CHECK_EQUAL( output, expected );
}

BOOST_AUTO_TEST_CASE( test_wcslen_OLECHAR )
{
	static const size_t sizes[] = { 0, 1, 2, 5, 10, 100, 1000, 10000 };

	for (const size_t* it = begin(sizes); it != end(sizes); ++it)
	{
		const size_t nchars = *it;
		std::vector<OLECHAR> string(nchars, 'a');
		string.push_back(0);
		BOOST_CHECK_EQUAL( wcslen(&string[0]), nchars );
	}
}
