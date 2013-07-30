/**
 * \file Imager.cpp
 * \brief Implementation of the Imager Module Plugin
 * CImager is an abstraction of the Image Capture functionality supported by
 * Motorola Devices.
 */

// SPS
//#include "Error.h"
#include "DShowCam.h"
#include "Imager.h"
//#include "ImagerMod.h"
#include "wininet.h"
#include <sstream>
// SPS

#include <windows.h>

// SPS  - taken from error.h
#pragma once
/**
 * General error codes
 */

#define WM_STOPVIEWER (WM_APP + 500)

enum errType
{
	SUCCESS = 0,				
	ERR,
	MEM_ALLOC_ERR,			///< error dynamically allocating memory
	FILE_ERR,				
	ERR_DLL_LOAD,			///< error dynamically loading a dll
	DLL_ERR,
	TIMEOUT_ERR,
	NULL_HANDLE_ERR,
	NULL_PTR,
	BAD_PARAM,
	XML_ERR,				///< a XML syntax error
	THREAD_ERR,
	WND_DIM_ERR
};
// SPS

// SPS - taken from PBUtils.cpp
static const char base64_chars[] = 
						 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						 "abcdefghijklmnopqrstuvwxyz"
						 "0123456789+/";

//  Encodes the specified byte array into base64 encoding, this is used for
//  DataURI encoding.
//  szEncoded data MUST be allocated by the caller and must be of an appropriate
//  length, note: base 64 encoded data is about 1/3 larger than the byte equivilent
//  iLen is the Length of the bytes to be encoded.
static void EncodeToBase64(unsigned char const* bytes_to_encode, unsigned int iLen, char* szEncodedData)
{
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (iLen--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) 
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
			{
				*szEncodedData = base64_chars[char_array_4[i]];
				szEncodedData++;
			}
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
		{
			*szEncodedData = base64_chars[char_array_4[j]];
			szEncodedData++;
		}

		while((i++ < 3))
		{
			*szEncodedData = '=';
			szEncodedData++;
		}
	}
}
// SPS

//Flag to indicate transfer mode use for image file transfer. TRUE-HTTP transfer else FTP transfer
extern BOOL m_bHttpTransfer;

//Vaiable to hold FTP server password
//extern TCHAR			m_tcImagerPassword[MAX_PATH];
//variable to hold FTP user name 
//extern TCHAR			m_tcImagerUsername[MAX_PATH];

//Buffer to hold Navigation strings
//TCHAR m_IMGNav[2050];
//URL to navigate to for file transfer
TCHAR m_ImagerURL[2050];

//Navigation address passed by user
TCHAR m_NavAddress[2050];

//Not used - delete
HWND g_hParent=NULL;

//List of Image Capture devices available, only one device available as of now
//Color camera to be added in the future.
extern PListDevName g_ListDevName;

//#define STATIC_DSCAM

#ifdef STATIC_DSCAM
CDShowCam g_DSCam;
#endif

//CImagerMod instance which creates CImager instances and an interface between COre and CImager
// SPS
// extern CImagerMod *g_ImagerMod;
// SPS
#ifdef LOG_TO_FILE
extern FILE *LogFile;
#endif
//Message Queue DATA structure
//were Used for color camera.
#define MAX_METALENGTH 30
//#define MAX_URL	2050

struct MetaTagQEntry
{
	TCHAR parameter[MAX_METALENGTH+1];
	TCHAR Value[MAX_URL+1];
};


////////////////////////////////////////////////////////////////////////
// Function:	CImager
// Description:	CImager constructor - Variable initialization done Here, 
//              Loads Image capture Library, Initializes Image Captue API function pointers.
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
//SPS
//CImager::CImager(HINSTANCE hInstance,HWND hParent,int iInstanceID, PBModule* pModule)
//	:m_iInstanceID(iInstanceID)
CImager::CImager(HINSTANCE hInstance,HWND hParent,int iInstanceID) :m_iInstanceID(iInstanceID)
//SPS
{
#ifdef LOG_TO_FILE
	LogFile = 	fopen("CameraLog2.txt","w");
#endif
	LOG(INFO) + "3000:CImager constructor ...";

    m_bAppHasFocus = TRUE;
    m_szCaptureSound = NULL;
	m_pRcm = new CRcmLoader();
	if (m_pRcm && m_pRcm->LoadRcmDLL())
		m_bRcmLoaded = true;
	else
		m_bRcmLoaded = false;
    m_hTriggerNotification = NULL;
    m_hTriggerEvent = NULL;
    m_hTriggerQuit = NULL;
    m_hTriggerThread = NULL;
	m_PreviewOn = FALSE;
	m_bImagerLoadFailed = true;
	m_bDisableDuringNavigate = true;  //  Default behaviour is to disable during navigate
	hModule = LoadLibraryEx(_T("ImgAPI32.dll"), NULL, 0);
	if (hModule != NULL) {
		m_bImagerLoadFailed = false;
		if (NULL == (Image_FindFirst = (IMAGE_FINDFIRSTPROC) GetProcAddress(hModule, _T("IMAGE_FindFirst")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_FindNext = (IMAGE_FINDNEXTPROC) GetProcAddress(hModule, _T("IMAGE_FindNext")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_FindClose = (IMAGE_FINDCLOSEPROC) GetProcAddress(hModule, _T("IMAGE_FindClose")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_Open = (IMAGE_OPENPROC) GetProcAddress(hModule, _T("IMAGE_Open")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_Close = (IMAGE_CLOSEPROC) GetProcAddress(hModule, _T("IMAGE_Close")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_StartAcquisition = (IMAGE_STARTACQUISITIONPROC) GetProcAddress(hModule, _T("IMAGE_StartAcquisition")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_StopAcquisition = (IMAGE_STOPACQUISITIONPROC) GetProcAddress(hModule, _T("IMAGE_StopAcquisition")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_StartViewfinder = (IMAGE_STARTVIEWFINDERPROC) GetProcAddress(hModule, _T("IMAGE_StartViewfinder")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_StopViewfinder = (IMAGE_STOPVIEWFINDERPROC) GetProcAddress(hModule, _T("IMAGE_StopViewfinder")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_GetImage = (IMAGE_GETIMAGEPROC) GetProcAddress(hModule, _T("IMAGE_GetImage")))) m_bImagerLoadFailed = true; 		
		if (NULL == (Image_SetCapCurrValue = (IMAGE_SETCAPCURRVALUEPROC) GetProcAddress(hModule, _T("IMAGE_SetCapCurrValue")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_UnLockImager = (IMAGE_UNLOCKIMAGERPROC) GetProcAddress(hModule, _T("IMAGE_UnLockImager")))) m_bImagerLoadFailed = true;
	}
	m_bTransfer = FALSE;
	m_lpSzFilename = NULL;
	m_lpSzFiletype[0] = (TCHAR)'\0';
	m_hImager = INVALID_HANDLE_VALUE;

	//m_rcWinPos.left = GetSystemMetrics(SM_CXSCREEN)/2-50;
	//m_rcWinPos.right = GetSystemMetrics(SM_CXSCREEN)/2+50;
	//m_rcWinPos.top = GetSystemMetrics(SM_CYSCREEN)/2-50;
	//m_rcWinPos.bottom = GetSystemMetrics(SM_CYSCREEN)/2+50;

	// SPS
	// Screen is portrait while camera is lansacape, convert viewer to fit 
	// while retaining aspect ratio
	m_rcWinPos.left = 0;
	m_rcWinPos.top = 0;
	//m_rcWinPos.bottom = GetSystemMetrics(SM_CYSCREEN) -100;
	//m_rcWinPos.right = GetSystemMetrics(SM_CXSCREEN);
	m_rcWinPos.bottom = 360;	// 480 * (480/640)
	m_rcWinPos.right = 480; 	// 640 * (480/640)

	m_iDesiredWidth  = m_rcWinPos.right;
	m_iDesiredHeight = m_rcWinPos.bottom;
	// SPS

	m_ImgNo = 1;
	//m_bHttpTransfer = TRUE;
	//memset(m_tcImagerPassword, 0, sizeof(TCHAR) * MAX_PATH); 
	//memset(m_tcImagerUsername, 0, sizeof(TCHAR) * MAX_PATH); 
	// SPS
	m_hInst=hInstance;
	m_hParentWnd=hParent;
	m_hWndMessageWindow=NULL;

	// SPS

	m_bImgLight = FALSE;//default state
	m_bImgAim = FALSE; //default state

	//Initialize Msg Queues
	m_MetamsgqOpt.dwSize		= sizeof(m_MetamsgqOpt);
	m_MetamsgqOpt.dwFlags		= MSGQUEUE_ALLOW_BROKEN;
	m_MetamsgqOpt.dwMaxMessages = 0;
	m_MetamsgqOpt.cbMaxMessage	= sizeof(MetaTagQEntry);
	m_MetamsgqOpt.bReadAccess	= TRUE;
	m_hMetaMsgQRead				= NULL;
	m_hMetaMsgQRead				= CreateMsgQueue(L"ImagerMetaMsgQ", &m_MetamsgqOpt);
	m_MetamsgqOpt.bReadAccess	= FALSE;
	m_hMetaMsgQWrite			= NULL;
	m_hMetaMsgQWrite			= CreateMsgQueue(L"ImagerMetaMsgQ", &m_MetamsgqOpt);
	m_iDesiredWidth				= -1;
	m_iDesiredHeight			= -1;
	
	//Initialize dev name to default dev
	wcscpy(m_szDevName,L"IMG1:");

	//Initialize it to ZERO to indicate no navigate string
	memset(m_EnumIMGNav,0,sizeof(m_EnumIMGNav));

	//Create DirectShow camera object
#ifdef STATIC_DSCAM
	m_pDSCam = &g_DSCam;
#else
	m_pDSCam = new CDShowCam();

#endif
	
	//Create list of available imager devices on terminal.
	//CreateListOfAvailableDevices();

#ifndef STATIC_DSCAM

	LOG(INFO) + "3020:Deleting m_pDSCam ...";

	// CDShowCam only used to allow device list to be created so we can
	// get rid of it now to save memory.
	if (m_pDSCam) delete m_pDSCam;
	m_pDSCam = NULL;

#endif
	
	//Check that default dev exists otherwise select the first exisiting device
	ListDevName *pDevName = g_ListDevName;
	while (pDevName)
	{
		if (wcscmp(pDevName->DeviceName, m_szDevName) == 0)
			break;
		pDevName = pDevName->pNext;
	}
	if ((!pDevName) && (g_ListDevName))
	{
		wcsncpy(m_szDevName,g_ListDevName->DeviceName, 50);
	}
	m_szImagerCaptureURI = NULL;

	// SPS - Look into how config is handled in Rhodes - TBD
	rho::String confSetting = RHOCONF().getString("Imager\\DisableImagerDuringNavigation");
	// SPS

    if (confSetting.compare("0")==0)
		m_bDisableDuringNavigate = false;
	else
		m_bDisableDuringNavigate = true;

	LOG(INFO) + "3099:CImager created ok.";

	m_bSaveAsFile = true;
}

// Define the WindowProc to be used by this Imager.
LRESULT CALLBACK CImager::MessageWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	stringstream ss;
	CImager *lImager;

	ss << "HWND: " << hWnd << " Processing message: " << hex << Msg;

	LOG(INFO) + ss.str();

    switch(Msg)
    {
		case WM_STOPVIEWER:
			LOG(INFO) + __FUNCTION__ + "Processing WM_STOPVIEWER message.";
			lImager = (CImager *)GetWindowLong(hWnd, GWL_USERDATA);

			lImager->StopViewer();
			break;

		case WM_PAINT:
			break;

		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    return 0;
}

bool CImager::onWndMsg(MSG& oMsg)
{
	bool handled = false;
	std::stringstream ss;
	ss << "Received message " << oMsg.message;
	LOG(INFO) + __FUNCTION__ + ss.str();

	if (oMsg.message == WM_STOPVIEWER)
	{
		LOG(INFO) + __FUNCTION__ + "Processing WM_STOPVIEWER in CImager";
		handled = true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////
// Function:	~CImager
// Description:	CImager Destructor - Viewfinder is stopped, 
//              Loads Image capture Library, Initializes Image Captue API function pointers.
//				Deinitializes imager and closes Image captue device handle
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
CImager::~CImager()
{	
	LOG(INFO) + "3100:Imager destructor entered";
	
	if (m_PreviewOn)
	{	int result = StopViewer();
		if (result!=0)
		{
			LOG(ERROR) + "3110:StopViewer Failed";
		}
	}
	if (m_bImagerLoadFailed && (m_hImager != INVALID_HANDLE_VALUE)) {
		Image_UnLockImager(m_hImager);
		Image_Close(m_hImager);
	}
	if(g_ListDevName!=NULL)
		LocalFree(g_ListDevName);

	if (hModule != NULL) FreeLibrary(hModule);
	delete [] m_lpSzFilename;
	delete[] m_szImagerCaptureURI;
    delete [] m_szCaptureSound;

#ifndef STATIC_DSCAM
	LOG(INFO) + "3120:CImager::~CImager deleting m_pDSCam";
	
	if (m_pDSCam != NULL) delete m_pDSCam;
	m_pDSCam = NULL;
#endif
#ifdef LOG_TO_FILE	
	fclose(LogFile);
#endif
	LOG(INFO) + "3199:Imager destructor entered";
}


////////////////////////////////////////////////////////////////////////
// Function:	InitImager
// Description:	Finds first Imager device and opens handle to image capture device, 
//              image capture device handle is used to call supported Image capture APIs.
//				
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::InitImager()
{

	DWORD dwRes = E_IMG_SUCCESS;
	IMAGE_FINDINFO ImageFindInfo;
	LOG(INFO) + "3200:Init imager entered";
	
	// Found the imager number requested so now open it (if it's an imager)
	if (wcsncmp(m_szDevName, L"IMG", 3) != 0)
	{
		// if it's not an imager device we don't need to open it here
		m_hImager = INVALID_HANDLE_VALUE;
	}
	else if (m_bImagerLoadFailed)
		return 999;
	else if (m_szDevName[0] != L'\0')
	{
		memset(&ImageFindInfo, 0, sizeof(ImageFindInfo));
		SI_INIT(&ImageFindInfo);
		SI_SET_USED(&ImageFindInfo, tszRegistryBasePath);

		dwRes = Image_Open(m_szDevName /*ImageFindInfo.tszDeviceName*/, &m_hImager);

		if (dwRes != E_IMG_SUCCESS) {
			TCHAR tcErr[200];
			wsprintf(tcErr, L"3210:Image_Open error %d", dwRes);
			
			return dwRes;
		}
	}

	SetName(DEFAULT_FILENAME);
	LOG(INFO) + "3299:Init imager left";
	
	return dwRes;
}


	
////////////////////////////////////////////////////////////////////////
// Function:	StartViewer
// Description:	This function starts Viewfinder for opened image capture device.
//              image capture device handle is used to call supported Image capture APIs.
//				Sets JPEG as default image format. Sets the Light and Aim to default state FALSE
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::StartViewer()
{

	DWORD dwCapValue;
	DWORD dwRes; 
	TCHAR tcString[200];
	stringstream ss;

	LOG(INFO) + "3300:Calling StartViewer ...";

	// Create the viewer window
	m_hWndViewer = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"static", NULL, WS_VISIBLE | WS_POPUP, 
				m_rcWinPos.left,
				m_rcWinPos.top,
				m_rcWinPos.right,
				m_rcWinPos.bottom, 
				m_hParentWnd, NULL, m_hInst, NULL);

	// Populate the Window message structure
    WndCls.style         = 0;
	WndCls.lpfnWndProc   = (WNDPROC)CImager::MessageWndProc;
    WndCls.cbClsExtra    = 0;
    WndCls.cbWndExtra    = 0;
    WndCls.hInstance     = m_hInst;
    WndCls.hIcon         = NULL;
    WndCls.hCursor       = LoadCursor(NULL, IDC_ARROW);
    WndCls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndCls.lpszMenuName  = NULL;
    WndCls.lpszClassName = L"messagewindow";

    RegisterClass(&WndCls);

	// Create the message window
	m_hWndMessageWindow = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"messagewindow", NULL, WS_CHILD, 50, 50, 50, 50, m_hParentWnd, NULL, m_hInst, NULL);
	SetWindowLong(m_hWndMessageWindow, GWL_USERDATA, (long)this);
	ss << "Created Window handled " << m_hWndMessageWindow;
	LOG(INFO) + __FUNCTION__ + ss.str();
	ss.clear(); ss.str("");

	// Post a message to the Window
	ss << "Posting message WM_STOPVIEWER to Window Handle " << m_hWndMessageWindow;
	LOG(INFO) + __FUNCTION__ + ss.str();
	ss.clear(); ss.str("");
	PostMessage(m_hWndMessageWindow, WM_STOPVIEWER, 1, 1);

	wsprintf(tcString, L"3310:start viewer 1 ->left=%d, right=%d, top=%d, bottom=%d", m_rcWinPos.left, m_rcWinPos.right, m_rcWinPos.top, m_rcWinPos.bottom);
	LOG(INFO) + tcString;

	if (wcsncmp(m_szDevName, L"IMG", 3) == 0)
	{
		// If any of the functions failed to initialise return immediately
		if (m_bImagerLoadFailed || (m_hImager == INVALID_HANDLE_VALUE))
		{
			DestroyWindow(m_hWndViewer);
			LOG(ERROR) + "3315:m_bImagerLoadFailed || (m_hImager == INVALID_HANDLE_VALUE";
			return 999;
		}

		// Set the width of the target window for the video viewfinder
		dwCapValue = m_rcWinPos.right;// - m_rcWinPos.left + 1;
		if((dwRes = Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINWIDTH, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS))
		{
			LOG(ERROR) + "3320:Imager_SetCapCurrValue error";
			DestroyWindow(m_hWndViewer);
			return 999;
		}
		
		// Set the height of the target window for the video viewfinder
		dwCapValue = m_rcWinPos.bottom;// - m_rcWinPos.top + 1;
		if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHEIGHT, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
		{
			LOG(ERROR) + "3325:Imager_SetCapCurrValue error";
			DestroyWindow(m_hWndViewer);
			return 999;
		}
		BOOL bCrop = TRUE;
		if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_MATCHVFCAPCROP, sizeof(BOOL), &bCrop) != E_IMG_SUCCESS)
		{
			LOG(ERROR) + "3330:Imager_SetCapCurrValue error";
			DestroyWindow(m_hWndViewer);
			return 999;
		}
		
		// Specifies the window handle to be used to display the viewfinder
		dwCapValue = (DWORD)m_hWndViewer;
		if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHANDLE, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
		{
			LOG(ERROR) + "3335:Imager_SetCapCurrValue error";
			DestroyWindow(m_hWndViewer);
			return 999;
		}

		dwCapValue = TWFF_JPEG; 
		if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_FILEFORMAT, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
		{
			LOG(ERROR) + "3340:Imager_SetCapCurrValue error";
			DestroyWindow(m_hWndViewer);
			return 999;
		}
		
		// Start acquiring images and Turn on the viewfinder
		if ((dwRes = Image_StartAcquisition(m_hImager)) != E_IMG_SUCCESS) 
		{
			LOG(ERROR) + "3345:Imager_StartAcquisition error";
			DestroyWindow(m_hWndViewer);
			return dwRes;
		}

		if ((dwRes = Image_StartViewfinder(m_hImager)) != E_IMG_SUCCESS) 
		{
			LOG(ERROR) + "3350:Imager_StartViewfinder error";
			DestroyWindow(m_hWndViewer);
			return dwRes;
		}
		
		Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_LAMPSTATE, sizeof(BOOL), &m_bImgLight);
		Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_AIMING, sizeof(BOOL), &m_bImgAim);

	}
	else
	{
		// Using a DirectShow Camera device
		CamConfig cfg;

#ifndef STATIC_DSCAM
		LOG(INFO) + "3355:Deleting m_pDSCam ...";
		
		if (m_pDSCam) delete m_pDSCam;
		m_pDSCam = new CDShowCam();

#endif
		LOG(INFO) + "3360:InitFilterGrp() ...";
		
		if(SUCCEEDED(m_pDSCam->InitFilterGrp()))
		{
			cfg.sCamID = m_szDevName;	// Pass that selected Camera Id to the "BuildFilterGraph(...)" function.
			cfg.rc = m_rcWinPos;
			// NB: rcWinPos right & bottom are wrong!! it's not being used as a rectangle..
			// it represents: top=top, left=left, bottom=height, right=width so some addition is
			// needed to get the actual rect bottom and right values
//			cfg.rc.bottom += cfg.rc.top;
//			cfg.rc.right += cfg.rc.left;
			cfg.rc.top = 0;
			cfg.rc.left = 0;
			cfg.hwndOwnerWnd = m_hWndViewer;
			cfg.bIsAudioEnb = FALSE;	// no audio capture
			cfg.bIsCapEnb = TRUE;		// enable capture
			cfg.bIsPrvEnb = TRUE;		// enable preview
			cfg.bIsStillEnb = TRUE;		// enable still capture
			LOG(INFO) + "3365:BuildFilterGrp() ...";
			
			if(!m_pDSCam->BuildFilterGrp(&cfg))		
			{
				LOG(ERROR) + "3370:------FGraph Build FAIL------";
				if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
				return 1;
			}
		}		
		else
		{
			LOG(ERROR) + "3372:------Init FGraph FAIL------";
			if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
			return 1;
		}
		LOG(INFO) + "3374:RunGrp() ...";
		
		if(!m_pDSCam->RunGrp())			
		{
			LOG(ERROR) + "3376:------FGraph Run FAIL------";
			if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
			return 1;
		}

		HRESULT hr;

		CamPropTbl propTbl;
		propTbl.p = CameraControl_Flash;
		hr = m_pDSCam->Get_PropRng(CAM, &propTbl);
		if (hr != E_PROP_ID_UNSUPPORTED)
		{
			propTbl.plVal = m_bImgLight ? propTbl.plMax : propTbl.plMin;
			m_pDSCam->Set_PropVal(CAM, &propTbl);
		}
		else
		{
			LOG(WARNING) + "3378:hr == E_PROP_ID_UNSUPPORTED";
		}

		//  Set the resolution of the saved image
		if (!(m_iDesiredHeight == -1 && m_iDesiredWidth == -1))
		{
			PinType myPIN = S;
			ImageRes myRes;
			myRes.nHeight = m_iDesiredHeight;
			myRes.nWidth = m_iDesiredWidth;
			if (!m_pDSCam->Set_Resolution(&myRes, myPIN))
			{
				LOG(ERROR) + "3380:Set Image Resolution failed "; 
			}
		}
	}

	m_PreviewOn = TRUE;
	LOG(INFO) + "3399:StartViewer called.";
	
	if (m_hParentWnd)
		SetFocus(m_hParentWnd);

    StartTriggerWatch();

	return E_IMG_SUCCESS;
}


////////////////////////////////////////////////////////////////////////
// Function:	StopViewer
// Description:	This function stop Viewfinder for opened image capture device.
//              
//				
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::StopViewer()
{
	DWORD dwRes;
	
	LOG(INFO) + "3400:Calling stop viewer ...";
	
	if (wcsncmp(m_szDevName, L"IMG", 3) == 0)
	{
		// If any of the functions failed to initialise return immediately
		if (m_bImagerLoadFailed || (m_hImager == INVALID_HANDLE_VALUE)) {
			return 999;
		}

		// Stop the viewfinder first
		if (E_IMG_SUCCESS != (dwRes = Image_StopViewfinder(m_hImager))) {
			LOG(ERROR) + "3410:Viewer error";
			return dwRes;
		}

		// Stop the acquisition
		if (E_IMG_SUCCESS != (dwRes = Image_StopAcquisition(m_hImager))) {
			LOG(ERROR) + "3415:Acquisition error";
			return dwRes;
		}

		// Destroy the window
		if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
	}
	else
	{
		// Destroy the window
		if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);

		// destroy the DirectShow graph
		if (m_pDSCam) 
		{	BOOL hr = m_pDSCam->StopGrp();
			if (hr!=TRUE)
			{
				LOG(ERROR) + "3420:StopGrp()  failed";
				return 1;
			}
		}
		else 
		{
			LOG(ERROR) + "3425:m_pDSCam is null";
			return 1;
		}
		
		m_pDSCam->ReleaseGrp();
#ifndef STATIC_DSCAM
		LOG(INFO) + "3430:Deleting m_pDSCam";
		
		delete m_pDSCam;
		m_pDSCam = NULL;
#endif
	}
	m_PreviewOn = FALSE;
	
	LOG(INFO) + "3499:StopViewer called.";
	return 0;
}


////////////////////////////////////////////////////////////////////////
// Function:	SetViewerPosition
// Description:	This sets the window size and position.  It then adjusts
//				the imager viewer window (inside the first window) and 
//				adjusts the cropping as required.
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::SetViewerPosition(int x, int width, int y, int height)
{
	TCHAR tcErr[200];

	wsprintf(tcErr, L"x=%d, width=%d, y=%d, height=%d", x, width, y, height);
	LOG(INFO) + "3500:Calling SetViewerPosition " + tcErr + "..." ;
	
	wsprintf(tcErr, L"3502:Current left=%d, right=%d, top=%d,  bottom=%d", m_rcWinPos.left, m_rcWinPos.right, m_rcWinPos.top, m_rcWinPos.bottom);
	LOG(INFO) + tcErr;

	if (x != -9999) m_rcWinPos.left = x;
	if (width != -9999) m_rcWinPos.right = width;
	if (width == 0) m_rcWinPos.right = GetSystemMetrics(SM_CXSCREEN); //SPS
	if (y != -9999) m_rcWinPos.top = y;
	if (height != -9999) m_rcWinPos.bottom = height;
	if (height == 0) m_rcWinPos.bottom = GetSystemMetrics(SM_CYSCREEN); // SPS

	wsprintf(tcErr, L"3504:New left=%d, right=%d, top=%d, bottom=%d", m_rcWinPos.left, m_rcWinPos.right, m_rcWinPos.top, m_rcWinPos.bottom);
	LOG(INFO) + tcErr;

	if (wcsncmp(m_szDevName, L"IMG", 3) == 0)
	{
		if (IsWindow(m_hWndViewer)) {
			DWORD dwRes;
			LOG(INFO) + "3506:Calling MoveWindow";

			MoveWindow(m_hWndViewer,
					m_rcWinPos.left,
					m_rcWinPos.top,
					m_rcWinPos.right,
					m_rcWinPos.bottom, 
					FALSE);

			// Set the width of the target window for the video viewfinder
			DWORD dwCapValue = m_rcWinPos.right;// - m_rcWinPos.left + 1;
			if((dwRes = Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINWIDTH, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS))
			{
				wsprintf(tcErr, L"3510:Imager_SetCapCurrValue error %d", dwRes);
				
				return 999;
			}
		
			// Set the height of the target window for the video viewfinder
			dwCapValue = m_rcWinPos.bottom;// - m_rcWinPos.top + 1;
			if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHEIGHT, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
			{
				wsprintf(tcErr, L"3515:Imager_SetCapCurrValue error %d", dwRes);
				
				return 999;
			}
			BOOL bCrop = TRUE;
			if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_MATCHVFCAPCROP, sizeof(BOOL), &bCrop) != E_IMG_SUCCESS)
			{
				wsprintf(tcErr, L"3520:Imager_SetCapCurrValue error %d", dwRes);
				
				return 999;
			}
			
			// Specifies the window handle to be used to display the viewfinder
			dwCapValue = (DWORD)m_hWndViewer;
			if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHANDLE, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
			{
				wsprintf(tcErr, L"3525:Imager_SetCapCurrValue error %d", dwRes);
				
				return 999;
			}
		}
	}
	else if (m_pDSCam)
	{
		if (IsWindow(m_hWndViewer)) {
			LOG(INFO) + "3527:Calling MoveWindow";
			MoveWindow(m_hWndViewer,
						m_rcWinPos.left,
						m_rcWinPos.top,
						m_rcWinPos.right,
						m_rcWinPos.bottom, 
						FALSE);

			LOG(INFO) + "3529:Calling ResizeWindow";

			m_pDSCam->ResizePreview(m_rcWinPos.right, m_rcWinPos.bottom);
		}
	}
	LOG(INFO) + "3599:SetViewerPosition called.";
	return 0;
}


////////////////////////////////////////////////////////////////////////
// Function:	SetOptions
// Description:	Sets the lamp and aim options on the imager
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::SetOptions(int Lamp, int Aim)
{

	BOOL bChange = FALSE;
	if ((Lamp == 0) && (m_bImgLight)) {
		m_bImgLight = FALSE; 
		bChange = TRUE;
	}
	else if ((Lamp == 1) && (!m_bImgLight)) {
		m_bImgLight = TRUE;
		bChange = TRUE;
	}
	if ((Aim == 0) && (m_bImgAim)) {
		m_bImgAim = FALSE; 
		bChange = TRUE;
	}
	else if ((Aim == 1) && (!m_bImgAim)) {
		m_bImgAim = TRUE;
		bChange = TRUE;
	}
	if (bChange && m_hWndViewer) {
		if (wcsncmp(m_szDevName, L"IMG", 3) == 0)
		{
			Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_LAMPSTATE, sizeof(BOOL), &m_bImgLight);
			Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_AIMING, sizeof(BOOL), &m_bImgAim);
		}
		else if (m_pDSCam)
		{
			HRESULT hr;

			CamPropTbl propTbl;
			propTbl.p = CameraControl_Flash;
			hr = m_pDSCam->Get_PropRng(CAM, &propTbl);
			if (hr != E_PROP_ID_UNSUPPORTED)
			{
				propTbl.plVal = m_bImgLight ? propTbl.plMax : propTbl.plMin;
				hr = m_pDSCam->Set_PropVal(CAM, &propTbl);
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////
// Function:	DeInit
// Description:	Deinitializes imager.
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImager::DeInit()
{
	LOG(INFO) + "3600:Calling DeInit ...";
	
	if (m_PreviewOn)
	{	int result = StopViewer();
		if (result!=0)
		{
			LOG(ERROR) + "3610:StopViewer Failed";
		}
	}
	if (/*m_bImagerLoadFailed && */(m_hImager != INVALID_HANDLE_VALUE)) {
		LOG(ERROR) + "3620:m_hImager is INVALID_HANDLE_VALUE";
		
		Image_UnLockImager(m_hImager);
		Image_Close(m_hImager);
		m_hImager = INVALID_HANDLE_VALUE;
	}
	
	LOG(INFO) + "3699:DeInit called.";
	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Function:	SendImage
// Description:	Saves the Image file to memory.  If a URL has been specified
//				it then sends the image to the URL using http.
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
// Rewritten:	Paul Henderson
// Date			August 2010	
// Added Image Capture as URI capabilities (DCC, June 2012)
////////////////////////////////////////////////////////////////////////
DWORD CImager::SaveImage()
{
	DWORD dwRes;

	//  If the user has defined the ImagerCaptureEvent then send the captured 
	//  image back to them as a Data URI
	bool bSaveBytes = false;
	BYTE* pImageAsBytes = NULL;
	int iLengthOfImage = 0;

	if (m_pTakePictureEvent != NULL && m_pTakePictureEvent->hasCallback())
		bSaveBytes = true;

	if (wcsncmp(m_szDevName, L"IMG", 3) == 0)
	{
		if ((dwRes = Image_GetImage(m_hImager, &g_dwImageBufSize, &g_pImageBuffer)) != E_IMG_SUCCESS) 
		{
			LOG(INFO) + "getImage Error";
			return ERR;
		}
	
		DWORD dwBytesWritten;
		HANDLE hFile = 
		CreateFile(m_lpSzFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile)
		{ 
			WriteFile(hFile,g_pImageBuffer,g_dwImageBufSize,&dwBytesWritten,NULL);
			if (bSaveBytes)
			{
				LOG(INFO) + L"Image file saved as [" + m_lpSzFilename + L"]";
				pImageAsBytes = (BYTE*)g_pImageBuffer;
				iLengthOfImage = dwBytesWritten;
			}
			CloseHandle(hFile);
		}
	}
	else if (m_pDSCam)
	{
		m_pDSCam->CaptureStill(m_lpSzFilename);
		if (bSaveBytes)
		{
			//  Rather than get bogged down the Direct Show again we'll just
			//  read the file back in from disk.
			HANDLE hFile = CreateFile(m_lpSzFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile)
			{
				DWORD dwFileSize = GetFileSize(hFile, NULL);
				if (dwFileSize > 0)
				{
					pImageAsBytes = new BYTE[dwFileSize];
					iLengthOfImage = dwFileSize;
					if (pImageAsBytes)
					{
						DWORD dwBytesRead = 0;
						do
						{
							if (!ReadFile(hFile, pImageAsBytes, dwFileSize, &dwBytesRead, NULL))
							{
								//  Some error has occured reading the file
								LOG(WARNING) + "Unable to send image data as URI, could not read data"; 
								delete[] pImageAsBytes;
								pImageAsBytes = NULL;
								break;
							}
						}
						while (dwBytesRead != 0);
					}
				}
				else
				{
					LOG(WARNING) + "Unable to send image data as URI, size was unexpected";
				}
				CloseHandle(hFile);
			}
			else
				LOG(WARNING) + "Unable to send image data as URI, could not find captured image";
		}
	}

	//  If the user has defined the ImagerCaptureEvent then send the captured 
	//  image back to them as a Data URI
	if (pImageAsBytes)
	{
		LOG(INFO) + "Calling InvokeImagerCaptureEvent ...";
		InvokeImagerCaptureEvent(pImageAsBytes, iLengthOfImage);
		LOG(INFO) + "InvokeImagerCaptureEvent called.";
		if (m_pDSCam)
		{
			//  If using direct show we are responsible for freeing
			//  the memory associated with the byte array, else
			//  it is done by the EMDK
			delete[] pImageAsBytes;
		}
	}
	else
		LOG(ERROR) + "pImage bytes not set";

	return 0;
}

////////////////////////////////////////////////////////////////////////
// Function:	EnumImagerNav
// Description: Saves Imager enummertion url 
//				
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////

// SPS
// BOOL CImager::EnumImagerNav(PBMetaStruct *pbMetaStructure, PPBSTRUCT /*pPBStructure  --unused*/)
//{
//
//	if((pbMetaStructure->lpValue!= NULL)||(wcscmp(pbMetaStructure->lpValue,L"")!=0))
//		wcscpy(m_EnumIMGNav,pbMetaStructure->lpValue );
//
//	return TRUE;
//}
// SPS


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CImager::SetName(LPCTSTR lpSzFilename)
{
	int iLen;
	delete [] m_lpSzFilename;	

	//if the name was set to L"", then the default name will be selected
	if((iLen = wcslen(lpSzFilename))== 0){
		m_lpSzFilename = new TCHAR[wcslen(DEFAULT_FILENAME)+ 2];
		if(!m_lpSzFilename) return FALSE;
		wcscpy(m_lpSzFilename,L"\\");
		wcscat(m_lpSzFilename,DEFAULT_FILENAME);	
	}
	else{	
		m_lpSzFilename = new TCHAR[iLen + 2];	
		if(m_lpSzFilename == NULL){
			//error
			return FALSE;
		}
		wcscpy(m_lpSzFilename,L"\\");
		wcscat(m_lpSzFilename,lpSzFilename);
		
	}
	
	return TRUE;
}

BOOL CImager::DirectShowShutterClick()
{
	BOOL result = FALSE;

	if (m_pDSCam)
		result = m_pDSCam->HandlesCaptureSound();
	return result;
}

BOOL CImager::SetDirectShowShutterSound(LPWSTR szSoundFile)
{
	BOOL result = FALSE;

	if (m_pDSCam)
		result = m_pDSCam->Set_CaptureSound(szSoundFile);
	return result;
}

BOOL CImager::SetDesiredWidth(int iDesiredWidth)
{
	BOOL bRetVal = TRUE;

	LOG(INFO) + "Calling SetDesiredWidth ...";

	WCHAR szValue[MAX_PATH];
	wsprintf(szValue, L"Changing from [%d] to [%d]", m_iDesiredWidth, iDesiredWidth);
	LOG(INFO) + szValue;

	if (m_iDesiredWidth != iDesiredWidth)
	{
		m_iDesiredWidth = iDesiredWidth;
		if (m_PreviewOn)
		{
			PinType myPIN = S;
			ImageRes myRes;
			myRes.nHeight = m_iDesiredHeight;
			myRes.nWidth = m_iDesiredWidth;

			wsprintf(szValue, L"Width[%d] Height[%d]", m_iDesiredWidth, m_iDesiredHeight);
			LOG(INFO) + szValue;

			bRetVal = m_pDSCam->Set_Resolution(&myRes, myPIN);
			if (!bRetVal)
			{
				LOG(WARNING) + "Set Image Resolution failed";
			}
		}
	}

	LOG(INFO) + "SetDesiredWidth called.";
	return bRetVal;	
}

BOOL CImager::SetDesiredHeight(int iDesiredHeight)
{
	BOOL bRetVal = TRUE;
	if (m_iDesiredHeight != iDesiredHeight)
	{
		m_iDesiredHeight = iDesiredHeight;
		if (m_PreviewOn)
		{
			PinType myPIN = S;
			ImageRes myRes;
			myRes.nHeight = m_iDesiredHeight;
			myRes.nWidth = m_iDesiredWidth;
			bRetVal = m_pDSCam->Set_Resolution(&myRes, myPIN);
			if (!bRetVal)
			{
				LOG(WARNING) + "Set Image Resolution failed";
			}
		}
	}
	return bRetVal;	
}


int CImager::SetImagerCaptureEvent(rho::apiGenerator::CMethodResult* eventCallback)
{
	//  store the passed navigation URI
	m_pTakePictureEvent = eventCallback;
	return TRUE;
}


int CImager::InvokeImagerCaptureEvent(BYTE* bData, int iLength)
{
	LOG(INFO) + "1234:Calling InvokeImagerCaptureEvent ...";

	if (!m_pTakePictureEvent) 
	{
		LOG(ERROR) + "1234:Callback not set on attempted invocation";
		return 0;
	}

	//  Convert the Image to base 64, this representation is about 1/3
	//  larger than the binary input
	char* szEncodedString = new char[iLength * 2];
	memset(szEncodedString, 0, iLength * 2);
	char* szEncodedString2 = new char[(iLength+25) * 2];
	memset(szEncodedString2, 0, (iLength+25) * 2);

	EncodeToBase64(bData, iLength, szEncodedString);

	// SPS - don't need to save as WCHAR as callback data is char
	//  The DataURI is longer than the base 64 representation because of the header
	//int iLengthOfReturnString = strlen(szEncodedString) + 30;
	//WCHAR* szImagerAsDataURI = new WCHAR[iLengthOfReturnString];

	//if (szImagerAsDataURI)
	//{
	//	memset(szImagerAsDataURI, 0, iLengthOfReturnString * sizeof(WCHAR));
	//	wcscpy(szImagerAsDataURI, L"data:image/jpeg;base64,");
	//	mbstowcs(szImagerAsDataURI + wcslen(L"data:image/jpeg;base64,"), szEncodedString, iLengthOfReturnString);
	//}
	// SPS

	// SPS - Deal with navigations
	//m_pModule->SendPBNavigate(tcImagerCaptureEventNames, m_iInstanceID, 
	//	m_szImagerCaptureURI, szImagerAsDataURI, NULL); 
	
	// Handle callback
	// TBD - get real values instead of hardcoded values
	char szValue[MAX_PATH];
	rho::Hashtable<rho::String, rho::String> callbackData;

	// status
	sprintf(szValue, "%s", "ok"); // or "error" //TBD
	callbackData.put("status", szValue);

	// imageUri
	if (m_bSaveAsFile)
	{
		LOG(INFO) + "1234:imageUri file reference [" + GetImageFilename() + "]";
		callbackData.put("imageUri", GetImageFilename()); 
		callbackData.put("image_uri", GetImageFilename());
	}
	else
	{
		LOG(INFO) + "1234:imageUri is a data uri";
		// TMP - hardcoded data uri for testing purposes
		//const char image_uri[] = "data:image/jpeg;base64,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%3D%3D";
		//callbackData.put("imageUri", image_uri);
		//callbackData.put("image_uri", image_uri);
		// TMP
		strcpy(szEncodedString2, "data:image/jpeg;base64,");
		strcat(szEncodedString2, szEncodedString);
		LOG(INFO) + "1234:imageUri [" + szEncodedString2 + "]";
		callbackData.put("imageUri ", szEncodedString2);
		callbackData.put("image_ur", szEncodedString);
	}

	// imageHeight
	sprintf(szValue, "%d", 1536); // TBD
	callbackData.put("imageHeight", szValue);
	callbackData.put("image_height", szValue);
	// imageWidth
	sprintf(szValue, "%d", 2048); // TBD
	callbackData.put("imageWidth", szValue);
	callbackData.put("image_width", szValue);
	// imageFormat
	//sprintf(szValue, "%s", "jpeg"); // TBD
	callbackData.put("imageFormat", GetCompressionFormat());
	callbackData.put("image_format", GetCompressionFormat());
	// message
	sprintf(szValue, "%s", "error message text"); // TBD
	callbackData.put("message", szValue);

	//delete[] szImagerAsDataURI;
	delete[] szEncodedString;

	// Stop camera viewfinder
	//LOG(INFO) + "1234:Calling StopViewer() ...";
	//this->StopViewer();

	// Success - invoke the callback
	LOG(INFO) + "1234:InvokeImagerCaptureEvent called.";
	m_pTakePictureEvent->set(callbackData);
}

BOOL CImager::StartTriggerWatch (void)
{
	if (!m_bRcmLoaded)
	{
		LOG(WARNING) + "Trigger functionality is not available on this device";
		return FALSE;
	}

	DWORD error;
	LPWSTR perror;

	// Already started?
	if (m_hTriggerThread)
		return TRUE;

	// Create auto-reset event which system will signal when trigger occurs
	m_hTriggerEvent = CreateEvent (NULL, FALSE, FALSE, L"CameraTrigger");

	// Create event to signal thread to quit
	m_hTriggerQuit = CreateEvent (NULL, TRUE, FALSE, NULL);

	// Create thread
	m_hTriggerThread = CreateThread (NULL, 0, StaticTriggerProc, this, 0, NULL);

	// Register with system for trigger notifications
	if ((error = m_pRcm->lpfn_RCM_RegisterTriggerEvent (TRIGGER_ALL_MASK, FALSE, L"CameraTrigger", &m_hTriggerNotification)) != E_RCM_SUCCESS)
	{
		perror = new WCHAR [64];
		StringCchPrintf (perror, 64, L"RegisterTriggerEvent failed (%lu)", error);
		LOG(WARNING) + perror;
		delete [] perror;

		return FALSE;
	}

	LOG(INFO) + "Trigger watch started";

	return TRUE;
}

void CImager::StopTriggerWatch (void)
{
	if (!m_bRcmLoaded)
	{
		LOG(WARNING) + "Trigger functionality is not available on this device";
		return;
	}

	// Started?
	if (!m_hTriggerThread)
		return;

	// Unregister for notifications
	m_pRcm->lpfn_RCM_DeregisterTrigger (m_hTriggerNotification);

	// Stop thread and wait
	SetEvent (m_hTriggerQuit);
	if (WaitForSingleObject (m_hTriggerThread, 5000) == WAIT_TIMEOUT)
		TerminateThread (m_hTriggerThread, 0);
	CloseHandle (m_hTriggerThread);
	CloseHandle (m_hTriggerQuit);

	CloseHandle (m_hTriggerEvent);

	m_hTriggerThread = 0;

	LOG(INFO) + "Trigger watch stopped";
}

void CImager::TriggerProc (void)
{
	stringstream ss;

	if (!m_bRcmLoaded)
	{
		LOG(WARNING) + "Trigger functionality is not available on this device";
		return;
	}

	char nav_param [16];
	DWORD status, error;
	LPWSTR perror;
	HANDLE wait [2];
	BOOL posterror;

	wait [0] = m_hTriggerQuit;
	wait [1] = m_hTriggerEvent;

	LOG(WARNING) + "TriggerProc started";
	if (WaitForMultipleObjects (2, wait, FALSE, INFINITE) != WAIT_OBJECT_0)
    {
		if (m_szCaptureSound &&	!DirectShowShutterClick())
        {
            // error/length checking is now done in the setter
			PlaySound(m_szCaptureSound,NULL,SND_FILENAME|SND_ASYNC);
        }

        SaveImage();

		// Send a message to the Window that Imager 
		//SendMessage(m_hWndMessageWindow, WM_STOPVIEWER, 0, NULL);
		ss << "PostMessage " << WM_STOPVIEWER << " to m_hWndMessageWindow:" << m_hWndMessageWindow;
		LOG(INFO) + ss.str();
		ss.str(""); ss.clear();
		posterror = PostMessage(m_hWndMessageWindow, WM_STOPVIEWER, 0, 0);
		ss << "PostMessage returned " << posterror;
		LOG(INFO) + ss.str();
		LOG(INFO) + "Posted Message WM_STOPVIEWER to m_hWndMessageWindow";
        //StopViewer();
		LOG(INFO) + "SaveImage done.";
	}

	CloseHandle (m_hTriggerThread);
    m_hTriggerThread = NULL;
	CloseHandle (m_hTriggerQuit);
    m_hTriggerQuit = NULL;
	CloseHandle (m_hTriggerEvent);
    m_hTriggerEvent = NULL;
	LOG(WARNING) + "TriggerProc done";
}

DWORD CImager::StaticTriggerProc (LPVOID pparam)
{
	CImager *pthis = (CImager*) pparam;
	pthis->TriggerProc ();

	return 0;
}

BOOL CImager::SoftTrigger()
{
    if (m_hTriggerThread && m_hTriggerEvent)
    {
        SetEvent(m_hTriggerEvent);
        return TRUE;
    }
    return FALSE;
}


const char* CImager::SetImageFilename(const char* cstr)
{
    if (cstr)
    {
        int iLen = strlen(cstr);

        if (iLen > 0)
        {
            mbstowcs(m_lpSzFilename, cstr, iLen+1);
			LOG(INFO) + "CImager::SetImageFilename [" + m_lpSzFilename +"]";
        }
    }
    return cstr;
}

const char* CImager::GetImageFilename(void)
{
	static char szFilename[MAX_PATH+1];
	int iLen = wcslen(m_lpSzFilename);

	szFilename[0] = '\0';

    if ((iLen > 0) && (iLen <= MAX_PATH))
    {
        wcstombs(szFilename, m_lpSzFilename, iLen+1);
	}

	LOG(INFO) + "CImager::GetImageFilename [" + szFilename +"]";
	return (const char*)szFilename;
}


const char* CImager::GetCompressionFormat(void)
{
	static char szFiletype[MAX_PATH+1];
	int iLen = wcslen(m_lpSzFiletype);

	szFiletype[0] = '\0';

    if ((iLen > 0) && (iLen <= MAX_PATH))
    {
        wcstombs(szFiletype, m_lpSzFiletype, iLen+1);
	}

	LOG(INFO) + "CImager::GetImageFileType [" + szFiletype +"]";
	return (const char*)szFiletype;
}


BOOL CImager::SetCompressionFormat(const char* cstr)
{
	BOOL bRet = false;
    if (cstr)
    {
        int iLen = strlen(cstr);

        if (iLen > 0)
        {
            mbstowcs(m_lpSzFiletype, cstr, iLen+1);
			bRet = true;
			LOG(INFO) + "CImager::SetCompressionFormat [" + m_lpSzFiletype +"]";
        }
    }
    return bRet;
}


BOOL CImager::SetCaptureSound(const char* cstr)
{
    BOOL bRet = FALSE;

    if (cstr)
    {
        if (m_szCaptureSound)
            delete [] m_szCaptureSound;

        int iLen = strlen(cstr);

        if (_strnicmp(cstr, "file://", 7) == 0)
        {
            // if the capture sound is a file URI strip off the header
            iLen -= 7;
            cstr += 7;
        }

        if (iLen > 0)
        {
            m_szCaptureSound = new WCHAR [iLen+1];
            mbstowcs(m_szCaptureSound, cstr, iLen+1);
            CDShowCam::Set_CaptureSound(m_szCaptureSound);
            bRet = TRUE;
        }
    }
	LOG(INFO) + "CImager::SetCaptureSound [" + m_szCaptureSound +"]";
    return bRet;
}

const char* CImager::GetCaptureSound()
{
    static char szFilename[MAX_PATH+1];

    szFilename[0] = '\0';

    int iLen = wcslen(m_szCaptureSound);

    if ((iLen > 0) && (iLen <= MAX_PATH))
    {
        wcstombs(szFilename, m_szCaptureSound, iLen+1);
    }

    return (const char*)szFilename;
}


void CImager::ApplicationFocusChange(bool bActivate)
{
    if (m_bAppHasFocus == bActivate)
        return;

    m_bAppHasFocus = bActivate;

    if (!m_bAppHasFocus)
    {
        // JH TODO
        // application has lost focus - if the camera is open we need to close
        // it and store the ID so that it can be restarted when we regain focus
    }
    else
    {
        // JH TODO
        // application has regained focus - if we have a stored camera ID we need
        // to restart the camera.. this could be more difficult that it sounds as
        // it may need to be started on a different thread than this one
    }
}

