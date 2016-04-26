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

echo ***  GENERATING ACTIVEX RC FILE FOR %param1%  ***
echo. 
REM Delete the file if it already exists (removing read only attribute) - ClearCase work around
ATTRIB -R %param10%
del %param10% /q

set fileName=%param10%

echo // Microsoft Visual C++ generated resource script.>%fileName%
echo //>>%fileName%
echo #include "%param3%">>%fileName%
echo.>>%fileName%
echo #define APSTUDIO_READONLY_SYMBOLS>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Generated from the TEXTINCLUDE 2 resource.>>%fileName%
echo //>>%fileName%
echo #include "winres.h">>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo #undef APSTUDIO_READONLY_SYMBOLS>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo // English (U.K.) resources>>%fileName%
echo.>>%fileName%
echo #if ^!defined(AFX_RESOURCE_DLL) ^|^| defined(AFX_TARG_ENG)>>%fileName%
echo #ifdef _WIN32>>%fileName%
echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_UK>>%fileName%
echo #pragma code_page(1252)>>%fileName%
echo #endif //_WIN32>>%fileName%
echo.>>%fileName%
echo #ifdef APSTUDIO_INVOKED>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // TEXTINCLUDE>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo 1 TEXTINCLUDE >>%fileName%
echo BEGIN>>%fileName%
echo     "%param3%\0">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo 2 TEXTINCLUDE >>%fileName%
echo BEGIN>>%fileName%
echo     "#include ""winres.h""\r\n">>%fileName%
echo     "\0">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo 3 TEXTINCLUDE >>%fileName%
echo BEGIN>>%fileName%
echo     "#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)\r\n">>%fileName%
echo     "LANGUAGE 9, 1\r\n">>%fileName%
echo     "#pragma code_page(1252)\r\n">>%fileName%
if "%param2%"=="NoSIP" (
REM NoSIP
echo     "#include ""PBNoSIP.rc2""  // non-Microsoft Visual C++ edited resources\r\n">>%fileName%
	)
if "%param2%"=="CeODAX" (
REM CeODAX
echo     "#include ""CeODAX.rc2""  // non-Microsoft Visual C++ edited resources\r\n">>%fileName%
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
echo     "#include ""PocketBrowser.rc2""  // non-Microsoft Visual C++ edited resources\r\n">>%fileName%
	)
echo     "#endif\r\n">>%fileName%
if "%param2%"=="NoSIP" (
REM NoSIP
echo     "1 TYPELIB ""PBNoSIP.tlb""\r\n">>%fileName%
	)
if "%param2%"=="CeODAX" (
REM CeODAX
echo     "1 TYPELIB ""CeODAX.tlb""\r\n">>%fileName%
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
echo     "1 TYPELIB ""PocketBrowser.tlb""\r\n">>%fileName%
	)
echo     "\0">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo #endif    // APSTUDIO_INVOKED>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Version>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo VS_VERSION_INFO VERSIONINFO>>%fileName%
echo  FILEVERSION %param5%,%param6%,%param7%,%param8% >>%fileName%
echo  PRODUCTVERSION %param5%,%param6%,%param7%,%param8% >>%fileName%
echo  FILEFLAGSMASK 0x3fL>>%fileName%
echo #ifdef _DEBUG>>%fileName%
echo  FILEFLAGS 0x1L>>%fileName%
echo #else>>%fileName%
echo  FILEFLAGS 0x0L>>%fileName%
echo #endif>>%fileName%
echo  FILEOS 0x4L>>%fileName%
echo  FILETYPE 0x2L>>%fileName%
echo  FILESUBTYPE 0x0L>>%fileName%
echo BEGIN>>%fileName%
echo     BLOCK "StringFileInfo">>%fileName%
echo     BEGIN>>%fileName%
echo         BLOCK "040904e4">>%fileName%
echo         BEGIN>>%fileName%
echo             VALUE "CompanyName", "Symbol Technologies, Inc">>%fileName%
echo             VALUE "FileDescription", %param1%>>%fileName%
echo             VALUE "FileVersion", "%param5%, %param6%, %param7%, %param8%">>%fileName%
echo             VALUE "InternalName", "%param2%">>%fileName%
echo             VALUE "LegalCopyright", "Copyright (C) %date:~-4,4%">>%fileName%
echo             VALUE "OriginalFilename", "%param4%">>%fileName%
echo             VALUE "ProductName", "Symbol RhoElements v1">>%fileName%
echo             VALUE "ProductVersion", "%param5%, %param6%, %param7%, %param8%, %param9%">>%fileName%
echo         END>>%fileName%
echo     END>>%fileName%
echo     BLOCK "VarFileInfo">>%fileName%
echo     BEGIN>>%fileName%
echo         VALUE "Translation", 0x409, 1252>>%fileName%
echo     END>>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // REGISTRY>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
if "%param2%"=="NoSIP" (
REM NoSIP
echo IDR_PBNOSIP             REGISTRY                "PBNoSIP.rgs">>%fileName%
	)
if "%param2%"=="CeODAX" (
REM CeODAX
echo IDR_CEODAX              REGISTRY                "CeODAX.rgs">>%fileName%
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
echo IDR_POCKETBROWSER       REGISTRY                "PocketBrowser.rgs">>%fileName%
echo IDR_NARROWBAND          REGISTRY                "NarrowBand.rgs">>%fileName%
echo IDR_MICROFLASH          REGISTRY                "MicroFlash.rgs">>%fileName%
echo IDR_AIRBEAMSMART        REGISTRY                "AirBEAMSmart.rgs">>%fileName%
	)
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // String Table>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo STRINGTABLE >>%fileName%
echo BEGIN>>%fileName%
if "%param2%"=="NoSIP" (
REM NoSIP
	echo     IDS_PROJNAME            "PBNoSIP">>%fileName%
	)
if "%param2%"=="CeODAX" (
REM CeODAX
	echo     IDS_PROJNAME            "CeODAX">>%fileName%
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
	echo     IDS_PROJNAME            "PocketBrowser">>%fileName%
	)
echo END>>%fileName%
echo.>>%fileName%
echo #endif    // English (U.S.) resources>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo // English (U.K.) resources>>%fileName%
echo.>>%fileName%
echo #if ^!defined(AFX_RESOURCE_DLL) ^|^| defined(AFX_TARG_ENG)>>%fileName%
echo #ifdef _WIN32>>%fileName%
echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_UK>>%fileName%
echo #pragma code_page(1252)>>%fileName%
echo #endif //_WIN32>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // REGISTRY>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
if "%param2%"=="NoSIP" (
REM NoSIP
	echo IDR_NSEDIT              REGISTRY                "NSEdit.rgs">>%fileName%
	)
if "%param2%"=="CeODAX" (
REM CeODAX
	echo IDR_ODAX                REGISTRY                "ODAX.rgs">>%fileName%
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
	echo IDR_GENERIC             REGISTRY                "Generic.rgs">>%fileName%
	)
echo #endif    // English (U.K.) resources>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo #ifndef APSTUDIO_INVOKED>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Generated from the TEXTINCLUDE 3 resource.>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo #if ^!defined^(AFX_RESOURCE_DLL^) ^|^| defined^(AFX_TARG_ENG^)>>%fileName%
echo LANGUAGE 9, 1 >>%fileName%
echo #pragma code_page^(1252^)>>%fileName%
if "%param2%"=="NoSIP" (
REM NoSIP
echo #include "PBNoSIP.rc2"  // non-Microsoft Visual C++ edited resources>>%fileName%
echo #endif>>%fileName%
echo 1 TYPELIB "PBNoSIP.tlb">>%fileName%
	)
if "%param2%"=="CeODAX" (
REM CeODAX
echo #include "CeODAX.rc2"  // non-Microsoft Visual C++ edited resources>>%fileName%
echo #endif>>%fileName%
echo 1 TYPELIB "CeODAX.tlb">>%fileName%
	)
if "%param2%"=="PocketBrowser" (
REM PocketBrowser Generic
echo #include "PocketBrowser.rc2"  // non-Microsoft Visual C++ edited resources>>%fileName%
echo #endif>>%fileName%
echo 1 TYPELIB "PocketBrowser.tlb">>%fileName%
	)
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo #endif    // not APSTUDIO_INVOKED>>%fileName%
echo.>>%fileName%

