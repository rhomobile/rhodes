/***************************************************************
  wince.c

  author : uema2
  date   : Nov 30, 2002

  You can freely use, copy, modify, and redistribute
  the whole contents.
***************************************************************/

#include <windows.h>
#include <tchar.h>
#include "wp8.h"
#include "ruby\config.h"
#include <errno.h>
#include <process.h>
#include "missing_wp8.h"

#include "../../common/RhoDefs.h"

/* global for GetCommandLineA */
char *_commandLine;

extern char _currentdir[];

/* ---------------  files functions. ------------------- */

RHO_GLOBAL DWORD SetFilePointer(_In_ HANDLE hFile,
								_In_ LONG lDistanceToMove,
								_Inout_opt_ PLONG lpDistanceToMoveHigh,
								_In_ DWORD dwMoveMethod)
{
	return 0;
}

RHO_GLOBAL DWORD GetFileType(_In_ HANDLE hFile)
{
	return 0;
}

RHO_GLOBAL BOOL MoveFileW(_In_ LPCWSTR lpExistingFileName, _In_ LPCWSTR lpNewFileName)
{
	return FALSE;
}


BOOL LockFile(HANDLE hFile,
	DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh,
	DWORD nNumberOfBytesToLockLow, DWORD nNumberOfBytesToLockHigh)
{
	return FALSE;
}

BOOL UnlockFile( HFILE hFile,
	DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh,
	DWORD nNumberOfBytesToUnlockLow, DWORD nNumberOfBytesToUnlockHigh)
{
	return FALSE;
}

RHO_GLOBAL HANDLE CreateFile(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile)
{
	return INVALID_HANDLE_VALUE;
}

RHO_GLOBAL HANDLE CreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile)
{
	return INVALID_HANDLE_VALUE;
}

RHO_GLOBAL BOOL GetFileInformationByHandle(_In_ HANDLE hFile, _Out_ LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
{
	return FALSE;
}

/* --------------- EnvironmentVariable functions. ----------------- */

DWORD GetEnvironmentVariable(LPCSTR name, LPSTR value, DWORD size)
{
    return 0;
#if 0
	/* use registry instead of "environment valuable". */
	HKEY	hk;
	LONG	lret;
	LPBYTE	lpData;
	DWORD	dwType=REG_SZ, cbData;
	TCHAR   buf[MAX_PATH]={0};
	LPWSTR  wname;
	LPSTR   avalue;

	lret = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("Software\\ruby_mswince"),
				0, KEY_QUERY_VALUE, &hk );

	if ( lret != ERROR_SUCCESS )
	{
		strcpy( value, "" );
		return 0;
	}

	lpData = (LPBYTE)buf;
	cbData = MAX_PATH*sizeof(*buf);
	wname  = wce_mbtowc( name );

	lret = RegQueryValueEx( hk, wname,
		NULL, &dwType, lpData, &cbData );
	RegCloseKey( hk );

	if ( lret != ERROR_SUCCESS )
	{
		strcpy( value, "" );
		free( wname );
		return 0;
	}

	avalue = wce_wctomb( (LPCTSTR)lpData );
	strcpy( value, avalue );
	free( avalue );
	free( wname );

	return strlen(value);
#endif //0
}

BOOL SetEnvironmentVariable(LPCSTR name, LPCSTR value)
{
    return FALSE;
#if 0
	/* use registry instead of "environment valuable". */
	HKEY	hk;
	LONG	lret;
	LPBYTE	lpData;
	DWORD	ret, dwType=REG_SZ, cbData;
	LPWSTR  wname, wvalue;

	lret = RegCreateKeyEx( HKEY_LOCAL_MACHINE,
			_T("Software\\ruby_mswince"),
			0, _T(""), 0,
			0, NULL, &hk, &ret );
	if( lret != ERROR_SUCCESS )
		return FALSE;

	wname  = wce_mbtowc(name);
	wvalue = wce_mbtowc(value);

	lpData = (LPBYTE)wvalue;
	cbData = (wcslen(wvalue) + 1) * sizeof(*wvalue);
	lret = RegSetValueEx( hk, wname,
		0, dwType, lpData, cbData );
	RegCloseKey( hk );
	free(wname);
	free(wvalue);
	return lret == ERROR_SUCCESS;
#endif //0
}

LPVOID GetEnvironmentStrings(VOID)
{
	return NULL;
}

BOOL FreeEnvironmentStrings(LPSTR lpszEnvironmentBlock)
{
	return FALSE;
}

/* DuplicateHandle, LockFile, etc... */
/* I have no idea...  */
BOOL GenerateConsoleCtrlEvent(DWORD dwCtrlEvent, DWORD dwProcessGroupID)
{
	return 0;
}

/* --------------------- process stuff fucntiuons ----------------------- */

RHO_GLOBAL BOOL GetProcessTimes(_In_ HANDLE hProcess,
								_Out_ LPFILETIME lpCreationTime,
								_Out_ LPFILETIME lpExitTime,
								_Out_ LPFILETIME lpKernelTime,
								_Out_ LPFILETIME lpUserTime)
{
	return FALSE;
}

RHO_GLOBAL BOOL GetVersionEx(OSVERSIONINFOA *v)
{
	return FALSE;
}

BOOL CreateProcess(LPCSTR appname, LPCSTR commandline,
	LPSECURITY_ATTRIBUTES att, LPSECURITY_ATTRIBUTES threadatt,
	BOOL bOpt, DWORD dwFlag, LPVOID lpEnv, LPSTR dir,
	LPSTARTUPINFO lpsi, LPPROCESS_INFORMATION lppi)
{
	return FALSE;
}

RHO_GLOBAL HANDLE OpenProcess(_In_ DWORD dwDesiredAccess,
  							  _In_ BOOL bInheritHandle,
						      _In_ DWORD dwProcessId)
{
	return INVALID_HANDLE_VALUE;
}

RHO_GLOBAL BOOL GetExitCodeProcess(_In_ HANDLE hProcess,  _Out_ LPDWORD lpExitCode)
{
	return FALSE;
}

RHO_GLOBAL BOOL TerminateProcess(_In_ HANDLE hProcess, _In_ UINT uExitCode)
{
	return FALSE;
}

RHO_GLOBAL DWORD GetModuleFileNameA(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPSTR lpFilename,
    _In_ DWORD nSize)
{
	return (HMODULE)0;
}

RHO_GLOBAL DWORD GetModuleFileName(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPSTR lpFilename,
    _In_ DWORD nSize)
{
	return (HMODULE)0;
}

RHO_GLOBAL HMODULE GetModuleHandle(_In_opt_ LPCWSTR lpModuleName)
{
	return (HMODULE)0;
}

/* ---------------- CharNext, CharPrev. ---------------------*/

LPSTR CharNext(LPCSTR a)
{
	return NULL;
}

RHO_GLOBAL LPCWSTR CharPrevW(LPCWSTR start, LPCWSTR ptr)
{
	return NULL;
}

/*------------------- LoadLibrary -----------------------*/

/* WinCE doesn't have "CreatePipe". */

RHO_GLOBAL HANDLE CreateNamedPipe(
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

RHO_GLOBAL BOOL PeekNamedPipe(
    _In_ HANDLE hNamedPipe,
    _Out_writes_bytes_to_opt_(nBufferSize, *lpBytesRead) LPVOID lpBuffer,
    _In_ DWORD nBufferSize,
    _Out_opt_ LPDWORD lpBytesRead,
    _Out_opt_ LPDWORD lpTotalBytesAvail,
    _Out_opt_ LPDWORD lpBytesLeftThisMessage)
{
	return FALSE;
}

RHO_GLOBAL BOOL PeekConsoleInput(
    _In_ HANDLE hConsoleInput,
    _Out_writes_(nLength) PINPUT_RECORD lpBuffer,
    _In_ DWORD nLength,
    _Out_ LPDWORD lpNumberOfEventsRead)
{
	return FALSE;
}

RHO_GLOBAL BOOL ReadConsoleInput(
	_In_ HANDLE hConsoleInput,
    _Out_writes_to_(nLength, *lpNumberOfEventsRead) PINPUT_RECORD lpBuffer,
    _In_ DWORD nLength,
    _Out_ _Deref_out_range_(<=, nLength) LPDWORD lpNumberOfEventsRead)
{
	return FALSE;
}

/* need in ruby/io.c. */
int ReadDataPending()
{
	return 0;
}

/*---------------- sync functions. ---------------------------- */

RHO_GLOBAL BOOL GetExitCodeThread(_In_ HANDLE hThread, _Out_ LPDWORD lpExitCode)
{
	return FALSE;
}

RHO_GLOBAL BOOL TerminateThread(_In_ HANDLE hThread, _In_ DWORD dwExitCode)
{
	return FALSE;
}

RHO_GLOBAL HANDLE CreateThread(_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
								_In_ SIZE_T dwStackSize,
								_In_ LPTHREAD_START_ROUTINE lpStartAddress,
								_In_opt_ __drv_aliasesMem LPVOID lpParameter,
								_In_ DWORD dwCreationFlags,
								_Out_opt_ LPDWORD lpThreadId)
{
	return INVALID_HANDLE_VALUE;
}

RHO_GLOBAL VOID Sleep(_In_ DWORD dwMilliseconds)
{	
}

RHO_GLOBAL UINT GetConsoleCP(VOID)
{
	return 0;
}

RHO_GLOBAL DWORD WaitForSingleObject(_In_ HANDLE hHandle, _In_ DWORD dwMilliseconds)
{
	return 0;
}

RHO_GLOBAL DWORD TlsAlloc(VOID)
{
	return 0;
}

RHO_GLOBAL LPVOID TlsGetValue(_In_ DWORD dwTlsIndex)
{
	return 0;
}


RHO_GLOBAL BOOL TlsSetValue(_In_ DWORD dwTlsIndex, _In_opt_ LPVOID lpTlsValue)
{
	return FALSE;
}

RHO_GLOBAL HANDLE CreateEvent(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_ BOOL bManualReset,
    _In_ BOOL bInitialState,
    _In_opt_ LPCSTR lpName)
{
	return INVALID_HANDLE_VALUE;
}

RHO_GLOBAL HANDLE CreateEventW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_ BOOL bManualReset,
    _In_ BOOL bInitialState,
    _In_opt_ LPCWSTR lpName)
{
	return INVALID_HANDLE_VALUE;
}

RHO_GLOBAL VOID InitializeCriticalSection(_Out_ LPCRITICAL_SECTION lpCriticalSection)
{
	InitializeCriticalSectionEx(lpCriticalSection, 0, 0);
}

RHO_GLOBAL DWORD WaitForMultipleObjects( 
    _In_ DWORD nCount,
    _In_reads_(nCount) CONST HANDLE *lpHandles,
    _In_ BOOL bWaitAll,
    _In_ DWORD dwMilliseconds)
{
	return 0;
}

RHO_GLOBAL DWORD  ResumeThread(_In_ HANDLE hThread)
{
	return 0;
}

RHO_GLOBAL BOOL SetThreadPriority(_In_ HANDLE hThread, _In_ int nPriority)
{
	return FALSE;
}

/* --------------  file attributes functions. ------------------- */

RHO_GLOBAL DWORD GetFileAttributesA(_In_ LPCSTR lpFileName)
{
	return 0;
}

RHO_GLOBAL DWORD GetFileAttributesW(_In_ LPCWSTR lpFileName)
{
	return 0;
}

RHO_GLOBAL DWORD GetFileSize(_In_ HANDLE hFile, _Out_opt_ LPDWORD lpFileSizeHigh)
{
	
	return 0;
}

RHO_GLOBAL BOOL SetFileTime(_In_ HANDLE hFile,
							_In_opt_ CONST FILETIME * lpCreationTime,
							_In_opt_ CONST FILETIME * lpLastAccessTime,
							_In_opt_ CONST FILETIME * lpLastWriteTime)
{
	return FALSE;
}

/* --------------  file find functions. ------------------- */

RHO_GLOBAL HANDLE FindFirstFile(LPCWSTR path, WIN32_FIND_DATAA *data)
{
	return INVALID_HANDLE_VALUE;
}

RHO_GLOBAL HANDLE FindFirstFileW(_In_ LPCWSTR lpFileName, _Out_ LPWIN32_FIND_DATAW lpFindFileData)
{
	return INVALID_HANDLE_VALUE;
}


/* unsorted */
RHO_GLOBAL DWORD GetLogicalDrives(VOID)
{
	return 0;
}

RHO_GLOBAL void LocalFileTimeToFileTime(FILETIME *lft, FILETIME *ft)
{ 
}

RHO_GLOBAL BOOL GetOverlappedResult(_In_ HANDLE hFile,
									_In_ LPOVERLAPPED lpOverlapped,
									_Out_ LPDWORD lpNumberOfBytesTransferred,
									_In_ BOOL bWait)
{
	return FALSE;
}

RHO_GLOBAL HANDLE GetStdHandle(_In_ DWORD nStdHandle)
{
	return INVALID_HANDLE_VALUE;
}

RHO_GLOBAL BOOL GetConsoleMode(_In_ HANDLE hConsoleHandle, _Out_ LPDWORD lpMode)
{
	return FALSE;
}

RHO_GLOBAL BOOL VirtualFree(_In_ LPVOID lpAddress, 
							_In_ SIZE_T dwSize,  
							_In_ DWORD dwFreeType)
{
	return FALSE;
}

RHO_GLOBAL BOOL GetHandleInformation(_In_ HANDLE hObject, 
									 _Out_ LPDWORD lpdwFlags)
{
	return FALSE;
}

RHO_GLOBAL int lstrlenW(LPCWSTR lpString)
{
	return 0;
}

RHO_GLOBAL LPWSTR lstrcpyW(LPWSTR lpString1, LPCWSTR lpString2)
{
	return NULL;
}

RHO_GLOBAL LPWSTR lstrcatW(LPWSTR lpString1, LPCWSTR lpString2)
{
	return NULL;
}

RHO_GLOBAL int wsprintfW(_Out_ LPWSTR lpOut, _In_ _Printf_format_string_ LPCWSTR lpFormat, ...)
{
	return 0;
}

RHO_GLOBAL int vsnprintf(char * _DstBuf, size_t _MaxCount, const char * _Format, va_list _ArgList)
{
	return 0;
}

RHO_GLOBAL wchar_t* wce_mbtowc(const char* a)
{
	return NULL;
}