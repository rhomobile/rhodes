@echo off

REM %1 is File Description
REM %2 is Internal Name
REM %3 is Resource Header FileName
REM %4 is Original Filename
REM %5 is Major Version Number
REM %6 is Minor Version Number 1
REM %7 is Minor Version Number 2
REM %8 is Minor Version Number 3
REM %9 is the Hot Fix Version Number
REM %10 is FileName to output to

set param1=%1
set param2=%2
set param3=%3
set param4=%4
set param5=%5
set param6=%6
set param7=%7
set param8=%8
set param9=%9
SHIFT
SHIFT
SHIFT
SHIFT
SHIFT
SHIFT
SHIFT
SHIFT
SHIFT
SHIFT
set param10=%0

echo // Microsoft Visual C++ generated resource script.
echo //
echo #include "%param3%"
echo.
echo #define APSTUDIO_READONLY_SYMBOLS
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // Generated from the TEXTINCLUDE 2 resource.
echo //
echo #include "winres.h"
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo #undef APSTUDIO_READONLY_SYMBOLS
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo // English (U.K.) resources
echo.
echo #if ^!defined(AFX_RESOURCE_DLL) ^|^| defined(AFX_TARG_ENG)
echo #ifdef _WIN32
echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_UK
echo #pragma code_page(1252)
echo #endif //_WIN32
echo.
echo #ifdef APSTUDIO_INVOKED
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // TEXTINCLUDE
echo //
echo.
echo 1 TEXTINCLUDE 
echo BEGIN
echo     "%param3%\0"
echo END
echo.
echo 2 TEXTINCLUDE 
echo BEGIN
echo     "#include ""winres.h""\r\n"
echo     "\0"
echo END
echo.
echo 3 TEXTINCLUDE 
echo BEGIN
echo     "#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)\r\n"
echo     "LANGUAGE 9, 1\r\n"
echo     "#pragma code_page(1252)\r\n"
if "%param2%"=="NoSIP" (
REM NoSIP
echo     "#include ""PBNoSIP.rc2""  // non-Microsoft Visual C++ edited resources\r\n"
	)
if "%param2%"=="CeODAX" (
REM CeODAX
echo     "#include ""CeODAX.rc2""  // non-Microsoft Visual C++ edited resources\r\n"
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
echo     "#include ""PocketBrowser.rc2""  // non-Microsoft Visual C++ edited resources\r\n"
	)
echo     "#endif\r\n"
if "%param2%"=="NoSIP" (
REM NoSIP
echo     "1 TYPELIB ""PBNoSIP.tlb""\r\n"
	)
if "%param2%"=="CeODAX" (
REM CeODAX
echo     "1 TYPELIB ""CeODAX.tlb""\r\n"
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
echo     "1 TYPELIB ""PocketBrowser.tlb""\r\n"
	)
echo     "\0"
echo END
echo.
echo #endif    // APSTUDIO_INVOKED
echo.
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // Version
echo //
echo.
echo VS_VERSION_INFO VERSIONINFO
echo  FILEVERSION %param5%,%param6%,%param7%,%param8% 
echo  PRODUCTVERSION %param5%,%param6%,%param7%,%param8% 
echo  FILEFLAGSMASK 0x3fL
echo #ifdef _DEBUG
echo  FILEFLAGS 0x1L
echo #else
echo  FILEFLAGS 0x0L
echo #endif
echo  FILEOS 0x4L
echo  FILETYPE 0x2L
echo  FILESUBTYPE 0x0L
echo BEGIN
echo     BLOCK "StringFileInfo"
echo     BEGIN
echo         BLOCK "040904e4"
echo         BEGIN
echo             VALUE "CompanyName", "Symbol Technologies, Inc"
echo             VALUE "FileDescription", %param1%
echo             VALUE "FileVersion", "%param5%, %param6%, %param7%, %param8%"
echo             VALUE "InternalName", "%param2%"
echo             VALUE "LegalCopyright", "Copyright (C) %date:~-4,4%"
echo             VALUE "OriginalFilename", "%param4%"
echo             VALUE "ProductName", "Symbol RhoElements v1"
echo             VALUE "ProductVersion", "%param5%, %param6%, %param7%, %param8%, %param9%"
echo         END
echo     END
echo     BLOCK "VarFileInfo"
echo     BEGIN
echo         VALUE "Translation", 0x409, 1252
echo     END
echo END
echo.
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // REGISTRY
echo //
echo.
if "%param2%"=="NoSIP" (
REM NoSIP
echo IDR_PBNOSIP             REGISTRY                "PBNoSIP.rgs"
	)
if "%param2%"=="CeODAX" (
REM CeODAX
echo IDR_CEODAX              REGISTRY                "CeODAX.rgs"
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
echo IDR_POCKETBROWSER       REGISTRY                "PocketBrowser.rgs"
echo IDR_NARROWBAND          REGISTRY                "NarrowBand.rgs"
echo IDR_MICROFLASH          REGISTRY                "MicroFlash.rgs"
echo IDR_AIRBEAMSMART        REGISTRY                "AirBEAMSmart.rgs"
	)
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // String Table
echo //
echo.
echo STRINGTABLE 
echo BEGIN
if "%param2%"=="NoSIP" (
REM NoSIP
	echo     IDS_PROJNAME            "PBNoSIP"
	)
if "%param2%"=="CeODAX" (
REM CeODAX
	echo     IDS_PROJNAME            "CeODAX"
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
	echo     IDS_PROJNAME            "PocketBrowser"
	)
echo END
echo.
echo #endif    // English (U.S.) resources
echo /////////////////////////////////////////////////////////////////////////////
echo.
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo // English (U.K.) resources
echo.
echo #if ^!defined(AFX_RESOURCE_DLL) ^|^| defined(AFX_TARG_ENG)
echo #ifdef _WIN32
echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_UK
echo #pragma code_page(1252)
echo #endif //_WIN32
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // REGISTRY
echo //
echo.
if "%param2%"=="NoSIP" (
REM NoSIP
	echo IDR_NSEDIT              REGISTRY                "NSEdit.rgs"
	)
if "%param2%"=="CeODAX" (
REM CeODAX
	echo IDR_ODAX                REGISTRY                "ODAX.rgs"
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
	echo IDR_GENERIC             REGISTRY                "Generic.rgs"
	)
echo #endif    // English (U.K.) resources
echo /////////////////////////////////////////////////////////////////////////////
echo.
echo.
echo.
echo #ifndef APSTUDIO_INVOKED
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // Generated from the TEXTINCLUDE 3 resource.
echo //
echo.
echo #if ^!defined^(AFX_RESOURCE_DLL^) ^|^| defined^(AFX_TARG_ENG^)
echo LANGUAGE 9, 1 
echo #pragma code_page^(1252^)
if "%param2%"=="NoSIP" (
REM NoSIP
echo #include "PBNoSIP.rc2"  // non-Microsoft Visual C++ edited resources
echo #endif
echo 1 TYPELIB "PBNoSIP.tlb"
	)
if "%param2%"=="CeODAX" (
REM CeODAX
echo #include "CeODAX.rc2"  // non-Microsoft Visual C++ edited resources
echo #endif
echo 1 TYPELIB "CeODAX.tlb"
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
echo #include "PocketBrowser.rc2"  // non-Microsoft Visual C++ edited resources
echo #endif
echo 1 TYPELIB "PocketBrowser.tlb"
	)
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo #endif    // not APSTUDIO_INVOKED
echo.

