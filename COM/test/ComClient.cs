/*
 *	$Id$
 *
 *	C# COM Ball Client example and test
 *
 *	MarkS - 2005-01-21
 *
 * Ball COM Support Library Test
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
