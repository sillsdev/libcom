/*
 *	$Id$
 *
 *	C# COM Client example and test
 *
 *	MarkS - 2005-01-21
 *
 * Ball COM Support Library Test
 * Copyright (C) 2007 SIL
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

// ComClient.cs
// Created  2005.01.21, MarkS
// ComClient accesses a Ball through COM (hopefully).
// Modified 2006.12.12 MarkS
// Okay, I'm re-working some things to try to get Mono COM RCW's to connect to our FakeCOMLibrary, and that into the Ball COM object.
// instantaneously, LD_PRELOAD=./libballmodule.so LD_LIBRARY_PATH=$HOME/repo/libraries/COMSupportLibrary mono ComClient.exe
// Modified 20070130 MarkS
// Cleaning up.

using System;
using System.Runtime.InteropServices;
using System.Reflection;
using IBallMetadata;

public class ComClient {
    public static void Main(string[] args) {

		System.Console.WriteLine("Let's create a Ball.");
		//IBall ball = (IBall) Factory.BallClass(); // TODO should be using this line. (?really?)
		Ball ball = new Ball();

		System.Console.WriteLine("bouncing ball.");
		ball.bounce();
		System.Console.WriteLine("Rolling ball by 3.");
		ball.roll(3);
		System.Console.WriteLine("Rolling ball by 4.");
		int dist;
		dist = ball.roll(4);
		System.Console.WriteLine("Ball was rolled a total of " + dist + ".");
    }
}
