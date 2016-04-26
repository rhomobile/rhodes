#include "LightSensor.h"

//Initialize the static variables
WCHAR*		CLightSensor::m_szLightSensorURI = NULL;
PBModule*	CLightSensor::m_pModule = NULL;
int			CLightSensor::m_iInstanceID = 0;
int			CLightSensor::m_iMinimumInterval = 0;
long		CLightSensor::m_dwQuietStart = 0;	
LONG		CLightSensor::pScale = 1;

CRITICAL_SECTION csLightSensor;	

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
CLightSensor::CLightSensor (int iInstanceID, PBModule* pModule)
{
	m_pModule			= pModule;
	m_iInstanceID		= iInstanceID;
	
	m_szLightSensorURI	= NULL;
	m_iMinimumInterval	= 1000;
	m_dwQuietStart		= GetTickCount();

	// Try to load Sensor library
	m_hSensorLibrary = LoadLibrary (L"SensorApi.dll");
	if (!m_hSensorLibrary)
	{		
		m_pModule->Log(PB_LOG_ERROR, L"Error - Unable to Initialise the SensorApi library", _T(__FUNCTION__), __LINE__);
		return;
	}

	// Get function pointers from Sensor API library
	lpfnSENSOR_Open							= (LPFN_SENSOR_OPEN)						GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_Open")); 
	lpfnSENSOR_Close						= (LPFN_SENSOR_CLOSE)						GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_Close")); 
	lpfnSENSOR_FindFirst					= (LPFN_SENSOR_FINDFIRST)					GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_FindFirst")); 
	lpfnSENSOR_FindNext						= (LPFN_SENSOR_FINDNEXT)					GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_FindNext")); 
	lpfnSENSOR_FindClose					= (LPFN_SENSOR_FINDCLOSE)					GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_FindClose")); 
	lpfnSENSOR_RegisterDataNotification		= (LPFN_SENSOR_REGISTERDATANOTIFICATION)	GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_RegisterDataNotification"));
	lpfnSENSOR_DeregisterDataNotification	= (LPFN_SENSOR_DEREGISTERDATANOTIFICATION)	GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_DeregisterDataNotification"));
	lpfnSENSOR_StartSampling				= (LPFN_SENSOR_STARTSAMPLING)				GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_StartSampling"));
	lpfnSENSOR_StopSampling					= (LPFN_SENSOR_STOPSAMPLING)				GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_StopSampling"));
	lpfnSENSOR_GetProperty					= (LPFN_SENSOR_GETPROPERTY)					GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_GetProperty"));
	lpfnSENSOR_SetProperty					= (LPFN_SENSOR_SETPROPERTY)					GetProcAddress(m_hSensorLibrary,TEXT("SENSOR_SetProperty"));	

	if((NULL == lpfnSENSOR_Open) || 
	   (NULL == lpfnSENSOR_Close) ||
	   (NULL == lpfnSENSOR_FindFirst) ||
	   (NULL == lpfnSENSOR_FindNext) ||
	   (NULL == lpfnSENSOR_FindClose) ||
	   (NULL == lpfnSENSOR_RegisterDataNotification) ||
	   (NULL == lpfnSENSOR_DeregisterDataNotification) ||
	   (NULL == lpfnSENSOR_StartSampling) ||
	   (NULL == lpfnSENSOR_StopSampling)||
	   (NULL == lpfnSENSOR_GetProperty)||
	   (NULL == lpfnSENSOR_SetProperty))
	{
		FreeLibrary(m_hSensorLibrary);
		m_hSensorLibrary = NULL;
		return;
	}

	
	hSensor = CreateEvent (NULL, false, false, NULL);	

	if (hSensor == NULL)
	{
		//Log error & exit
		m_pModule->Log(PB_LOG_ERROR, L"Error in creating Sensor Handle", _T(__FUNCTION__), __LINE__);	
		return;
	}
	

	// Protect access to latest data
	InitializeCriticalSection (&csLightSensor);

}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
CLightSensor::~CLightSensor ()
{
	if (m_hSensorLibrary != NULL)
	{
		//In case the user forgot to close the Sensor	
		UnregisterSensor();	
	}

	DeleteCriticalSection (&csLightSensor);
			
	CloseHandle (hSensor);		

	if(m_hSensorLibrary != NULL)
	{
		lpfnSENSOR_Open							= NULL;
		lpfnSENSOR_Close						= NULL;
		lpfnSENSOR_FindFirst					= NULL;
		lpfnSENSOR_FindNext						= NULL;
		lpfnSENSOR_FindClose					= NULL;
		lpfnSENSOR_RegisterDataNotification		= NULL;
		lpfnSENSOR_DeregisterDataNotification	= NULL;
		lpfnSENSOR_StartSampling				= NULL;
		lpfnSENSOR_StopSampling					= NULL;
		lpfnSENSOR_GetProperty					= NULL;
		lpfnSENSOR_SetProperty					= NULL;

		FreeLibrary(m_hSensorLibrary);
		m_hSensorLibrary = NULL;
	}
}

DWORD CLightSensor::RegisterSensor()
{
	DWORD dwResult = S_OK;

	if (m_hSensorLibrary != NULL)
	{
		dwResult = StartSensor(SENSOR_TYPE_AMBIENT_LIGHT, &hSensor, 
			&CLightSensor::OnReceiveAmbientLightSensorData, &pScale);
	}
			
	return dwResult;
}

DWORD CLightSensor::UnregisterSensor()
{
	if (m_hSensorLibrary != NULL)
	{
		// Stop sampling
		lpfnSENSOR_StopSampling(hSensor);

		// Deregister for sensor data callback
		lpfnSENSOR_DeregisterDataNotification(hSensor);

		// Close sensor and return the result
		return lpfnSENSOR_Close(hSensor);
	}
	return S_OK;
}

BOOL CLightSensor::IsSensorAvailable(SENSOR_TYPE sensorType)
{
	DWORD dwResult;
	HANDLE hFindHandle = NULL;
	SENSOR_INFO_T tSensorInfo;

	memset(&tSensorInfo, 0, sizeof(SENSOR_INFO_T));
	SI_INIT(&tSensorInfo); // Sets the dwAllocated field of the structure pointed to &tSensorInfo to the size of the structure. 
	tSensorInfo.eType= sensorType; // Modify the field (eType)
	SI_SET_USED(&tSensorInfo, eType) //Sets the dwUsed field of the structure pointed to &tSensorInfo to the extent of field (eType).

	dwResult = lpfnSENSOR_FindFirst(&tSensorInfo, &hFindHandle);

	if(E_SENSOR_SUCCESS != dwResult)
	{
		return FALSE;

	} 

	if (NULL != hFindHandle)
	{
		lpfnSENSOR_FindClose(hFindHandle);
	}

	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CLightSensor::SetLightSensorURI(LPCWSTR tcNavURI)
{
	//  store the passed navigation URI
	if (!tcNavURI)
		return FALSE;
	if (m_szLightSensorURI)
		delete[] m_szLightSensorURI;

	m_szLightSensorURI = new WCHAR[(wcslen(tcNavURI) * sizeof(WCHAR)) + 1];
	wcscpy(m_szLightSensorURI, tcNavURI);
	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CLightSensor::StartSensor(SENSOR_TYPE sensorType, HANDLE* phSensor, PFN_SENSOR_DATA_CALLBACK pfnSensorDataCallback, LONG* pscaleFactor)
{
	DWORD dwResult;
	SENSOR_INFO_T tSensorInfo;

	memset(&tSensorInfo, 0,sizeof(SENSOR_INFO_T));
	SI_INIT(&tSensorInfo);// sets the dwallocated field of the structure pointed to &tsensorinfo to the size of the structure. 
	tSensorInfo.eType= sensorType; // modify the field (etype)
	SI_SET_USED(&tSensorInfo, eType) //Sets the dwUsed field of the structure pointed to &tSensorInfo to the extent of field (eType).

	// Open the sensor in default mode. 
	dwResult = lpfnSENSOR_Open(SENSOR_OPEN_MODE_DEFAULT,&tSensorInfo, phSensor);

	if(E_SENSOR_SUCCESS != dwResult)
	{
		return (dwResult);
	}

	// Getting the scale factor
	dwResult = lpfnSENSOR_GetProperty(*phSensor, SENSOR_PROP_SCALE, pscaleFactor);
	
	if(E_SENSOR_SUCCESS != dwResult)
	{
		lpfnSENSOR_Close(*phSensor);

		return (dwResult);
	}

	if (SAMPLING_TYPE_ON_RATE == tSensorInfo.eSamplingType) // Sampling type - on rate
	{
		// Setting the Sample rate to 1Hz
		LONG nSampleRate = SAMPLE_RATE_1_HZ; 

		dwResult = lpfnSENSOR_SetProperty(*phSensor, SENSOR_PROP_SAMPLE_RATE, nSampleRate);
		
		if(E_SENSOR_SUCCESS != dwResult)
		{
			// In case of a failure, continue with the default sample rate without returning 
		}
	}

	// Register for sensor data callback. Notify threshold should be 1 if default buffer size is used
	dwResult = lpfnSENSOR_RegisterDataNotification(*phSensor, NOTIFIER_TYPE_CALLBACK, pfnSensorDataCallback, 1);

	if(E_SENSOR_SUCCESS != dwResult)
	{
		lpfnSENSOR_Close(*phSensor);

		return (dwResult);
	}

	// Start sampling
	dwResult = lpfnSENSOR_StartSampling(*phSensor);
	
	if(E_SENSOR_SUCCESS != dwResult)
	{
		lpfnSENSOR_Close(*phSensor);

		return (dwResult);
	}

	return (dwResult);
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
void CLightSensor::SendEventData()
{
	long elapsed = GetTickCount() - m_dwQuietStart;
	if (elapsed >= m_iMinimumInterval && m_szLightSensorURI != NULL  && wcscmp(m_szLightSensorURI, L"")!=0)
	{
		WCHAR pValues[16];
		wsprintf(pValues, L"%u", lSensorValue);		
		
		m_pModule->SendPBNavigate(tcLightSensorEventNames, m_iInstanceID, m_szLightSensorURI, pValues, NULL);
		m_dwQuietStart = GetTickCount();
	}
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CLightSensor::OnReceiveAmbientLightSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLightSensor);

		// Convert values to signed values and record
		if (pScale)
		{
			lSensorValue = ptSensorData->data.scalar.nValue/pScale;
		}
		else
		{
			lSensorValue = ptSensorData->data.scalar.nValue;
		}

		if (lSensorValue)
		{
			//do not send undefined values
			SendEventData();	
		}

		LeaveCriticalSection (&csLightSensor);	
	}	
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
bool CLightSensor::IsSensorAPIAvailable()
{
	if (m_hSensorLibrary != NULL)
		return true;
	else
		return false;
}