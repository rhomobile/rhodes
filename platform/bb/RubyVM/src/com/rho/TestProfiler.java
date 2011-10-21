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

package com.rho;

public class TestProfiler {
	private static final RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() :
		new RhoProfiler();

	static void Sleep(int nDelay){
	    try{ Thread.sleep(nDelay); }catch(Exception exc){}
	}

	static void testLocalNamedCounter(){
		PROF.START("LocalCounter1");
	    Sleep(2126);
	    PROF.STOP("LocalCounter1");
	}

	static void testGlobalNamedCounter(){
		PROF.CREATE_COUNTER("GlobalCounter1");

		PROF.START("GlobalCounter1");
	    Sleep(1234);
	    PROF.STOP("GlobalCounter1");

	    PROF.START("GlobalCounter1");
	    Sleep(1234);
	    PROF.STOP("GlobalCounter1");

	    PROF.DESTROY_COUNTER("GlobalCounter1");
	}

	static void testGlobalNamedCounterFlush(){
		PROF.CREATE_COUNTER("GlobalCounter2");

		PROF.START("GlobalCounter2");
	    Sleep(1234);
	    PROF.STOP("GlobalCounter2");

	    PROF.FLUSH_COUNTER("GlobalCounter2","Step1");

	    PROF.START("GlobalCounter2");
	    Sleep(1234);
	    PROF.STOP("GlobalCounter2");

	    PROF.DESTROY_COUNTER("GlobalCounter2");
	}

	static public void runAllTests(){
	    testLocalNamedCounter();
	    testGlobalNamedCounter();
	    testGlobalNamedCounterFlush();
	}

}
