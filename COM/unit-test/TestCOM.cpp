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

#include <vector>
#include <algorithm>
#include <ostream>
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

BOOST_AUTO_TEST_CASE( test_StringFromGUID2 )
{
	// Params to be passed
	GUID guid("f350127b-de04-44c5-ad9c-e1bd72e68086");
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
