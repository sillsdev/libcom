#!/bin/sh

# 
# COM Support Library helper script
# Copyright (C) 2007 SIL
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
# http://www.gnu.org/licenses/lgpl.html
# 

for FILE
do
OUTPUT=`echo "$FILE" | sed -e 's/_idl\.h$/_GUIDs.cpp/'`
sed -n \
-e "
1i\\
\/\/ `echo $OUTPUT`, AUTOMATICALLY GENERATED ON `date` FROM `echo $FILE`\\
\/\/#include <COM.h>\\
#include \"$FILE\"\\
" \
-e '
/MIDL_INTERFACE("/{
	h
	n
	G
	s/[ \t]*:.*\n//
	s/^[ \t]*/template<> const GUID __uuidof(/
	s/[ \t]*MIDL_INTERFACE/)/
	s/$/;/
	p
}' \
-e '
/DECLSPEC_UUID("/{
	h
	n
	G
	s/^/template<> const GUID __uuidof(/
	s/class DECLSPEC_UUID/)/
	s/[ \t]*;.*\n//
	s/$/;/
	p
}' \
 "$FILE" >"$OUTPUT"
done
