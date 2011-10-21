/***************************************************************
  process.c
***************************************************************/

#include <windows.h>
#include "process.h"

int _getpid(void)
{
	return (int)GetCurrentProcessId();
}

int rb_w32_getpid(void)
{
	return (int)GetCurrentProcessId();
}

int _getppid(void)
{
	return (int)GetCurrentProcessId();
}

int rb_w32_getppid(void)
{
	return (int)GetCurrentProcessId();
}

/* I wonder _exec and _swawn should be replaced with CreateProcess... */
int _execl(const char *cmdname, const char *arg0,
		   va_list va_args)
{
	return 0;
}

int execv(const char *path, char *const argv[])
{
	return 0;
}

void abort(void)
{
    exit(-1);
}

int _cwait( int *termstat, int procHandle, int action )
{
	return 0;
}

intptr_t _spawnle(int mode,
	const char *cmdname, const char *arg0,
	va_list va_argn)
{
	return 0;
}

intptr_t _spawnvpe(int mode,
	const char *cmdname, const char *const *argv,
	const char *const *envp)
{
	return 0;
}

