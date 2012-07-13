/*
 *	$Id$
 *
 *	Example and test of C# passing a C# object via CCW to a C++ COM server
 *
 *	MarkS - 2007-11-05
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
using IBakeryMetadata;

public class BakeryJarTestClient {
	public static void Main(string[] args) {
		System.Console.WriteLine("Bakery + Jar CCW COM test.");

		//IBakery Bakery = (IBakery) Factory.BakeryClass(); // TODO should be using this line. (?really?)
		Bakery bakery = new Bakery();
		Kitchen.Jar jar = new Kitchen.Jar();

		System.Console.WriteLine("Number of cookies in jar: " + jar.GetNumberCookies());
		System.Console.WriteLine("Baking 8 cookies.");
		bakery.bakeCookies(8, jar);

		System.Console.WriteLine("Number of cookies in jar: " + jar.GetNumberCookies());
		System.Console.WriteLine("Baking 3 cookies.");
		bakery.bakeCookies(3, jar);

		int totalCookiesBaked = bakery.getNumberCookiesBaked();
		if (11 != totalCookiesBaked) {
			System.Console.WriteLine("Error: Did not bake correct number of cookies.");
			throw new Exception("error"); // TODO right way
		} else {
			System.Console.WriteLine("Successfully baked a total of " + totalCookiesBaked + " cookies.");
		}
	}
}