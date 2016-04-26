#include <windows.h>
#include "NetworkModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CNetworkModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Network");
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent(PB_PRIMARY_MESSAGE);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
void CNetworkModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(PB_PRIMARY_MESSAGE);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CNetworkModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	// Create new server, add instance data, store in plugin framework
	CNetworkInstanceData* pInstanceData = new CNetworkInstanceData();
	CNetwork *pNetwork = new CNetwork(pInstStruct->instID, this);
	if (pNetwork->Initialise())
		pInstanceData->m_pNetwork = pNetwork;
	CWAN *pWAN = new CWAN(pInstStruct->instID, pPBStructure->hWnd, this);
	if (pWAN->Initialise())
		pInstanceData->m_pWAN = pWAN;

	pInstStruct->pWrappedObj = pInstanceData;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CNetworkModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	CNetworkInstanceData *pInstanceData = (CNetworkInstanceData*) (pInstStruct->pWrappedObj);

	if (pInstanceData && pInstanceData->m_pNetwork)
		delete pInstanceData->m_pNetwork;
	if (pInstanceData && pInstanceData->m_pWAN)
		delete pInstanceData->m_pWAN;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CNetworkModule::onBeforeNavigate(int iInstID)
{
	CNetworkInstanceData* pInstanceData = (CNetworkInstanceData*) GetObjFromID(iInstID);
	BOOL bRetVal = FALSE;
	if (pInstanceData && pInstanceData->m_pNetwork != NULL)
	{
		//  We have obtained a pointer to the appropriate CNetwork object
		//  Set the Nav String to ""
		pInstanceData->m_pNetwork->StopNetworkChecking();
		pInstanceData->m_pNetwork->SetNetworkEventURI(L"");
		bRetVal = TRUE;
	}
	if (pInstanceData && pInstanceData->m_pWAN != NULL)
	{
		pInstanceData->m_pWAN->SetWANEventURI(L"");
		bRetVal = TRUE;
	}

	return bRetVal;
}

BOOL CNetworkModule::onPrimaryMessage (MSG *pmsg, int ncurrid)
{
	if (pmsg->message == WM_USER_CONNECTION_MANGER_STATUS)
	{
		CNetworkInstanceData* pInstanceData = (CNetworkInstanceData*) GetObjFromID(ncurrid);
		if (pInstanceData && pInstanceData->m_pWAN)
			pInstanceData->m_pWAN->ConnectionManagerStatusUpdate();
	}
	return FALSE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CNetworkModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	Log(PB_LOG_WARNING, L"The Network API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Network API for future releases", _T(__FUNCTION__), __LINE__);

	CNetworkInstanceData *pInstanceData = (CNetworkInstanceData*) (pParam);

	if (!pInstanceData)
		return FALSE;

	/////////////////////////////////////
	//  META TAGS HANDLED BY CNETWORK  //
	/////////////////////////////////////
	if (cmp (pbMetaStructure->lpParameter, L"NetworkEvent") && pInstanceData->m_pNetwork)
	{
		//  Test the value is not NULL
		if (pbMetaStructure->lpValue > 0)
		{
			return pInstanceData->m_pNetwork->SetNetworkEventURI(pbMetaStructure->lpValue);
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"Host") && pInstanceData->m_pNetwork)
	{
		if (pbMetaStructure->lpValue > 0)
		{
			return pInstanceData->m_pNetwork->SetHost(pbMetaStructure->lpValue);
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"Port") && pInstanceData->m_pNetwork)
	{
		if (pbMetaStructure->lpValue > 0)
		{
			return pInstanceData->m_pNetwork->SetPort(_wtoi(pbMetaStructure->lpValue));
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"NetworkPollInterval") && pInstanceData->m_pNetwork)
	{
		if (pbMetaStructure->lpValue > 0)
		{
			return pInstanceData->m_pNetwork->SetNetworkPollInterval(_wtoi(pbMetaStructure->lpValue));
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"ConnectionTimeout") && pInstanceData->m_pNetwork)
	{
		if (pbMetaStructure->lpValue > 0)
		{
			return pInstanceData->m_pNetwork->SetConnectionTimeout(_wtoi(pbMetaStructure->lpValue));
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"Start") && pInstanceData->m_pNetwork)
	{
		return pInstanceData->m_pNetwork->StartNetworkChecking();
	}
	else if (cmp (pbMetaStructure->lpParameter, L"Stop") && pInstanceData->m_pNetwork)
	{
		return pInstanceData->m_pNetwork->StopNetworkChecking();
	}
	/////////////////////////////////
	//  META TAGS HANDLED BY CWAN  //
	/////////////////////////////////
	//  pInstanceData->m_pWAN will be NULL if the connection manager failed to 
	//  initialise
	else if (cmp (pbMetaStructure->lpParameter, L"WANStatusEvent"))
	{
		if (pInstanceData->m_pWAN)
		{
			if (pbMetaStructure->lpValue > 0)
			{
				pInstanceData->m_pWAN->SetWANEventURI(pbMetaStructure->lpValue);
				return TRUE;
			}
		}
		else
			Log(PB_LOG_WARNING, L"Attempted to set WANStatusEvent but WAN is not supported on this device", _T(__FUNCTION__), __LINE__);
	}
	else if (cmp (pbMetaStructure->lpParameter, L"ConnectWAN"))
	{
		if (pInstanceData->m_pWAN)
			return pInstanceData->m_pWAN->Connect(pbMetaStructure->lpValue, TRUE);
		else
			Log(PB_LOG_WARNING, L"Attempted to set ConnectWAN but WAN is not supported on this device", _T(__FUNCTION__), __LINE__);
	}
	else if (cmp (pbMetaStructure->lpParameter, L"DisconnectWAN"))
	{
		if (pInstanceData->m_pWAN)
			return pInstanceData->m_pWAN->Disconnect(TRUE);
		else
			Log(PB_LOG_WARNING, L"Attempted to set DisconnectWAN but WAN is not supported on this device", _T(__FUNCTION__), __LINE__);
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, 
			L"Unrecognised Meta Tag Provided to Network Module or it is not supported by the device",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

CNetworkInstanceData::CNetworkInstanceData()
{
	m_pNetwork = NULL; 
	m_pWAN = NULL;
}

