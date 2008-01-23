#!/bin/sh

#
#	$Id$
#
#	Alter the output of MIDL so that it is suitable for use with FieldWorks
#
#	Neil Mayhew - 2007-02-16
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

	sed -e '
		/EXTERN_C const \(IID\|CLSID\|LIBID\|DIID\) \(IID\|CLSID\|LIBID\|DIID\)_\(..*\);/{
			s//#define \2_\3 __uuidof(\3)/
		}
	' $FILE
done
