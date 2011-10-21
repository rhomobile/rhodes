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

#include "statistic/RhoProfiler.h"
#include "logging/RhoLog.h"

#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
#include <unistd.h>
#define Sleep sleep
#endif

void testNoNameCounter(){
//    PROF_START;
//    Sleep(1000);
//    PROF_STOP;
}

void testLocalNamedCounter(){
    PROF_START("LocalCounter1");
    Sleep(2126);
    PROF_STOP("LocalCounter1");
}

void testGlobalNamedCounter(){
    PROF_CREATE_COUNTER("GlobalCounter1");

    PROF_START("GlobalCounter1");
    Sleep(1234);
    PROF_STOP("GlobalCounter1");

    PROF_START("GlobalCounter1");
    Sleep(1234);
    PROF_STOP("GlobalCounter1");

    PROF_DESTROY_COUNTER("GlobalCounter1");
}

void testGlobalNamedCounterFlush(){
    PROF_CREATE_COUNTER("GlobalCounter2");

    PROF_START("GlobalCounter2");
    Sleep(1234);
    PROF_STOP("GlobalCounter2");

    PROF_FLUSH_COUNTER("GlobalCounter2","Step1");

    PROF_START("GlobalCounter2");
    Sleep(1234);
    PROF_STOP("GlobalCounter2");

    PROF_DESTROY_COUNTER("GlobalCounter2");
}

int Func1(){
    Sleep(500);
    return 1;
}

void testProfCall(){
    int n = 0;
    PROF_CALL(n = Func1());
}

extern "C" void runAllProfilingTests()
{
    testNoNameCounter();
    testLocalNamedCounter();
    testProfCall();
    testGlobalNamedCounter();
    testGlobalNamedCounterFlush();
}
