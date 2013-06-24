/***************************************************************
  wince.c

  author : uema2
  date   : Nov 30, 2002

  You can freely use, copy, modify, and redistribute
  the whole contents.
***************************************************************/

#include <windows.h>
#include <tchar.h>

#include "../../common/RhoDefs.h"

#include "ruby\config.h"
#include "ruby/defines.h"

#include <errno.h>
#include <process.h>

/* global for GetCommandLineA */
char *_commandLine;

extern char _currentdir[];

/* ---------------  files functions. ------------------- */
BOOL
WINAPI
MoveFileW(
    _In_ LPCWSTR lpExistingFileName,
    _In_ LPCWSTR lpNewFileName
    )
{
	return FALSE;
}

/* --------------- EnvironmentVariable functions. ----------------- */
LPCH WINAPI GetEnvironmentStrings(VOID)
{
	return NULL;
}

BOOL WINAPI FreeEnvironmentStringsA(LPSTR lpszEnvironmentBlock)
{
	return FALSE;
}

BOOL WINAPI FreeEnvironmentStringsW(LPWSTR lpszEnvironmentBlock)
{
	return FALSE;
}

/* DuplicateHandle, LockFile, etc... */
/* I have no idea...  */
BOOL WINAPI GenerateConsoleCtrlEvent(
    _In_ DWORD dwCtrlEvent,
    _In_ DWORD dwProcessGroupId)
{
	return 0;
}

// --------------------- process stuff fucntiuons -----------------------

BOOL WINAPI GetProcessTimes(_In_ HANDLE hProcess,
								_Out_ LPFILETIME lpCreationTime,
								_Out_ LPFILETIME lpExitTime,
								_Out_ LPFILETIME lpKernelTime,
								_Out_ LPFILETIME lpUserTime)
{
	return FALSE;
}

BOOL
WINAPI
GetVersionExA(
    _Inout_ LPOSVERSIONINFOA lpVersionInformation
    )
{
	return FALSE;
}

BOOL
WINAPI
CreateProcessA(
    _In_opt_ LPCSTR lpApplicationName,
    _Inout_opt_ LPSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOA lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
    )
{
	return FALSE;
}

BOOL
WINAPI
CreateProcessW(
    _In_opt_ LPCWSTR lpApplicationName,
    _Inout_opt_ LPWSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCWSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOW lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
    )
{
	return FALSE;
}

BOOL
WINAPI
GetExitCodeProcess(
    _In_ HANDLE hProcess,
    _Out_ LPDWORD lpExitCode
    )
{
	return FALSE;
}

BOOL WINAPI TerminateProcess(
    _In_ HANDLE hProcess,
    _In_ UINT uExitCode
    )
{
	return FALSE;
}

DWORD
WINAPI
GetFileAttributesA(
    _In_ LPCSTR lpFileName
    )
{
	WIN32_FILE_ATTRIBUTE_DATA fileInformation; 
	if(GetFileAttributesEx(lpFileName, GetFileExInfoStandard, &fileInformation))
		return fileInformation.dwFileAttributes;
	else
		return INVALID_FILE_ATTRIBUTES;
}

DWORD
WINAPI
GetModuleFileNameA(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPSTR lpFilename,
    _In_ DWORD nSize
    )
{
	return 0;
}

HMODULE
WINAPI
LoadLibraryA(
    _In_ LPCSTR lpLibFileName
    )
{
	return NULL;
}

HMODULE
WINAPI
LoadLibraryW(
    _In_ LPCWSTR lpLibFileName
    )
{
	return NULL;
}

/* ---------------- CharNext, CharPrev. ---------------------*/

LPSTR WINAPI CharNextA(LPCSTR szString)
{
	//CHAR* b = (CHAR*)(a);
	//b = b + 1; //HOT FIX - need to remove const_cast statement
	++szString;
    while ((*szString & 0xc0) == 0x80)
        ++szString;
    return (LPSTR)szString;	
}

LPWSTR WINAPI CharNextW(LPCWSTR a)
{
	return (LPWSTR)++a;
}
/*
LPSTR
WINAPI
CharPrevA(
    _In_ LPCSTR lpszStart,
    _In_ LPCSTR lpszCurrent)
{
	return NULL;
}*/

LPWSTR
WINAPI
CharPrevW(
    _In_ LPCWSTR lpszStart,
    _In_ LPCWSTR lpszCurrent)
{
	if ( lpszStart == lpszCurrent )
		return (LPWSTR)lpszStart;

	return (LPWSTR)--lpszCurrent;
}

/*------------------- LoadLibrary -----------------------*/

/* WinCE doesn't have "CreatePipe". */

HANDLE WINAPI CreateNamedPipeA(
    _In_     LPCSTR lpName,
    _In_     DWORD dwOpenMode,
    _In_     DWORD dwPipeMode,
    _In_     DWORD nMaxInstances,
    _In_     DWORD nOutBufferSize,
    _In_     DWORD nInBufferSize,
    _In_     DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	return INVALID_HANDLE_VALUE;
}

HANDLE WINAPI CreateNamedPipeW(
    _In_     LPCWSTR lpName,
    _In_     DWORD dwOpenMode,
    _In_     DWORD dwPipeMode,
    _In_     DWORD nMaxInstances,
    _In_     DWORD nOutBufferSize,
    _In_     DWORD nInBufferSize,
    _In_     DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	return INVALID_HANDLE_VALUE;
}

VOID WINAPI InitializeCriticalSection(_Out_ LPCRITICAL_SECTION lpCriticalSection)
{
	InitializeCriticalSectionEx(lpCriticalSection, 0, 0);
}

DWORD WINAPI WaitForMultipleObjects(
    _In_ DWORD nCount,
    _In_reads_(nCount) CONST HANDLE *lpHandles,
    _In_ BOOL bWaitAll,
    _In_ DWORD dwMilliseconds
    )
{
	return WaitForMultipleObjectsEx(nCount, lpHandles, bWaitAll, dwMilliseconds, 0);;
}


HANDLE WINAPI GetStdHandle(_In_ DWORD nStdHandle)
{
	return INVALID_HANDLE_VALUE;
}

BOOL WINAPI GetHandleInformation(
    _In_ HANDLE hObject,
    _Out_ LPDWORD lpdwFlags
    )
{
	return FALSE;
}

int
WINAPI
lstrlenA(
    _In_ LPCSTR lpString
    )
{
	return strlen(lpString);
}

int
WINAPI
lstrlenW(
    _In_ LPCWSTR lpString
    )
{
	return wcslen(lpString);
}

LPSTR
WINAPI
lstrcpyA(
    _Out_writes_(_String_length_(lpString2) + 1) LPSTR lpString1, // deprecated: annotation is as good as it gets
    _In_  LPCSTR lpString2
    )
{
	return strcpy(lpString1, lpString2);
}

LPWSTR
WINAPI
lstrcpyW(
    _Out_writes_(_String_length_(lpString2) + 1) LPWSTR lpString1, // deprecated: annotation is as good as it gets
    _In_  LPCWSTR lpString2
    )
{
	return wcscpy(lpString1, lpString2);
}

LPSTR
WINAPI
lstrcatA(
    _Inout_updates_z_(_String_length_(lpString1) + _String_length_(lpString2) + 1) LPSTR lpString1, // deprecated: annotation is as good as it gets
    _In_    LPCSTR lpString2
    )
{
	return strcat(lpString1, lpString2);
}

LPWSTR
WINAPI
lstrcatW(
    _Inout_updates_z_(_String_length_(lpString1) + _String_length_(lpString2) + 1) LPWSTR lpString1, // deprecated: annotation is as good as it gets
    _In_    LPCWSTR lpString2
    )
{
	return wcscat(lpString1, lpString2);
}

int wsprintfA( LPSTR p1,  LPCSTR p2, ...)
{
	int nRes = 0;
    va_list ap;
    va_start(ap, p2);
    nRes = vsprintf(p1, p2, ap);
    va_end(ap);

	return nRes;
}

int wsprintfW( LPWSTR p1,  LPCWSTR p2,  ...)
{
	int nRes = 0;
    va_list ap;
    va_start(ap, p2);
    nRes = vswprintf(p1, p2, ap);
    va_end(ap);

	return nRes;
}


int rb_w32_open( const char *file, int mode, ... )
{
    int ret = 0;
    va_list ap;
    va_start(ap, mode);
    ret = _open(file, mode);
    va_end(ap);

    return ret;
}

int rb_w32_isatty(int fd)
{
    return 0;//_isatty(fd);
}

int rb_w32_pipe(int *phandles )//, unsigned int psize, int textmode)
{
	return -1;
}

#undef fclose
int
rb_w32_fclose(FILE *fp)
{
    return fclose(fp);
}

size_t
rb_w32_read(int fd, void *buf, size_t size)
{
    if (rb_w32_is_socket(fd))
    	return rb_w32_recv(fd, (char*)buf, size, 0);

    return _read(fd,buf,size);
}

size_t
rb_w32_write(int fd, const void *buf, size_t size)
{
    if (rb_w32_is_socket(fd))
    	return rb_w32_send(fd, (char*)buf, size, 0);

    return _write(fd,buf,size);
}

wchar_t* wce_mbtowc(const char* a)
{
	int length;
	wchar_t *wbuf;

	length = MultiByteToWideChar(CP_UTF8, 0, 
		a, -1, NULL, 0);
	wbuf = (wchar_t*)malloc( (length+1)*sizeof(wchar_t) );
	MultiByteToWideChar(CP_UTF8, 0,
		a, -1, wbuf, length);

	return wbuf;
}

/* wchar_t -> char */
char* wce_wctomb(const wchar_t* w)
{
	DWORD charlength;
	char* pChar;

	charlength = WideCharToMultiByte(CP_UTF8, 0, w,
					-1, NULL, 0, NULL, NULL);
	pChar = (char*)malloc(charlength+1);
	WideCharToMultiByte(CP_UTF8, 0, w,
		-1, pChar, charlength, NULL, NULL);

	return pChar;
}

HANDLE WINAPI CreateEventWP8(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_ BOOL bManualReset,
    _In_ BOOL bInitialState,
    _In_opt_ LPCWSTR lpName)
{
	DWORD flags = 0;
	
	if(bManualReset) 
		flags = CREATE_EVENT_MANUAL_RESET;
	
	if(bInitialState)
		flags |= CREATE_EVENT_INITIAL_SET;

	return CreateEventExW(lpEventAttributes, lpName, flags, EVENT_ALL_ACCESS);
}

HANDLE WINAPI CreateMutexWP8(
    _In_opt_  LPSECURITY_ATTRIBUTES lpMutexAttributes,
	_In_      BOOL bInitialOwner,
	_In_opt_  LPCWSTR lpName)
{
	DWORD flags = 0;
	
	if(bInitialOwner)
		flags |= CREATE_MUTEX_INITIAL_OWNER;

	return CreateMutexExW(lpMutexAttributes, lpName, flags, MUTEX_ALL_ACCESS);
}

DWORD
WINAPI
WaitForSingleObjectWP8(
    _In_ HANDLE hHandle,
    _In_ DWORD dwMilliseconds
    )
{
	return WaitForSingleObjectEx(hHandle, dwMilliseconds, 0);
}

BOOL WINAPI GetExitCodeThreadWP8(
  _In_   HANDLE hThread,
  _Out_  LPDWORD lpExitCode
)
{
	lpExitCode = 0;
	return TRUE;
}
