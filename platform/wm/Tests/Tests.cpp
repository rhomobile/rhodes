void runAllLogTests();
extern "C" void runAllProfilingTests();
extern "C" void InitRhoLog(const char* path);

int wmain(int argc, wchar_t* argv[])
{
    InitRhoLog("");

    runAllLogTests();


//	runAllProfilingTests();
    return 0;
}

#if defined( __SYMBIAN32__ ) || defined(_WIN32_WCE)
extern "C" char *getenv(const char *env)
{
	return 0;
}
#endif