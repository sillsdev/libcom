/*
 *	$Id$
 *
 *	C# COM test client
 *
 *	MarkS - 2008-02-08
 *
 * COM Support Library Test
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

using System;
using System.Runtime.InteropServices;
using System.Reflection;
using IWallInterop;
using IBagInterop;
//using IBallMetadata;

/**
 * Tester
 */
public class Tester {
    public static void Main(string[] args) {

		System.Console.WriteLine("Starting Tester.");

		System.Console.WriteLine("Writing Hello on wall.");
		Wall wall = new Wall();
		wall.writeBSTROnWall("Hello");
		System.Console.WriteLine("Writing Brrrrr! on wall.");
		wall.writeBSTROnWall("Brrrrr!");
		System.Console.WriteLine("Reading wall:");
		string wallContents = "overwrite me";
		wallContents = wall.readWallBSTR();
		System.Console.WriteLine("Wall says: " + wallContents);
		System.Console.WriteLine("Again, Wall says: '" + wall.readWallBSTR() + "'.");

		System.Console.WriteLine("\n\nDoing Bag-Ball test");
		//TODO What if I create it something like: IBag bag = (IBall) Factory.BagClass();  Eberhard might have said I was supposed to be doing this sort of thing that way.
		Bag bag = new Bag();
		Ball ball = (Ball) bag.ProvideBall(); // TODO what if I cast it as an IBall rather than Ball

		long distance_rolled;
		distance_rolled = ball.roll(2);
		System.Console.WriteLine("Rolling ball by 2. Accumulated distance: {0} (should say 2)", distance_rolled);
		if (distance_rolled != 2) {
			System.Console.WriteLine("Tester.exe: Error, distance wasn't correct. Exiting.");
			Environment.Exit(-1);
		}
		bag.InspectBall(ball);
		distance_rolled = ball.roll(9);
		System.Console.WriteLine("Rolling ball by 9. Accumulated distance: {0} (should say 14)", distance_rolled);
		if (distance_rolled != 14) {
			System.Console.WriteLine("Tester.exe: Error, distance wasn't correct. Exiting.");
			Environment.Exit(-1);
		}
    }
}
