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
/*
DWORD
WINAPI
SetFilePointer(
    _In_ HANDLE hFile,
    _In_ LONG lDistanceToMove,
    _Inout_opt_ PLONG lpDistanceToMoveHigh,
    _In_ DWORD dwMoveMethod
    )
{
	return 0;
}

DWORD
WINAPI
GetFileType(
    _In_ HANDLE hFile
    )
{
	return 0;
}

BOOL
WINAPI
MoveFileA(
    _In_ LPCSTR lpExistingFileName,
    _In_ LPCSTR lpNewFileName
    )
{
	return FALSE;
}
*/
BOOL
WINAPI
MoveFileW(
    _In_ LPCWSTR lpExistingFileName,
    _In_ LPCWSTR lpNewFileName
    )
{
	return FALSE;
}

/*
BOOL LockFile(HANDLE hFile,
	DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh,
	DWORD nNumberOfBytesToLockLow, DWORD nNumberOfBytesToLockHigh)
{
	return FALSE;
}


BOOL WINAPI UnlockFile( HANDLE hFile,
	DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh,
	DWORD nNumberOfBytesToUnlockLow, DWORD nNumberOfBytesToUnlockHigh)
{
	return FALSE;
}

HANDLE WINAPI CreateFileA(
    _In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile)
{
	return INVALID_HANDLE_VALUE;
}

HANDLE WINAPI CreateFileW(
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

BOOL WINAPI GetFileInformationByHandle(_In_ HANDLE hFile, _Out_ LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
{
	return FALSE;
}
*/
/* --------------- EnvironmentVariable functions. ----------------- */
/*
DWORD GetEnvironmentVariable(LPCSTR name, LPSTR value, DWORD size)
{
    return 0;
#if 0
	// use registry instead of "environment valuable".
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
	// use registry instead of "environment valuable".
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
*/
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

/*BOOL
WINAPI
GetVersionExW(
    _Inout_ LPOSVERSIONINFOW lpVersionInformation
    )
{
	return FALSE;
}*/

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

/*
HANDLE WINAPI OpenProcess(_In_ DWORD dwDesiredAccess,
  							  _In_ BOOL bInheritHandle,
						      _In_ DWORD dwProcessId)
{
	return INVALID_HANDLE_VALUE;
}
*/
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
GetModuleFileNameA(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPSTR lpFilename,
    _In_ DWORD nSize
    )
{
	return 0;
}
/*
DWORD
WINAPI
GetModuleFileNameW(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPWSTR lpFilename,
    _In_ DWORD nSize
    )
{
	return 0;
}
*/

/*HMODULE WINAPI GetModuleHandleA(_In_opt_ LPCSTR lpModuleName)
{
	return (HMODULE)0;
}

HMODULE WINAPI GetModuleHandleW(_In_opt_ LPCWSTR lpModuleName)
{
	return (HMODULE)0;
}
*/

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

LPSTR WINAPI CharNextA(LPCSTR a)
{
	CHAR* b = (CHAR*)(a);
	b = b + 1; //HOT FIX - need to remove const_cast statement
	return b;
}

LPWSTR WINAPI CharNextW(LPCWSTR a)
{
	return NULL;
}

LPSTR
WINAPI
CharPrevA(
    _In_ LPCSTR lpszStart,
    _In_ LPCSTR lpszCurrent)
{
	return NULL;
}

LPWSTR
WINAPI
CharPrevW(
    _In_ LPCWSTR lpszStart,
    _In_ LPCWSTR lpszCurrent)
{
	return NULL;
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
/*
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

// need in ruby/io.c.
int ReadDataPending()
{
	return 0;
}

//---------------- sync functions. ----------------------------
RHO_GLOBAL BOOL TerminateThread(_In_ HANDLE hThread, _In_ DWORD dwExitCode)
{
	return FALSE;
}

HANDLE
WINAPI
CreateThread(
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ SIZE_T dwStackSize,
    _In_ LPTHREAD_START_ROUTINE lpStartAddress,
    _In_opt_ __drv_aliasesMem LPVOID lpParameter,
    _In_ DWORD dwCreationFlags,
    _Out_opt_ LPDWORD lpThreadId
    )
{
	return INVALID_HANDLE_VALUE;
}*/
/*
VOID
WINAPI
Sleep(
    _In_ DWORD dwMilliseconds
    )
{	
}*/

/*
RHO_GLOBAL UINT GetConsoleCP(VOID)
{
	return 0;
}
*/




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

/*
RHO_GLOBAL BOOL SetThreadPriority(_In_ HANDLE hThread, _In_ int nPriority)
{
	return FALSE;
}
*/
// --------------  file attributes functions. ------------------- 
/*
DWORD WINAPI GetFileAttributesA(
    _In_ LPCSTR lpFileName
    )
{
	return 0;
}

DWORD WINAPI GetFileAttributesW(
    _In_ LPCWSTR lpFileName
    )
{
	return 0;
}

DWORD WINAPI GetFileSize(_In_ HANDLE hFile, _Out_opt_ LPDWORD lpFileSizeHigh)
{
	
	return 0;
}


BOOL WINAPI SetFileTime(_In_ HANDLE hFile,
							_In_opt_ CONST FILETIME * lpCreationTime,
							_In_opt_ CONST FILETIME * lpLastAccessTime,
							_In_opt_ CONST FILETIME * lpLastWriteTime)
{
	return FALSE;
}
*/
// --------------  file find functions. ------------------- 
/*
DWORD WINAPI GetLogicalDrives( VOID )
{
	return 0;
}


BOOL WINAPI LocalFileTimeToFileTime(
    _In_ CONST FILETIME * lpLocalFileTime,
    _Out_ LPFILETIME lpFileTime
    )
{ 
	return FALSE;
}
*/
/*
RHO_GLOBAL BOOL GetOverlappedResult(_In_ HANDLE hFile,
									_In_ LPOVERLAPPED lpOverlapped,
									_Out_ LPDWORD lpNumberOfBytesTransferred,
									_In_ BOOL bWait)
{
	return FALSE;
}
*/

HANDLE WINAPI GetStdHandle(_In_ DWORD nStdHandle)
{
	return INVALID_HANDLE_VALUE;
}
/*
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
*/

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
	return 0;
}

int
WINAPI
lstrlenW(
    _In_ LPCWSTR lpString
    )
{
	return 0;
}

LPSTR
WINAPI
lstrcpyA(
    _Out_writes_(_String_length_(lpString2) + 1) LPSTR lpString1, // deprecated: annotation is as good as it gets
    _In_  LPCSTR lpString2
    )
{
	return NULL;
}

LPWSTR
WINAPI
lstrcpyW(
    _Out_writes_(_String_length_(lpString2) + 1) LPWSTR lpString1, // deprecated: annotation is as good as it gets
    _In_  LPCWSTR lpString2
    )
{
	return NULL;
}

LPSTR
WINAPI
lstrcatA(
    _Inout_updates_z_(_String_length_(lpString1) + _String_length_(lpString2) + 1) LPSTR lpString1, // deprecated: annotation is as good as it gets
    _In_    LPCSTR lpString2
    )
{
	return NULL;
}

LPWSTR
WINAPI
lstrcatW(
    _Inout_updates_z_(_String_length_(lpString1) + _String_length_(lpString2) + 1) LPWSTR lpString1, // deprecated: annotation is as good as it gets
    _In_    LPCWSTR lpString2
    )
{
	return NULL;
}

/*
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
*/

int wsprintfA( LPSTR p1,  LPCSTR p2, ...)
{
	return 0;
}

int wsprintfW( LPWSTR p1,  LPCWSTR p2,  ...)
{
	return 0;
}

//File access
/*
#include "common/RhoMutexLock.h"
wchar_t* wce_mbtowc(const char* a);
char* wce_wctomb(const wchar_t* w);
#define map_errno rb_w32_map_errno

RHO_INIT_LOCK(FileHandlers);
#define  g_nMaxFileHandlers  256
static HANDLE g_arFileHandlers[g_nMaxFileHandlers];
int get_NewFileNumber()
{
    int i = 0;
    int nRes = -1;
    RHO_LOCK(FileHandlers);
    for( i = 0; i < g_nMaxFileHandlers; i++ )
    {
        if ( g_arFileHandlers[i] == 0 )
        {
            nRes = i;
            break;
        }
    }
    RHO_UNLOCK(FileHandlers);
    return nRes+1;
}

void set_FileNumber(int fNumber, HANDLE osfh)
{
    RHO_LOCK(FileHandlers);
    g_arFileHandlers[fNumber-1] = osfh;
    RHO_UNLOCK(FileHandlers);
}

HANDLE get_OSHandleByFileNumber(int fNumber)
{
    HANDLE res;
    RHO_LOCK(FileHandlers);
    res = g_arFileHandlers[fNumber-1];
    RHO_UNLOCK(FileHandlers);
    return res;
}*/

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
/*
int _open(const char *path, int oflag, va_list arg)
{
    wchar_t *wfile = wce_mbtowc(path);
    int nRet = _wopen(wfile, oflag, arg);
    free(wfile);

    return nRet;
}

int close(int fd)
{
    HANDLE fHandle = get_OSHandleByFileNumber(fd);
    set_FileNumber( fd, 0);
	CloseHandle( fHandle );
	return 0;
}
*/
#undef fclose
int
rb_w32_fclose(FILE *fp)
{
    return fclose(fp);
}

/*
int _read(int fd, void *buffer, int length)
{
	DWORD dw;
    HANDLE fHandle = get_OSHandleByFileNumber(fd);

	if ( ReadFile( fHandle, buffer, length, &dw, NULL ) == FALSE )
    {
        errno = map_errno(GetLastError());
        return -1;
    }

	return (int)dw;
}*/

size_t
rb_w32_read(int fd, void *buf, size_t size)
{
    if (rb_w32_is_socket(fd))
    	return rb_w32_recv(fd, (char*)buf, size, 0);

    return _read(fd,buf,size);
}
/*
int _write(int fd, const void *buffer, unsigned count)
{
    DWORD dw;
    if ( fd < 0 ) {
        //char* buf = (char*) malloc(count+1);
        //memcpy(buf,buffer,count);
        //buf[count] = 0;
        //printf("%s",buf);
        //free(buf);
        //dw = count;
        //TBD: fix output of the long strings
        dw = fwrite( buffer, 1, count, stdout);
    } else {
        HANDLE fHandle = get_OSHandleByFileNumber(fd);
        if ( WriteFile( fHandle, buffer, count, &dw, NULL ) == FALSE )
        {
            errno = map_errno(GetLastError());
            return -1;
        }

    }
    return (int)dw;
}*/

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

BOOL WINAPI TerminateThreadWP8(_In_ HANDLE hThread, _In_ DWORD dwExitCode)
{
	return FALSE;
}

BOOL WINAPI GetExitCodeThreadWP8(
  _In_   HANDLE hThread,
  _Out_  LPDWORD lpExitCode
)
{
	lpExitCode = 0;
	return TRUE;
}
