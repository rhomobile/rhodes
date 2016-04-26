#include "Sensors.h"

//Initialize the static variables
WCHAR*		CRawSensor::m_szRawSensorsURI = NULL;
PBModule*	CRawSensor::m_pModule = NULL;
int			CRawSensor::m_iInstanceID = 0;
int			CRawSensor::m_iMinimumInterval = 0;
long		CRawSensor::m_dwQuietStart = 0;	
LONG		CRawSensor::pScale[SENSOR_COUNT] = {1};

CRITICAL_SECTION csLatestData;	

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
CRawSensor::CRawSensor (int iInstanceID, PBModule* pModule)
{
	m_pModule			= pModule;
	m_iInstanceID		= iInstanceID;
	
	m_szRawSensorsURI	= NULL;
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

	//Create the array of events
	for (int i=0;i<SENSOR_COUNT;i++)
	{
		hSensor[i] = CreateEvent (NULL, false, false, NULL);	

		if (hSensor[i] == NULL)
		{
			//Log error & exit
			m_pModule->Log(PB_LOG_ERROR, L"Error in creating Sensor Handles", _T(__FUNCTION__), __LINE__);	
			return;
		}
	}

	// Protect access to latest data
	InitializeCriticalSection (&csLatestData);

}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
CRawSensor::~CRawSensor ()
{
	//In case the user forgot to close the Sensors
	if (m_hSensorLibrary != NULL)
	{
		UnregisterSensor(SENSOR_TYPE_ACCELEROMETER);
		UnregisterSensor(SENSOR_TYPE_ORIENTATION);
		UnregisterSensor(SENSOR_TYPE_TILT_ANGLE);
		UnregisterSensor(SENSOR_TYPE_MOTION);
		UnregisterSensor(SENSOR_TYPE_ECOMPASS);
		UnregisterSensor(SENSOR_TYPE_MAGNETOMETER);
		UnregisterSensor(SENSOR_TYPE_GYROSCOPE);
		UnregisterSensor(SENSOR_TYPE_AMBIENT_LIGHT);
		UnregisterSensor(SENSOR_TYPE_PROXIMITY);
		UnregisterSensor(SENSOR_TYPE_PROXIMITY_LONG_RANGE);
		UnregisterSensor(SENSOR_TYPE_PRESSURE);
		UnregisterSensor(SENSOR_TYPE_TEMPERATURE);
		UnregisterSensor(SENSOR_TYPE_HUMIDITY);
	}

	DeleteCriticalSection (&csLatestData);

	for (int i=0;i<SENSOR_COUNT;i++)
	{		
		CloseHandle (hSensor[i]);
	}	

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

DWORD CRawSensor::RegisterSensor(_SENSOR_TYPE eSensorType)
{
	DWORD dwResult = S_OK;

	if (m_hSensorLibrary != NULL)
	{
		switch(eSensorType)
		{
			case SENSOR_TYPE_ACCELEROMETER:
				dwResult = StartSensor(SENSOR_TYPE_ACCELEROMETER, &hSensor[SENSOR_TYPE_ACCELEROMETER], 
					&CRawSensor::OnReceiveAccelerometerSensorData, &pScale[SENSOR_TYPE_ACCELEROMETER]);
				break;
			case SENSOR_TYPE_ORIENTATION:
				dwResult = StartSensor(SENSOR_TYPE_ORIENTATION, &hSensor[SENSOR_TYPE_ORIENTATION], 
					&CRawSensor::OnReceiveOrientationSensorData, &pScale[SENSOR_TYPE_ORIENTATION]);
				break;
			case SENSOR_TYPE_TILT_ANGLE:
				dwResult = StartSensor(SENSOR_TYPE_TILT_ANGLE, &hSensor[SENSOR_TYPE_TILT_ANGLE], 
					&CRawSensor::OnReceiveTiltAngleSensorData, &pScale[SENSOR_TYPE_TILT_ANGLE]);
				break;
			case SENSOR_TYPE_MOTION:
				dwResult = StartSensor(SENSOR_TYPE_MOTION, &hSensor[SENSOR_TYPE_MOTION], 
					&CRawSensor::OnReceiveMotionSensorData, &pScale[SENSOR_TYPE_MOTION]);
				break;
			case SENSOR_TYPE_ECOMPASS:
				dwResult = StartSensor(SENSOR_TYPE_ECOMPASS, &hSensor[SENSOR_TYPE_ECOMPASS], 
					&CRawSensor::OnReceiveECompassSensorData, &pScale[SENSOR_TYPE_ECOMPASS]);
				break;
			case SENSOR_TYPE_MAGNETOMETER:
				dwResult = StartSensor(SENSOR_TYPE_MAGNETOMETER, &hSensor[SENSOR_TYPE_MAGNETOMETER], 
					&CRawSensor::OnReceiveMagnetometerSensorData, &pScale[SENSOR_TYPE_MAGNETOMETER]);
				break;
			case SENSOR_TYPE_GYROSCOPE:
				dwResult = StartSensor(SENSOR_TYPE_GYROSCOPE, &hSensor[SENSOR_TYPE_GYROSCOPE], 
					&CRawSensor::OnReceiveGyroscopeSensorData, &pScale[SENSOR_TYPE_GYROSCOPE]);
				break;
			case SENSOR_TYPE_AMBIENT_LIGHT:
				dwResult = StartSensor(SENSOR_TYPE_AMBIENT_LIGHT, &hSensor[SENSOR_TYPE_AMBIENT_LIGHT], 
					&CRawSensor::OnReceiveAmbientLightSensorData, &pScale[SENSOR_TYPE_AMBIENT_LIGHT]);
				break;
			case SENSOR_TYPE_PROXIMITY:
				dwResult = StartSensor(SENSOR_TYPE_PROXIMITY, &hSensor[SENSOR_TYPE_PROXIMITY], 
					&CRawSensor::OnReceiveProximitySensorData, &pScale[SENSOR_TYPE_PROXIMITY]);
				break;
			case SENSOR_TYPE_PROXIMITY_LONG_RANGE:
				dwResult = StartSensor(SENSOR_TYPE_PROXIMITY_LONG_RANGE, &hSensor[SENSOR_TYPE_PROXIMITY_LONG_RANGE], 
					&CRawSensor::OnReceiveProximityLongRangeSensorData, &pScale[SENSOR_TYPE_PROXIMITY_LONG_RANGE]);
				break;
			case SENSOR_TYPE_PRESSURE:
				dwResult = StartSensor(SENSOR_TYPE_PRESSURE, &hSensor[SENSOR_TYPE_PRESSURE], 
					&CRawSensor::OnReceivePressureSensorData, &pScale[SENSOR_TYPE_PRESSURE]);
				break;
			case SENSOR_TYPE_TEMPERATURE:
				dwResult = StartSensor(SENSOR_TYPE_TEMPERATURE, &hSensor[SENSOR_TYPE_TEMPERATURE], 
					&CRawSensor::OnReceiveTemperatureSensorData, &pScale[SENSOR_TYPE_TEMPERATURE]);
				break;
			case SENSOR_TYPE_HUMIDITY:
				dwResult = StartSensor(SENSOR_TYPE_HUMIDITY, &hSensor[SENSOR_TYPE_HUMIDITY], 
					&CRawSensor::OnReceiveHumiditySensorData, &pScale[SENSOR_TYPE_HUMIDITY]);
				break;
			default:
				break;
		}
	}
	return dwResult;
}

DWORD CRawSensor::UnregisterSensor(_SENSOR_TYPE eSensorType)
{
	if (m_hSensorLibrary != NULL)
	{
		// Stop sampling
		lpfnSENSOR_StopSampling(hSensor[eSensorType]);

		// Deregister for sensor data callback
		lpfnSENSOR_DeregisterDataNotification(hSensor[eSensorType]);

		// Close sensor and return the result
		return lpfnSENSOR_Close(hSensor[eSensorType]);
	}
	return S_OK;
}

BOOL CRawSensor::IsSensorAvailable(SENSOR_TYPE sensorType)
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
BOOL CRawSensor::SetRawSensorsURI(LPCWSTR tcNavURI)
{
	//  store the passed navigation URI
	if (!tcNavURI)
		return FALSE;
	if (m_szRawSensorsURI)
		delete[] m_szRawSensorsURI;

	m_szRawSensorsURI = new WCHAR[(wcslen(tcNavURI) * sizeof(WCHAR)) + 1];
	wcscpy(m_szRawSensorsURI, tcNavURI);
	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::StartSensor(SENSOR_TYPE sensorType, HANDLE* phSensor, PFN_SENSOR_DATA_CALLBACK pfnSensorDataCallback, LONG* pscaleFactor)
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
void CRawSensor::SendEventData()
{
	long elapsed = GetTickCount() - m_dwQuietStart;
	if (elapsed >= m_iMinimumInterval && m_szRawSensorsURI != NULL && wcscmp(m_szRawSensorsURI, L"")!=0)
	{
		LPCWSTR *pValues = new LPCWSTR [SENSOR_VALUES_COUNT];
		for (int i=0;i<SENSOR_VALUES_COUNT;i++)
		{
			pValues[i] = wcEventValues[i];
		}	
		
		m_pModule->SendPBNavigate(tcRawSensorEventNames, m_iInstanceID, m_szRawSensorsURI, SENSOR_VALUES_COUNT, pValues);		
		
		delete[] pValues;
		m_dwQuietStart = GetTickCount();
	}
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveAccelerometerSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_VECTOR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_ACCELEROMETER])
		{
			wsprintf(wcEventValues[0], L"%4.4f", (float)ptSensorData->data.vector.nX/pScale[SENSOR_TYPE_ACCELEROMETER]);		
			wsprintf(wcEventValues[1], L"%4.4f", (float)ptSensorData->data.vector.nY/pScale[SENSOR_TYPE_ACCELEROMETER]);		
			wsprintf(wcEventValues[2], L"%4.4f", (float)ptSensorData->data.vector.nZ/pScale[SENSOR_TYPE_ACCELEROMETER]);
		}
		else
		{
			wsprintf(wcEventValues[0], L"%4.4f", (float)ptSensorData->data.vector.nX);		
			wsprintf(wcEventValues[1], L"%4.4f", (float)ptSensorData->data.vector.nY);		
			wsprintf(wcEventValues[2], L"%4.4f", (float)ptSensorData->data.vector.nZ);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}
	return 0;
}


/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveOrientationSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{ 
	if(DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		LONG orientation;
		if (pScale[SENSOR_TYPE_ORIENTATION])
		{
			orientation = ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_ORIENTATION];
		}
		else
		{
			orientation = ptSensorData->data.scalar.nValue;
		}

		switch(orientation)
		{
			case DEV_ORIENTATION_PORTRAIT_DOWN:
				wsprintf(wcEventValues[3], L"Portrait Down");				
				break;
			case DEV_ORIENTATION_PORTRAIT_UP:
				wsprintf(wcEventValues[3], L"Portrait Up");					
				break;
			case DEV_ORIENTATION_LANDSCAPE_LEFT:
				wsprintf(wcEventValues[3], L"Landscape Left");
				break;
			case DEV_ORIENTATION_LANDSCAPE_RIGHT:
				wsprintf(wcEventValues[3], L"Landscape Right");
				break;
			case DEV_ORIENTATION_FACE_UP:
				wsprintf(wcEventValues[3], L"face Up");	
				break;
			case DEV_ORIENTATION_FACE_DOWN:
				wsprintf(wcEventValues[3], L"face Down");
				break;
			case DEV_ORIENTATION_UNKNOWN:
			default:
				wsprintf(wcEventValues[3], L"Unknown Orientation");	
				break;
		}
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveTiltAngleSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_VECTOR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_TILT_ANGLE])
		{
			wsprintf(wcEventValues[4], L"%3.2f", (float)ptSensorData->data.vector.nX/pScale[SENSOR_TYPE_TILT_ANGLE]);		
			wsprintf(wcEventValues[5], L"%3.2f", (float)ptSensorData->data.vector.nY/pScale[SENSOR_TYPE_TILT_ANGLE]);		
			wsprintf(wcEventValues[6], L"%3.2f", (float)ptSensorData->data.vector.nZ/pScale[SENSOR_TYPE_TILT_ANGLE]);
		}
		else
		{
			wsprintf(wcEventValues[4], L"%3.2f", (float)ptSensorData->data.vector.nX);		
			wsprintf(wcEventValues[5], L"%3.2f", (float)ptSensorData->data.vector.nY);		
			wsprintf(wcEventValues[6], L"%3.2f", (float)ptSensorData->data.vector.nZ);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}	
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveMotionSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_MOTION])
		{
			wsprintf(wcEventValues[7], L"%4.4f", (float)ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_MOTION]);
		}
		else
		{
			wsprintf(wcEventValues[7], L"%4.4f", (float)ptSensorData->data.scalar.nValue);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveECompassSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_ECOMPASS])
		{
			wsprintf(wcEventValues[8], L"%4.4f", (float)ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_ECOMPASS]);
		}
		else
		{
			wsprintf(wcEventValues[8], L"%4.4f", (float)ptSensorData->data.scalar.nValue);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}	
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveMagnetometerSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_VECTOR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_MAGNETOMETER])
		{
			wsprintf(wcEventValues[9], L"%4.4f", (float)ptSensorData->data.vector.nX/pScale[SENSOR_TYPE_MAGNETOMETER]);		
			wsprintf(wcEventValues[10], L"%4.4f", (float)ptSensorData->data.vector.nY/pScale[SENSOR_TYPE_MAGNETOMETER]);		
			wsprintf(wcEventValues[11], L"%4.4f", (float)ptSensorData->data.vector.nZ/pScale[SENSOR_TYPE_MAGNETOMETER]);
		}
		else
		{
			wsprintf(wcEventValues[9], L"%4.4f", (float)ptSensorData->data.vector.nX);		
			wsprintf(wcEventValues[10], L"%4.4f", (float)ptSensorData->data.vector.nY);		
			wsprintf(wcEventValues[11], L"%4.4f", (float)ptSensorData->data.vector.nZ);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}	
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveGyroscopeSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_VECTOR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_GYROSCOPE])
		{
			wsprintf(wcEventValues[12], L"%4.4f", (float)ptSensorData->data.vector.nX/pScale[SENSOR_TYPE_GYROSCOPE]);		
			wsprintf(wcEventValues[13], L"%4.4f", (float)ptSensorData->data.vector.nY/pScale[SENSOR_TYPE_GYROSCOPE]);		
			wsprintf(wcEventValues[14], L"%4.4f", (float)ptSensorData->data.vector.nZ/pScale[SENSOR_TYPE_GYROSCOPE]);
		}
		else
		{
			wsprintf(wcEventValues[12], L"%4.4f", (float)ptSensorData->data.vector.nX);		
			wsprintf(wcEventValues[13], L"%4.4f", (float)ptSensorData->data.vector.nY);		
			wsprintf(wcEventValues[14], L"%4.4f", (float)ptSensorData->data.vector.nZ);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}	
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveAmbientLightSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_AMBIENT_LIGHT])
		{
			wsprintf(wcEventValues[15], L"%4.4f", (float)ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_AMBIENT_LIGHT]);
		}
		else
		{
			wsprintf(wcEventValues[15], L"%4.4f", (float)ptSensorData->data.scalar.nValue);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}	
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveProximitySensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_PROXIMITY])
		{
			wsprintf(wcEventValues[16], L"%4.4f", (float)ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_PROXIMITY]);
		}
		else
		{
			wsprintf(wcEventValues[16], L"%4.4f", (float)ptSensorData->data.scalar.nValue);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveProximityLongRangeSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_PROXIMITY_LONG_RANGE])
		{
			wsprintf(wcEventValues[17], L"%4.4f", (float)ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_PROXIMITY_LONG_RANGE]);
		}
		else
		{
			wsprintf(wcEventValues[17], L"%4.4f", (float)ptSensorData->data.scalar.nValue);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}	
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceivePressureSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_PRESSURE])
		{
			wsprintf(wcEventValues[18], L"%4.4f", (float)ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_PRESSURE]);
		}
		else
		{
			wsprintf(wcEventValues[18], L"%4.4f", (float)ptSensorData->data.scalar.nValue);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}
	return 0;
}


/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveTemperatureSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_TEMPERATURE])
		{
			wsprintf(wcEventValues[19], L"%4.4f", (float)ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_TEMPERATURE]);
		}
		else
		{
			wsprintf(wcEventValues[19], L"%4.4f", (float)ptSensorData->data.scalar.nValue);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CRawSensor::OnReceiveHumiditySensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{	
	if (DATA_TYPE_SCALAR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScale[SENSOR_TYPE_HUMIDITY])
		{
			wsprintf(wcEventValues[20], L"%4.4f", (float)ptSensorData->data.scalar.nValue/pScale[SENSOR_TYPE_HUMIDITY]);
		}
		else
		{
			wsprintf(wcEventValues[20], L"%4.4f", (float)ptSensorData->data.scalar.nValue);
		}
		
		SendEventData();	

		LeaveCriticalSection (&csLatestData);	
	}
	return 0;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
bool CRawSensor::IsSensorAPIAvailable()
{
	if (m_hSensorLibrary != NULL)
		return true;
	else
		return false;
}