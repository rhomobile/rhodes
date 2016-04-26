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

echo // Microsoft Visual C++ generated resource script.
echo //
echo #include "%3"
echo.
echo #define APSTUDIO_READONLY_SYMBOLS
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // Generated from the TEXTINCLUDE 2 resource.
echo //

if "%2"=="RhoElements" (
    REM Core Only
    echo #define APSTUDIO_HIDDEN_SYMBOLS
    echo #include "windows.h"
    echo #include "resdefce.h"
    echo #undef APSTUDIO_HIDDEN_SYMBOLS
) Else (
    echo #include "afxres.h"
)

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

if "%2"=="RhoElements" (
    REM Core Only
    echo /////////////////////////////////////////////////////////////////////////////
    echo //
    echo // Icon
    echo //
    echo.
    echo // Icon with lowest ID value placed first to ensure application icon
    echo // remains consistent on all systems.
    echo IDI_PBCORE              ICON                    "PBCore.ico"
    echo.
    echo /////////////////////////////////////////////////////////////////////////////
    echo //
    echo // Menu
    echo //
    echo.
    echo IDR_MENU MENU 
    echo BEGIN
    echo     POPUP "&File"
    echo     BEGIN
    echo         MENUITEM "E&xit",                       IDM_FILE_EXIT
    echo     END
    echo     POPUP "&Help"
    echo     BEGIN
    echo         MENUITEM "&About",                      IDM_HELP_ABOUT
    echo     END
    echo END
    echo.
    echo.
    echo /////////////////////////////////////////////////////////////////////////////
    echo //
    echo // Accelerator
    echo //
    echo.
    echo IDC_PBCORE ACCELERATORS 
    echo BEGIN
    echo     "A",            IDM_HELP_ABOUT,         VIRTKEY, CONTROL, NOINVERT
    echo     "Q",            IDOK,                   VIRTKEY, CONTROL, NOINVERT
    echo END
    echo.
    echo.
    echo /////////////////////////////////////////////////////////////////////////////
    echo //
    echo // Dialog
    echo //
    echo.
    echo IDD_ABOUTBOX DIALOG  0, 0, 125, 55
    echo STYLE DS_SETFONT ^| DS_MODALFRAME ^| WS_POPUP ^| WS_CAPTION ^| WS_SYSMENU
    echo EXSTYLE 0x80000000L
    echo CAPTION "About PBCore"
    echo FONT 8, "MS Sans Serif"
    echo BEGIN
    echo     ICON            IDI_PBCORE,IDC_STATIC,11,17,20,20
    echo     LTEXT           "[PRODUCT_NAME]",IDC_STATIC,38,10,70,8,SS_NOPREFIX
    echo     LTEXT           "Copyright (C) 2009",IDC_STATIC,38,25,70,8 
    echo END
    echo.
    echo.
    echo /////////////////////////////////////////////////////////////////////////////
    echo //
    echo // DESIGNINFO
    echo //
    echo.
    echo #ifdef APSTUDIO_INVOKED
    echo GUIDELINES DESIGNINFO 
    echo BEGIN
    echo     IDD_ABOUTBOX, DIALOG
    echo     BEGIN
    echo         LEFTMARGIN, 7 
    echo         RIGHTMARGIN, 118
    echo         TOPMARGIN, 7 
    echo         BOTTOMMARGIN, 48
    echo     END
    echo END
    echo #endif    // APSTUDIO_INVOKED
    echo.
    echo.
)

echo #ifdef APSTUDIO_INVOKED
echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // TEXTINCLUDE
echo //
echo.
echo 1 TEXTINCLUDE 
echo BEGIN
echo     "%3\0"
echo END
echo.
echo 2 TEXTINCLUDE 
echo BEGIN

if "%2"=="RhoElements" (
    REM Core Only
    echo     "#define APSTUDIO_HIDDEN_SYMBOLS\r\n"
    echo     "#include ""windows.h""\r\n"
    echo     "#include ""resdefce.h""\r\n"
    echo     "#undef APSTUDIO_HIDDEN_SYMBOLS\r\n"
) Else (
    echo     "#include ""afxres.h""\r\n"
)

echo     "\0"
echo END
echo.
echo 3 TEXTINCLUDE 
echo BEGIN
echo     "\r\n"

if "%2"=="RhoElements" (
    REM Core Only
    echo     "#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)\r\n"
    echo     "LANGUAGE 9, 1\r\n"
    echo     "#pragma code_page(1252)\r\n"
    echo     "#include ""PBCore.rc2""  // non-Microsoft Visual C++ edited resources\r\n"
    echo     "#endif\r\n"
)

echo     "\0"
echo END
echo.
echo #endif    // APSTUDIO_INVOKED
echo.
echo.

if "%2"=="WTG_DeviceApplication_PLG" (
    REM Device & Application Plugin Only
    echo /////////////////////////////////////////////////////////////////////////////
    echo //
    echo // Bitmap
    echo //
    echo.
    echo IDB_HOURGLASS           BITMAP                  "Hourglass.bmp"
    echo.
)

if "%2"=="RhoElements" (
    REM Core Only
    echo /////////////////////////////////////////////////////////////////////////////
    echo //
    echo // String Table
    echo //
    echo.
    echo STRINGTABLE 
    echo BEGIN
    echo     IDS_APP_TITLE           "PBCore"
    echo     IDC_PBCORE              "PBCORE"
    echo END
    echo.
    echo STRINGTABLE 
    echo BEGIN
    echo     IDS_TOOLS               "Tools"
    echo END
    echo.
    echo #endif    // English ^(U.S.^) resources
    echo /////////////////////////////////////////////////////////////////////////////
    echo.
    echo.
    echo /////////////////////////////////////////////////////////////////////////////
    echo // English ^(U.K.^) resources
    echo.
    echo #if ^!defined^(AFX_RESOURCE_DLL^) ^|^| defined^(AFX_TARG_ENG^)
    echo #ifdef _WIN32
    echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_UK
    echo #pragma code_page^(1252^)
    echo #endif //_WIN32
    echo.
)

echo /////////////////////////////////////////////////////////////////////////////
echo //
echo // Version
echo //
echo.
echo VS_VERSION_INFO VERSIONINFO
echo  FILEVERSION %5,%6,%7,%8 
echo  PRODUCTVERSION %5,%6,%7,%8 
echo  FILEFLAGSMASK 0x17L
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
echo         BLOCK "080904b0"
echo         BEGIN
echo             VALUE "CompanyName", "Symbol Technologies, Inc"
echo             VALUE "FileDescription", %1
echo             VALUE "FileVersion", "%5, %6, %7, %8"
echo             VALUE "InternalName", "%2"
echo             VALUE "LegalCopyright", "Copyright (C) %date:~-4,4%"
echo             VALUE "OriginalFilename", "%4"
echo             VALUE "ProductName", "Symbol RhoElements v1"
echo             VALUE "ProductVersion", "%5, %6, %7, %8, %9"
echo         END
echo     END
echo     BLOCK "VarFileInfo"
echo     BEGIN
echo         VALUE "Translation", 0x809, 1200
echo     END
echo END
echo.
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

if "%2"=="RhoElements" (
    REM Core Only
    echo #if ^!defined^(AFX_RESOURCE_DLL^) ^|^| defined^(AFX_TARG_ENG^)
    echo LANGUAGE 9, 1 
    echo #pragma code_page^(1252^)
    echo #include "PBCore.rc2"  // non-Microsoft Visual C++ edited resources
    echo #endif
)

echo.
echo /////////////////////////////////////////////////////////////////////////////
echo #endif    // not APSTUDIO_INVOKED
echo.

