#include <windows.h>
#include "Keylight.h"

//  Maximum allowable length of a log message
#define MAX_LOG_MESSAGE			1024

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
CKeylight::CKeylight(int iInstanceID, PBModule* pModule)
{
	//  Initialise member variables
	m_dwKeylightDLL			= 0;
	m_dwIntensityLevels		= -1;
	m_pModule				= pModule;
	m_iInstanceID			= iInstanceID;
	m_dwKeylightOn			= false;
	m_dwCurrentIntensity	= 0;
	m_dwDefaultState		= false;
	m_dwDefaultIntensity	= 0;
	m_bAppHasFocus			= false;

	NullifyExportedFunctionPointers();

}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
CKeylight::~CKeylight()
{
	UnloadKeylightDLL();
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
BOOL CKeylight::Initialise(bool bAppHasFocus)
{
	TCHAR LogMessage[MAX_LOG_MESSAGE];

	//  Attempt to load the Keylight DLL
	BOOL bLoadResult = LoadKeylightDLL();
	if (!bLoadResult)
	{
		//  Keylight DLL has failed to load, log this error
		wsprintf(LogMessage, L"%s %s %s", L"Unable to Load", KEYLIGHT_API_DLL, 
			L"DLL");
		m_pModule->Log(PB_LOG_WARNING, LogMessage, 
			_T(__FUNCTION__), __LINE__);
	}
	else
	{
		//  Keylight DLL has successfully loaded
		//  Find out the number of intensity levels supported by this device, 
		//  this is stored as a member variable to prevent the user requesting
		//  an intensity too great.
		LPDWORD pdwIntensityLevels = &m_dwIntensityLevels;
		DWORD retVal;
		retVal = lpfn_KeylightGetIntensityLevels(pdwIntensityLevels);
		if (retVal != E_KLT_SUCCESS)
		{
			m_pModule->Log(PB_LOG_WARNING, 
				L"Unable to obtain keylight intensity levels", 
				_T(__FUNCTION__), __LINE__);
			return FALSE;	
		}
		
		//  Log the number of Intensity levels available to inform the user
		wsprintf(LogMessage, 
			L"The current device supports levels of keylight intensity between 0 and %i inclusive", 
			m_dwIntensityLevels - 1);
		m_pModule->Log(PB_LOG_INFO, LogMessage, _T(__FUNCTION__), __LINE__);

		//  Set the current keylight on / off and intensity values
		DWORD dwTempVariable;
		retVal = lpfn_KeylightGetState(&dwTempVariable);
		if (retVal == E_KLT_SUCCESS)
			m_dwKeylightOn = dwTempVariable;
		else
		{
			m_pModule->Log(PB_LOG_WARNING,
				L"Unable to obtain Keylight current setting", 
				_T(__FUNCTION__), __LINE__);
		}
		retVal = lpfn_KeylightGetIntensity(&dwTempVariable);
		if (retVal == E_KLT_SUCCESS)
			m_dwCurrentIntensity = dwTempVariable;
		else
		{
			m_pModule->Log(PB_LOG_WARNING,
				L"Unable to obtain Keylight current intensity", 
				_T(__FUNCTION__), __LINE__);
		}
		// Use the retrieved initial values as defaults
		m_dwDefaultState = m_dwKeylightOn;
		m_dwDefaultIntensity = m_dwCurrentIntensity;
		m_bAppHasFocus = bAppHasFocus;
	}
	
	return bLoadResult;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
BOOL CKeylight::TurnOn()
{
	//  Ascertain whether the SetState method has been successfully loaded
	//  from the DLL
	if (lpfn_KeylightSetState != NULL)
	{
		if (!m_bAppHasFocus)
		{
			m_dwKeylightOn = true;
			return TRUE;
		}
		//  Turn the Keylight On
		DWORD retVal = lpfn_KeylightSetState(KEYLIGHT_ON);
		if (retVal != E_KLT_SUCCESS)
		{
			//  EMDK layer has thrown an error turning the keylight on, 
			//  log this.
			m_pModule->Log(PB_LOG_WARNING, 
				L"Error illuminating keylight", 
				_T(__FUNCTION__), __LINE__);
			return FALSE;
		}
		else
		{	
			//  Keylight successfully turned on.
			m_dwKeylightOn = true;
			return TRUE;
		}
	}
	else
	{
		//  The SetState function has not been made to point to the exported 
		//  DLL function, the class is not initialised.
		m_pModule->Log(PB_LOG_WARNING, 
		L"Attempted to illuminate the keylight without initialising", 
		_T(__FUNCTION__), __LINE__);
		return FALSE;
	}
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
BOOL CKeylight::TurnOff()
{
	//  Ascertain whether the SetState method has been successfully loaded
	//  from the DLL
	if (lpfn_KeylightSetState != NULL)
	{
		if (!m_bAppHasFocus)
		{
			m_dwKeylightOn = false;
			return TRUE;
		}
		//  Turn the keylight off
		DWORD retVal = lpfn_KeylightSetState(KEYLIGHT_OFF);
		if (retVal != E_KLT_SUCCESS)
		{
			//  EMDK layer has thrown an error turning the keylight off, 
			//  log this.
			m_pModule->Log(PB_LOG_WARNING, 
				L"Error illuminating keylight", 
				_T(__FUNCTION__), __LINE__);
			return FALSE;
		}
		else
		{
			//  Keylight successfully turned off
			m_dwKeylightOn = false;
			return TRUE;
		}
	}
	else
	{
		//  The SetState function has not been made to point to the exported 
		//  DLL function, the class is not initialised.
		m_pModule->Log(PB_LOG_WARNING, 
		L"Attempted to extinguish the keylight without initialising", 
		_T(__FUNCTION__), __LINE__);
		return FALSE;
	}
}


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
BOOL CKeylight::SetIntensity(DWORD dwValueToSet)
{
	//  Ascertain whether the function has been successfully loaded from the DLL
	if (lpfn_KeylightSetIntensity != NULL)
	{
		//  If the user attempts to set the intensity to a range which the
		//  device does not support then set to the closest appropriate
		//  value, e.g. (m_dwIntensityLevels + 10) will
		//  set to (m_dwIntensitylevels - 1), the maximum allowable value.
		if (dwValueToSet >= m_dwIntensityLevels)
		{
			//  User requested a value greater than the max value, log
			//  and set to maximum.
			DWORD originallyRequestedLevel = dwValueToSet;
			dwValueToSet = m_dwIntensityLevels - 1;
			TCHAR LogMessage[MAX_LOG_MESSAGE];
			wsprintf(LogMessage, 
				L"%s%i%s%i%s", 
				L"Attempted to set keylight intensity to ", 
				originallyRequestedLevel, 
				L" however the maximum allowable value is ", 
				m_dwIntensityLevels - 1, 
				L", defaulting to maximum");
			m_pModule->Log(PB_LOG_WARNING, LogMessage, 
				_T(__FUNCTION__), __LINE__);
		}

		//  Set the keylight intensity
		m_dwCurrentIntensity = dwValueToSet;
		if (m_bAppHasFocus)
		{
			DWORD retVal = lpfn_KeylightSetIntensity(dwValueToSet);
			if (retVal != E_KLT_SUCCESS)
			{
				//  EMDK layer has thrown an error setting the keylight intensity,
				//  log this.
				m_pModule->Log(PB_LOG_WARNING, L"Error setting keylight intensity", 
					_T(__FUNCTION__), __LINE__);
				return FALSE;
			}
		}
		//  Intensity level successfully set or not in focus
		return TRUE;
	}
	else
	{
		//  Keylight Set Intensity function exported from DLL is NULL
		m_pModule->Log(PB_LOG_WARNING, 
		L"Attempted to set the intensity of the keylight without initialising", 
		_T(__FUNCTION__), __LINE__);
		return FALSE;
	}
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009  (DCC: Initial Creation)
*/
BOOL CKeylight::RefreshHardware()
{
	//  m_dwKeylightOn and m_dwCurrentIntensity hold the current state of the 
	//  keylight as known by this class instance.  Ensure the hardware is in 
	//  sync with these values.
	BOOL bRetVal = SetIntensity(m_dwCurrentIntensity);
	if (m_dwKeylightOn)
	{
		//  Turn the Keylight On
		bRetVal = TurnOn() & bRetVal;
	}
	else
	{
		//  Turn the Keylight Off
		bRetVal = TurnOff() & bRetVal;
	}
	return bRetVal;
}

//////////////////////////////////
//  Private Functions
//////////////////////////////////
	
/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
BOOL CKeylight::LoadKeylightDLL()
{
	NullifyExportedFunctionPointers();

	m_dwKeylightDLL = (DWORD)LoadLibrary(KEYLIGHT_API_DLL);
	if (!m_dwKeylightDLL)
	{
		TCHAR LogMessage[MAX_LOG_MESSAGE];
		wsprintf(LogMessage, L"%s %s %s", L"Could not find", KEYLIGHT_API_DLL, 
			L"DLL");
		m_pModule->Log(PB_LOG_ERROR, LogMessage, 
			_T(__FUNCTION__), __LINE__);
	}
	
	//  Map our function pointers to functions exported from the DLL
	lpfn_KeylightGetIntensityLevels = 
		(LPFN_KEYLIGHT_GET_INTENSITY_LEVELS)GetProcAddress
		((HMODULE)m_dwKeylightDLL, KEYLIGHT_GETINTENSITYLEVELS);
	lpfn_KeylightGetIntensity = 
		(LPFN_KEYLIGHT_GET_INTENSITY)GetProcAddress
		((HMODULE)m_dwKeylightDLL, KEYLIGHT_GETINTENSITY);
	lpfn_KeylightSetIntensity = 
		(LPFN_KEYLIGHT_SET_INTENSITY)GetProcAddress
		((HMODULE)m_dwKeylightDLL, KEYLIGHT_SETINTENSITY);
	lpfn_KeylightGetState = 
		(LPFN_KEYLIGHT_GET_STATE)GetProcAddress
		((HMODULE)m_dwKeylightDLL, KEYLIGHT_GETSTATE);
	lpfn_KeylightSetState = 
		(LPFN_KEYLIGHT_SET_STATE)GetProcAddress
		((HMODULE)m_dwKeylightDLL, KEYLIGHT_SETSTATE);

	//  All functions should now be loaded
	if ((!lpfn_KeylightGetIntensityLevels) ||
		(!lpfn_KeylightGetIntensity) ||
		(!lpfn_KeylightSetIntensity) ||
		(!lpfn_KeylightGetState) ||
		(!lpfn_KeylightSetState))
	{
		//  One of the functions failed to load
		UnloadKeylightDLL();

		TCHAR LogMessage[MAX_LOG_MESSAGE];
		wsprintf(LogMessage, L"%s %s %s", L"Unable to load all functions from", 
			KEYLIGHT_API_DLL, L"DLL");
		m_pModule->Log(PB_LOG_ERROR, LogMessage, 
			_T(__FUNCTION__), __LINE__);

		return FALSE;
	}

	//  Everything Loaded OK
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
void CKeylight::UnloadKeylightDLL()
{
	//  If the Keylight DLL had previously been loaded then unload it and
	//  set all function pointers to NULL;
	if (m_dwKeylightDLL)
		FreeLibrary((HMODULE)m_dwKeylightDLL);
	
	//  Nullify member variables
	m_dwKeylightDLL = 0;
	NullifyExportedFunctionPointers();

}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		September 2008  (DCC: Initial Creation)
* \date		November 2009   (DCC: Ported to PB3.0 Plug-in format)
*/
void CKeylight::NullifyExportedFunctionPointers()
{
	lpfn_KeylightGetIntensityLevels	= NULL;
	lpfn_KeylightGetIntensity		= NULL;
	lpfn_KeylightSetIntensity		= NULL;
	lpfn_KeylightGetState			= NULL;
	lpfn_KeylightSetState			= NULL;
}

/**
* \author	John Heywood (JH, VBC863)
* \date		September 2012  (JH: Initial Creation)
*/
BOOL CKeylight::ApplicationFocusChange(bool bActivated)
{
	BOOL bRet = FALSE;

	if (m_bAppHasFocus == bActivated)
	{
		// nothing to do
		bRet = TRUE;
	}
	else if (m_dwKeylightDLL)
	{
		// set the flag to prevent/allow hardware access
		m_bAppHasFocus = bActivated;
		if (bActivated)
		{
			// We have just gained focus
			bRet = RefreshHardware();
		}
		else
		{
			// we have just lost focus - reset hardware to defaults.
			DWORD dwRet = lpfn_KeylightSetIntensity(m_dwDefaultIntensity);
			dwRet |= lpfn_KeylightSetState(m_dwDefaultState);
			bRet = (dwRet == E_KLT_SUCCESS);
		}
	}
	return bRet;
}