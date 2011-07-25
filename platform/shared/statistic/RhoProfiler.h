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

#ifndef _RHOPROFILER_H_
#define _RHOPROFILER_H_

#include "common/RhoPort.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#if RHO_STRIP_PROFILER==0
void rhoStartProfilerCounter(const char* file, int line, const char* szCounterName );
void rhoStopProfilerCounter(const char* file, int line, const char* szCounterName );
void rhoCreateProfilerCounter(const char* file, int line, const char* szCounterName );
void rhoDestroyProfilerCounter(const char* file, int line, const char* szCounterName );
void rhoFlushProfilerCounter(const char* file, int line, const char* szCounterName, const char* msg );

#define PROF_START(name) rhoStartProfilerCounter(__FILE__, __LINE__, name )
#define PROF_STOP(name)  rhoStopProfilerCounter(__FILE__, __LINE__, name )
#define PROF_CALL(exp) PROF_START(#exp); exp; PROF_STOP(#exp);

//Global accumulative counters
#define PROF_CREATE_COUNTER(name) rhoCreateProfilerCounter(__FILE__, __LINE__, name )
#define PROF_DESTROY_COUNTER(name) rhoDestroyProfilerCounter(__FILE__, __LINE__, name )
#define PROF_FLUSH_COUNTER(name,msg) rhoFlushProfilerCounter(__FILE__, __LINE__, name, msg )

#else

#define PROF_START(name) 
#define PROF_STOP(name)  
#define PROF_CALL(exp) exp;

//Global accumulative counters
#define PROF_CREATE_COUNTER(name) 
#define PROF_DESTROY_COUNTER(name)
#define PROF_FLUSH_COUNTER(name,msg)

#endif 

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOPROFILER_H_
