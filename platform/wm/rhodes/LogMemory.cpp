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

#include "stdafx.h"
#include "LogMemory.h"
#include "logging/RhoLog.h"
#include <sstream>

#define DIV 1024

CLogMemory* CLogMemory::gInstance = 0;

CLogMemory* CLogMemory::getInstance()
{
	if (gInstance==0)
		gInstance = new CLogMemory();
	return gInstance;
}

rho::String CLogMemory::collect()
{
	std::ostringstream oss;

	//Get the memory stats
	MEMORYSTATUS sMemStat;
	sMemStat.dwLength = sizeof(MEMORYSTATUS);

	GlobalMemoryStatus(&sMemStat); 

	sMemStat.dwTotalPhys /= DIV;
	sMemStat.dwAvailPhys /= DIV;
	sMemStat.dwTotalVirtual /= DIV;
	sMemStat.dwAvailVirtual /= DIV;

	oss <<
		"Stats: Load=" << sMemStat.dwMemoryLoad <<
		"%  TotalPhy=" << sMemStat.dwTotalPhys <<
		"KB  FreePhy=" << sMemStat.dwAvailPhys <<
		"KB  TotalVM=" << sMemStat.dwTotalVirtual <<
		"KB  FreeVM=" << sMemStat.dwAvailVirtual <<
		"KB";

	return rho::String(oss.str());
}