/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes;

import java.io.CharArrayWriter;
import java.io.PrintWriter;
import java.io.Writer;

public class Logger {

	// Errors
	public native static void E(String tag, String msg);
	// Warnings
	public native static void W(String tag, String msg);
	// Info
	public native static void I(String tag, String msg);
	// Debug
	public native static void D(String tag, String msg);
	// Trace
	public native static void T(String tag, String msg);
	
	public static void E(String tag, Throwable e) {
	    Writer bufWriter = new CharArrayWriter(256);
	    PrintWriter writer = new PrintWriter(bufWriter);
	    e.printStackTrace(writer);

	    if (writer.checkError()) {
		        E(tag, e.getMessage());
		        E("Logger", "Cannot print stack trace");
	    }

		E(tag, bufWriter.toString());
	}
	
	public static void W(String tag, Throwable e) {
		W(tag, e.getMessage());
	}
	
	public static void I(String tag, Throwable e) {
		I(tag, e.getMessage());
	}
	
	public static void D(String tag, Throwable e) {
		D(tag, e.getMessage());
	}
	
	public static void T(String tag, Throwable e) {
		T(tag, e.getMessage());
	}
	
}
