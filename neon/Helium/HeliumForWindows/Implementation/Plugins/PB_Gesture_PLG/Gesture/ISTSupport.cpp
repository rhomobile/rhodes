/******************************************************************************/
#include <windows.h>
#include "ISTSupport.h"

// Critical section to modify the sensor data
CRITICAL_SECTION csLatestData;	

// Initilaize all the static variable sof the sensor class

HANDLE CSENSORSUPPORT::hCancel = NULL;
HANDLE CSENSORSUPPORT::hDataShakeReady = NULL;
HANDLE CSENSORSUPPORT::hDataTiltReady = NULL;
long CSENSORSUPPORT::pScaleShake = 1;
long CSENSORSUPPORT::pScaleTilt = 1;
SENSORDATA CSENSORSUPPORT::sLatestShakeData = {1};
SENSORDATA CSENSORSUPPORT::sLatestTiltData = {1};

/******************************************************************************/
/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
CSENSORSUPPORT::CSENSORSUPPORT ()
{
	// Try to load Sensor library
	m_hSensorLibrary = LoadLibrary (L"SensorApi.dll");
	if (!m_hSensorLibrary)
	{		
		//m_pModule->Log(PB_LOG_ERROR, L"Error - Unable to Initialise the SensorApi library", _T(__FUNCTION__), __LINE__);
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
	}

	//Sensor handles
	hSensorShake = CreateEvent (NULL, false, false, NULL);
	hSensorTilt = CreateEvent (NULL, false, false, NULL);		

	// Event to cancel IST thread
	hCancel = CreateEvent (NULL, true, false, NULL);

	// Event to signal to users of this class that data is ready
	hDataShakeReady = CreateEvent (NULL, true, false, NULL);

	// Event to signal to users of this class that data is ready
	hDataTiltReady = CreateEvent (NULL, true, false, NULL);

	// Protect access to latest data
	InitializeCriticalSection (&csLatestData);

}

/******************************************************************************/
/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
CSENSORSUPPORT::~CSENSORSUPPORT ()
{
	if (!Supported ())
		return;

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

	//In case the user forgot to close the Sensors
	UnregisterSensor(SENSOR_TYPE_ACCELEROMETER);	
	UnregisterSensor(SENSOR_TYPE_TILT_ANGLE);	

	CloseHandle (hSensorShake);
	CloseHandle (hSensorTilt);	

	CloseHandle (hDataShakeReady);
	CloseHandle (hDataTiltReady);

	SetEvent (hCancel);
	CloseHandle (hCancel);

	DeleteCriticalSection (&csLatestData);

	hDataShakeReady = NULL;
	hDataTiltReady = NULL;
	hCancel = NULL;	
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CSENSORSUPPORT::RegisterSensor(_SENSOR_TYPE eSensorType)
{
	DWORD dwResult = S_OK;
	
	switch (eSensorType)
	{
		case SENSOR_TYPE_ACCELEROMETER:
			dwResult = StartSensor(SENSOR_TYPE_ACCELEROMETER, &hSensorShake, 
				&CSENSORSUPPORT::OnReceiveAccelerometerSensorData, &pScaleShake);

			// NOTE: Divide by 1000 for detecting the shake gestures in our algorithm
			if (pScaleShake == 1000000)
				pScaleShake /= 1000;

			break;
		case SENSOR_TYPE_TILT_ANGLE:
			dwResult = StartSensor(SENSOR_TYPE_TILT_ANGLE, &hSensorTilt, 
				&CSENSORSUPPORT::OnReceiveTiltAngleSensorData, &pScaleTilt);
			break;
		default:
			break;
	}
			
	return dwResult;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CSENSORSUPPORT::UnregisterSensor(_SENSOR_TYPE eSensorType)
{
	DWORD dwResult = S_OK;

	switch (eSensorType)
	{
		case SENSOR_TYPE_ACCELEROMETER:
			// Stop sampling
			lpfnSENSOR_StopSampling(hSensorShake);

			// Deregister for sensor data callback
			lpfnSENSOR_DeregisterDataNotification(hSensorShake);

			// Close sensor and return the result
			dwResult = lpfnSENSOR_Close(hSensorShake);
			break;

		case SENSOR_TYPE_TILT_ANGLE:
			// Stop sampling
			lpfnSENSOR_StopSampling(hSensorTilt);

			// Deregister for sensor data callback
			lpfnSENSOR_DeregisterDataNotification(hSensorTilt);

			// Close sensor and return the result
			dwResult = lpfnSENSOR_Close(hSensorTilt);
			break;
	}
	return dwResult;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CSENSORSUPPORT::StartSensor(SENSOR_TYPE sensorType, HANDLE* phSensor, PFN_SENSOR_DATA_CALLBACK pfnSensorDataCallback, LONG* pscaleFactor)
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
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CSENSORSUPPORT::OnReceiveAccelerometerSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{
	SENSORDATA latest;

	if (DATA_TYPE_VECTOR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScaleShake)
		{
			latest.lXValue = ptSensorData->data.vector.nX/pScaleShake;		
			latest.lYValue = ptSensorData->data.vector.nY/pScaleShake;		
			latest.lZValue = ptSensorData->data.vector.nZ/pScaleShake;
		}
		else
		{
			latest.lXValue = ptSensorData->data.vector.nX;		
			latest.lYValue = ptSensorData->data.vector.nY;		
			latest.lZValue = ptSensorData->data.vector.nZ;
		}
		
		SetLatestShakeData (&latest);

		// Signal any waiting consumers
		PulseEvent (hDataShakeReady);

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
DWORD CSENSORSUPPORT::OnReceiveTiltAngleSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop) 
{
	SENSORDATA latest;

	if (DATA_TYPE_VECTOR == ptSensorData->eType)
	{
		EnterCriticalSection (&csLatestData);

		// Convert values to signed values and record
		if (pScaleTilt)
		{
			latest.lXValue = ptSensorData->data.vector.nX/pScaleTilt;		
			latest.lYValue = ptSensorData->data.vector.nY/pScaleTilt;		
			latest.lZValue = ptSensorData->data.vector.nZ/pScaleTilt;
		}
		else
		{
			latest.lXValue = ptSensorData->data.vector.nX;		
			latest.lYValue = ptSensorData->data.vector.nY;		
			latest.lZValue = ptSensorData->data.vector.nZ;
		}
		
		SetLatestTiltData (&latest);

		// Signal any waiting consumers
		PulseEvent (hDataTiltReady);

		LeaveCriticalSection (&csLatestData);	
	}	
	return 0;
}


/******************************************************************************/
/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
void CSENSORSUPPORT::SetLatestShakeData (SENSORDATA *pdata)
{
	EnterCriticalSection (&csLatestData);
	memcpy (&sLatestShakeData, pdata, sizeof SENSORDATA);
	LeaveCriticalSection (&csLatestData);
}

/******************************************************************************/
/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
void CSENSORSUPPORT::GetLatestShakeData (SENSORDATA *pdata)
{
	EnterCriticalSection (&csLatestData);
	memcpy (pdata, &sLatestShakeData, sizeof SENSORDATA);
	LeaveCriticalSection (&csLatestData);
}

/******************************************************************************/
/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
void CSENSORSUPPORT::SetLatestTiltData (SENSORDATA *pdata)
{
	EnterCriticalSection (&csLatestData);
	memcpy (&sLatestTiltData, pdata, sizeof SENSORDATA);
	LeaveCriticalSection (&csLatestData);
}

/******************************************************************************/
/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
void CSENSORSUPPORT::GetLatestTiltData (SENSORDATA *pdata)
{
	EnterCriticalSection (&csLatestData);
	memcpy (pdata, &sLatestTiltData, sizeof SENSORDATA);
	LeaveCriticalSection (&csLatestData);
}

/******************************************************************************/
/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		October 2012 Initial Creation, HP
*/
/******************************************************************************/
DWORD CSENSORSUPPORT::WaitForSensorData ()
{
	HANDLE wait [2];

	wait [0] = hDataShakeReady;
	wait [1] = hDataTiltReady;
	wait [2] = hCancel;

	DWORD dwResult = WaitForMultipleObjects (3, wait, false, INFINITE);

	if (dwResult == WAIT_OBJECT_0)
	{
		return SENSOR_TYPE_ACCELEROMETER;
	}
	else if (dwResult == WAIT_OBJECT_0 + 1)
	{
		return SENSOR_TYPE_TILT_ANGLE;
	}
	else if (dwResult == WAIT_OBJECT_0 + 2)
	{
		return SENSOR_TYPE_UNDEFINED;
	}

	return SENSOR_TYPE_UNDEFINED;
}

