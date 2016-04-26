/**
 * \file ImagerMod.h
 * \brief Declaration of CImagerMod class.
 *
 * CImagerMod Class is derived from base class PBModule. Handles META TAG processing request from Core, META TAG should be in EMML 1.1 format.
 *
 */
#pragma once
#include "../../common/public/pbplugin.h"
#include "Imager.h"

static TCHAR* tcImagerEnumEventNames[] = 
{
	L"imagerArray=deviceName,friendlyName",
	NULL
};

static TCHAR* tcImagerCaptureEventNames[] = 
{
	L"imageData",
	NULL
};


	//  Different calling conventions are required when exporting functions on the
	//  device and on the desktop
/**
 * CImagerMod Class Manages Imager instances and Imager tag processing
 *
 */
class CImagerMod :
	public PBModule
{
public:
	CImagerMod(void);

	static DWORD SendNavigateURI (LPVOID pparam);
	CImager *m_pImagerInst;
/**
 * Public Function will be called before navigating to new page.
 * .
 * \param iInstID CScanner instance ID.
 * \return TRUE if successful.
 * \return FALSE if failed.
 */
	BOOL onBeforeNavigate(int iInstID);

/**
 * Public Function will be called before navigating to new page.
 * .
 * \param pPBStructure passed by core.
 * \return TRUE if successful.
 * \return FALSE if failed.
 */
void onDeInit(PBStruct *pPBStructure);
/**
 * Public Function Initializes member m_szModName with string Imager.
 * called when first Imager instance has been preloaded.
 * \param pPBStructure PPBSTRUCT passed by Core.
 * \return TRUE if suceeded.
 * \return FALSE if failed.
 */
	BOOL onInit(PPBSTRUCT pPBStructure);
/**
 * This function is inherited from PBModule::onRhoAppFocus.
 * Please see that function for further definition.
 */
	BOOL onRhoAppFocus(bool bActivate, int iInstID);
/**
 * Public Function called each time Core needs service for Imager Meta tag.
 * .
 * \param pbMetaStructure PBMetaStruct structure passed by Core containing META Tag parameters.
 * \param pInstStruct PPBINSTSTRUCT structure passed by Core.
 * \param pParam Imager instance from which Core needs service.
 * \return TRUE if suceeded.
 * \return FALSE if failed.
 */
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, VOID* pParam);
/**
 * Public Function called when instance calls dispose for disposing an instance.
 * this function also does any cleanup activity for instance getting deleted.
 * \param pPBStructure PPBSTRUCT structure passed by Core.
 * \param pInstStruct PPBINSTSTRUCT structure passed by Core.
 * \return TRUE if suceeded.
 * \return FALSE if failed.
 */
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
/**
 * Public Function creates the new Imager class instance and updates the linked list of
 * all the instances.called every time new Imager instance is creates.
 * \param pPBStructure PPBSTRUCT passed by Core.
 * \param pInstStruct PPBINSTSTRUCT structure passed by Core.
 * \return TRUE if suceeded.
 * \return FALSE if failed.
 */
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
/**
 * Public Function checks the list of available imager and camera devices for user specified
 * Imager device, returns true is available else false.
 * \param devName to be searched in the list.
 * \param bFlag returned with device type if TRUE Imager else Directshow Camera.
 * \return TRUE if device found.
 * \return FALSE if device not found.
 */
	BOOL CheckImagerOrCamera(LPCTSTR devName, BOOL *bFlag);
/**
 * Public Function Enables Imager device
 * \param pImager pointer to Imager instance.
 * \param szDevicename Imager device name.
 * \return TRUE if Imager initialized successfully.
 * \return FALSE if Imager not initialized successfully.
 */
	BOOL EnableImager(CImager *pImager, LPCTSTR szDevicename);
/**
 * Public Function Ennumerates imager devices on terminal and calls PBSendnavigate
 * \param pImager pointer to Imager instance.
 * \return TRUE if Enummeration succeeds.
 * \return FALSE if Enummeration fails.
 */
	BOOL EnumImager(CImager *pImager);
	static int CALLBACK IMOCBFunc(PVARSTRUCT pVars,int iTABID,LPARAM lParam);

public:
	
	~CImagerMod(void);
private:
	IMOREF m_FileTransferIMO;	///< Inter module object to communicate with File Transfer
	
	//Inidcates Imager is turned on
	BOOL m_bImgOn;
	bool m_bAppHasFocus;
	TCHAR m_szBgImager[50];
};

