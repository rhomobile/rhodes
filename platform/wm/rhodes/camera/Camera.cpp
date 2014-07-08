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

#include "stdafx.h"

#include <atltime.h>
#include "ext/rho/rhoruby.h"
#include "../MainWindow.h"
#include "Camera.h"
#include "common/RhodesApp.h"

#ifdef _MSC_VER
// warning C4800: 'int' : forcing to bool 'true' or 'false' (performance warning)
#pragma warning ( disable : 4800 )
#endif

typedef HRESULT (WINAPI* LPFN_CAMERA_CAPTURE_T)(PSHCAMERACAPTURE);
typedef BOOL (WINAPI* LPFN_GETOPEN_FILEEX_T)(LPOPENFILENAMEEX);
extern "C" LPFN_CAMERA_CAPTURE_T lpfn_Camera_Capture;
extern "C" LPFN_GETOPEN_FILEEX_T lpfn_GetOpen_FileEx;

extern "C" HWND getMainWnd();

//#if defined(_WIN32_WCE)

IMPLEMENT_LOGCLASS(Camera,"Camera");

Camera::Camera(void) {
}

Camera::~Camera(void) {
}

HRESULT Camera::takePicture(HWND hwndOwner,LPTSTR pszFilename) 
{
    HRESULT         hResult = S_OK;
#if defined(_WIN32_WCE) //&& !defined( OS_PLATFORM_MOTCE )
	if(RHO_IS_WMDEVICE)
	{
		SHCAMERACAPTURE shcc;

		StringW imageDir;
		convertToStringW(rho_rhodesapp_getblobsdirpath(), imageDir);

		StringW strFileName = generate_filename(L".jpg");

		// Set the SHCAMERACAPTURE structure.
		ZeroMemory(&shcc, sizeof(shcc));
		shcc.cbSize             = sizeof(shcc);
		shcc.hwndOwner          = hwndOwner;
		shcc.pszInitialDir      = imageDir.c_str();
		shcc.pszDefaultFileName = strFileName.c_str();
		shcc.pszTitle           = TEXT("Camera");
		shcc.VideoTypes			= CAMERACAPTURE_VIDEOTYPE_MESSAGING;
		shcc.nResolutionWidth   = 176;
		shcc.nResolutionHeight  = 144;
		shcc.StillQuality       = CAMERACAPTURE_STILLQUALITY_LOW;
		shcc.nVideoTimeLimit    = 15;
		shcc.Mode               = CAMERACAPTURE_MODE_STILL;

		// Display the Camera Capture dialog.
		hResult = lpfn_Camera_Capture(&shcc);

		// The next statements will execute only after the user takes
		// a picture or video, or closes the Camera Capture dialog.
		if (S_OK == hResult) 
		{
			LOG(INFO) + "takePicture get file: " + shcc.szFile;

			LPTSTR fname = get_file_name( shcc.szFile, imageDir.c_str() );
			if (fname) {

				StringCchCopy( pszFilename, MAX_PATH, fname );
				free(fname);
			} else {
				LOG(ERROR) + "takePicture error get file: " + shcc.szFile;

				hResult = E_INVALIDARG;
			}
		}else
		{
			LOG(ERROR) + "takePicture failed with code: " + LOGFMT("0x%X") + hResult;
		}
	}
#endif //_WIN32_WCE

    return hResult;
}

HRESULT Camera::selectPicture(HWND hwndOwner,LPTSTR pszFilename) 
{
	RHO_ASSERT(pszFilename);
#if defined( _WIN32_WCE ) //&& !defined( OS_PLATFORM_MOTCE )
	OPENFILENAMEEX ofnex = {0};
	OPENFILENAME ofn = {0};
#else
    OPENFILENAME ofn = {0};
#endif
    pszFilename[0] = 0;

	ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = hwndOwner;
	ofn.lpstrFilter     = NULL;
	ofn.lpstrFile       = pszFilename;
	ofn.nMaxFile        = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle      = _T("Select an image");
#if defined( _WIN32_WCE ) //&& !defined( OS_PLATFORM_MOTCE )
	BOOL bRes = false;
	if(RHO_IS_WMDEVICE)
	{
		ofnex.ExFlags = OFN_EXFLAG_THUMBNAILVIEW|OFN_EXFLAG_NOFILECREATE|OFN_EXFLAG_LOCKDIRECTORY;
		bRes = lpfn_GetOpen_FileEx(&ofnex);
	}
	else
		bRes = GetOpenFileName(&ofn);

    if (bRes)
#else
    if (GetOpenFileName(&ofn))
#endif

    {
		HRESULT hResult = S_OK;

        /*
		TCHAR rhoroot[MAX_PATH];
		wchar_t* root  = wce_mbtowc(rho_rhodesapp_getblobsdirpath());
		wsprintf(rhoroot,L"%s",root);
		free(root);

		create_folder(rhoroot);*/

        StringW strBlobRoot = convertToStringW( RHODESAPP().getBlobsDirPath() );

        LPCTSTR szExt = wcsrchr(pszFilename, '.');
		StringW strFileName = generate_filename(szExt);
		StringW strFullName = strBlobRoot + L"\\" + strFileName;

		if (copy_file( pszFilename, strFullName.c_str() )) 
        {
			wcscpy( pszFilename, strFileName.c_str() );
		} else {
			hResult = E_INVALIDARG;
		}

		return hResult;
	} else if (GetLastError()==ERROR_SUCCESS) {
		return S_FALSE; //user cancel op
	}
	return E_INVALIDARG;
}

bool Camera::copy_file(LPCTSTR from, LPCTSTR to) 
{
	RHO_ASSERT(from);
	RHO_ASSERT(to);
/*
	SHFILEOPSTRUCT SHFileOp;
	ATL::CString source(from);
	ATL::CString destination(to);

	// add required string terminators
	source+=_T("\0\0");
	destination+=_T("\0\0");

	// set up File Operation structure
	ZeroMemory(&SHFileOp, sizeof(SHFILEOPSTRUCT));
	SHFileOp.hwnd = NULL;
	SHFileOp.wFunc = FO_COPY;
	SHFileOp.pFrom = source;
	SHFileOp.pTo = destination;
	SHFileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
*/
	//if(SHFileOperation(&SHFileOp) != 0) {
    if ( !CopyFile(from, to, TRUE) )
    {
        DWORD dwErr = GetLastError();
		return false;
	}
	return true;
}

LPTSTR Camera::get_file_name(LPCTSTR from, LPCTSTR to) 
{
	int len; 
    StringW destinationString = convertToStringW(from);
	size_t found;
	len = destinationString.find_last_of('\\');
	LOG(INFO) + "Test Check: " + len;
	StringW name = destinationString.substr(len+1);
	int length1 = wcslen(name.c_str());
    wchar_t* returnName = (wchar_t*) malloc(length1*sizeof(wchar_t)+1); 
	wcscpy(returnName,name.c_str());
     len = wcslen(to);
	if (wcsncmp( to, from, len )!=0) 
    {
        StringW strPathTo = to;
        strPathTo += L"\\";
        strPathTo += name;

        if ( !copy_file( from, strPathTo.c_str() ) )
            return 0;
    } 
    if (wcsncmp( to, from, len )!=0) 
	{
		if( DeleteFile(from) != 0 )
    LOG(INFO) + "File Deletion Failed " ;
  else
    LOG(INFO) + "File Deleted Successfull " ;
	}

	return returnName;
}

StringW Camera::generate_filename(LPCTSTR szExt) 
{
    TCHAR filename[256];

	CTime time(CTime::GetCurrentTime());
	tm tl, tg;
	time.GetLocalTm(&tl);
	time.GetGmtTm(&tg);
	int tz = tl.tm_hour-tg.tm_hour; //TBD: fix tz

    wsprintf(filename, L"Image_%02i-%02i-%0004i_%02i.%02i.%02i_%c%03i%s", 
		tg.tm_mon, tg.tm_mday, 1900+tg.tm_year, tg.tm_hour, tg.tm_min, tg.tm_sec,  
        tz>0?'_':'-',abs(tz),(szExt?szExt:L""));

	return filename;
}

//#endif //_WIN32_WCE

void choose_picture(char* callback_url, rho_param *options_hash) {
//#if defined(_WIN32_WCE)
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd,WM_SELECTPICTURE,0,(LPARAM)strdup(callback_url));
//#endif
}

void take_picture(char* callback_url, rho_param * options_hash) {
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd,WM_TAKEPICTURE,0,(LPARAM)strdup(callback_url));
}

VALUE get_camera_info(const char* camera_type) {
     return rho_ruby_get_NIL();
}

void save_image_to_device_gallery(const char* image_path, rho_param* options_hash) {

}
