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
REM %11 is the MIME Type

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
set param11=%1

echo // Microsoft Visual C++ generated resource script.
echo //
echo #include "%param3%"
echo.
echo #define APSTUDIO_READONLY_SYMBOLS
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // Generated from the TEXTINCLUDE 2 resource.
echo //
echo #include "afxres.h"
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo #undef APSTUDIO_READONLY_SYMBOLS
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo // English (U.S.) resources
echo.
echo #if ^!defined(AFX_RESOURCE_DLL) ^|^| defined(AFX_TARG_ENU)
echo #ifdef _WIN32
echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US
echo #pragma code_page(1252)
echo #endif //_WIN32
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
echo  FILEOS 0x40004L
echo  FILETYPE 0x2L
echo  FILESUBTYPE 0x0L
echo BEGIN
echo     BLOCK "StringFileInfo"
echo     BEGIN
echo         BLOCK "040904e4"
echo         BEGIN
echo             VALUE "CompanyName", "Symbol Technologies, Inc"
echo             VALUE "FileDescription", %param1%
echo             VALUE "FileOpenName", "npwtg_legacy.dll"
echo             VALUE "FileVersion", "%param5%, %param6%, %param7%, %param8%"
echo             VALUE "InternalName", "%param2%"
echo             VALUE "LegalCopyright", "Copyright (C) %date:~-4,4%"
echo             VALUE "LegalTrademarks", "Symbol Technologies Inc."
echo             VALUE "MIMEType", %param11%
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
echo     "#include ""afxres.h""\r\n"
echo     "\0"
echo END
echo.
echo 3 TEXTINCLUDE 
echo BEGIN
echo     "\r\n"
echo     "\0"
echo END
echo.
echo #endif    // APSTUDIO_INVOKED
echo.
echo #endif    // English (U.S.) resources
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
echo.
echo /////////////////////////////////////////////////////////////////////////////
echo #endif    // not APSTUDIO_INVOKED
echo.

