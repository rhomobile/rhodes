// Tests2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <gtest/gtest.h>

void runAllLogTests();
extern "C" void runAllProfilingTests();
extern "C" void InitRhoLog(const char* path);

int _tmain(int argc, _TCHAR* argv[])
{
    InitRhoLog("");

    //runAllLogTests();


  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

//	return 0;
}

int rho_testprintf(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int nres = rho_testvprintf(fmt, args);
    va_end(args);

    return nres;
}
int rho_testvprintf(const char * fmt, va_list args)
{
    CAtlStringA str;
    str.FormatV(fmt,args);
    OutputDebugStringA(str);
    return vfprintf(stdout,fmt, args);
}

#if defined( __SYMBIAN32__ ) || defined(_WIN32_WCE)
extern "C" char *getenv(const char *env)
{
	return 0;
}
#endif