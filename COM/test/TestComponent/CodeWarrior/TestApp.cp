/* *  Copyright � 2002 Metrowerks Corporation.  All Rights Reserved. * *  Questions and comments to: *       <mailto:support@metrowerks.com> *       <http://www.metrowerks.com/> */#include <stdio.h>extern int BeepTwice( void );int main( void ){	int result;	printf ("This app calls to a PPC shared library for routine BeepTwice().\n\n");		result = BeepTwice();		printf ("BeepTwice() returned %x.\n\n", result);		return 0;}