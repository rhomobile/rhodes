#include "ZoomModule.h"
//#include <stdlib.h>
#include <stdio.h>
#include"..\..\Engine\common\pbengine_defs.h"


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
CZoomModule::CZoomModule(PPBCORESTRUCT pPBCoreStructure)
{
	m_pPBCoreStructure = pPBCoreStructure;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CZoomModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"zoom");
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CZoomModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
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
BOOL CZoomModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
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
BOOL CZoomModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) pParam;

	if (cmp (pbMetaStructure->lpParameter, L"Text"))
	{

		
		// check if the value is float because JS sometimes intereprets numbers as doubles even though they don't have a decimal point
		if ( isFloat(pbMetaStructure->lpValue))
		{
			int iTextZoom = _wtoi(pbMetaStructure->lpValue);
			if ((iTextZoom >= 0) && (iTextZoom < 5))
			{
				return m_pPBCoreStructure->pBrowserSetTxtZoom(pData->instanceID, iTextZoom, m_szModName);
			}
			else 
			{
				Log(PB_LOG_ERROR, L"Text zoom value Provided to Zoom Module ot ouf range. Valid values are 0-4",
					_T(__FUNCTION__), __LINE__);
				return FALSE;	
			}
		
		}
		else
		{
			// no need to log error, it will be logged at later point
			return FALSE;
		}

	}
	else if (cmp(pbMetaStructure->lpParameter, L"Page"))
	{
		if ( isFloat(pbMetaStructure->lpValue))
		{
			
			int iLen = wcslen(pbMetaStructure->lpValue);
			//convert to char*
			char* pNum = new char[iLen+1];
			if(pNum){
				wcstombs(pNum,pbMetaStructure->lpValue,iLen);
				double iTextZoom = atof(pNum);
				delete [] pNum;
				return m_pPBCoreStructure->pBrowserSetPageZoom(pData->instanceID, iTextZoom, m_szModName);
			}
			else
			{
				Log(PB_LOG_ERROR, L"Could not allocate enough memory",
			_T(__FUNCTION__), __LINE__);
				return false;
			}
			
			
			
		}
		else
		{
			// no need to log error, it will be logged at later point
			return FALSE;
		}
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Zoom Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}