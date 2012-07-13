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
	echo "// Automatically generated on $DATE from $FILE by $0"
	echo "//#include <COM.h>"
	echo "#include \"$FILE\""
	echo

	sed -n -e '
		s/\r$//
		/struct __declspec(uuid("/{
			h
			n
			# Remove C comments
			s|/\*.*\*/||g
			# Remove trailing semi-colons
			s/;$//
			# Get rid of everything after colons (inheritances)
			# s/[ \t]*:.*\n//
			s/:.*//
			G
			# Prepend beginning of libcom-style GUID definition
			s/^[ \t]*/DEFINE_UUIDOF(/
			s/[ \t]*struct __declspec(uuid(/,/
			s/)$/;/
			# Change to static initialization
			s/"\(........\)-\(....\)-\(....\)-\(....\)-\(............\)"/0x\1,0x\2,0x\3,{\4\5}/
			s/{\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)}/0x\1,0x\2,0x\3,0x\4,0x\5,0x\6,0x\7,0x\8/
			p
		}
		/MIDL_INTERFACE("/{
			h
			n
			G
			s/[ \t]*:.*\n//
			s/^[ \t]*/DEFINE_UUIDOF(/
			s/[ \t]*MIDL_INTERFACE(/,/
			s/$/;/
			s/"\(........\)-\(....\)-\(....\)-\(....\)-\(............\)"/0x\1,0x\2,0x\3,{\4\5}/
			s/{\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)}/0x\1,0x\2,0x\3,0x\4,0x\5,0x\6,0x\7,0x\8/
			p
		}
		/DECLSPEC_UUID("/{
			h
			n
			G
			s/^/DEFINE_UUIDOF(/
			s/class DECLSPEC_UUID(/,/
			s/[ \t]*;.*\n//
			s/$/;/
			s/"\(........\)-\(....\)-\(....\)-\(....\)-\(............\)"/0x\1,0x\2,0x\3,{\4\5}/
			s/{\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)}/0x\1,0x\2,0x\3,0x\4,0x\5,0x\6,0x\7,0x\8/
			p
		}
	' $FILE
done
