#!/bin/sh

#
#	$Id$
#
#	Create a GUID definitions file from MIDL output
#
#	MarkS - 2007-01-22
# 
# COM Support Library helper script
# Copyright (C) 2007 SIL International
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

if [ $# -eq 0 ]
then
	echo "Usage: $0 FILE..."
	exit 1
fi

DATE=`date`

for FILE
do
	echo "// AUTOMATICALLY GENERATED ON $DATE FROM $FILE by $0"
	echo "//#include <COM.h>"
	echo "#include \"$FILE\""
	echo

	sed -n -e '
		/MIDL_INTERFACE("/{
			h
			n
			G
			s/[ \t]*:.*\n//
			s/^[ \t]*/template<> const GUID __uuidof(/
			s/[ \t]*MIDL_INTERFACE/)/
			s/$/;/
			p
		}
		/DECLSPEC_UUID("/{
			h
			n
			G
			s/^/template<> const GUID __uuidof(/
			s/class DECLSPEC_UUID/)/
			s/[ \t]*;.*\n//
			s/$/;/
			p
		}
	' $FILE
done
