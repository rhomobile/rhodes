#include "ApplicationModule.h"
#include <vector>

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
CApplicationModule::CApplicationModule(PPBCORESTRUCT pPBCoreStructure)
{
	m_pPBCoreStructure = pPBCoreStructure;
	// Initially the window minimized state will be set to false.
	isWindowMinimized = false;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CApplicationModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Application");
	RegisterForEvent (PB_APPFOCUSEVENT);
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent(PB_WINDOWCHANGED_EVENT);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CApplicationModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent (PB_APPFOCUSEVENT);
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(PB_WINDOWCHANGED_EVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CApplicationModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create this module's instance data
	INSTANCE_DATA *pData = new INSTANCE_DATA;
	pData->instanceID = pInstStruct->instID;
	//  Set URIs to empty strings
	memset(pData->tcApplicationEvent, 0, sizeof(TCHAR));
	memset(pData->tcVersionEvent, 0, sizeof(TCHAR));
	pInstStruct->pWrappedObj = pData;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CApplicationModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	if (pData)
	{
		delete pData;
	}
	
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CApplicationModule::onBeforeNavigate(int iInstID)
{
	INSTANCE_DATA *pData = NULL;
	pData = (INSTANCE_DATA*) GetObjFromID(iInstID);

	if (pData != NULL)
	{
		//  Set the Nav String to ""
		wcscpy(pData->tcApplicationEvent, L"");
		wcscpy(pData->tcVersionEvent, L"");
		return TRUE;
	}
	else
		return FALSE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CApplicationModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	Log(PB_LOG_WARNING, L"The Application API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Application API for future releases", _T(__FUNCTION__), __LINE__);

	INSTANCE_DATA *pData = (INSTANCE_DATA*) pParam;

	if (cmp (pbMetaStructure->lpParameter, L"ApplicationEvent"))
	{
		//  Set the URI to be sent when Browser minimises or restores
		//  Note only the application minimising or restoring Browser will 
		//  receive the callback.
		if (wcslen(pbMetaStructure->lpValue) <= MAX_URL)
			wcscpy(pData->tcApplicationEvent, pbMetaStructure->lpValue);
		else
			return FALSE;
	}
	else if (cmp (pbMetaStructure->lpParameter, L"VersionEvent"))
	{
		//  Set the URI to respond to the GetVersionComponents method.
		if (wcslen(pbMetaStructure->lpValue) <= MAX_URL)
			wcscpy(pData->tcVersionEvent, pbMetaStructure->lpValue);
		else
			return FALSE;
	}
	else if (cmp (pbMetaStructure->lpParameter, L"Quit"))
	{
		//  Quit Browser
		if (m_pPBCoreStructure)
			return m_pPBCoreStructure->pBrowserQuit(pData->instanceID, m_szModName);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Minimize"))
	{
		//  Minimise Browser
		if (m_pPBCoreStructure)
			return m_pPBCoreStructure->pBrowserMininmize(pData->instanceID, m_szModName);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Restore"))
	{
		//  Restore Browser
		if (m_pPBCoreStructure)
			return m_pPBCoreStructure->pBrowserRestore(pData->instanceID, m_szModName);
	}

	else if (cmp(pbMetaStructure->lpParameter, L"GetVersion"))
	{
		if (*pData->tcVersionEvent)
		{
			//  Calculate the Product version via the core pointer
			CVersion* listComponentVersionsHead = new CVersion();
			int iLengthOfVersionString = m_pPBCoreStructure->pGetComponentVersions(NULL, listComponentVersionsHead);
			if (iLengthOfVersionString > 0)
			{
				//  Retrieve the versions of the Core, Plugins, engines and ActiveX components
				TCHAR* tcProductVersion = new TCHAR[iLengthOfVersionString];
				m_pPBCoreStructure->pGetComponentVersions(tcProductVersion, listComponentVersionsHead);
				//  tcProductVersion will be returned to the user

				/*
				//  DCC (23/01/2013): Deleting this code as we move to semantic versioning and 
				//  common API set we are no longer supporting individual component versions in the 
				//  Version API
				//  Work out if hot fixes have been applied
				TCHAR* tcHotFixArray = new TCHAR[wcslen(L"new Array()") + 1];
				wcscpy(tcHotFixArray, L"new Array()");
				std::vector<int> vInstalledHotFixes;
				CVersion* tempVer = listComponentVersionsHead;
				while (tempVer != NULL && tempVer->pNext != NULL)
				{
					//  Head element is empty so do ->pNext first
					tempVer = tempVer->pNext;
					if (tempVer->iHotFixVer > 0)
					{
						//  Hot fix number is > 0, add this to the list of Hot Fixes if we haven't
						//  already done so
						bool bFoundHotFix = false;
						for (uint i = 0; i < vInstalledHotFixes.size(); i++)
						{
							if (vInstalledHotFixes.at(i) == tempVer->iHotFixVer)
							{
								bFoundHotFix = true;
								break;
							}
						}
						if (!bFoundHotFix)
						{
							//  This is a new Hot Fix, add it to the Vector
							vInstalledHotFixes.push_back(tempVer->iHotFixVer);
						}
					}
				}
				//  vInstalledHotFixes now contains a vector of installed hot fixes
				if (vInstalledHotFixes.size() > 0)
				{
					delete[] tcHotFixArray;
					tcHotFixArray = new TCHAR[wcslen(L"new Array()") + (vInstalledHotFixes.size() * 2) + 1];
					wcscpy(tcHotFixArray, L"new Array(");
					for (uint i=0; i<vInstalledHotFixes.size(); i++)
					{
						wsprintf(tcHotFixArray, L"%s'%i'", tcHotFixArray, vInstalledHotFixes.at(i));
						if (i != vInstalledHotFixes.size() - 1)
							wcscat(tcHotFixArray, L",");
					}

					//  We have finished populating the hot fix array
					wcscat(tcHotFixArray, L")");
				}	
				//  tcHotFixArray will be sent to the User
		
				//  The third step is to pass an array of component versions to the user
				TCHAR* tcComponentsJSArray = new TCHAR[MAX_URL];
				memset(tcComponentsJSArray, 0, MAX_URL * sizeof(TCHAR));
				wcscpy(tcComponentsJSArray, L"new Array(");
				tempVer = listComponentVersionsHead;
				while (tempVer != NULL && tempVer->pNext != NULL)
				{
					//  Head element is empty so do ->pNext first
					tempVer = tempVer->pNext;
					//  Check the array is not getting too large
					WCHAR newComponentVersion[50];
					wsprintf(newComponentVersion, L"new Array('%s','%i.%i.%i.%i.%i')", 
						tempVer->tcComponentName, tempVer->iMajorVer, tempVer->iFeatureVer, 
						tempVer->iMaintenanceVer, tempVer->iReleaseCandidateVer, tempVer->iHotFixVer);
					wcscat(tcComponentsJSArray, newComponentVersion);
					if (wcslen(tcComponentsJSArray) > MAX_URL - 30)
						break;
					if (tempVer->pNext != NULL)
						wcscat(tcComponentsJSArray, L",");
				}
				wcscat(tcComponentsJSArray, L")");
				*/
				//  Send to Application
				SendPBNavigate(tcVersionEventNames, pData->instanceID, pData->tcVersionEvent, 
					tcProductVersion, L"new Array()", L"new Array(new Array('',''))", NULL);

				//  Delete memory we allocated
				delete[] tcProductVersion;
				//  DCC (23/01/2013): Deleting this code as we move to semantic versioning and 
				//  common API set we are no longer supporting individual component versions in the 
				//  Version API
				/*
				delete[] tcHotFixArray;
				delete[] tcComponentsJSArray;
				*/
				//  Delete memory associated with version list
				CVersion* nextNode = listComponentVersionsHead->pNext;
				CVersion* currentNode = listComponentVersionsHead;
				while (nextNode)
				{
					//  The Head of the list has no version data, all data is contained in subsequent nodes
					delete currentNode;
					currentNode = nextNode;
					nextNode = currentNode->pNext;
				}
				delete listComponentVersionsHead;
			}
		}
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Application Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		April 2010
*/
BOOL CApplicationModule::onWindowChanged (int iEvent)
{
	INSTANCE_DATA *pData = NULL;
	pData = (INSTANCE_DATA*) GetObjFromID(0);
	switch (iEvent)
	{
	case PB_WINRESTORED:
		{
			//  Send out the navigate String
			if (*pData->tcApplicationEvent && (wcscmp(pData->tcApplicationEvent, L"") != 0) && isWindowMinimized)
			{
				//  There is a registered navigate string
				SendPBNavigate(tcApplicationEventNames, pData->instanceID, pData->tcApplicationEvent, L"Restored", NULL);
			}
			//Setting it to false as window is already restored and the callback has already been fired.
			isWindowMinimized = false;
		}
		break;
	case PB_WINMINIMIZED:
		{
			//Setting it to true as window is going to be minimized.
			isWindowMinimized = true;
			//  Send out the navigate String
			if (*pData->tcApplicationEvent && (wcscmp(pData->tcApplicationEvent, L"") != 0))
			{
				//  There is a registered navigate string
				SendPBNavigate(tcApplicationEventNames, pData->instanceID, pData->tcApplicationEvent, L"Minimized", NULL);
			}
		}
		break;
	}

	//  Do not block this message from proceeding further in the pump
	return FALSE;
}
