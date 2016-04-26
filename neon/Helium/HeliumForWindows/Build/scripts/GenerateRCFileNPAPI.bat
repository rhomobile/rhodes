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

echo ***  GENERATING RC FILE FOR %param1%  ***
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
echo #include "afxres.h">>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo #undef APSTUDIO_READONLY_SYMBOLS>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo // English (U.S.) resources>>%fileName%
echo.>>%fileName%
echo #if ^!defined(AFX_RESOURCE_DLL) ^|^| defined(AFX_TARG_ENU)>>%fileName%
echo #ifdef _WIN32>>%fileName%
echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US>>%fileName%
echo #pragma code_page(1252)>>%fileName%
echo #endif //_WIN32>>%fileName%
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
echo  FILEOS 0x40004L>>%fileName%
echo  FILETYPE 0x2L>>%fileName%
echo  FILESUBTYPE 0x0L>>%fileName%
echo BEGIN>>%fileName%
echo     BLOCK "StringFileInfo">>%fileName%
echo     BEGIN>>%fileName%
echo         BLOCK "040904e4">>%fileName%
echo         BEGIN>>%fileName%
echo             VALUE "CompanyName", "Symbol Technologies, Inc">>%fileName%
echo             VALUE "FileDescription", %param1%>>%fileName%
echo             VALUE "FileOpenName", "npwtg_legacy.dll">>%fileName%
echo             VALUE "FileVersion", "%param5%, %param6%, %param7%, %param8%">>%fileName%
echo             VALUE "InternalName", "%param2%">>%fileName%
echo             VALUE "LegalCopyright", "Copyright (C) %date:~-4,4%">>%fileName%
echo             VALUE "LegalTrademarks", "Symbol Technologies Inc.">>%fileName%
echo             VALUE "MIMEType", %param11%>>%fileName%
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
echo     "#include ""afxres.h""\r\n">>%fileName%
echo     "\0">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo 3 TEXTINCLUDE >>%fileName%
echo BEGIN>>%fileName%
echo     "\r\n">>%fileName%
echo     "\0">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo #endif    // APSTUDIO_INVOKED>>%fileName%
echo.>>%fileName%
echo #endif    // English (U.S.) resources>>%fileName%
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
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo #endif    // not APSTUDIO_INVOKED>>%fileName%
echo.>>%fileName%

