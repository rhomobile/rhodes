#include "StdAfx.h"
#include "EMMP.h"
#include "../../../Common/Public/pbplugin.h"


CEMMP::CEMMP(PBModule * pMod, int iTabID)
	:m_pProfilePageName(NULL)
	,m_pMod(pMod)
	,m_hTransfered(NULL)
{
	//get the EMML object
	m_pEMMLObj = pMod->GetEMMLObj(iTabID);

}

CEMMP::~CEMMP(void)
{
	//clean up
	delete [] m_pProfilePageName;
	m_pProfilePageName = NULL;

	CloseHandle(m_hTransfered);
	m_hTransfered = NULL;

}

int CEMMP::FileTransferredEvent(PVARSTRUCT pVars,int iTABID,LPARAM lParam)
{
	CEMMP* pEMMP = (CEMMP*) lParam;

	pEMMP->m_pMod->Log(PB_LOG_INFO, L"EMML Profile downloaded", _T(__FUNCTION__), __LINE__);
	SetEvent(pEMMP->m_hTransfered);

	return 1;
}

int CEMMP::Import(LPCTSTR lpProfilePage)
{
	
	//check to see if it's local (ie, starts with file://)
	if(wcsnicmp(lpProfilePage, L"file://", 7) == 0)
	{
		m_pMod->Log(PB_LOG_INFO, L"Importing local EMML Profile Page", _T(__FUNCTION__), __LINE__);
		//store the local file name
		DWORD dwProfileCount = m_pEMMLObj->ImportEMMLProfilePage(&lpProfilePage[7], m_pProfilePageName);
		TCHAR tcMsg[1000];
		if(m_pProfilePageName)
			wsprintf(tcMsg, L"[%d] profile(s) imported from file [%s] to named profile [%s]", dwProfileCount, lpProfilePage, m_pProfilePageName);
		else
			wsprintf(tcMsg, L"[%d] profile(s) imported from file [%s] to temporary profiles", dwProfileCount, lpProfilePage);

		m_pMod->Log(PB_LOG_INFO, tcMsg, _T(__FUNCTION__), __LINE__);


		//delete the profile name now it's been imported
		delete [] m_pProfilePageName;
		m_pProfilePageName = NULL;

		return true;

	}
	//not local so download using FileTransfer
	else 
	{
		m_pMod->Log(PB_LOG_INFO, L"Importing remote EMML Profile Page", _T(__FUNCTION__), __LINE__);

		if(((CEMMPMod*)m_pMod)->m_FileTransferIMO == NULL)
			((CEMMPMod*)m_pMod)->m_FileTransferIMO = m_pMod->CreateIMO(L"filetransfer");

		if(!((CEMMPMod*)m_pMod)->m_FileTransferIMO)
		{
			m_pMod->Log(PB_LOG_ERROR, L"Could not create FileTransfer IMO", _T(__FUNCTION__), __LINE__);

			return 0; //return that no profiles were imported
		}

		//Sabir VT
		//Fix for EMML import for HTTP
		//Method name is case sensitive, corrected,
		//destination file path was having escape character, fixed
		//transfer protocol was not set, before Transfer method invocation
		m_pMod->SetIMOCallBack(((CEMMPMod*)m_pMod)->m_FileTransferIMO, &CEMMP::FileTransferredEvent, (LPARAM)this);
		m_pMod->SetIMOProperty(((CEMMPMod*)m_pMod)->m_FileTransferIMO, L"TransferEvent", L"Javascript://IMO");
		m_pMod->SetIMOProperty(((CEMMPMod*)m_pMod)->m_FileTransferIMO, L"source", lpProfilePage);
		m_pMod->SetIMOProperty(((CEMMPMod*)m_pMod)->m_FileTransferIMO, L"destination", L"file://\\\\_tempprofile.emmp");
		m_pMod->SetIMOProperty(((CEMMPMod*)m_pMod)->m_FileTransferIMO, L"overwrite", L"true");
		m_pMod->SetIMOProperty(((CEMMPMod*)m_pMod)->m_FileTransferIMO, L"Protocol", L"Http");

		m_hTransfered = CreateEvent(0, TRUE, FALSE, 0);
		m_pMod->CallIMOMethod(((CEMMPMod*)m_pMod)->m_FileTransferIMO, L"Transfer");

		if(WaitForSingleObject(m_hTransfered, 60000) == WAIT_OBJECT_0)
		{
			DWORD dwProfileCount = m_pEMMLObj->ImportEMMLProfilePage(L"_tempprofile.emmp", m_pProfilePageName);
			
			TCHAR tcMsg[1000];
			if(m_pProfilePageName)
				wsprintf(tcMsg, L"[%d] profile(s) imported from file [%s] to named profile [%s]", dwProfileCount, lpProfilePage, m_pProfilePageName);
			else
				wsprintf(tcMsg, L"[%d] profile(s) imported from file [%s] to temporary profiles", dwProfileCount, lpProfilePage);

			m_pMod->Log(PB_LOG_INFO, tcMsg, _T(__FUNCTION__), __LINE__);
		}
		else
		{
			m_pMod->Log(PB_LOG_ERROR, L"Timeout occurred downloading EMML profile.", _T(__FUNCTION__), __LINE__);
			return false;
		}

		CloseHandle(m_hTransfered);
		m_hTransfered = NULL;

		//delete the profile name now it's been imported
		delete [] m_pProfilePageName;
		m_pProfilePageName = NULL;

		return true;
	}

	return false;
}

void CEMMP::SetName(LPCTSTR lpProfilePageName)
{
	if(lpProfilePageName == NULL || 
			lpProfilePageName[0] == NULL)
		return;


	//delete the name first just in case
	delete [] m_pProfilePageName;
	m_pProfilePageName = NULL;

	//create some memory for the new name
	m_pProfilePageName = new TCHAR[wcslen(lpProfilePageName)+1];

	//copy over the name if there was memory allocated
	if(m_pProfilePageName)
		wcscpy(m_pProfilePageName, lpProfilePageName);

}

BOOL CEMMP::Delete(LPCTSTR lpProfilePageName)
{
	m_pEMMLObj->DeleteEMMLProfilePage(lpProfilePageName);

	TCHAR tcMsg[1000];
	wsprintf(tcMsg, L"Deleted Named EMML Profile Page [%s]", lpProfilePageName);

	m_pMod->Log(PB_LOG_INFO, tcMsg, _T(__FUNCTION__), __LINE__);

	return true;
}
LRESULT CALLBACK CEMMP::DoEMMLTagFunc( LPPBNAMEVALUE ppbnvpStruct, LPARAM lParam )
{

	DEBUGMSG(1, (L"EMML Proc: %s, %s, %s\n", ppbnvpStruct->szModule, ppbnvpStruct->szName, ppbnvpStruct->szValue));

	CEMMP* pEMMP = (CEMMP*) lParam;

	if(ppbnvpStruct->szValue == NULL || ppbnvpStruct->szValue[0] == NULL)
	{
		pEMMP->m_pMod->CallModMethod(ppbnvpStruct->szModule, ppbnvpStruct->szName);
	}
	else
	{
		pEMMP->m_pMod->SetModProperty(ppbnvpStruct->szModule, ppbnvpStruct->szName, ppbnvpStruct->szValue);
	}
	
	
	return 0;
}

BOOL CEMMP::Apply(LPCTSTR lpProfilePageName)
{	
	m_pEMMLObj->ApplyEMMLProfile(lpProfilePageName, &CEMMP::DoEMMLTagFunc, (LPARAM)this);

	TCHAR tcMsg[1000];
	wsprintf(tcMsg, L"Applied EMML Profile [%s]", lpProfilePageName);

	m_pMod->Log(PB_LOG_INFO, tcMsg, _T(__FUNCTION__), __LINE__);

	return true;
}

