#include "gpswrapper.h"
#include <iostream>
#include <service.h>

//extern BOOL Log	(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
//			LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);

using namespace std;

typedef HANDLE (CALLBACK* GPSOpenDeviceType)(HANDLE, HANDLE, const WCHAR*, DWORD);
typedef DWORD  (CALLBACK* GPSCloseDeviceType)(HANDLE);
typedef DWORD  (CALLBACK* GPSGetPositionType)(HANDLE, GPS_POSITION*, DWORD, DWORD);
typedef DWORD  (CALLBACK* GPSGetDeviceStateType)(GPS_DEVICE*);

EngineLocation* CGpsWrapper::s_pEkCachedPosition = NULL;
EngineLocationError CGpsWrapper::s_lastErrorCode = UNKNOWN_ERROR;
LPCTSTR CGpsWrapper::s_lastErrorMessage = L"UNKNOWN_ERROR";

CGpsWrapper::CGpsWrapper(IGpsWrapperObserver& observer, bool bGeolocationEnabled):
	mObserver(observer)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	m_hGpsHandle = NULL;
	m_hThreadHandle = NULL;
	m_hLocationDataChangedEvent = NULL;
	m_hDeviceStateChangedEvent = NULL;
	m_dwLastError = ERROR_SUCCESS;
	m_bThreadActive = FALSE;
	//Initialise mCachedPosition
	if (s_pEkCachedPosition == NULL)
		s_pEkCachedPosition = new EngineLocation();
	//Check if Geolocation is enabled in the settings
#ifndef RHO_ELEMENTS
	//  Legacy behaviour in RE 1
	extern CConfig	*g_pConfig;
	LPCTSTR value = g_pConfig->GetSettingPtr(L"Configuration\\Geolocation\\GeolocationEnabled", L"Value");
	if (value)
		(wcscmp(value, L"1") == 0) ? m_bIsEnabled = TRUE : m_bIsEnabled = FALSE;
	else
		m_bIsEnabled = FALSE;
#else
	m_bIsEnabled = bGeolocationEnabled;
#endif
	DEBUGMSG(TRUE, (TEXT("%s, %d -- END\n"), _T(__FUNCTION__), __LINE__));
}

CGpsWrapper::~CGpsWrapper(void)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	if (m_hGpsHandle)
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- Stopping service...\n"), _T(__FUNCTION__), __LINE__));
		Stop();
	}
	if (m_pGpsIdDll)
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- Unload gpsapi.dll\n"), _T(__FUNCTION__), __LINE__));
		FreeLibrary(m_pGpsIdDll);
	}
	delete s_pEkCachedPosition;
	s_pEkCachedPosition = NULL;

	m_bThreadActive = FALSE;

	g_pGpsWrapperInstance = NULL;
	DEBUGMSG(TRUE, (TEXT("%s, %d -- END\n"), _T(__FUNCTION__), __LINE__));
}

BOOL CGpsWrapper::Init()
{
	/**
	* LOAD GPSID (Intermediate Driver) 
	*/
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	g_pGpsWrapperInstance = this;
	m_pGpsIdDll = LoadLibrary(L"GPSAPI.dll");
	if (!m_pGpsIdDll)
	{
		mObserver.EngineLog(PB_LOG_WARNING, L"gpsapi.dll not available!", 
			_T(__FUNCTION__), __LINE__, L"CGpsWrapper");
		s_lastErrorCode = POSITION_UNAVAILABLE;
		s_lastErrorMessage = L"Device does not support Geolocation";
		return FALSE;
	}
	DEBUGMSG(TRUE, (TEXT("%s, %d -- END\n"), _T(__FUNCTION__), __LINE__));
	return TRUE;
}

BOOL CGpsWrapper::Start(void)
{
	// Stop if already started
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	Stop();

	// Create events
	if (!m_pGpsIdDll)
	{
		mObserver.EngineLog(PB_LOG_ERROR, L"gpsapi.dll not available!", 
			_T(__FUNCTION__), __LINE__, L"CGpsWrapper");
		SetLastError(POSITION_UNAVAILABLE, L"Geolocation is not available on this device");
		return FALSE;
	}
	m_hLocationDataChangedEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if (!m_hDeviceStateChangedEvent)
		m_hDeviceStateChangedEvent  = CreateEvent(NULL,FALSE,FALSE,NULL);

	// Open the device
	GPSOpenDeviceType pfOpenDevice;
	pfOpenDevice = (GPSOpenDeviceType)GetProcAddress(m_pGpsIdDll, L"GPSOpenDevice");
	if (!pfOpenDevice)
	{
		SetLastError(POSITION_UNAVAILABLE, L"Geolocation is not available on this device");
		return FALSE;
	}
	m_hGpsHandle = pfOpenDevice(m_hLocationDataChangedEvent, m_hDeviceStateChangedEvent, NULL, 0);

	// If successfull then create the thread otherwise stop
	if(m_hGpsHandle)
		ListenToGpsEvents();
	else
		Stop();

	// Return success or not
	DEBUGMSG(TRUE, (TEXT("%s, %d -- END\n"), _T(__FUNCTION__), __LINE__));
	return (NULL != m_hGpsHandle);
}

BOOL CGpsWrapper::Stop(void)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));

	//Close the handle to the GPS device
	if (!m_pGpsIdDll)
	{
		mObserver.EngineLog(PB_LOG_ERROR, L"gpsapi.dll not available!", 
			_T(__FUNCTION__), __LINE__, L"CGpsWrapper");
		SetLastError(POSITION_UNAVAILABLE, L"Geolocation is not available on this device");
		return FALSE;
	}
	GPSCloseDeviceType pfGPSCloseDevice;
	pfGPSCloseDevice = (GPSCloseDeviceType)GetProcAddress(m_pGpsIdDll, L"GPSCloseDevice");
	if (pfGPSCloseDevice)
		m_dwLastError = pfGPSCloseDevice(m_hGpsHandle);
	m_hGpsHandle = NULL;

	//Terminates the listening thread
	if (m_hThreadHandle)
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- Terminates listening thread\n"), _T(__FUNCTION__), __LINE__));
		m_bThreadActive = FALSE;
		WaitForSingleObject(m_hThreadHandle, GPSINFO_WAIT_TIMEOUT);
		m_hThreadHandle = NULL;
	}

	//Close the events
	if (m_hLocationDataChangedEvent)
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- Terminates location data event\n"), _T(__FUNCTION__), __LINE__));
		CloseHandle(m_hLocationDataChangedEvent);
		m_hLocationDataChangedEvent = NULL;
	}

	if (m_hDeviceStateChangedEvent)
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- Terminates gps status data event"), _T(__FUNCTION__), __LINE__));
		CloseHandle(m_hDeviceStateChangedEvent);
		m_hDeviceStateChangedEvent = NULL;
	}

	DEBUGMSG(TRUE, (TEXT("%s, %d -- END\n"), _T(__FUNCTION__), __LINE__));
	return (ERROR_SUCCESS == m_dwLastError);
}

DWORD CGpsWrapper::GpsListener(LPVOID lParam)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	CGpsWrapper* pGpsWrapper = (CGpsWrapper*)lParam;
	BOOL bContinue = TRUE;
	DWORD res = S_FALSE;
	HANDLE hEvents[] =	  {
							pGpsWrapper->m_hLocationDataChangedEvent,
							pGpsWrapper->m_hDeviceStateChangedEvent
						  };
	HANDLE gpsHandle = pGpsWrapper->m_hGpsHandle;

	if (!pGpsWrapper->m_bIsEnabled)
	{
		pGpsWrapper->mObserver.EngineLog(PB_LOG_INFO, L"Authorization denied\n", 
			_T(__FUNCTION__), __LINE__, L"CGpsWrapper");
		CGpsWrapper::SetLastError(PERMISSION_DENIED, L"Geolocation is disabled. Please check your settings");
		pGpsWrapper->mObserver.OnGpsLocationError(CGpsWrapper::s_lastErrorCode, 
			CGpsWrapper::s_lastErrorMessage);
		return S_FALSE;
	}

	GPSGetPositionType pfGPSGetPosition;
	pfGPSGetPosition = (GPSGetPositionType)GetProcAddress(pGpsWrapper->m_pGpsIdDll, L"GPSGetPosition");
	if (!pfGPSGetPosition)
	{
		pGpsWrapper->mObserver.EngineLog(PB_LOG_ERROR, L"Initialisation error", 
			_T(__FUNCTION__), __LINE__, L"CGpsWrapper");
		CGpsWrapper::SetLastError(UNKNOWN_ERROR, L"An error occurred during Gps sub-system initialization");
		pGpsWrapper->mObserver.OnGpsLocationError(CGpsWrapper::s_lastErrorCode, 
			CGpsWrapper::s_lastErrorMessage);
		return S_FALSE;
	}
	GPSGetDeviceStateType pfGPSGetDeviceState;
	pfGPSGetDeviceState = (GPSGetDeviceStateType)GetProcAddress(pGpsWrapper->m_pGpsIdDll, L"GPSGetDeviceState");
	if (!pfGPSGetDeviceState)
	{
		pGpsWrapper->mObserver.EngineLog(PB_LOG_ERROR, L"Initialisation error", 
			_T(__FUNCTION__), __LINE__, L"CGpsWrapper");
		CGpsWrapper::SetLastError(UNKNOWN_ERROR, L"An error occurred during Gps sub-system initialization");
		pGpsWrapper->mObserver.OnGpsLocationError(CGpsWrapper::s_lastErrorCode, 
			CGpsWrapper::s_lastErrorMessage);
		return S_FALSE;
	}

	while (pGpsWrapper->m_bThreadActive)
	{
		// Wait for the evetns to occur
		DWORD dwWaitRes	= WaitForMultipleObjects(GPSINFO_EVENT_COUNT,
									hEvents,
									FALSE,
									GPSINFO_WAIT_TIMEOUT) - WAIT_OBJECT_0;
		
		switch (dwWaitRes)
		{
			case WAIT_OBJECT_0:
			{
				GPS_POSITION	sPosition;
				memset(&sPosition, 0, sizeof(GPS_POSITION));

				sPosition.dwVersion	= GPS_VERSION_1;
				sPosition.dwSize	= sizeof(GPS_POSITION);

				if(pfGPSGetPosition(pGpsWrapper->m_hGpsHandle, &sPosition, 1000, 0) == ERROR_SUCCESS)
				{
					if(pGpsWrapper)
					{
						pGpsWrapper->ConvertGpsData(sPosition);
						pGpsWrapper->mObserver.OnGpsPositionReceived(CGpsWrapper::s_pEkCachedPosition);
					}
				}
				break;
			}
			case WAIT_OBJECT_0 + 1:
			{
				GPS_DEVICE		sState;

				memset(&sState, 0, sizeof(GPS_DEVICE));

				sState.dwVersion	= GPS_VERSION_1;
				sState.dwSize		= sizeof(GPS_DEVICE);

				if(pfGPSGetDeviceState(&sState) == ERROR_SUCCESS)
				{
					if(pGpsWrapper)	
						pGpsWrapper->mObserver.OnGpsStateReceived(&sState);
				}

				break;
			}
			case WAIT_TIMEOUT:
				DEBUGMSG(TRUE, (TEXT("%s, %d -- Timeout occurred\n"), _T(__FUNCTION__), __LINE__));
				break;
			// WAIT_TIMEOUT or any other values returned, here we can check for any error. but it is not done now.
			default:
			{
				CGpsWrapper::SetLastError(UNKNOWN_ERROR, L"Unknown error");
				pGpsWrapper->mObserver.OnGpsLocationError(CGpsWrapper::s_lastErrorCode, 
						CGpsWrapper::s_lastErrorMessage);
				break;
			}
		}
	}
	DEBUGMSG(TRUE, (TEXT("%s, %d -- END\n"), _T(__FUNCTION__), __LINE__));
	return S_OK;
}

void CGpsWrapper::ListenToGpsEvents(void)
{
	m_bThreadActive = TRUE;
	m_hThreadHandle = CreateThread(NULL, 0, GpsListener, this, 0, &m_dwThreadId);
}

LRESULT CGpsWrapper::StartLocationUpdates(EngineLocationInterface* locationInterface,
											BOOL highAccuracy)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	if (Start())
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- S_OK returned\n"), _T(__FUNCTION__), __LINE__));
		return S_OK;
	}
	else
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- S_FALSE returned\n"), _T(__FUNCTION__), __LINE__));
		//CGpsWrapper::SetLastError(UNKNOWN_ERROR, L"An error occurred on Gps sub-system initialization");
		mObserver.OnGpsLocationError(CGpsWrapper::s_lastErrorCode, 
			CGpsWrapper::s_lastErrorMessage);
		return S_FALSE;
	}
}

LRESULT CGpsWrapper::StopLocationUpdates(EngineLocationInterface* locationInterface)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	if (Stop())
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- S_OK returned\n"), _T(__FUNCTION__), __LINE__));
		return S_OK;
	}
	else
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- S_FALSE returned\n"), _T(__FUNCTION__), __LINE__));
		CGpsWrapper::SetLastError(UNKNOWN_ERROR, L"An error occurred while stopping Gps sub-system");
		return S_FALSE;
	}
}

LRESULT CGpsWrapper::GetCachedLocation(EngineLocationInterface* locationInterface,
							EngineLocation* location)
{
	location = s_pEkCachedPosition;
	return S_OK;
}

void CGpsWrapper::ConvertGpsData(GPS_POSITION position)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	if (!s_pEkCachedPosition)
	{
		DEBUGMSG(TRUE, (TEXT("%s, %d -- mEkChachedPosition is NULL\n"), _T(__FUNCTION__), __LINE__));
		return;
	}
	if (position.dwValidFields & GPS_VALID_LATITUDE)
		s_pEkCachedPosition->latitude = position.dblLatitude;
	if (position.dwValidFields & GPS_VALID_LONGITUDE)
		s_pEkCachedPosition->longitude = position.dblLongitude;
	if (position.dwValidFields & GPS_VALID_ALTITUDE_WRT_SEA_LEVEL)
	{
		s_pEkCachedPosition->altitude = position.flAltitudeWRTSeaLevel;
		s_pEkCachedPosition->hasAltitude = true;
	}
	if (position.dwValidFields & GPS_VALID_HEADING)
	{
		s_pEkCachedPosition->heading = position.flHeading;
		s_pEkCachedPosition->hasHeading = true;
	}
	if (position.dwValidFields & GPS_VALID_SPEED)
	{
		s_pEkCachedPosition->speed = position.flSpeed * KNOT_TO_MTS;
		s_pEkCachedPosition->hasSpeed = true;
	}
	if (position.dwValidFields & GPS_VALID_POSITION_DILUTION_OF_PRECISION)
		s_pEkCachedPosition->accuracy = position.flPositionDilutionOfPrecision * GPS_HOR_ACCURACY_MT;
	if (position.dwValidFields & GPS_VALID_VERTICAL_DILUTION_OF_PRECISION)
	{
		s_pEkCachedPosition->altitudeAccuracy = position.flVerticalDilutionOfPrecision * GPS_VER_ACCURACY_MT;
		s_pEkCachedPosition->hasAltitudeAccuracy = true;
	}
	s_pEkCachedPosition->timestamp = position.stUTCTime;

	DEBUGMSG(TRUE, (TEXT("%s, %d -- END\n"), _T(__FUNCTION__), __LINE__));
}

void CGpsWrapper::SetLastError(EngineLocationError errorCode, LPCTSTR errorMessage)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	DEBUGMSG(TRUE, (TEXT("errorCode=%d errorMessage=%s"), errorCode, errorMessage));
	
	s_lastErrorCode = errorCode;
	s_lastErrorMessage = errorMessage;

	DEBUGMSG(TRUE, (TEXT("%s, %d -- END\n"), _T(__FUNCTION__), __LINE__));
}

LRESULT startLocationUpdates(EngineLocationInterface* locationInterface,
							 BOOL highAccuracy)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	if (g_pGpsWrapperInstance)
		return g_pGpsWrapperInstance->StartLocationUpdates(locationInterface, highAccuracy);
	return S_FALSE;
}
LRESULT stopLocationUpdates(EngineLocationInterface* locationInterface)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	if (g_pGpsWrapperInstance)
		return g_pGpsWrapperInstance->StopLocationUpdates(locationInterface);
	return S_FALSE;
}

LRESULT getCachedLocation(EngineLocationInterface* locationInterface,
						  EngineLocation* location)
{
	DEBUGMSG(TRUE, (TEXT("%s, %d -- START\n"), _T(__FUNCTION__), __LINE__));
	if (g_pGpsWrapperInstance)
		return g_pGpsWrapperInstance->GetCachedLocation(locationInterface, location);
	return S_FALSE;
}

