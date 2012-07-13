#
#	$Id$
#
#	Autoconf file
#
#	MarkS - 2007-02-15
#
# COM Support Library
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


AC_INIT(configure.ac)

AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(libcom, 0.4.0)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AM_PROG_LIBTOOL

AC_ARG_WITH([fieldworks],
  [AS_HELP_STRING([--with-fieldworks],
    [Use the FieldWorks GenericFactory class in testing
     (from the already-built FieldWorks source tree in $FWROOT using architecture $FWARCH)
     @<:@default=no@:>@])],
  [],
  [with_fieldworks=no])

AC_ARG_VAR(FWROOT, [The root of the FieldWorks source tree @<:@default=$top_srcdir/../FieldWorks@:>@])
AC_ARG_VAR(FWARCH, [The architecture of the FieldWorks source tree @<:@default=$(uname -m)@:>@])

AM_CONDITIONAL([USE_FW_GENERIC_FACTORY], [test "x$with_fieldworks" = xyes])
test "${FWROOT+set}" = set || FWROOT='$(top_srcdir)'/../FieldWorks
test "${FWARCH+set}" = set || FWARCH=$(uname -m)

AC_ARG_WITH([remote-midl],
  [AS_HELP_STRING([--with-remote-midl],
    [Use midl remotely to generate headers and interops when building test/
     @<:@default=no@:>@])],
  [],
  [with_remote_midl=no])

AM_CONDITIONAL([USE_REMOTE_MIDL], [test "x$with_remote_midl" = xyes])

AC_OUTPUT(Makefile src/Makefile test/Makefile unit-test/Makefile)
