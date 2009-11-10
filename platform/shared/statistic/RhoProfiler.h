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
