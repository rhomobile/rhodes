/**
* \file Raw Sensor header file
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include "SensorAPI.h"
#include "logging/RhoLog.h"
#include "../shared/api_generator/MethodResult.h"

/******************************************************************************/
#define SENSOR_COUNT			SENSOR_TYPE_HUMIDITY+1 //+1 since the first sensor starts from Index 1

// Call back function pointer type definitions
typedef DWORD (WINAPI *LPFN_SENSOR_OPEN)(OPEN_MODE eMode, SENSOR_INFO_T * ptSensorInfo, HANDLE * phSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_CLOSE)(HANDLE hSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_FINDFIRST)(SENSOR_INFO_T * ptSensorInfo, HANDLE * phFindHandle); 
typedef DWORD (WINAPI *LPFN_SENSOR_FINDNEXT)(HANDLE hFindHandle, SENSOR_INFO_T * ptSensorInfo); 
typedef DWORD (WINAPI *LPFN_SENSOR_FINDCLOSE)(HANDLE hFindHandle);
typedef DWORD (WINAPI *LPFN_SENSOR_REGISTERDATANOTIFICATION)(HANDLE hSensor, NOTIFIER_TYPE eNotifierType, LPVOID lpNotifier, DWORD dwNotifyThreshold);
typedef DWORD (WINAPI *LPFN_SENSOR_DEREGISTERDATANOTIFICATION)(HANDLE hSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_STARTSAMPLING)(HANDLE hSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_STOPSAMPLING)(HANDLE hSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_GETPROPERTY)(HANDLE hSensor, SENSOR_PROPERTY eProperty, LONG * pnValue);
typedef DWORD (WINAPI *LPFN_SENSOR_SETPROPERTY)(HANDLE hSensor, SENSOR_PROPERTY eProperty, LONG nValue);

/**
* CRawSensor - The class that contains the Sensor related information provided by Sensor API
*/
class CRawSensor 
{
public:
	/**
	*  Initialise Variables		
	*  \param iInstanceID	Unique identifier of the RhoElements instance in which 
	*						this Sensor module is running.
	*  \param pModule		Pointer to the RhoElements class
	*						which created this sensor.  Used for logging and 
	*						sending navigations back to the core.
	*/
	CRawSensor				();

	/**
	*  Used to free memory and  values
	*/
	~CRawSensor	();

	/**
	*  Sets the URI to be used in the Raw Sensors Event
	*/
	BOOL SetRawSensorsURI	(LPCWSTR tcNavURI);

	/**
	*  Opens a particular Sensor and registers for data events
	*/
	DWORD RegisterSensor (_SENSOR_TYPE eSensorType);

	/**
	*  Closes a particular Sensor and deregisters for data events
	*/
	DWORD UnregisterSensor	(_SENSOR_TYPE eSensorType);

	/**
	* Function to send the data through SendPbNavigate() API
	*/
	static void SendEventData();

	/**
	* Accesor function for minimumInterval attribute 
	*/
	void SetInterval(int interval) { m_iMinimumInterval = interval;}

	/**
	* To know if the device supports Sensor APIs
	*/
	bool IsSensorAPIAvailable();

private:

	//rho::apiGenerator::CMethodResult*	m_pSwipeEvent;			///<  SwipeEvent
	static WCHAR*		m_szRawSensorsURI;	///<  Registered URI for Raw Sensor event (blob data)	

	static int			m_iMinimumInterval;	///<  Minimum Interval specified by the user
	static long			m_dwQuietStart;		///<  System tick count when the minimum interval was started	
	static LONG			pScale[SENSOR_COUNT];

	HMODULE			m_hSensorLibrary;	///<  Handle to the loaded Sensor library
	HANDLE hSensor[SENSOR_COUNT];		///< Handle to opened IST objects	

	/** \name Pointers to Sensor API fucntions in the Sensor library */
	//@{	
	LPFN_SENSOR_OPEN						lpfnSENSOR_Open;
	LPFN_SENSOR_CLOSE						lpfnSENSOR_Close;
	LPFN_SENSOR_FINDFIRST					lpfnSENSOR_FindFirst;
	LPFN_SENSOR_FINDNEXT					lpfnSENSOR_FindNext; 
	LPFN_SENSOR_FINDCLOSE					lpfnSENSOR_FindClose;
	LPFN_SENSOR_REGISTERDATANOTIFICATION	lpfnSENSOR_RegisterDataNotification;
	LPFN_SENSOR_DEREGISTERDATANOTIFICATION	lpfnSENSOR_DeregisterDataNotification;
	LPFN_SENSOR_STARTSAMPLING				lpfnSENSOR_StartSampling;
	LPFN_SENSOR_STOPSAMPLING				lpfnSENSOR_StopSampling;
	LPFN_SENSOR_GETPROPERTY					lpfnSENSOR_GetProperty;
	LPFN_SENSOR_SETPROPERTY					lpfnSENSOR_SetProperty;

	DWORD StartSensor(SENSOR_TYPE sensorType, HANDLE* phSensor, PFN_SENSOR_DATA_CALLBACK pfnSensorDataCallback, LONG* pscaleFactor);
	BOOL IsSensorAvailable(SENSOR_TYPE sensorType);

	//Sensor Data Callback funtions
	static DWORD OnReceiveAccelerometerSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop); 
	static DWORD OnReceiveOrientationSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);
	static DWORD OnReceiveTiltAngleSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);	
	static DWORD OnReceiveMotionSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop); 
	static DWORD OnReceiveECompassSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);
	static DWORD OnReceiveMagnetometerSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);
	static DWORD OnReceiveGyroscopeSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop); 
	static DWORD OnReceiveAmbientLightSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);
	static DWORD OnReceiveProximitySensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);
	static DWORD OnReceiveProximityLongRangeSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop); 
	static DWORD OnReceivePressureSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);
	static DWORD OnReceiveTemperatureSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);
	static DWORD OnReceiveHumiditySensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);
};
