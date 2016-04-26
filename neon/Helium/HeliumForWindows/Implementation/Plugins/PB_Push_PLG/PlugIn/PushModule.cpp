/******************************************************************************/
#include <windows.h>
#include "PushModule.h"

/******************************************************************************/
extern void EnableUnattended (void);
extern void DisableUnattended (void);
extern void ResetPowerCountdown (void);

/******************************************************************************/
BOOL CPushModule::StringToBool (LPCTSTR pstring)
{
	if (cmp (pstring, L"true") || cmp (pstring, L"yes") || cmp (pstring, L"enabled") || cmp (pstring, L"enable"))
		return true;

	return false;
}

/******************************************************************************/
CPushModule *CPushModule::pThis;

/******************************************************************************/
CPushModule::CPushModule ()
{
	pThis = this;
}

/******************************************************************************/
CPushModule::~CPushModule ()
{
}

/******************************************************************************/
BOOL CPushModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Push");

	return true;
}

/******************************************************************************/
void CPushModule::onDeInit(PPBSTRUCT pPBStructure)
{
}

/******************************************************************************/
BOOL CPushModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	// Allocate structure to hold instance data
	INSTANCE_DATA *pdata = new INSTANCE_DATA;
	pdata->nID = pInstStruct->instID;
	wcscpy (pdata->sNavigate, L"");
	pdata->bUnattendedEnabled = FALSE;
	pdata->bMessagesRegistered = FALSE;
	pdata->pPowerUp = NULL;

	// Create new server, add instance data, store in plugin framework
	CHttpServer *pserver = new CHttpServer (OnRequest);
	pserver->pUserData = pdata;
	pInstStruct->pWrappedObj = pserver;

	return TRUE;
}

/******************************************************************************/
BOOL CPushModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	CHttpServer *pserver = (CHttpServer*) (pInstStruct->pWrappedObj);

	if (pserver)
	{
		pserver->Stop();

		// Free instance data
		if (pserver->pUserData)
		{
			INSTANCE_DATA *pdata = (INSTANCE_DATA*) pserver->pUserData;

			if (pdata->bUnattendedEnabled)
				DisableUnattended ();

			if (pdata->pPowerUp)
				delete pdata->pPowerUp;

			delete pdata;
		}

		delete pserver;
	}

	return TRUE;
}

/******************************************************************************/
BOOL CPushModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	CHttpServer *pserver = (CHttpServer*) (pParam);

	if (!pserver)
		return FALSE;

	INSTANCE_DATA *pdata = (INSTANCE_DATA*) pserver->pUserData;

	if (cmp (pbMetaStructure->lpParameter, L"port"))
	{
		if (pbMetaStructure->lpValue)
			pserver->SetPort (_ttoi (pbMetaStructure->lpValue));
	}
	else if (cmp (pbMetaStructure->lpParameter, L"passkey"))
	{
		if (pbMetaStructure->lpValue)
		{
			//  Test to see whether or not the passkey is being disabled
			if (cmp(pbMetaStructure->lpValue, L""))
				pserver->SetPasskey(L"");
			else
				pserver->SetPasskey (pbMetaStructure->lpValue);
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"response"))
	{
		if (pbMetaStructure->lpValue)
			pserver->SetResponseFile (pbMetaStructure->lpValue);
	}
	else if (cmp (pbMetaStructure->lpParameter, L"path"))
	{
		if (pbMetaStructure->lpValue)
			pserver->SetPath(pbMetaStructure->lpValue);
	}
	else if (cmp (pbMetaStructure->lpParameter, L"detected"))
	{
		if (pbMetaStructure->lpValue)
			WSAFECOPY (pdata->sNavigate, pbMetaStructure->lpValue);
	}
	else if (cmp (pbMetaStructure->lpParameter, L"start"))
	{
		if (pserver->Start() != HS_SUCCESS)
		{
			WCHAR error [64];
			pserver->GetLastError (error, 64);
			LOG (PB_LOG_WARNING, error);
			return FALSE;
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"stop"))
	{
		pserver->Stop();
	}
	else if (cmp (pbMetaStructure->lpParameter, L"poweron"))
	{
		if (!pdata->pPowerUp)
		{
			pdata->pPowerUp = new CPowerUp (OnPowerUp);
			pdata->pPowerUp->pUserData = (void*) (pPBStructure->iTabID);
			pdata->pPowerUp->Start();
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"unattended"))
	{
		if (pbMetaStructure->lpValue)
		{
			// Only enable unattended once per instance
			if (StringToBool (pbMetaStructure->lpValue))
			{
				if (!pdata->bUnattendedEnabled)
				{
					EnableUnattended ();

					// Register this instance for Windows messages. This means that if there are 
					// multiple instances with unattended enabled each one will get the messages
					// and reset the timer. Not a real problem and unavoidable as there is no
					// non-instance specific way to register for messages.
					if (!pdata->bMessagesRegistered)
					{
						RegisterForMessage (REGISTER_ANY_WINDOW, WM_LBUTTONDOWN);
						RegisterForMessage (REGISTER_ANY_WINDOW, WM_KEYDOWN);
						pdata->bMessagesRegistered = TRUE;
					}

					pdata->bUnattendedEnabled = TRUE;
				}
			}
			else
			{
				if (pdata->bUnattendedEnabled)
				{
					DisableUnattended ();

					// We could unregister for messages here but this is supported in PBModule.

					pdata->bUnattendedEnabled = FALSE;
				}
			}
		}
	}
	else
		// Unknown tag
		return FALSE;

	return TRUE;
}

/******************************************************************************/
BOOL CPushModule::onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject)
{
	CHttpServer *pserver = (CHttpServer*) (pobject);

	if (!pserver)
		return FALSE;

	INSTANCE_DATA *pdata = (INSTANCE_DATA*) pserver->pUserData;

	if (pdata->bUnattendedEnabled)
		ResetPowerCountdown ();

	return FALSE;
}

/******************************************************************************/
/* static */ void CPushModule::OnRequest (CHttpRequest *prequest, CHttpServer *pserver)
{
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) pserver->pUserData;

	// Do nothing if no navigate string defined
	if (wcslen (pdata->sNavigate) == 0)
		return;

	// Get number of name/value pairs found
	int count = prequest->GetCount ();

	// Number of strings to pass to SendPBNavigate - could be less than 'count' later
	int length = 0;

	int n;
	LPCWSTR *pstrings = new LPCWSTR [count];
	TCHAR** pnames = new TCHAR*[count + 1];

	// Ignore names and copy values, unless 'passkey'
	for (n = 0; n < count; n++)
	{
		if (wcsicmp (prequest->GetName (n), L"passkey"))
		{
			pnames[length] = new TCHAR[wcslen(prequest->GetName(n)) + 1];
			wcscpy(pnames[length], prequest->GetName(n));
			pstrings [length++] = prequest->GetValue (n);
		}
	}
	pnames[length] = NULL;

	pThis->SendPBNavigate (pnames, pdata->nID, pdata->sNavigate, length, pstrings);

	delete [] pstrings;
	for (int i = 0; i < count; i++)
		delete [] pnames[i];
	delete [] pnames;
}

/******************************************************************************/
/* static */ void CPushModule::OnPowerUp (void* pparam)
{
	pThis->Log (PB_LOG_INFO, L"Power on detected", _T(__FUNCTION__), __LINE__);

	// Need to use specified navigate string
//	pThis->SendPBNavigate ((int) pparam, L"Power on ('%s')", L"Hello", NULL);
}
