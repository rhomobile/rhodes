#pragma once

#include <windows.h>
#include "..\PB_ImageCapture_PLG\DShowCam.h"


class CVideoCaptureModule;

enum VC_RETURN 
{ 
	VC_SAVED, VC_FAILED 
};

enum VC_STATE
{
	STATE_INACTIVE = 0,
	STATE_PREVIEW,
	STATE_CAPTURE,
	STATE_STOPPING
};

#define VC_DEFAULT_DURATION 5000
#define VC_MIN_DURATION		500
#define VC_DEFAULT_FILENAME L"\\VideoCapture.wmv"


class CVideoCapture
{
public:
	CVideoCapture(HINSTANCE hInstance, HWND hParent, CVideoCaptureModule *pModule, bool bHasFocus);
	~CVideoCapture();

	BOOL StartPreview();
	void Start();
	void Stop();
	void Cancel();
	void SetDuration(int iMilliSeconds);
	LPWSTR SetFilename(LPCWSTR lpFilename);
	LPWSTR setString(LPCWSTR pStr);//use try/catch for mem fail
	BOOL ApplicationFocusChange(bool bActivated);
	bool IsCameraPresent();
private:
	
	static DWORD WINAPI stopTimerProc(LPVOID lpParam);
	static DWORD WINAPI transferProc(LPVOID lpParam);
	void FindCamera();
	
	CVideoCaptureModule *m_pModule;

	CDShowCam*		m_pDSCam;
	unsigned int	m_iMaxDuration;
	unsigned long	m_iMaxFileSizeBytes;
	LPWSTR			m_lpzFilename;
	HANDLE			m_hFile;
	SHCAMERACAPTURE m_shcc;
	VC_STATE		m_eState;
	RECT			m_rcWinPos;
	LPWSTR			m_szCamName;
	ImgFmt*			m_pCapsArray;
	HWND			m_hParent;
	HINSTANCE		m_hInstance;
	HWND			m_hWndViewer;
	bool			m_bAppHasFocus;
    bool            m_bCameraPresent;
	HANDLE			m_hStopEvent;
	CRITICAL_SECTION m_csLock;

};

BOOL InitDialog(HWND hwndDlg);
int StartCamera(HWND hwndDlg);
