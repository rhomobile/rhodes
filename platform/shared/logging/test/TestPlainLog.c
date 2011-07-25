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

#include "../RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "PlainC"

void testTraceIf(){
    int i = 0;

    RAWTRACE_IF(i==0,"Conditional trace");

    i = 1;
    RAWTRACE_IF(i==0,"ERROR: Conditional trace");

    i = 2;
    RAWTRACE_IF1(i==2,"Conditional trace : %s", "Param1");

}

void testPlainLog(){

    RAWTRACE("RAW: Trace 1");
    RAWTRACE1("RAW: Trace %d", 2);
    RAWTRACE2("RAW: Trace %d : %d", 3,4);

    RAWTRACEC("CatRaw1", "RAW: Trace categorized");
    RAWTRACEC1("CatRaw1", "RAW: Trace categorized: %d", 34);
    RAWTRACEC2("CatRaw1", "RAW: Trace categorized %s : %d", "34", 56);

    RAWLOG_INFO("RAW: Info 1");
    RAWLOG_INFO1("RAW: Info %d", 55);
    RAWLOG_INFO2("RAW: Info %d:%d", 55, 66);

    RAWLOGC_INFO("CatRaw2", "RAW: Info categorized");
    RAWLOGC_INFO1("CatRaw2", "RAW: Info categorized : %d", 66);
    RAWLOGC_INFO2("CatRaw2", "RAW: Info categorized : %d:%d", 66, 77);

    testTraceIf();

    //RHO_ASSERT(0);
    //RHO_ASSERTS(0,"Too bad!");
}