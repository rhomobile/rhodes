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
