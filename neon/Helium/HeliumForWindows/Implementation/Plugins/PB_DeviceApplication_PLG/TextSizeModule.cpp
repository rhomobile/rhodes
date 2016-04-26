#include "TextSizeModule.h"
#include"..\..\Engine\common\pbengine_defs.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
CTextSizeModule::CTextSizeModule(PPBCORESTRUCT pPBCoreStructure)
{
	m_pPBCoreStructure = pPBCoreStructure;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTextSizeModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"TextSize");
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTextSizeModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create this module's instance data
	INSTANCE_DATA *pData = new INSTANCE_DATA;
	pData->instanceID = pInstStruct->instID;
	pInstStruct->pWrappedObj = pData;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTextSizeModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	if (pData)
		delete pData;
	
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTextSizeModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) pParam;

	if (cmp (pbMetaStructure->lpParameter, L"Largest"))
	{
		//  Set the text to the largest size
		if (m_pPBCoreStructure)
			return m_pPBCoreStructure->pBrowserSetTxtZoom(pData->instanceID, TEXT_ZOOM_BIGGEST, m_szModName);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Larger"))
	{
		//  Set the text to the larger size
		if (m_pPBCoreStructure)
			return m_pPBCoreStructure->pBrowserSetTxtZoom(pData->instanceID, TEXT_ZOOM_BIGGER, m_szModName);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Medium"))
	{
		//  Set the text to the medium size
		if (m_pPBCoreStructure)
			return m_pPBCoreStructure->pBrowserSetTxtZoom(pData->instanceID, TEXT_ZOOM_NORMAL, m_szModName);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Smaller"))
	{
		//  Set the text to the smaller size
		if (m_pPBCoreStructure)
			return m_pPBCoreStructure->pBrowserSetTxtZoom(pData->instanceID, TEXT_ZOOM_SMALLER, m_szModName);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Smallest"))
	{
		//  Set the text to the smallest size
		if (m_pPBCoreStructure)
			return m_pPBCoreStructure->pBrowserSetTxtZoom(pData->instanceID, TEXT_ZOOM_SMALLEST, m_szModName);
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to TextSize Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}