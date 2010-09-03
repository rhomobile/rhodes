#include "stdafx.h"

#if defined(_WIN32_WCE)
#include <aygshell.h>
#endif
#include <atltime.h>
#include "ext/rho/rhoruby.h"
#include "../MainWindow.h"
#include "Camera.h"
#include "common/RhodesApp.h"

#ifdef _MSC_VER
// warning C4800: 'int' : forcing to bool 'true' or 'false' (performance warning)
#pragma warning ( disable : 4800 )
#endif

extern "C" HWND getMainWnd();

#if defined(_WIN32_WCE)
// strdup is implemented as part of ruby CE port
extern "C" char *strdup(const char * str);
extern "C" wchar_t* wce_mbtowc(const char* a);
#endif

#if defined(_WIN32_WCE)

static bool copy_file(LPTSTR from, LPTSTR to);
static LPTSTR get_file_name(LPTSTR from, LPTSTR to);
static LPTSTR generate_filename(LPTSTR filename, LPCTSTR szExt );
static void create_folder(LPTSTR Path);

Camera::Camera(void) {
}

Camera::~Camera(void) {
}

HRESULT Camera::takePicture(HWND hwndOwner,LPTSTR pszFilename) {
    HRESULT         hResult;
    SHCAMERACAPTURE shcc;

    wchar_t* root  = wce_mbtowc(rho_rhodesapp_getblobsdirpath());
    wsprintf(pszFilename,L"%s",root );
    free(root);

	create_folder(pszFilename);

    //LPCTSTR szExt = wcsrchr(pszFilename, '.');
    TCHAR filename[256];
	generate_filename(filename,L".jpg");

    // Set the SHCAMERACAPTURE structure.
    ZeroMemory(&shcc, sizeof(shcc));
    shcc.cbSize             = sizeof(shcc);
    shcc.hwndOwner          = hwndOwner;
    shcc.pszInitialDir      = pszFilename;
    shcc.pszDefaultFileName = filename;
    shcc.pszTitle           = TEXT("Camera");
    shcc.VideoTypes			= CAMERACAPTURE_VIDEOTYPE_MESSAGING;
    shcc.nResolutionWidth   = 176;
    shcc.nResolutionHeight  = 144;
    shcc.nVideoTimeLimit    = 15;
    shcc.Mode               = CAMERACAPTURE_MODE_STILL;

    // Display the Camera Capture dialog.
    hResult = SHCameraCapture(&shcc);

    // The next statements will execute only after the user takes
    // a picture or video, or closes the Camera Capture dialog.
    if (S_OK == hResult) {
        LPTSTR fname = get_file_name(shcc.szFile,pszFilename);
		if (fname) {
			StringCchCopy(pszFilename, MAX_PATH, fname);
			free(fname);
		} else {
			hResult = E_INVALIDARG;
		}
    }

    return hResult;
}

HRESULT Camera::selectPicture(HWND hwndOwner,LPTSTR pszFilename) {
	RHO_ASSERT(pszFilename);
	OPENFILENAMEEX ofn = {0};

	ofn.lStructSize     = sizeof(ofn);
	ofn.lpstrFilter     = NULL;
	ofn.lpstrFile       = pszFilename;
	ofn.nMaxFile        = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle      = _T("Select an image");
	ofn.ExFlags         = OFN_EXFLAG_THUMBNAILVIEW|OFN_EXFLAG_NOFILECREATE|OFN_EXFLAG_LOCKDIRECTORY;

	if (GetOpenFileNameEx(&ofn)) {
		HRESULT hResult = S_OK;

		TCHAR rhoroot[MAX_PATH];
		wchar_t* root  = wce_mbtowc(rho_rhodesapp_getblobsdirpath());
		wsprintf(rhoroot,L"%s",root);
		free(root);

		create_folder(rhoroot);

        LPCTSTR szExt = wcsrchr(pszFilename, '.');
		TCHAR filename[256];
		generate_filename(filename, szExt);
		
		int len = wcslen(rhoroot) + wcslen(L"\\") + wcslen(filename);
		wchar_t* full_name = (wchar_t*) malloc((len+2)*sizeof(wchar_t));
		wsprintf(full_name,L"%s\\%s",rhoroot,filename);

		if (copy_file(pszFilename,full_name)) {
			StringCchCopy(pszFilename, MAX_PATH, filename);	
		} else {
			hResult = E_INVALIDARG;
		}

		free(full_name);

		return hResult;
	} else if (GetLastError()==ERROR_SUCCESS) {
		return S_FALSE; //user cancel op
	}
	return E_INVALIDARG;
}

bool copy_file(LPTSTR from, LPTSTR to) {
	RHO_ASSERT(from);
	RHO_ASSERT(to);

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

	if(SHFileOperation(&SHFileOp) != 0) {
		return false;
	}
	return true;
}

LPTSTR get_file_name(LPTSTR from, LPTSTR to) {
	int len = wcslen(to);
	if (wcsncmp(to,from,len)==0) {
		LPTSTR fname = from+len;
		if ( (wcsncmp(L"\\",fname,1)==0) || 
			 (wcsncmp(L"/",fname,1)==0) ) {
			fname++;
		}
		len = wcslen(fname);
		wchar_t* name = (wchar_t*) malloc(len*sizeof(wchar_t)+1);
		wcscpy(name,fname);
		return name;
	} else {
		return NULL;
	}
}

LPTSTR generate_filename(LPTSTR filename, LPCTSTR szExt) {
	RHO_ASSERT(filename);

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

void create_folder(LPTSTR Path)
{
	TCHAR DirName[256];
	LPTSTR p = Path;
	LPTSTR q = DirName; 
	while(*p)
	{
		if (('\\' == *p) || ('/' == *p))
		{
			if (':' != *(p-1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = '\0';
	}
	CreateDirectory(DirName, NULL);
}

#endif //_WIN32_WCE

void take_picture(char* callback_url) {
//#if defined(_WIN32_WCE)
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd,WM_TAKEPICTURE,0,(LPARAM)strdup(callback_url));
//#endif
}

void choose_picture(char* callback_url) {
//#if defined(_WIN32_WCE)
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd,WM_SELECTPICTURE,0,(LPARAM)strdup(callback_url));
//#endif
}

