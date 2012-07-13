/*
 *	$Id$
 *
 *	Jar C# COM server
 *
 *	MarkS - 2007-10-31
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

namespace Kitchen
{
	/**
	 * Cookie jar interface
	 */
	[ComVisible(true)]
	[Guid("5e139206-aca8-4530-b2b9-308e16f81bda")]
	[InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
	public interface IJar
	{
		[DispId(1)]
		int GetNumberCookies();
		[DispId(2)]
		void AddCookies(int number);
	}

	/**
	 * Cookie jar
	 */
	[ComVisible(true)]
	[Guid("b1de8884-c20c-4743-a1b3-851daad6d188")]
	[ClassInterface(ClassInterfaceType.None)]
	[ProgId("Kitchen.Jar")]
	public class Jar : IJar
	{
		/** Constructs a jar */
		public Jar() {
			m_numberCookies = 0;
		}

		/** Gets the number of cookies in this jar
		 * @return number of cookies in this jar
		 */
		public int GetNumberCookies() {
			return m_numberCookies;
		}

		/** Add cookies to this jar
		 * @param number number of cookies to add to this jar
		 */
		public void AddCookies(int number) {
			m_numberCookies += number;
		}

		/** Number of cookies in this jar */
		private int m_numberCookies;
	}
}
