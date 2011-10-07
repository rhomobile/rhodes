/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
