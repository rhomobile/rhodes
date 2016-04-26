#include "SNTPModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CSNTPModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"SNTP");
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CSNTPModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create a new SNTP server and response object to associate with this
	//  module
	//  Create this module's instance data
	INSTANCE_DATA *pData = new INSTANCE_DATA;
	pData->pSNTPClient = new CSNTPClient(this);
	pData->pSNTPresponse = new NtpServerResponse;
	pInstStruct->pWrappedObj = pData;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CSNTPModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	
	if (pData)
	{
		delete pData->pSNTPClient;
		delete pData->pSNTPresponse;
	}
	delete pData;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CSNTPModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) pParam;
	if (cmp(pbMetaStructure->lpParameter, L"ServerIP") &&
		pbMetaStructure->lpValue != NULL && !cmp(pbMetaStructure->lpValue, L""))
	{
		char pcSNTPServer[MAX_URL];
		memset(pcSNTPServer, 0, MAX_URL);
		wcstombs(pcSNTPServer, pbMetaStructure->lpValue, 
			wcslen(pbMetaStructure->lpValue));
		if (pData->pSNTPClient->GetServerTime(pcSNTPServer, *pData->pSNTPresponse))
		{
			SYSTEMTIME sys = pData->pSNTPresponse->m_ReceiveTime;
			SetSystemTime(&sys);
		}
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_INFO, L"Unrecognised Meta Tag Provided to SNTP Module, possibly from Regex'd PB2.x tags",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}