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

#pragma once

#include "logging/RhoLog.h"

class Camera
{
    DEFINE_LOGCLASS;

public:
	Camera(void);
	virtual ~Camera(void);

public: 
	HRESULT takePicture(HWND hwndOwner,LPTSTR pszFilename);
 	HRESULT selectPicture(HWND hwndOwner,LPTSTR pszFilename);

private:
    StringW generate_filename(LPCTSTR szExt );
    bool copy_file(LPCTSTR from, LPCTSTR to);
    LPTSTR get_file_name(LPCTSTR from, LPCTSTR to);

};

extern "C" void choose_picture(char* callback_url, rho_param *options_hash);
extern "C" void take_picture(char* callback_url, rho_param * options_hash);
extern "C" VALUE get_camera_info(const char* camera_type);
extern "C" void save_image_to_device_gallery(const char* image_path, rho_param* options_hash);

#if defined(_WIN32_WCE)

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

typedef enum tagOFN_EXFLAG
{
    OFN_EXFLAG_EXPLORERVIEW                      = 0x00000000,
    OFN_EXFLAG_DETAILSVIEW                       = 0x00000001,
    OFN_EXFLAG_THUMBNAILVIEW                     = 0x00000002,
    OFN_EXFLAG_MESSAGING_FILE_CREATE             = 0x00000004,
    OFN_EXFLAG_CAMERACAPTURE_MODE_VIDEOONLY      = 0x00000008,
    OFN_EXFLAG_CAMERACAPTURE_MODE_VIDEOWITHAUDIO = 0x00000010,
    OFN_EXFLAG_CAMERACAPTURE_MODE_VIDEODEFAULT   = 0x00000020,
    OFN_EXFLAG_LOCKDIRECTORY                     = 0x00000100,
    OFN_EXFLAG_NOFILECREATE                      = 0x00000200,
    OFN_EXFLAG_HIDEDRMPROTECTED                  = 0x00010000,     //If this flag is set and the DRM engine is installed - the PicturePicker will not show ANY DRM content
    OFN_EXFLAG_HIDEDRMFORWARDLOCKED              = 0x00020000     //If this flag is set and the DRM engine is installed - the PicturePicker will not show ANY DRM FORWARD LOCK content
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

#endif
