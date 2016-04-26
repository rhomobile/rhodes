/**
 * \file Imager.h:
 * \brief Declaration of CImager class.
 * Calls Image Capture APIs to service Imager META TAG requests from core.
 */
#include "ImgCAPI.h"
#include "../../common/public/PBPlugin.h"
#include "../../common/Public/PBUtil.h"
#include "PowerEventListener.h"
#include "DShowCam.h"
#if !defined(AFX_IMAGER_H__3892DEB7_7A23_4A68_A117_F19017B916D9__INCLUDED_)
#define AFX_IMAGER_H__3892DEB7_7A23_4A68_A117_F19017B916D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * Function pointer for Image Capture APIs (Exposed by image capture library ImgAPI32.dll)
 */
typedef DWORD (*IMAGE_FINDFIRSTPROC)(LPIMAGE_FINDINFO, PHANDLE);
typedef DWORD (*IMAGE_FINDNEXTPROC)(LPIMAGE_FINDINFO, HANDLE);
typedef DWORD (*IMAGE_FINDCLOSEPROC)(HANDLE);
typedef DWORD (*IMAGE_OPENPROC)(PCTSTR, PHANDLE);
typedef DWORD (*IMAGE_CLOSEPROC)(HANDLE);
typedef DWORD (*IMAGE_STARTACQUISITIONPROC)(HANDLE);
typedef DWORD (*IMAGE_STOPACQUISITIONPROC)(HANDLE);
typedef DWORD (*IMAGE_STARTVIEWFINDERPROC)(HANDLE);
typedef DWORD (*IMAGE_STOPVIEWFINDERPROC)(HANDLE);
typedef DWORD (*IMAGE_GETIMAGEPROC)(HANDLE, LPDWORD, LPVOID);
typedef DWORD (*IMAGE_SETCAPCURRVALUEPROC) (HANDLE, DWORD, DWORD, LPVOID);
typedef DWORD (*IMAGE_UNLOCKIMAGERPROC)(HANDLE);

#define MAX_LENGTH 100
#define DEFAULT_FILENAME L"Img"
/**
 * Linked list of Available Image capture devices
 */
typedef struct ListDevName{
	TCHAR DeviceName[MAX_LENGTH];
	TCHAR DeviceFriendlyName[MAX_LENGTH];
	BOOL bDeviceType;//TRUE-Imager, FALSE-DShow Camera
	ListDevName* pNext;
}*PListDevName;
enum EnumImageCapDevType { IMAGER, DSHOW_CAMERA, NONE };

/**
 * CImager Class encpasulates image capture device functionality
 */
class CImager 
{
public:
	int		m_iInstanceID;
	BOOL m_bTransfer;//gets set when an IMO destination string is set - saves calling IMO when no destination has been set
	
	
	/**
 * Public Function Sets imager lamp and Aim state as requested by Core through META Tags.
 *1 sets both to ON State, 0 set both to OFF state.
 * \param Lamp new state of lamp.
 * \param Aim new state of Aim.
 * \return 0.
 */
	DWORD SetOptions(int Lamp, int Aim);

/**
 * Public Function saves the captured image to local directory.
 * \param None.
 * \return errType defined in Error.h
 *\return 0 if successful
 */
	DWORD SaveImage();

	BOOL SetName(LPCTSTR lpSzFilename);//< Sets the name of the captured image 
	BOOL m_bImgAim;///< Current state of Imager's Aim, TRUE means ON, FALSE means OFF
	BOOL m_bImgLight;///< Current state of Imager's Lamp, TRUE means ON, FALSE means OFF
	int m_ImgNo;///< Flag to indicate if image capture device is Being used or not
	TCHAR m_szDevName[50];///< String for holding device Naem IMG1: etc.
	TCHAR m_EnumIMGNav[MAX_URL];///<array for holding the enumerated Imager devices
	WCHAR*	m_szImagerCaptureURI;	///<  Callback Event URI for ImagerCaptureEvent
	PBModule*	m_pModule;	///<  Pointer to the CImagerModule object owning this CImager
	int m_iDesiredHeight;	///<  The user requested height of the saved image
	int m_iDesiredWidth;	///<  The user requested width of the saved image

	/**
	*  Sets the ImagerCaptureEvent to the specified String
	*  \param szNavURI URI to set the event to
	*  \return TRUE on success
	*/
	BOOL SetImagerCaptureEvent(LPCWSTR szNavURI);
	
	/**
	*  Converts the binary data to base 64 and invokes the previously
	*  set ImagerCaptureEvent
	*  \param bData Binary representation of the image
	*  \param iLength Length of the binary data representing the image
	*  \return True
	*/
	BOOL InvokeImagerCaptureEvent(BYTE* bData, int iLength);

/**
 * Public Function Sets the viewfinder position on parent window.
 * \param x is Viewfinder windows top-left pixel position on horizontal pixels.
 * \param width Viewfinder windows width.
 * \param y is Viewfinder windows top-left pixel position on Veritcal pixels.
 * \param Height Viewfinder windows Height.
 * \return 9999 in case of error.
 *\return 0 if successful
 */
	DWORD SetViewerPosition(int x, int width, int y, int height);
/**
 * Public Function Stops image capture device's View Finder window.
 * \param None.
 *\return 0 if successful
 */
	DWORD StopViewer();
/**
 * Public Function Starts image capture device's View Finder.
 * \param None.
 *\return 0 if successful
 */
	DWORD StartViewer();
/**
 * Public Function Finds first available Image capture device and opens handle to it.
 * \return None.
 *\return 0 if successful else Throws Image capture API error
 */
	DWORD InitImager();
/**
 * Public Function Checks whether Direct Show camera driver is handing shutter sound.
 * \return TRUE if DSCam plays capture sound.
 * \return FALSE if DSCam does not play capture sound.
 */
	BOOL DirectShowShutterClick();
/**
 * Public Function Sets the Direct Show camera driver shutter sound.
 * \return TRUE if shutter sound set.
 * \return FALSE if setting shutter sound failed.
 */
	BOOL SetDirectShowShutterSound(LPWSTR szSoundFile);
	DWORD g_dwImageBufSize;///< Variable for Buffer size of captured image
	LPVOID g_pImageBuffer;///< Buffer to save captured image
//	HANDLE m_hImgEvent;
//	HANDLE m_hImgStopEvent;
	HANDLE m_hImager;///< Handle to image capture device returned by Image_Open API, used to invoke other APIs
	BOOL SetDesiredWidth(int iDesiredWidth);
	BOOL SetDesiredHeight(int iDesiredHeight);

/**
 * CImager Constructor Loads the Image Capture library. Core passes instance handle and client window handle each time
 * it creates Cimager instance.
 * \param hInstance Instance handle passed by core.
 * \param Window handle passed by core.
 *\return None
 */
	CImager(HINSTANCE hInstance,HWND hParent, int iTab, PBModule* pModule);
/**
 * CImager Destructor for Image Capture Module de-initialization
 * 
 * \param None
 *\return None
 */
	virtual ~CImager();
	IMAGE_GETIMAGEPROC Image_GetImage;///< Function pointer to Image Capture API, for Getting captured image
	IMAGE_STARTVIEWFINDERPROC Image_StartViewfinder;///< Function pointer to Image Capture API,to start View Finder
	IMAGE_STOPVIEWFINDERPROC Image_StopViewfinder;///< Function pointer to Image Capture API,to stop View Finder


	//new Additions
	PBSTRUCT m_pPBStructure;///< Variable to save PB context information
	HINSTANCE m_hInst;///< Variable to save instance handle passed by core
	HWND m_hParentWnd;///<Variable to save parent window handle passed by core
	bool m_bImagerLoadFailed;///<This flag is TRUE means Loadlibrary for ImgAPI32.dll suceeded
	BOOL DeInit();///<Stops ViewFinder if running and Closes Imager Handle
/**
 * Public function creates list of available imager devices
 * and stores in global structure g_ListDevName
 * \param None
 *\return None
 */
	BOOL CreateListOfAvailableDevices();
/**
 * Public function saves the enum navigation string and will return imager devices
 * on Enumerate tag
 * 
 * \param pbMetaStructure passed by PB with Tag parameters
 * \param pPBStructure passed by PB 
 *\return None
 */
	BOOL EnumImagerNav(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure);

	TCHAR tc_szImagers[MAXURL + 1];
	LPTSTR m_lpSzFilename;///<Pointer to the name of the image 
	bool m_bDisableDuringNavigate;
private:
	RECT m_rcWinPos;///<View Finder window position
	HMODULE hModule;///<Image Capture device handle
	HWND m_hWndViewer;///<Handle to ViewFinder window
	BOOL m_PreviewOn;
	


	IMAGE_FINDCLOSEPROC Image_FindClose;///<Function pointer to Image Capture API
	IMAGE_FINDFIRSTPROC Image_FindFirst;///<Function pointer to Image Capture API
	IMAGE_FINDNEXTPROC Image_FindNext;///<Function pointer to Image Capture API
	IMAGE_OPENPROC Image_Open;///<Function pointer to Image Capture API
	IMAGE_CLOSEPROC Image_Close;///<Function pointer to Image Capture API, closes Imae Capture Device 
	IMAGE_STARTACQUISITIONPROC Image_StartAcquisition;///<Function pointer to Image Capture API, Starts Acquisition
	IMAGE_STOPACQUISITIONPROC Image_StopAcquisition;///<Function pointer to Image Capture API, stops acquisition
	IMAGE_SETCAPCURRVALUEPROC Image_SetCapCurrValue;///<Function pointer to Image Capture API, Sets a capability value
	IMAGE_UNLOCKIMAGERPROC Image_UnLockImager;///<Function pointer to Image Capture API, unlocks imager device

	CDShowCam	*m_pDSCam;
	
public:
	//Msg Queue Options 
	MSGQUEUEOPTIONS m_MetamsgqOpt;///<In future may need for Color camera
	HANDLE m_hMetaMsgQRead, m_hMetaMsgQWrite;///<In future may need for Color camera

};

#endif // !defined(AFX_IMAGER_H__3892DEB7_7A23_4A68_A117_F19017B916D9__INCLUDED_)
