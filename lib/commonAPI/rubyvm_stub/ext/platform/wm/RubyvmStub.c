#include <Windows.h>

extern char *envarea;

void exit_handler(void)
{
    if (envarea) 
    {
        FreeEnvironmentStrings(envarea);
        envarea = NULL;
    }
}

void Init_RubyvmStub_extension()
{
    atexit(exit_handler);
}