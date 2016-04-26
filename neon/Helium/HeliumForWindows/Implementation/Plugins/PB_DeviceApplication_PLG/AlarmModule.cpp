#include "AlarmModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CAlarmModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Alarm");
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CAlarmModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	// Create new server, add instance data, store in plugin framework
	CAlarm *pAlarm = new CAlarm(pInstStruct->instID, this);
	pInstStruct->pWrappedObj = pAlarm;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CAlarmModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	CAlarm *pAlarm = (CAlarm*) (pInstStruct->pWrappedObj);

	if (pAlarm)
		delete pAlarm;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CAlarmModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	CAlarm *pAlarm = (CAlarm*) (pParam);
	if (!pAlarm)
		return FALSE;

	if (cmp (pbMetaStructure->lpParameter, L"Interval"))
	{
		if (!pbMetaStructure->lpValue)
			return FALSE;

		//  The Interval value is expressed as HH-MM-SS, therefore expect 
		//  a valid value.  Check the value is valid and populate the 
		//  m_AlarmInfo structure.
		BOOL parseSuccess = pAlarm->ParseInterval(pbMetaStructure->lpValue);

		//  Log an error if the value string was not in the correct format
		if (!parseSuccess)
		{
			Log(PB_LOG_WARNING, 
				L"Value for Interval is not in the correct format, expect HH-MM-SS", 
				_T(__FUNCTION__), __LINE__);
		}
		return parseSuccess;
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Repeat"))
	{
		if (cmp(pbMetaStructure->lpValue, L"True"))
			pAlarm->SetRepeat(TRUE);
		else if (cmp(pbMetaStructure->lpValue, L"False"))
			pAlarm->SetRepeat(FALSE);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Time"))
	{
		if (!pbMetaStructure->lpValue)
			return FALSE;

		//  The time to trigger the alarm is stored in the value, 
		//  check the value is valid and populate the m_AlarmInfo structure.
		BOOL parseSuccess = pAlarm->ParseTime(pbMetaStructure->lpValue);

		//  Log an error if the value string was not in the correct format
		if (!parseSuccess)
		{
			Log(PB_LOG_WARNING, 
				L"Value for Time is not in the correct format, expect yyyy-mm-ddthh:mm:sstzd (e.g. 2005-03-22t11:56:00+01:00)", 
				_T(__FUNCTION__), __LINE__);
		}

		return parseSuccess;
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Clear"))
	{
		BOOL retVal = pAlarm->ClearAlarmNotifications();
		pAlarm->SetNavigationURI(L"");
		return retVal;
	}
	else if (cmp(pbMetaStructure->lpParameter, L"AlarmTriggered"))
	{
		if (pbMetaStructure->lpValue && *pbMetaStructure->lpValue)
			pAlarm->SetNavigationURI(pbMetaStructure->lpValue);
		else
			pAlarm->SetNavigationURI(L"");
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Set"))
	{
		//  Set the Alarm (New for PB3.0)
		//  Value may contain the URI string if converted from EMML 1.0 spec
		if (pbMetaStructure->lpValue && *pbMetaStructure->lpValue)
			pAlarm->SetNavigationURI(pbMetaStructure->lpValue);

		pAlarm->Set();
	}
	else
	{
		// Unrecognised tag, to make compatible with PB2.x assume the parameter 
		//  is the callback URI
		if (*pbMetaStructure->lpParameter)
		{
			pAlarm->SetNavigationURI(pbMetaStructure->lpParameter);
			pAlarm->Set();
		}
	}

	return TRUE;
}