//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define DTM_ADDTEXT             (WM_USER + 101)
#define DTM_ADDTEXTW            (WM_USER + 102)
#define DTM_SETIMAGE            (WM_USER + 103)
#define DTM_ENDOFSOURCE         (WM_USER + 104)
#define DTM_ANCHOR              (WM_USER + 105)
#define DTM_ANCHORW             (WM_USER + 106)
#define DTM_ENABLESHRINK        (WM_USER + 107)
#define DTM_FITTOWINDOW         (WM_USER + 107)
#define DTM_SCROLLINTOVIEW      (WM_USER + 108)
#define DTM_IMAGEFAIL           (WM_USER + 109)
#define DTM_ENABLECONTEXTMENU   (WM_USER + 110)
#define DTM_SELECTALL           (WM_USER + 111)
#define DTM_ISSELECTION         (WM_USER + 112)
#define DTM_CLEAR               (WM_USER + 113)
#define DTM_ENABLECLEARTYPE     (WM_USER + 114)
#define DTM_ENABLESCRIPTING     (WM_USER + 115)
#define DTM_ZOOMLEVEL           (WM_USER + 116)
#define DTM_LAYOUTWIDTH         (WM_USER + 117)
#define DTM_LAYOUTHEIGHT        (WM_USER + 118)
#define DTM_COPYSELECTIONTONEWISTREAM   (WM_USER + 119)
#define DTM_NAVIGATE            (WM_USER + 120)
#define DTM_INTEGRALPAGING      (WM_USER + 121)
#define DTM_SCRIPTDISPATCH      (WM_USER + 122)
#define DTM_DOCUMENTDISPATCH    (WM_USER + 123)
#define DTM_BROWSERDISPATCH     (WM_USER + 124)
#define DTM_STOP                (WM_USER + 125)
#define DTM_ADDSTYLE            (WM_USER + 126)


#define NM_HOTSPOT              (WM_USER + 101)
#define NM_INLINE_IMAGE         (WM_USER + 102)
#define NM_INLINE_SOUND         (WM_USER + 103)
#define NM_TITLE                (WM_USER + 104)
#define NM_META                 (WM_USER + 105)
#define NM_BASE                 (WM_USER + 106)
#define NM_CONTEXTMENU          (WM_USER + 107)
#define NM_INLINE_XML           (WM_USER + 108)
#define NM_BEFORENAVIGATE       (WM_USER + 109)
#define NM_DOCUMENTCOMPLETE     (WM_USER + 110)
#define NM_NAVIGATECOMPLETE     (WM_USER + 111)
#define NM_TITLECHANGE          (WM_USER + 112)
#define NM_INLINE_STYLE         (WM_USER + 113)
#define NM_INLINE_FRAME         (WM_USER + 114)
#define NM_INLINE_IFRAME        (WM_USER + 115)
#define NM_IMAGE_FAILED         (WM_USER + 116)

#define DISPLAYCLASS    TEXT("DISPLAYCLASS")
#define WC_HTML         DISPLAYCLASS

#define HS_NOFITTOWINDOW        0x0001  // Disable "fit to window" feature
#define HS_CONTEXTMENU          0x0002  // Enable default context menus
#define HS_CLEARTYPE            0x0004  // Enable ClearType
#define HS_NOSCRIPTING          0x0008  // Disable scripting
#define HS_INTEGRALPAGING       0x0010  // Enable "ingeral paging" feature (last 'screen' always the height of the control)
#define HS_NOSCROLL             0x0020  // Disable scrolling
#define HS_NOIMAGES             0x0040  // Disable loading of images
#define HS_NOSOUNDS             0x0080  // Disable loading and playing sounds
#define HS_NOACTIVEX            0x0100  // Disable ActiveX controls
#define HS_NOSELECTION          0x0200  // Disable content selection
#define HS_NOFOCUSRECT          0x0400  // Disable the focus rectangle
#define HS_NOPUMPMESSAGES       0x8000  // Don't allow HTML control to pump messages on load

#define FRAME_SCROLLING_AUTO    1
#define FRAME_SCROLLING_YES     2
#define FRAME_SCROLLING_NO      3

// DTM_NAVIGATE flags
#define NAVIGATEFLAG_REFRESH            0x0020
#define NAVIGATEFLAG_RELATIVE           0x0040
#define NAVIGATEFLAG_ENTERED            0x0080
#define NAVIGATEFLAG_INTRADOCUMENT      0x0100
#define NAVIGATEFLAG_IGNORETARGET       0x0200
#define NAVIGATEFLAG_GETFROMCACHE       0x0400
#define NAVIGATEFLAG_NOCACHE            0x1000
#define NAVIGATEFLAG_RESYNCHRONIZE      0x2000
#define NAVIGATEFLAG_RELOAD             0x4000

EXTERN_C BOOL InitHTMLControl(HINSTANCE hinst);

typedef struct tagNM_HTMLVIEWA
{
    NMHDR   hdr;
    LPCSTR  szTarget;
    LPCSTR  szData;

    union
    {
        DWORD dwCookie;
        DWORD dwFlags;
    };

    LPCSTR  szExInfo;
} NM_HTMLVIEWA;

typedef struct tagNM_HTMLVIEWW
{
    NMHDR   hdr;
    LPCWSTR szTarget;
    LPCWSTR szData;

    union
    {
        DWORD dwCookie;
        DWORD dwFlags;
    };

    LPCWSTR szExInfo;
} NM_HTMLVIEWW;

#ifdef UNICODE
#define NM_HTMLVIEW NM_HTMLVIEWW
#else
#define NM_HTMLVIEW NM_HTMLVIEWA
#endif  // UNICODE

typedef struct tagINLINEIMAGEINFO
{
    DWORD       dwCookie;
    int         iOrigHeight;
    int         iOrigWidth;
    HBITMAP     hbm;
    BOOL        bOwnBitmap;
} INLINEIMAGEINFO;

#define HTMLCONTEXT_BACKGROUND          0x00
#define HTMLCONTEXT_LINK                0x01
#define HTMLCONTEXT_IMAGE               0x02
#define HTMLCONTEXT_IMAGENOTLOADED      0x04
#define HTMLCONTEXT_TEXT                0x08

typedef struct tagNM_HTMLCONTEXT
{
    NMHDR           hdr; 
    POINT           pt;
    UINT            uTypeFlags;
    LPTSTR          szLinkHREF;
    LPTSTR          szLinkName;
    DWORD           dwReserved1;
    DWORD           dwImageCookie;
    DWORD           dwReserved2;
} NM_HTMLCONTEXT;
