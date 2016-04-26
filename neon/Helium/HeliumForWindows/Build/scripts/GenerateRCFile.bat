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
if "%param2%"=="RhoElements" (
REM Core Only
echo #define APSTUDIO_HIDDEN_SYMBOLS>>%fileName%
echo #include "windows.h">>%fileName%
echo #include "resdefce.h">>%fileName%
echo #undef APSTUDIO_HIDDEN_SYMBOLS>>%fileName%
	) Else (
	echo #include "afxres.h">>%fileName%
	)
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
if "%param2%"=="RhoElements" (
REM Core Only
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Icon>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo // Icon with lowest ID value placed first to ensure application icon>>%fileName%
echo // remains consistent on all systems.>>%fileName%
echo IDI_PBCORE              ICON                    "PBCore.ico">>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Menu>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo IDR_MENU MENU >>%fileName%
echo BEGIN>>%fileName%
echo     POPUP "&File">>%fileName%
echo     BEGIN>>%fileName%
echo         MENUITEM "E&xit",                       IDM_FILE_EXIT>>%fileName%
echo     END>>%fileName%
echo     POPUP "&Help">>%fileName%
echo     BEGIN>>%fileName%
echo         MENUITEM "&About",                      IDM_HELP_ABOUT>>%fileName%
echo     END>>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Accelerator>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo IDC_PBCORE ACCELERATORS >>%fileName%
echo BEGIN>>%fileName%
echo     "A",            IDM_HELP_ABOUT,         VIRTKEY, CONTROL, NOINVERT>>%fileName%
echo     "Q",            IDOK,                   VIRTKEY, CONTROL, NOINVERT>>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Dialog>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo IDD_ABOUTBOX DIALOG  0, 0, 125, 55>>%fileName%
echo STYLE DS_SETFONT ^| DS_MODALFRAME ^| WS_POPUP ^| WS_CAPTION ^| WS_SYSMENU>>%fileName%
echo EXSTYLE 0x80000000L>>%fileName%
echo CAPTION "About PBCore">>%fileName%
echo FONT 8, "MS Sans Serif">>%fileName%
echo BEGIN>>%fileName%
echo     ICON            IDI_PBCORE,IDC_STATIC,11,17,20,20>>%fileName%
echo     LTEXT           "[PRODUCT_NAME]",IDC_STATIC,38,10,70,8,SS_NOPREFIX>>%fileName%
echo     LTEXT           "Copyright (C) 2009",IDC_STATIC,38,25,70,8 >>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // DESIGNINFO>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo #ifdef APSTUDIO_INVOKED>>%fileName%
echo GUIDELINES DESIGNINFO >>%fileName%
echo BEGIN>>%fileName%
echo     IDD_ABOUTBOX, DIALOG>>%fileName%
echo     BEGIN>>%fileName%
echo         LEFTMARGIN, 7 >>%fileName%
echo         RIGHTMARGIN, 118>>%fileName%
echo         TOPMARGIN, 7 >>%fileName%
echo         BOTTOMMARGIN, 48>>%fileName%
echo     END>>%fileName%
echo END>>%fileName%
echo #endif    // APSTUDIO_INVOKED>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
	)
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
if "%param2%"=="RhoElements" (
REM Core Only
echo     "#define APSTUDIO_HIDDEN_SYMBOLS\r\n">>%fileName%
echo     "#include ""windows.h""\r\n">>%fileName%
echo     "#include ""resdefce.h""\r\n">>%fileName%
echo     "#undef APSTUDIO_HIDDEN_SYMBOLS\r\n">>%fileName%
	) Else (
	echo     "#include ""afxres.h""\r\n">>%fileName%
	)
echo     "\0">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo 3 TEXTINCLUDE >>%fileName%
echo BEGIN>>%fileName%
echo     "\r\n">>%fileName%
if "%param2%"=="RhoElements" (
REM Core Only
echo     "#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)\r\n">>%fileName%
echo     "LANGUAGE 9, 1\r\n">>%fileName%
echo     "#pragma code_page(1252)\r\n">>%fileName%
echo     "#include ""PBCore.rc2""  // non-Microsoft Visual C++ edited resources\r\n">>%fileName%
echo     "#endif\r\n">>%fileName%
	)
echo     "\0">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo #endif    // APSTUDIO_INVOKED>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
if "%param2%"=="WTG_DeviceApplication_PLG" (
REM Device & Application Plugin Only
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Bitmap>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo IDB_HOURGLASS           BITMAP                  "Hourglass.bmp">>%fileName%
echo.>>%fileName%
	)
if "%param2%"=="RhoElements" (
REM Core Only
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // String Table>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo STRINGTABLE >>%fileName%
echo BEGIN>>%fileName%
echo     IDS_APP_TITLE           "PBCore">>%fileName%
echo     IDC_PBCORE              "PBCORE">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo STRINGTABLE >>%fileName%
echo BEGIN>>%fileName%
echo     IDS_TOOLS               "Tools">>%fileName%
echo END>>%fileName%
echo.>>%fileName%
echo #endif    // English ^(U.S.^) resources>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo.>>%fileName%
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo // English ^(U.K.^) resources>>%fileName%
echo.>>%fileName%
echo #if ^!defined^(AFX_RESOURCE_DLL^) ^|^| defined^(AFX_TARG_ENG^)>>%fileName%
echo #ifdef _WIN32>>%fileName%
echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_UK>>%fileName%
echo #pragma code_page^(1252^)>>%fileName%
echo #endif //_WIN32>>%fileName%
echo.>>%fileName%
	)
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo //>>%fileName%
echo // Version>>%fileName%
echo //>>%fileName%
echo.>>%fileName%
echo VS_VERSION_INFO VERSIONINFO>>%fileName%
echo  FILEVERSION %param5%,%param6%,%param7%,%param8% >>%fileName%
echo  PRODUCTVERSION %param5%,%param6%,%param7%,%param8% >>%fileName%
echo  FILEFLAGSMASK 0x17L>>%fileName%
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
echo         BLOCK "080904b0">>%fileName%
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
echo         VALUE "Translation", 0x809, 1200>>%fileName%
echo     END>>%fileName%
echo END>>%fileName%
echo.>>%fileName%
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
if "%param2%"=="RhoElements" (
REM Core Only
echo #if ^!defined^(AFX_RESOURCE_DLL^) ^|^| defined^(AFX_TARG_ENG^)>>%fileName%
echo LANGUAGE 9, 1 >>%fileName%
echo #pragma code_page^(1252^)>>%fileName%
echo #include "PBCore.rc2"  // non-Microsoft Visual C++ edited resources>>%fileName%
echo #endif>>%fileName%
	)
echo.>>%fileName%
echo /////////////////////////////////////////////////////////////////////////////>>%fileName%
echo #endif    // not APSTUDIO_INVOKED>>%fileName%
echo.>>%fileName%

