/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    aygshell.h

Abstract:

    Shell defines.

--*/


#ifndef __AYGSHELL_H__
#define __AYGSHELL_H__


#include <windows.h>
#include <sipapi.h>
#include <shlobj.h>
#include <prsht.h>
#include <winuserm.h>
#include <commctrl.h>
#include <commdlg.h>

#ifdef TPC
#include "vibrate.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// To enable autodoc tags
//@doc

// This event is set when the shell has initialized and shell APIs
// are ready to be called.
#define SHELL_API_READY_EVENT  TEXT("SYSTEM/ShellAPIReady")

//#define SHN_FIRST               (0U-1000U)       // Shell reserved
    
#define SHNN_FIRST              (0U-1000U)        // Shell Notifications
#define SHNN_LAST               (0U-1020U)        // Shell Notifications

//#define SHN_LAST                (0U-11000U)

//
// flags in the fdwFlags field of SIPINFO.
// some of these are defined in sipapi.h in the OS.
//
#define SIPF_DISABLECOMPLETION      0x08


//
// Supported system parameters.
//
#ifndef SPI_SETSIPINFO
#define SPI_SETSIPINFO          224
#endif
#define SPI_GETSIPINFO          225
#define SPI_SETCURRENTIM        226
#define SPI_GETCURRENTIM        227
#define SPI_SETCOMPLETIONINFO   223
#define SPI_APPBUTTONCHANGE     228
#define SPI_RESERVED            229
#define SPI_SYNCSETTINGSCHANGE  230


//Pocket PC  special controls
#define WC_SIPPREF    L"SIPPREF"


//
// SHSipInfo function.
//
WINSHELLAPI
BOOL
SHSipInfo(
    UINT uiAction,
    UINT uiParam,
    PVOID pvParam,
    UINT fWinIni );

BYTE SHGetAppKeyAssoc( LPCTSTR ptszApp );
BOOL SHSetAppKeyWndAssoc( BYTE bVk, HWND hwnd );

BOOL SHInitExtraControls(void);
BOOL SHCloseApps( DWORD dwMemSought );


//++++++
//
// SHInitDialog 
//

typedef struct tagSHINITDLGINFO
{
    DWORD dwMask;
    HWND  hDlg;
    DWORD dwFlags;
} SHINITDLGINFO, *PSHINITDLGINFO;


//
// The functions
//

BOOL SHInitDialog(PSHINITDLGINFO pshidi);

//
// Valid mask values
//

#define SHIDIM_FLAGS                0x0001

//
// Valid flags
//

#define SHIDIF_DONEBUTTON           0x0001
#define SHIDIF_SIZEDLG              0x0002
#define SHIDIF_SIZEDLGFULLSCREEN    0x0004
#define SHIDIF_SIPDOWN              0x0008
#define SHIDIF_FULLSCREENNOMENUBAR  0x0010
#define SHIDIF_EMPTYMENU            0x0020
#define SHIDIF_WANTSCROLLBAR        0x0040
#define SHIDIF_CANCELBUTTON         0x0080

//
// End SHInitDialog
//
//------

HBITMAP SHLoadImageResource(HINSTANCE hinst, UINT uIdGif);
HBITMAP SHLoadImageFile(LPCTSTR pszFileName);

//++++++
//
// Shell Menubar support
//

// Allows menus on softkeys (which have no ID) to be accessed through TB_GETBUTTONINFO/TB_SETBUTTONINFO
// by using an index (0 or 1) and setting TBIF_BYINDEX. 
#ifndef TBIF_BYINDEX
#define TBIF_BYINDEX 0x80000000
#endif

#define NOMENU 0xFFFF
#define IDC_COMMANDBANDS    100

// These defines MUST be < 100.  This is so apps can use these defines
// to get strings from the shell.
#define IDS_SHNEW           1
#define IDS_SHEDIT          2
#define IDS_SHTOOLS         3
#define IDS_SHVIEW          4
#define IDS_SHFILE          5
#define IDS_SHGO            6
#define IDS_SHFAVORITES     7
#define IDS_SHOPEN          8
#define IDS_SHMENU          9

//
// Shared New menu support
//
#define  IDM_SHAREDNEW        10
#define  IDM_SHAREDNEWDEFAULT 11
#define  IDM_SHAREDMENU       12

//
// Valid dwFlags
//
#define SHCMBF_EMPTYBAR      0x0001
#define SHCMBF_HIDDEN        0x0002 // create it hidden
#define SHCMBF_HIDESIPBUTTON 0x0004
#define SHCMBF_COLORBK       0x0008
#define SHCMBF_HMENU         0x0010 // specify an hmenu for resource rather than toolbar info


typedef struct tagSHMENUBARINFO
{
    DWORD     cbSize;               // IN  - Indicates which members of struct are valid
    HWND      hwndParent;           // IN
    DWORD     dwFlags;              // IN  - Some features we want
    UINT      nToolBarId;           // IN  - Which toolbar are we using
    HINSTANCE hInstRes;             // IN  - Instance that owns the resources
    int       nBmpId;
    int       cBmpImages;           // IN  - Count of bitmap images
    HWND      hwndMB;               // OUT
    COLORREF  clrBk;                // IN  - background color of the menu bar (excluding sip)
} SHMENUBARINFO, *PSHMENUBARINFO;

WINSHELLAPI BOOL  SHCreateMenuBar(SHMENUBARINFO *pmbi);



#define SHCMBM_SETSUBMENU   (WM_USER + 400) // wparam == id of button, lParam == hmenu, return is old hmenu
#define SHCMBM_GETSUBMENU   (WM_USER + 401) // lParam == ID
#define SHCMBM_GETMENU      (WM_USER + 402) // get the owning hmenu (as specified in the load resource)

// Returns menubar owned by a window
HWND WINAPI SHFindMenuBar(HWND hwnd);

/****************************************************************************
    SmartPhone's SoftKeyBar control
    
    Most of the interface of the soft key control is a very
    small subset of the MenuBar control.  The structures, messages
    and notifications relevant to Smart Phones are:

    Create the SoftKey control using SHCreateMenuBar.

    Messages that can be sent to the softkey control

        TB_SETBUTTONINFO
        TB_GETBUTTONINFO
            Support getting and setting of idCommand, pszText, fsState (TBSTATE_ENABLED, TBSTATE_HIDDEN only),
            and getting of menu handle associated with a button. (Use TBIF_LPARAM to get menu handle.)
            TB_SETBUTTONINFO and TB_GETBUTTONINFO support TBIF_BYINDEX to specify a softkey by index instead 
            of command ID.
        
        SHCMBM_OVERRIDEKEY
            Both Pocket PCs and Smart Phones support this message.
            This is used to modify the default handling of key messages sent
            to the soft key control of the foreground app.  The only keys that are
            currently valid: For Smart Phones, VK_TSOFT1, VK_TSOFT2, and VK_TBACK.
            For Pocket PCs, VK_TTALK.
            
            wParam = nVirtkey,
            dwMask = (DWORD)LOWORD(lParam),
            dwBits = (DWORD)HIWORD(lParam)
                SHMBOF_NODEFAULT    0x00000001 // do not do default handling of this key
                SHMBOF_NOTIFY       0x00000002 // send the owner the WM_* messages for this key

    Messages sent to the owner of the soft key control

        WM_INITMENUPOPUP
            see MSDN

 ****************************************************************************/


#define SHMBOF_NODEFAULT    0x00000001 // do not do default handling of this key
#define SHMBOF_NOTIFY       0x00000002 // send us the WM_* messages for this key

#define SHCMBM_OVERRIDEKEY  (WM_USER + 403)
#define SHCMBM_SETBKCOLOR   (WM_USER + 406) // lParam == COLORREF

// Does the default handling of pressing the back button.
void WINAPI SHNavigateBack();


// Enable/disable softkeys by command or index.
// Parameters:
//        hwndMenuBar Handle to the softkey bar as returned from SHCreateMenuBar or SHFindMenuBar
//        uid         The Command ID or index (if bByIndex is set) of the softkey to enable/disable
//        bByIndex    If TRUE, uid is interpreted as an index (0 or 1), otherwise uid is a command ID.
//        bEnable     Set TRUE to enable, FALSE to disable the softkey.
WINSHELLAPI HRESULT  SHEnableSoftkey(HWND hwndMenuBar, UINT uid,  BOOL bByIndex, BOOL bEnable);


//
// End Shell Menubar support
//
//------

//++++++
//
// Shell home screen support
//
HRESULT SHOnPluginDataChange(const CLSID* pclsidPlugin);
//
// End Shell Home screen support
//
//------

//++++++
//
// SHHandleWMActivate and SHHandleWMSettingChange fun
//

typedef struct
{
    DWORD cbSize;
    HWND hwndLastFocus;
    UINT fSipUp :1;
    UINT fSipOnDeactivation :1;
    UINT fActive :1;
    UINT fReserved :29;
} SHACTIVATEINFO, *PSHACTIVATEINFO;

#define SHA_INPUTDIALOG 0x00000001

WINSHELLAPI BOOL SHHandleWMActivate(HWND hwnd, WPARAM wParam, LPARAM lParam, SHACTIVATEINFO* psai, DWORD dwFlags);
WINSHELLAPI BOOL SHHandleWMSettingChange(HWND hwnd, WPARAM wParam, LPARAM lParam, SHACTIVATEINFO* psai);

//
// End SHHandleWMActivate and SHHandleWMSettingChange fun
//
//------


//++++++
//
// SHSipPreference
//

BOOL SHSipPreference(HWND hwnd, SIPSTATE st);

//
// End SHSipPreference
//
//------




//++++++
//
// SHRecognizeGesture structs
//

typedef struct tagSHRGI {
    DWORD cbSize;
    HWND hwndClient;
    POINT ptDown;
    DWORD dwFlags;
} SHRGINFO, *PSHRGINFO;


//
// Gesture notifications
//
#define  GN_CONTEXTMENU       1000


//
// Gesture flags
//
#define  SHRG_RETURNCMD       0x00000001
#define  SHRG_NOTIFYPARENT    0x00000002
// use the longer (mixed ink) delay timer
// useful for cases where you might click down first, verify you're
// got the right spot, then start dragging... and it's not clear
// you wanted a context menu
#define  SHRG_LONGDELAY       0x00000008 
#define  SHRG_NOANIMATION     0x00000010

//
// Struct sent through WM_NOTIFY when SHRG_NOTIFYPARENT is used
//
typedef struct tagNMRGINFO 
{
    NMHDR hdr;
    POINT ptAction;
    DWORD dwItemSpec;
} NMRGINFO, *PNMRGINFO;

WINSHELLAPI DWORD SHRecognizeGesture(SHRGINFO *shrg);

//
// End SHRecognizeGesture
//
//------


//++++++
//
// SHFullScreen
//

BOOL SHFullScreen(HWND hwndRequester, DWORD dwState);


//
// Valid states
//

#define SHFS_SHOWTASKBAR            0x0001
#define SHFS_HIDETASKBAR            0x0002
#define SHFS_SHOWSIPBUTTON          0x0004
#define SHFS_HIDESIPBUTTON          0x0008
#define SHFS_SHOWSTARTICON          0x0010
#define SHFS_HIDESTARTICON          0x0020

//
// End SHFullScreen
//
//------


//++++++
//
// SHDoneButton
//

BOOL SHDoneButton(HWND hwndRequester, DWORD dwState);


//
// Valid states
//

#define SHDB_SHOW                   0x0001
#define SHDB_HIDE                   0x0002
#define SHDB_SHOWCANCEL             0x0004


//
// Disable the navigation button bestowed by the Shell
// (NOTE: this only works if WS_CAPTION is not set)

#define WS_NONAVDONEBUTTON WS_MINIMIZEBOX

//
// End SHDoneButton
//
//------


//++++++
//
// SHGetAutoRunPath
//  pAutoRunPath must be at least MAX_PATH long

BOOL SHGetAutoRunPath( LPTSTR pAutoRunPath );

//
// End SHGetAutoRunPath
//
//------

//++++++
//
// SHSetNavBarText
//

BOOL SHSetNavBarText(HWND hwndRequester, LPCTSTR pszText);

//
// End SHSetNavBarText
//
//------

//++++++
//
// SHInputDialog
//

void SHInputDialog(HWND hwnd, UINT uMsg, WPARAM wParam);

//
// End SHInputDialog
//
//------

//++++++
//
// SHEnumPropSheetHandlers
//

// this is the maximum number of extension pages that can be added
// to a property sheet

#define MAX_EXTENSION_PAGES 6

// For property sheet extension - enumerates the subkeys under the
// class key hkey.  For each handler, the class is instantiated,
// queried for IShellPropSheetExt and AddPages is called.  The
// handle to the page is inserted in the array prghPropPages, and
// the pointer to the IShellPropSheetExt is added to prgpispse
// with one reference from the caller (these should be released
// by the caller after PropertySheet() is called).  These two arrays
// should be allocated before calling SHEnumPropSheetHandlers.
//
// Typical usage of this function would be:
//
//  - allocate an array of HPROPSHEETPAGEs for the standard pages plus
//    MAX_EXTENSION_PAGES extension pages
//  - fill a PROPSHEETPAGE struct and call CreatePropertySheetPage() on each
//    standard page
//  - store the HPROPSHEETPAGE for the standard pages at the beginning of
//    the array
//  - open a registry key where the app has defined ISV extension
//  - allocate an array of MAX_EXTENSION_PAGES IShellPropSheetExt interface
//    pointers
//  - call SHEnumPropSheetHandlers(), passing in the hkey, a pointer to the
//    first free HPROPSHEETPAGE array element, and a pointer to the array of
//    IShellPropSheetExt interface pointers
//  - call PropertySheet() to display the property sheet
//  - Release each interface pointer in the array of interface pointers
//  - free both arrays

// SHEnumPropSheetHandlers assumes that prghPropPages and prgpispse have been
// allocated with enough space for up to MAX_EXTENSION_PAGES elements.  The
// number of pages added is returned in *pcPages.

BOOL SHEnumPropSheetHandlers(HKEY hkey, int *pcPages, HPROPSHEETPAGE
        *prghPropPages, IShellPropSheetExt **prgpispse);

//
// End SHEnumPropSheetHandlers
//
//------


//++++++
//
// SHLoadContextMenuExtensions
//
//    Loads context menu extensions from handlers listed in the registry for
//    the context/class pair specified.  Menu items are added to hmenu in the
//    range [idCmdFirst, idCmdLast].  A handle to the context menu extensions
//    abstraction object is returned in *phCMExtensions.  It must be freed by
//    a call to SHFreeContextMenuExtensions.

BOOL SHLoadContextMenuExtensions(IUnknown *punkOwner, LPCTSTR pszContext,
    LPCTSTR pszClass, HMENU hmenu, UINT idCmdFirst, UINT idCmdLast,
    HANDLE *phCMExtensions);

//
// End SHLoadContextMenuExtensions
//
//------


//++++++
//
// SHInvokeContextMenuCommand
//
//    Invokes a command from a context menu.  Issues the command in the
//    extension that added it to the menu.

BOOL SHInvokeContextMenuCommand(HWND hwndOwner, UINT idCmd,
        HANDLE hCMExtensions);

//
// End SHInvokeContextMenuCommand
//
//------


//++++++
//
// SHFreeContextMenuExtensions
//

//    Releases memory allocated for context menu processing.

BOOL SHFreeContextMenuExtensions(HANDLE hCMExtensions);

//
// End SHFreeContextMenuExtensions
//
//------


//++++++
//
// SHCreateNewItem
//
//    Creates a New item, as if an item were chosen from the
//    global New menu dropdown.

HRESULT SHCreateNewItem (HWND hwndOwner, REFCLSID clsid);

//
// End SHCreateNewItem
//
//------


//++++++
//
//  SHGetEmergencyCallList
//
//       Gets a list of emergency calls

HRESULT SHGetEmergencyCallList(TCHAR *pwszBuffer, UINT uLenBuf);

//
// End SHGetEmergencyCallList
//
//------


//////////////////////////////////////////////////////////////////////////////
//
// Input Context API
//
// These are definitions and APIs for the interacting with the input context
// properties of individual windows
//
// {

// Word correct Options
enum SHIC_FEATURE
{
    SHIC_FEATURE_RESTOREDEFAULT = 0,
    SHIC_FEATURE_AUTOCORRECT    = 0x00000001L,
    SHIC_FEATURE_AUTOSUGGEST    = 0x00000002L,
    SHIC_FEATURE_HAVETRAILER    = 0x00000003L,
    SHIC_FEATURE_CLASS          = 0x00000004L
};

typedef enum SHIC_FEATURE SHIC_FEATURE;

// Predefined input context classes
#define SHIC_CLASS_DEFAULT  TEXT("")
#define SHIC_CLASS_EMAIL    TEXT("email")
#define SHIC_CLASS_URL      TEXT("url")
#define SHIC_CLASS_PHONE    TEXT("phone")
#define SHIC_CLASS_NAME     TEXT("name")
#define SHIC_CLASS_MAXLEN       (MAX_PATH - 11)

//@topic Input Context Features |
// The input context API supports the following features and their corresponding values:
//
//@flag   SHIC_FEATURE_RESTOREDEFAULT   | Restore original input context state. (no corresponding value)
//@flag   SHIC_FEATURE_AUTOCORRECT      | Turn auto-corrections on and off. (TRUE, FALSE)
//@flag   SHIC_FEATURE_AUTOCOMPLETE     | Turn dictionary suggestions on and off. (TRUE, FALSE)
//@flag   SHIC_FEATURE_HAVETRAILER      | Specify whether to append trailer characters after replacing words.
//                                      (TRUE, FALSE)
//@flag   SHIC_FEATURE_CLASS            | Make this control behave like a specific semantic type.
//                                      (SHIC_CLASS_DEFAULT, SHIC_CLASS_EMAIL, SHIC_CLASS_URL,
//                                      SHIC_CLASS_PHONE, SHIC_CLASS_NAME)
//
//@comm All SHIC_FEATUREs are inherited from parent if undefined. That is, if they are not defined in
//      a window or the window's SHIC class, the API looks at the parent chain to find the setting
//      that applies to the window.
//
//@xref <f SHSetInputContext> <f SHGetInputContext>
//


//++++++
//
//@func HRESULT | SHSetInputContext | Changes the state of an input context feature
//
//@parm HWND            | hwnd      | IN - Window whose context will be set
//@parm DWORD           | dwFeature | IN - Input context feature to change
//@parm const LPVOID    | lpValue   | IN - New value assigned to feature
//
//@rdesc Returns one of the following values:
//@flag S_OK                    | If everything went well
//@flag ERROR_INVALID_PARAMETER | if hwnd was NULL or lpValue was NULL for a feature
//                                that does not support it, such as SHIC_FEATURE_AUTOCORRECT,
//                                SHIC_FEATURE_AUTOCOMPLETE and SHIC_FEATURE_HAVETRAILER.
//@flag ERROR_NOT_SUPPORTED     | If the feature specified was invalid
//@flag ERROR_INVALID_DATA      | If the specified value is not a legal option
//
//@xref <l Input_Context_Features> <f SHGetInputContext>
//

HRESULT SHSetInputContext( HWND hwnd, DWORD dwFeature, const LPVOID lpValue );

//
// End SHSetInputContext
//
//------


//++++++
//
//@func HRESULT | SHGetInputContext | Retrieves current state of an input context feature
//
//@parm HWND    | hwnd      | IN - Window whose context will be retrieved
//@parm DWORD   | dwFeature | IN - Input context feature to retrieve
//@parm LPVOID  | lpValue   | OUT - Buffer to hold current value of feature
//@parm LPDWORD | pdwSize   | IN/OUT - size of the buffer passed in to retrieve the value
//
//@rdesc Returns one of the following values:
//@flag S_OK                        | If everything went well
//@flag ERROR_INVALID_PARAMETER     | If hwnd or lpdwSize passed were NULL
//@flag ERROR_NOT_SUPPORTED         | If the feature specified was invalid
//@flag ERROR_INSUFFICIENT_BUFFER   | If buffer passed is too small
//
//@comm Retrieves the current state/value of the specified
//      input context feature. If the value is not explicitly set, it
//      looks at the features set by the context class. If no class was
//      set explicitly, or the class didn't set that value, it returns
//      the default value for that feature, which would be the
//      currently active one.
//      If lpValue is NULL and lpdwSize is not NULL, it returns the
//      size of the buffer needed in lpdwSize.
//
//@xref <l Input_Context_Features> <f SHSetInputContext>
//

HRESULT SHGetInputContext( HWND hwnd, DWORD dwFeature, LPVOID lpValue, LPDWORD lpdwSize );


//
// End SHGetInputContext
//
//------


// }
//
// end Input Context API
//
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//
// SHUIMETRICS - UI Metrics that applications might be interested in
//
// These are definitions and APIs for Shell UI Metrics
//
// {

// Call RegisterWindowMessage on this string if you are interested in knowing when UI Metrics have changed
// wParam will be 0, lParam will be one of the SHUIMETRICTYPE values to indicate what value has changed.  
// Call SHGetUIMetrics to find out the new value if interested
#define SH_UIMETRIC_CHANGE    TEXT("SH_UIMETRIC_CHANGE")

// Enumeration of metrics you can ask for
typedef enum tagSHUIMETRIC
{
    SHUIM_INVALID = 0,    // Illegal

    // Note that you will receive a notification for SHUIM_FONTSIZE_POINT when one of these three values changes
    SHUIM_FONTSIZE_POINT,       // Application font size (hundredhts of a point) -- buffer is pointer to DWORD
    SHUIM_FONTSIZE_PIXEL,       // Application font size (in pixels) -- buffer is pointer to DWORD
    SHUIM_FONTSIZE_PERCENTAGE,  // Application font size as percentage of normal -- buffer is pointer to DWORD
} SHUIMETRIC;
 
HRESULT SHGetUIMetrics(SHUIMETRIC shuim, PVOID pvBuffer, DWORD cbBufferSize, DWORD *pcbRequired);

// }
//
// end Shell UI Metrics
//
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//
// SHNAPI - Shell Notification API
//
// These are definitions and APIs for the Shell Notifications system
//
// {

// notification priority

typedef enum _SHNP
{
    SHNP_INFORM = 0x1B1,      // bubble shown for duration, then goes away
    SHNP_ICONIC,              // no bubble, icon shown for duration then goes away
} SHNP;


// notification update mask
#define SHNUM_PRIORITY     0x0001
#define SHNUM_DURATION     0x0002
#define SHNUM_ICON         0x0004
#define SHNUM_HTML         0x0008
#define SHNUM_TITLE        0x0010
#define SHNUM_SOFTKEYS     0x0020
#define SHNUM_TODAYKEY     0x0040
#define SHNUM_TODAYEXEC    0x0080
#define SHNUM_SOFTKEYCMDS  0x0100
#define SHNUM_FLAGS        0x0200


// notification data

// Softkey Flags for use with SFTKEYNOTIFY structure

#define    NOTIF_SOFTKEY_FLAGS_DISMISS       0x0000  // Remove the notification when the softkey is pressed
#define    NOTIF_SOFTKEY_FLAGS_HIDE          0x0001  // Hide the notification when the softkey is pressed (but do not dismiss)
#define    NOTIF_SOFTKEY_FLAGS_STAYOPEN      0x0002  // Do not dismiss or hide the notification when the softkey is pressed
#define    NOTIF_SOFTKEY_FLAGS_SUBMIT_FORM   0x0004  // Submit the HTML form in the associated notification instead of sending WM_COMMAND to the sink
#define    NOTIF_SOFTKEY_FLAGS_DISABLED      0x0008  // This softkey is disabled

// Structure used to associate a command id from a notification's softkey bar 
// with a particular behaviour.

typedef struct _SOFTKEYCMD
{
    WPARAM wpCmd;                   // command to send with WM_COMMAND when pressed.
    DWORD grfFlags;                 // define special behaviour for this softkey as
                                    // per  NOTIF_SOFTKEY_FLAGS 
} SOFTKEYCMD;


// structure to define a single softkey for use in SHNOTIFICATIONDATA
typedef struct _SOFTKEYNOTIFY
{
    LPCTSTR pszTitle;        // Title to use on the softkey
    SOFTKEYCMD skc;          // Behaviour flags
} SOFTKEYNOTIFY;

// structure to define a menu for use in SHNOTIFICATIONDATA
typedef struct _SOFTKEYMENU
{
    HMENU   hMenu;
    SOFTKEYCMD *prgskc;    // optional array of SOFTKEYCMD values
    UINT    cskc;          // number of members of pskCmd 
} SOFTKEYMENU;


// number of soft keys on the notification soft key bar.
#define NOTIF_NUM_SOFTKEYS 2


typedef struct _SHNOTIFICATIONDATA
{
    DWORD        cbStruct;     // for verification and versioning
    DWORD        dwID;         // identifier for this particular notification
    SHNP         npPriority;   // priority
    DWORD        csDuration;   // duration of the notification (usage depends on prio)
    HICON        hicon;        // the icon for the notification
    DWORD        grfFlags;     // flags - see SHNF_ flags below
    CLSID        clsid;        // unique identifier for the notification class
    HWND         hwndSink;     // window to receive command choices, dismiss, etc.
    LPCTSTR      pszHTML;      // HTML content for the bubble
    LPCTSTR      pszTitle;     // Optional title for bubble
    LPARAM       lParam;       // User-defined parameter
    union
    {                            // Defines the softkey bar for the notification
        SOFTKEYMENU skm;        // Either pass an HMENU in skn (and set SHNF_HASMENU)
        SOFTKEYNOTIFY rgskn[NOTIF_NUM_SOFTKEYS];  // or two softkeys in rgskn. 
    };
    LPCTSTR    pszTodaySK;  // Text to put on SK2 on the Today screen. If NULL, will default to "Notification" 
    LPCTSTR    pszTodayExec;    // What to execute when SK2 is pressed. If NULL, the toast will be displayed.  

} SHNOTIFICATIONDATA;


// Flags

// For SHNP_INFORM priority and above, don't display the notification bubble
// when it's initially added; the icon will display for the duration then it
// will go straight into the tray.  The user can view the icon / see the
// bubble by opening the tray.
#define SHNF_STRAIGHTTOTRAY  0x00000001

// Critical information - highlights the border and title of the bubble.
#define SHNF_CRITICAL        0x00000002

// Force the message (bubble) to display even if settings says not to.
#define SHNF_FORCEMESSAGE    0x00000008

// Force the display to turn on for notification.
#define SHNF_DISPLAYON       0x00000010

// Force the notification to be silent and not vibrate, regardless of Settings
#define SHNF_SILENT          0x00000020

// Softkey bar is created from an HMENU passed in skm structure 
#define SHNF_HASMENU         0x00000040

// Draw the current time with the title
#define SHNF_TITLETIME       0x00000080

// A notification with "stack" support
#define SHNF_SPINNERS        0x00000100

// RE-play physical alerts on an update
#define SHNF_ALERTONUPDATE   0x00000200

//Capture the VK_TTALK button and forward it to the notification's sink window
#define SHNF_WANTVKTTALK     0x00000400

// notification message and codes for window-based notification
// the notification's dwID is in hdr.idFrom

typedef struct _NMSHN
{
    NMHDR   hdr;
    LPARAM lParam;
    DWORD dwReturn;
    union
    {
        LPCTSTR pszLink;
        BOOL    fTimeout;
        POINT   pt;
    };
} NMSHN;

#define SHNN_LINKSEL            (SHNN_FIRST-0)
// nmshn.pszLink contains the link text of the choice that was selected

#define SHNN_DISMISS            (SHNN_FIRST-1)
// nmshn.fTimeout is TRUE if duration expired, FALSE if user tapped away

#define SHNN_SHOW               (SHNN_FIRST-2)
// nmshn.pt contains the point to which the bubble points

#define SHNN_NAVPREV            (SHNN_FIRST-3)
// Toast stack left spinner clicked / DPAD LEFT

#define SHNN_NAVNEXT           (SHNN_FIRST-4)
// Toast stack right spinner clicked / DPAD RIGHT

#define SHNN_ACTIVATE           (SHNN_FIRST-5)
// Toast DPAD Action

#define SHNN_ICONCLICKED        (SHNN_FIRST-6)
// nmshn.pt contains the point where the user clicked

#define SHNN_HOTKEY             (SHNN_FIRST-7)
// A hotkey has been pressed - modifiers are in the loword of the nmshn.lParam, 
// the virtual key code is in the hiword.
// If the sink window returns 0 in response to this notification, then
// the notification toast will be hidden and VK_TTALK key default behavior
// will be performed.

//===========================================================================
//
// Interface: IShellNotificationCallback
//
//  The IShellNotificationCallback interface is used by the Shell to advise
// the notification owner of actions taken on the notification.
//
// [Member functions]
//
// IShellNotificationCallback::OnShow
//
//  Reserved.  Return E_NOTIMPL.
//
// IShellNotificationCallback::OnCommandSelected
//
//  This member function is called when the user selects a link of the form
// <A HREF="cmd:#">link</A>.
//
//  Parameters:
//   dwID       -- the identifier of the notification
//   wCmdID     -- this is the # in the link
//
// IShellNotificationCallback::OnLinkSelected
//
//  This member function is called when the user selects one of the action
// choice links in the notification bubble window.
//
//  Parameters:
//   dwID       -- the identifier of the notification
//   pszLink    -- the link content that was selected
//   lParam     -- the lParam of the notification
//
// IShellNotificationCallback::OnDismiss
//
//  This member function is called when the user taps away from the bubble
// window or if a SHNP_INFORM priority notification's duration expires.
//
//  Parameters:
//   dwID       -- the identifier of the notification
//   fTimeout   -- the notification timed out (SHNP_INFORM only)
//   lParam     -- the lParam of the notification
//   
//===========================================================================

#undef  INTERFACE
#define INTERFACE   IShellNotificationCallback

DECLARE_INTERFACE_(IShellNotificationCallback, IUnknown)
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    // *** IShellNotificationCallback methods ***
    STDMETHOD(OnShow)(THIS_ DWORD dwID, POINT pt, LPARAM lParam) PURE;
    STDMETHOD(OnCommandSelected)(THIS_ DWORD dwID, WORD wCmdID)
        PURE;
    STDMETHOD(OnLinkSelected)(THIS_ DWORD dwID, LPCTSTR pszLink, LPARAM lParam)
        PURE;
    STDMETHOD(OnDismiss)(THIS_ DWORD dwID, BOOL fTimeout, LPARAM lParam) PURE;
};


//++++++
//
// SHNotificationAdd
//
//   Add a notification.

LRESULT SHNotificationAdd(SHNOTIFICATIONDATA *pndAdd);

//
// End SHNotificationAdd
//
//------


//++++++
//
// SHNotificationUpdate
//
//   Update aspects of a pending notification.

LRESULT SHNotificationUpdate(DWORD grnumUpdateMask, SHNOTIFICATIONDATA *pndNew);

//
// End SHNotificationUpdate
//
//------

//++++++
//
// GetOpenFileNameEx
//
//  This function extends the GetOpenFileName provided by WinCE to support our Thumbnail view and provide support
//  for other extensoins (i.e. hide DRM content) not supported by defauly in WinCE

//
// Sort order
//
typedef enum tagOFN_SORTORDER
{
   OFN_SORTORDER_AUTO,
   OFN_SORTORDER_DATE,
   OFN_SORTORDER_NAME,
   OFN_SORTORDER_SIZE,
   OFN_SORTORDER_ASCENDING = 0x00008000

} OFN_SORTORDER;

//
// Extended Flags  
//
typedef enum tagOFN_EXFLAG
{
    OFN_EXFLAG_DETAILSVIEW          = 0x00000001,
    OFN_EXFLAG_THUMBNAILVIEW        = 0x00000002,
    OFN_EXFLAG_LOCKDIRECTORY        = 0x00000100,
    OFN_EXFLAG_NOFILECREATE         = 0x00000200,
    OFN_EXFLAG_HIDEDRMPROTECTED     = 0x00010000,     //If this flag is set and the DRM engine is installed - the PicturePicker will not show ANY DRM content
    OFN_EXFLAG_HIDEDRMFORWARDLOCKED = 0x00020000     //If this flag is set and the DRM engine is installed - the PicturePicker will not show ANY DRM FORWARD LOCK content
} OFN_EXFLAG;


typedef struct tagOPENFILENAMEEX
{
    // Fields which map to OPENFILENAME
   DWORD        lStructSize;
   HWND         hwndOwner;
   HINSTANCE    hInstance;
   LPCTSTR      lpstrFilter;
   LPTSTR       lpstrCustomFilter;
   DWORD        nMaxCustFilter;
   DWORD        nFilterIndex;
   LPTSTR       lpstrFile;
   DWORD        nMaxFile;
   LPTSTR       lpstrFileTitle;
   DWORD        nMaxFileTitle;
   LPCTSTR      lpstrInitialDir;
   LPCTSTR      lpstrTitle;
   DWORD        Flags;
   WORD         nFileOffset;
   WORD         nFileExtension;
   LPCTSTR      lpstrDefExt;
   LPARAM       lCustData;
   LPOFNHOOKPROC lpfnHook;
   LPCTSTR      lpTemplateName;

   // Extended fields
   DWORD       dwSortOrder;
   DWORD       ExFlags;
}OPENFILENAMEEX, *LPOPENFILENAMEEX ;


//
// The functions
//

BOOL GetOpenFileNameEx(LPOPENFILENAMEEX lpofnex);

//
// End GetOpenFileNameEX
//
//------


//++++++
//
// SHNotificationRemove
//
//   Remove a notification.  This is usually in reponse to some
//   action taken on the data outside of the notification system - for example
//   if a message is read or deleted.

LRESULT SHNotificationRemove(const CLSID *pclsid, DWORD dwID);

//
// End SHNotificationRemove
//
//------


//++++++
//
// SHNotificationGetData
//
//   Get the data for a notification.  Used by a handler to get information
//   stored in the notification by the poster.

LRESULT SHNotificationGetData(const CLSID *pclsid, DWORD dwID,
    SHNOTIFICATIONDATA *pndBuffer);

//
// End SHNotificationGetData
//
//------


// }
//
// end SHNAPI
//
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//
// SHIme - Shell IME API
//
// These are APIs are used by IMEs (Input Method Editors) to communicate
// with the Smartphone shell.
//
// 

typedef enum tagSHIME_MODE
{
    SHIME_MODE_NONE                = 0,
    SHIME_MODE_SPELL               = 1,
    SHIME_MODE_SPELL_CAPS          = 2,
    SHIME_MODE_SPELL_CAPS_LOCK     = 3,
    SHIME_MODE_AMBIGUOUS           = 4,
    SHIME_MODE_AMBIGUOUS_CAPS      = 5,
    SHIME_MODE_AMBIGUOUS_CAPS_LOCK = 6,
    SHIME_MODE_NUMBERS             = 7,
    SHIME_MODE_CUSTOM              = 8,
} SHIME_MODE;


// Notify the shell that the user pressed a key.  (The shell can use this notification
// to reset idle timers.)
HRESULT SHImeOnKeyPress(void);

// Get the message that should go in the IME translation buffer in
// order to clear the field the user is typing in.  Usually this is triggered
// by press-and-hold of the Back button.
HRESULT SHImeGetClearMessage(HIMC himc, UINT *pMsg, WPARAM *pWParam, LPARAM *pLParam);

// Give the shell a bitmap containing icons which the shell will use to draw
// the status icon when in a custom input mode.
// The bitmap is a monochrome mask, H pixels high by H*N pixels wide, where N
// is the number of icons, and H is 16 at 96DPI and 22 at 131DPI.
HRESULT SHImeSetCustomInputIcons(HBITMAP hbm);

// Set the status icon to the specified mode.
// If mode==SHIME_MODE_CUSTOM, then iCustomMode specifies the
// custom mode, which is also the index into the bitmap supplied 
// by SHImeSetCustomInputIcons().
HRESULT SHImeSetModeIcon(SHIME_MODE mode, DWORD iCustomMode);


//
// end SHIme
//
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//
// DrawFocusRectColor
//
// Draw a rectangle in the style and color used to indicate that the rectangle has
// the focus, respecting the current theme.
//

#define DFRC_FOCUSCOLOR             0  // Draw using focus color for current theme
#define DFRC_SELECTEDBRUSH          1  // Draw using selected brush (can be used for erasing)


BOOL DrawFocusRectColor(HDC hdc, const RECT* lprc, UINT uFlags);

//
// end DrawFocusRectColor
//
//////////////////////////////////////////////////////////////////////////////

// ExitWindowsEx : Shuts down the system.
//
BOOL ExitWindowsEx(
  UINT uFlags,       // shutdown operation
  DWORD dwReserved   // reserved
);
// Parameters
// uFlags 
//  [in] Specifies the type of shutdown. This parameter must be one of the following values. 
//  EWX_POWEROFF - Shuts down the system and turns off the power. (not supported on PocketPC)
//  EWX_REBOOT - Shuts down the system and reboots
// dwReserved 
//  [in] Reserved; this parameter is ignored.
// Return Values
//  If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. 
//
// Remarks
//  The ExitWindowsEx function returns as soon as it has initiated the shutdown. The shutdown then proceeds asynchronously. 

//////////////////////////////////////////////////////////////////////////////
//
// Flags for camera capture UI

typedef enum {
    CAMERACAPTURE_MODE_STILL = 0,
    CAMERACAPTURE_MODE_VIDEOONLY,
    CAMERACAPTURE_MODE_VIDEOWITHAUDIO,
} CAMERACAPTURE_MODE;

typedef enum {
    CAMERACAPTURE_STILLQUALITY_DEFAULT = 0,
    CAMERACAPTURE_STILLQUALITY_LOW,
    CAMERACAPTURE_STILLQUALITY_NORMAL,
    CAMERACAPTURE_STILLQUALITY_HIGH,
} CAMERACAPTURE_STILLQUALITY;

typedef enum {
    CAMERACAPTURE_VIDEOTYPE_ALL = 0xFFFF,
    CAMERACAPTURE_VIDEOTYPE_STANDARD = 1,
    CAMERACAPTURE_VIDEOTYPE_MESSAGING = 2,
} CAMERACAPTURE_VIDEOTYPES;

typedef struct tagSHCAMERACAPTURE
{
    DWORD                       cbSize;
    HWND                        hwndOwner;
    TCHAR                       szFile[MAX_PATH];   
    LPCTSTR                     pszInitialDir;
    LPCTSTR                     pszDefaultFileName;
    LPCTSTR                     pszTitle;
    CAMERACAPTURE_STILLQUALITY  StillQuality;
    CAMERACAPTURE_VIDEOTYPES    VideoTypes;
    DWORD                       nResolutionWidth;
    DWORD                       nResolutionHeight;
    DWORD                       nVideoTimeLimit;
    CAMERACAPTURE_MODE          Mode;
}SHCAMERACAPTURE, *PSHCAMERACAPTURE;


HRESULT SHCameraCapture(PSHCAMERACAPTURE pshcc);

//
// end of flags for camera capture UI
//
//////////////////////////////////////////////////////////////////////////////



#ifdef __cplusplus
}
#endif

#endif // __AYGSHELL_H__
