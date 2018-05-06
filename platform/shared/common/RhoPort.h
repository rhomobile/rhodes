/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#ifndef _RHOPORT_H_
#define _RHOPORT_H_

#include "RhoDefs.h"

#ifdef OS_SYMBIAN
#define strnicmp strncasecmp
#define stricmp strcasecmp
#endif

#ifdef OS_WINDOWS_DESKTOP
#define strncasecmp _strnicmp
#endif

#ifdef __SYMBIAN32__
#include <arpa/inet.h>
#endif

#ifdef OS_UWP
#define HANDLE_FLAG_INHERIT 1
#endif

#if defined( WINDOWS_PLATFORM )

#include <windows.h>
#include <time.h>
#include <stdlib.h>
#ifdef _MSC_VER
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#endif

#if defined(OS_WINCE)
#ifdef __cplusplus
extern "C"{
#endif
time_t time(time_t *);
struct tm * __cdecl localtime(const time_t *);
size_t strftime(char *, size_t, const char *,
	const struct tm *);
struct tm * gmtime(const time_t *);
time_t mktime(struct tm *);

_CRTIMP extern char * tzname[2];

//extern int _daylight;
//#define daylight _daylight

extern long _timezone;
#ifdef __cplusplus
};
#endif
#endif

#if defined(OS_WINDOWS_DESKTOP)
#define _USE_MATH_DEFINES
#endif

#if defined(OS_WINCE) || defined(OS_WP8) || defined(OS_UWP)
#define M_PI 3.14159265358979323846
#define M_LN2 0.69314718055994530942
#endif

#include <math.h>

typedef int socklen_t;

#if defined(OS_WINCE)
#include "ruby/wince/sys/types.h"
#include "ruby/wince/sys/stat.h"
#ifdef __cplusplus
extern "C"
#endif //__cplusplus
char *strdup(const char * str);

#else //!defined(OS_WINCE)
#include <sys/stat.h>
#endif //!defined(OS_WINCE)

#define LOG_NEWLINE "\r\n"
#define LOG_NEWLINELEN 2

//typedef __int32 int32;
//typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

#define strcasecmp _stricmp

#ifdef _WIN32_WCE
#define snprintf _snprintf
#endif

#define FMTI64 "%I64d"
#define FMTU64 "%I64u"

#else // !defined( WINDOWS_PLATFORM)

#define FMTI64 "%lli"
#define FMTU64 "%llu"

#  if defined(OS_ANDROID) || defined(OS_LINUX)
// Needed for va_list on Android
#    include <stdarg.h>
#    include <sys/select.h>
#    include <stdio.h>
#    include <ctype.h>
#  else
#    include <wchar.h>
#  endif // OS_ANDROID

#if defined(OS_SYMBIAN)
#  include <sys/select.h>
#endif

#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <unistd.h>
#  include <errno.h>
#  if defined(OS_MACOSX) || defined(OS_LINUX)
#    include <sys/time.h>
#  endif
#  include <stdlib.h>
#  include <string.h>
#  include <pthread.h>
#  include <fcntl.h>
#  include "common/stat.h"

#undef ASSERT
#define ASSERT RHO_ASSERT

#define LOG_NEWLINE "\n"
#define LOG_NEWLINELEN 1

//typedef int32_t int32;
//typedef uint32_t uint32;
typedef long long int64;
typedef unsigned long long uint64;

#if defined(OS_SYMBIAN) || defined(OS_LINUX) || defined(OS_MACOSX)
#define M_PI 3.14159265358979323846
#endif

#endif 

#if defined( OS_WINCE )
#  define	vsnprintf	_vsnprintf
#  define	vswnprintf	_vsnwprintf
#elif defined( OS_ANDROID )
RHO_GLOBAL int vswnprintf(wchar_t *, size_t, const wchar_t *, void *);
#else
#  define	vswnprintf vswprintf
#endif //OS_WINCE

#if defined( OS_WP8 ) || defined( OS_UWP )
#  include <stdlib.h>
#  include <errno.h>

#define stati64    _stat64
#define stat64    _stat64

#ifdef __cplusplus
extern "C" {
#endif

HANDLE WINAPI CreateThreadWP8(_In_opt_ LPSECURITY_ATTRIBUTES unusedThreadAttributes, _In_ SIZE_T unusedStackSize, _In_ LPTHREAD_START_ROUTINE lpStartAddress, _In_opt_ LPVOID lpParameter, _In_ DWORD dwCreationFlags, _Out_opt_ LPDWORD unusedThreadId);
DWORD WINAPI ResumeThreadWP8(_In_ HANDLE hThread);
BOOL WINAPI SetThreadPriorityWP8(_In_ HANDLE hThread, _In_ int nPriority);
DWORD WINAPI TerminateThreadWP8(HANDLE hThread, DWORD dwExitCode);

DWORD WINAPI TlsAllocWP8();
BOOL WINAPI TlsFreeWP8(_In_ DWORD dwTlsIndex);
LPVOID WINAPI TlsGetValueWP8(_In_ DWORD dwTlsIndex);
BOOL WINAPI TlsSetValueWP8(_In_ DWORD dwTlsIndex, _In_opt_ LPVOID lpTlsValue);

void WINAPI TlsShutdownWP8();

VOID WINAPI SleepWP8(_In_ DWORD dwMilliseconds);

int
wsprintfA(
    _Out_ LPSTR,
    _In_ _Printf_format_string_ LPCSTR,
    ...);

int
wsprintfW(
    _Out_ LPWSTR,
    _In_ _Printf_format_string_ LPCWSTR,
    ...);

#ifdef UNICODE
#define wsprintf  wsprintfW
#else
#define wsprintf  wsprintfA
#endif // !UNICODE

# define AreFileApisANSI() 1

#define FindFirstFileA(p1, p2) FindFirstFileExA(p1, FindExInfoStandard,  p2, FindExSearchNameMatch, NULL, FIND_FIRST_EX_CASE_SENSITIVE)
#define FindFirstFileW(p1, p2) FindFirstFileExW(p1, FindExInfoStandard,  p2, FindExSearchNameMatch, NULL, FIND_FIRST_EX_CASE_SENSITIVE)

#ifdef UNICODE
#define FindFirstFile FindFirstFileW
#else
#define FindFirstFile FindFirstFileA
#endif

#define STARTF_USESHOWWINDOW       0x00000001
#define STARTF_USESIZE             0x00000002
#define STARTF_USEPOSITION         0x00000004
#define STARTF_USECOUNTCHARS       0x00000008
#define STARTF_USEFILLATTRIBUTE    0x00000010
#define STARTF_RUNFULLSCREEN       0x00000020  // ignored for non-x86 platforms
#define STARTF_FORCEONFEEDBACK     0x00000040
#define STARTF_FORCEOFFFEEDBACK    0x00000080
#define STARTF_USESTDHANDLES       0x00000100


HANDLE
WINAPI
GetStdHandle(
    _In_ DWORD nStdHandle
    );

//#if defined(_UWP_LIB)
//#define RHOPORT_STRUCT_EXISTS
//#else
#define REDEFINE_WINAPI_OLD_FUNCTIONS
//#endif


#if !defined(_UWP_LIB)
LPCH
WINAPI
GetEnvironmentStrings(
    VOID
    );
#endif

#if !defined(_WP8_LIB) && !defined(_UWP_LIB)
_NullNull_terminated_
LPWCH
WINAPI
GetEnvironmentStringsW(
    VOID
    );
#endif

#ifdef UNICODE
#define GetEnvironmentStrings  GetEnvironmentStringsW
#else
#define GetEnvironmentStringsA  GetEnvironmentStrings
#endif // !UNICODE

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
BOOL
WINAPI
SetEnvironmentStringsW(
    _In_ _Pre_ _NullNull_terminated_ LPWCH NewEnvironment
    );
_NullNull_terminated_
LPWCH
WINAPI
GetEnvironmentStringsW(
	VOID
);
DWORD WINAPI GetEnvironmentVariableW(
	_In_opt_  LPCTSTR lpName,
	_Out_opt_ LPTSTR  lpBuffer,
	_In_      DWORD   nSize
);
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS



#ifdef UNICODE
#define SetEnvironmentStrings  SetEnvironmentStringsW
#endif

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS

typedef enum {
	AddrMode1616,
	AddrMode1632,
	AddrModeReal,
	AddrModeFlat
} ADDRESS_MODE;

BOOL
WINAPI
FreeEnvironmentStringsA(
    _In_ _Pre_ _NullNull_terminated_ LPCH penv
    );


BOOL
WINAPI
FreeEnvironmentStringsW(
    _In_ _Pre_ _NullNull_terminated_ LPWCH penv
    );

#ifdef UNICODE
#define FreeEnvironmentStrings  FreeEnvironmentStringsW
#else
#define FreeEnvironmentStrings  FreeEnvironmentStringsA
#endif // !UNICODE
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS

#if !defined(RHOPORT_STRUCT_EXISTS)
typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;
#else
typedef struct _PROCESS_INFORMATION PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;
#endif
#if !defined(RHOPORT_STRUCT_EXISTS)
typedef struct _STARTUPINFOA {
    DWORD   cb;
    LPSTR   lpReserved;
    LPSTR   lpDesktop;
    LPSTR   lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;
#else
typedef struct _STARTUPINFOA STARTUPINFOA, *LPSTARTUPINFOA;
#endif

#if !defined(RHOPORT_STRUCT_EXISTS)
typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;
#else
typedef struct _STARTUPINFOW STARTUPINFOW, *LPSTARTUPINFOW;
#endif


#ifdef UNICODE
typedef STARTUPINFOW STARTUPINFO;
typedef LPSTARTUPINFOW LPSTARTUPINFO;
#else
typedef STARTUPINFOA STARTUPINFO;
typedef LPSTARTUPINFOA LPSTARTUPINFO;
#endif // UNICODE

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
/*HANDLE
WINAPI
CreateFileA(
    _In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
    );


HANDLE
WINAPI
CreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
    );
*/
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS



#ifdef UNICODE
#define CreateFile  CreateFileW
#else
#define CreateFile  CreateFileA
#endif // !UNICODE

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
HMODULE
WINAPI
LoadLibraryA(
    _In_ LPCSTR lpLibFileName
    );

HMODULE
WINAPI
LoadLibraryW(
    _In_ LPCWSTR lpLibFileName
    );

#ifdef UNICODE
#define LoadLibrary  LoadLibraryW
#else
#define LoadLibrary  LoadLibraryA
#endif // !UNICODE

#endif // REDEFINE_WINAPI_OLD_FUNCTIONS

LPSTR
WINAPI
CharNextA(
    _In_ LPCSTR lpsz);

LPWSTR
WINAPI
CharNextW(
    _In_ LPCWSTR lpsz);
#ifdef UNICODE
#define CharNext  CharNextW
#else
#define CharNext  CharNextA
#endif // !UNICODE

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
/*
HMODULE
WINAPI
GetModuleHandleA(
    _In_opt_ LPCSTR lpModuleName
    );


HMODULE
WINAPI
GetModuleHandleW(
    _In_opt_ LPCWSTR lpModuleName
    );
*/
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS


#ifndef GetModuleHandle
#ifdef UNICODE
#define GetModuleHandle  GetModuleHandleW
#else
#define GetModuleHandle  GetModuleHandleA
#endif // !UNICODE
#endif

LPSTR
WINAPI
lstrcpyA(
    _Out_writes_(_String_length_(lpString2) + 1) LPSTR lpString1, // deprecated: annotation is as good as it gets
    _In_  LPCSTR lpString2
    );

LPWSTR
WINAPI
lstrcpyW(
    _Out_writes_(_String_length_(lpString2) + 1) LPWSTR lpString1, // deprecated: annotation is as good as it gets
    _In_  LPCWSTR lpString2
    );
#ifdef UNICODE
#define lstrcpy  lstrcpyW
#else
#define lstrcpy  lstrcpyA
#endif // !UNICODE

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
LPSTR
WINAPI
CharPrevA(
    _In_ LPCSTR lpszStart,
    _In_ LPCSTR lpszCurrent);

LPWSTR
WINAPI
CharPrevW(
    _In_ LPCWSTR lpszStart,
    _In_ LPCWSTR lpszCurrent);

#ifdef UNICODE
#define CharPrev  CharPrevW
#else
#define CharPrev  CharPrevA
#endif // !UNICODE

#endif // REDEFINE_WINAPI_OLD_FUNCTIONS

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
/*
HANDLE
WINAPI
OpenProcess(
    _In_ DWORD dwDesiredAccess,
    _In_ BOOL bInheritHandle,
    _In_ DWORD dwProcessId
    );

DWORD
WINAPI
SetFilePointer(
    _In_ HANDLE hFile,
    _In_ LONG lDistanceToMove,
    _Inout_opt_ PLONG lpDistanceToMoveHigh,
    _In_ DWORD dwMoveMethod
    );


DWORD
WINAPI
GetFileSize(
    _In_ HANDLE hFile,
    _Out_opt_ LPDWORD lpFileSizeHigh
    );
	*/
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS

#define CreateThread CreateThreadWP8

HANDLE
WINAPI
CreateEventWP8(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_ BOOL bManualReset,
    _In_ BOOL bInitialState,
    _In_opt_ LPCWSTR lpName);

#define CreateEventW CreateEventWP8
#define CreateEvent CreateEventW

HANDLE WINAPI CreateMutexWP8(
    _In_opt_  LPSECURITY_ATTRIBUTES lpMutexAttributes,
	_In_      BOOL bInitialOwner,
	_In_opt_  LPCWSTR lpName);

#define CreateMutex CreateMutexWP8


HANDLE
WINAPI
CreateNamedPipeW(
    _In_ LPCWSTR lpName,
    _In_ DWORD dwOpenMode,
    _In_ DWORD dwPipeMode,
    _In_ DWORD nMaxInstances,
    _In_ DWORD nOutBufferSize,
    _In_ DWORD nInBufferSize,
    _In_ DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );


HANDLE
WINAPI
CreateNamedPipeA(
    _In_     LPCSTR lpName,
    _In_     DWORD dwOpenMode,
    _In_     DWORD dwPipeMode,
    _In_     DWORD nMaxInstances,
    _In_     DWORD nOutBufferSize,
    _In_     DWORD nInBufferSize,
    _In_     DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );

#ifdef UNICODE
#define CreateNamedPipe  CreateNamedPipeW
#else
#define CreateNamedPipe  CreateNamedPipeA
#endif

#define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)

#if !defined(RHOPORT_STRUCT_EXISTS)
typedef struct _BY_HANDLE_FILE_INFORMATION {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD dwVolumeSerialNumber;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD nNumberOfLinks;
    DWORD nFileIndexHigh;
    DWORD nFileIndexLow;
} BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;
#else
typedef struct _BY_HANDLE_FILE_INFORMATION BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;
#endif

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS


#endif // REDEFINE_WINAPI_OLD_FUNCTIONS

BOOL
WINAPI
GetFileInformationByHandle(
    _In_ HANDLE hFile,
    _Out_ LPBY_HANDLE_FILE_INFORMATION lpFileInformation
    );

VOID
WINAPI
InitializeCriticalSection(
    _Out_ LPCRITICAL_SECTION lpCriticalSection
    );

BOOL
WINAPI
GetProcessTimes(
    _In_ HANDLE hProcess,
    _Out_ LPFILETIME lpCreationTime,
    _Out_ LPFILETIME lpExitTime,
    _Out_ LPFILETIME lpKernelTime,
    _Out_ LPFILETIME lpUserTime
    );

DWORD
WINAPI
TlsAlloc(
    VOID
    );

LPVOID
WINAPI
TlsGetValue(
    _In_ DWORD dwTlsIndex
    );


BOOL
WINAPI
TlsSetValue(
    _In_ DWORD dwTlsIndex,
    _In_opt_ LPVOID lpTlsValue
    );


BOOL
WINAPI
TlsFree(
    _In_ DWORD dwTlsIndex
    );

void WINAPI TlsShutdown();

#ifndef OS_UWP
DWORD
WINAPI
WaitForMultipleObjectsWP8(
    _In_ DWORD nCount,
    _In_reads_(nCount) CONST HANDLE *lpHandles,
    _In_ BOOL bWaitAll,
    _In_ DWORD dwMilliseconds
    );

#define WaitForMultipleObjects WaitForMultipleObjectsWP8
#else
DWORD
WINAPI
WaitForMultipleObjects(
	_In_ DWORD nCount,
	_In_reads_(nCount) CONST HANDLE *lpHandles,
	_In_ BOOL bWaitAll,
	_In_ DWORD dwMilliseconds
);
#endif
BOOL
WINAPI
GetHandleInformation(
    _In_ HANDLE hObject,
    _Out_ LPDWORD lpdwFlags
    );

#define ResumeThread ResumeThreadWP8
#define TlsAlloc TlsAllocWP8
#define TlsFree TlsFreeWP8
#define TlsGetValue TlsGetValueWP8
#define TlsSetValue TlsSetValueWP8
#define TlsShutdown TlsShutdownWP8

DWORD
WINAPI
WaitForSingleObjectWP8(
    _In_ HANDLE hHandle,
    _In_ DWORD dwMilliseconds
    );

#define WaitForSingleObject WaitForSingleObjectWP8

#define Sleep SleepWP8

VOID
WINAPI
ExitThread(
    _In_ DWORD dwExitCode
    );


#define TerminateThread TerminateThreadWP8

BOOL WINAPI GetExitCodeThreadWP8(
  _In_   HANDLE hThread,
  _Out_  LPDWORD lpExitCode
);

#define GetExitCodeThread GetExitCodeThreadWP8

#define SetThreadPriority SetThreadPriorityWP8

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
/*
DWORD
WINAPI
GetModuleFileNameA(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPSTR lpFilename,
    _In_ DWORD nSize
    );

DWORD
WINAPI
GetModuleFileNameW(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPWSTR lpFilename,
    _In_ DWORD nSize
    );
	*/
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS



#ifdef UNICODE
#define GetModuleFileName  GetModuleFileNameW
#else
#define GetModuleFileName  GetModuleFileNameA
#endif // !UNICODE

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
/*
BOOL
WINAPI
GetVersionExA(
    _Inout_ LPOSVERSIONINFOA lpVersionInformation
    );

BOOL
WINAPI
GetVersionExW(
    _Inout_ LPOSVERSIONINFOW lpVersionInformation
    );
*/
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS



#ifdef UNICODE
#define GetVersionEx  GetVersionExW
#else
#define GetVersionEx  GetVersionExA
#endif // !UNICODE

BOOL
WINAPI
GetExitCodeProcess(
    _In_ HANDLE hProcess,
    _Out_ LPDWORD lpExitCode
    );

int
WINAPI
lstrlenA(
    _In_ LPCSTR lpString
    );

int
WINAPI
lstrlenW(
    _In_ LPCWSTR lpString
    );

#ifdef UNICODE
#define lstrlen  lstrlenW
#else
#define lstrlen  lstrlenA
#endif // !UNICODE

LPSTR
WINAPI
lstrcatA(
    _Inout_updates_z_(_String_length_(lpString1) + _String_length_(lpString2) + 1) LPSTR lpString1, // deprecated: annotation is as good as it gets
    _In_    LPCSTR lpString2
    );

LPWSTR
WINAPI
lstrcatW(
    _Inout_updates_z_(_String_length_(lpString1) + _String_length_(lpString2) + 1) LPWSTR lpString1, // deprecated: annotation is as good as it gets
    _In_    LPCWSTR lpString2
    );
#ifdef UNICODE
#define lstrcat  lstrcatW
#else
#define lstrcat  lstrcatA
#endif // !UNICODE

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
    );

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
    );

#ifdef UNICODE
#define CreateProcess  CreateProcessW
#else
#define CreateProcess  CreateProcessA
#endif // !UNICODE

DWORD
WINAPI
GetLogicalDrives(
    VOID
    );

DWORD
WINAPI
GetFileType(
    _In_ HANDLE hFile
    );

BOOL
WINAPI
GenerateConsoleCtrlEvent(
    _In_ DWORD dwCtrlEvent,
    _In_ DWORD dwProcessGroupId);

BOOL
WINAPI
TerminateProcess(
    _In_ HANDLE hProcess,
    _In_ UINT uExitCode
    );

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
/*DWORD
WINAPI
GetFileAttributesA(
    _In_ LPCSTR lpFileName
    );

DWORD
WINAPI
GetFileAttributesW(
    _In_ LPCWSTR lpFileName
    );*/
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS




#ifdef UNICODE
#define GetFileAttributes  GetFileAttributesW
#else
#define GetFileAttributes  GetFileAttributesA
#endif // !UNICODE

BOOL
WINAPI
MoveFileA(
    _In_ LPCSTR lpExistingFileName,
    _In_ LPCSTR lpNewFileName
    );

BOOL
WINAPI
MoveFileW(
    _In_ LPCWSTR lpExistingFileName,
    _In_ LPCWSTR lpNewFileName
    );
#ifdef UNICODE
#define MoveFile  MoveFileW
#else
#define MoveFile  MoveFileA
#endif // !UNICODE

#ifdef REDEFINE_WINAPI_OLD_FUNCTIONS
/*
BOOL
WINAPI
UnlockFile(
    _In_ HANDLE hFile,
    _In_ DWORD dwFileOffsetLow,
    _In_ DWORD dwFileOffsetHigh,
    _In_ DWORD nNumberOfBytesToUnlockLow,
    _In_ DWORD nNumberOfBytesToUnlockHigh
    );

BOOL
WINAPI
LocalFileTimeToFileTime(
    _In_ CONST FILETIME * lpLocalFileTime,
    _Out_ LPFILETIME lpFileTime
    );
	*/
#endif // REDEFINE_WINAPI_OLD_FUNCTIONS



BOOL
WINAPI
SetFileTime(
    _In_ HANDLE hFile,
    _In_opt_ CONST FILETIME * lpCreationTime,
    _In_opt_ CONST FILETIME * lpLastAccessTime,
    _In_opt_ CONST FILETIME * lpLastWriteTime
    );

#ifdef __cplusplus
}
#endif

#endif

//#include "tcmalloc/rhomem.h"

#ifdef __cplusplus
extern "C" {
#endif
	
char* str_assign_ex( char* data, int len); 
char* str_assign(char* data); 
#ifdef __cplusplus
}
#endif

#define RHO_TRACE_POINT RHO_LOG("trace point")

#endif //_RHOPORT_H_
