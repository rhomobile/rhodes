#ifndef _RHOPROFILER_H_
#define _RHOPROFILER_H_

#include "common/RhoPort.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

void rhoStartProfilerCounter(const char* file, int line, const char* szCounterName );
void rhoStopProfilerCounter(const char* file, int line, const char* szCounterName );
void rhoCreateProfilerCounter(const char* file, int line, const char* szCounterName );
void rhoDestroyProfilerCounter(const char* file, int line, const char* szCounterName );
void rhoFlushProfilerCounter(const char* file, int line, const char* szCounterName, const char* msg );

#define PROF_START(name) rhoStartProfilerCounter(__FILE__, __LINE__, #name )
#define PROF_STOP(name)  rhoStopProfilerCounter(__FILE__, __LINE__, #name )
#define PROF_CALL(exp) PROF_START(#exp); exp; PROF_STOP(#exp);

//Global accumulative counters
#define PROF_CREATE_COUNTER(name) rhoCreateProfilerCounter(__FILE__, __LINE__, #name )
#define PROF_DESTROY_COUNTER(name) rhoDestroyProfilerCounter(__FILE__, __LINE__, #name )
#define PROF_FLUSH_COUNTER(name,msg) rhoFlushProfilerCounter(__FILE__, __LINE__, #name, msg )

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOPROFILER_H_