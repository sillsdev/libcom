/*----------------------------------------------------------------------------
Copyright 2002, SIL International. All rights reserved.

File: WinSupportLibReadMe.txt
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$id$

Version 1.3 of the MDU's WinSupportLib is now on the CVS server in Calgary.

There are three CodeWarrior projects for WinSupportLib:

  WinSuppLibShared.mcp	Compiles the CFM shared libraries in debug and final
  						modes into libraries/bin; also compiles the special
  						test version into the bundle-packaged test app
  						(in debug and final forms).
  WinSuppLibDynamic.mcp	Compiles the Mach-O libraries in debug and final
  						modes into Frameworks in libraries/bin; also compiles
  						the special test version into Frameworks within the
  						bundle-packaged test app (in debug and final forms).
  WinSuppLibTest.mcp	Compiles the test program in both debug and final forms
  						into bundle-packaged form.

Bugs/Features
=============

WinSupportLib v1.3
------------------
1	WinSupportLib assumes that apps which use it will be bundle-packaged apps.
	Some of the Windows APIs it implements will not work if you try to use it
	with an app that is not packaged as bundle.

2	Include WinSupport.h (in libraries/include) when compiling your application.
	This header contains the public definitions of the Windows APIs the library
	implements.

3	The Mach-O versions of the Frameworks and test apps don't yet run. Anyone
	wanting to use WinSupportLib should do so only wihin CFM applications. Do
	this by linking with the stub shared library and copying the debug and
	final shared libraries from libraries/bin into your bundle-packaged app's
	MacOSClassic folder. The WinSuppLibShared.mcp project will also generate
	symbolics files for use when debugging if you wish to step through the code
	in WinSupportLib.
----------------------------------------------------------------------------*/
