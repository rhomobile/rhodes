/** 
* \file ISTSupport.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>
#include "SensorApi.h"

/******************************************************************************/
/** Sensor data returned to class user via \ref CSENSORSUPPORT::GetLatestData */
struct SENSORDATA
{
	/** \name Raw data from Sensor library */
	//@{
	LONG lXValue, lYValue, lZValue;
	//@}
};

// Sensor API type definitions
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

/******************************************************************************/
/**
* Thread safe wrapper around Sensor API.
* The class opens the Sensor library, if it's available on the device. It then runs
* a thread which loops continously, each time blocking until Sensor data is available.
* When data is available it is copied into a private member. Users of the class
* can call \ref WaitForSensorData to block until data is available, or can wait on the
* \ref hDataReady event. Then they can call GetLatestData to get a copy of the data.
* NOTE: The reason for using the static variables/methods in this class is to make the
* sensor data receive callback function as static. It is complicated method to specify
* non static memeber functions as callback functions
*/
/******************************************************************************/
class CSENSORSUPPORT
{
public:
	CSENSORSUPPORT ();	///< Loads Sensor library, if available, and creates thread
	~CSENSORSUPPORT ();	///< Terminates thread and unloads library

	/** Tests if Sensor library is present on the device
	* \return TRUE if Sensor library was found during class constructor */
	bool Supported () { return m_hSensorLibrary != NULL; }
	
	/**
	*  Opens a particular Sensor and registers for data events
	*/
	DWORD RegisterSensor (_SENSOR_TYPE eSensorType);

	/**
	*  Closes a particular Sensor and deregisters for data events
	*/
	DWORD UnregisterSensor	(_SENSOR_TYPE eSensorType);		

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
	
	//@}	

	/**
	* Waits for the sensor data from the sensors
	*/
	static DWORD WaitForSensorData ();

	/**
	* Gets the latest accelerometer data
	*/
	static void GetLatestShakeData (SENSORDATA *pdata);	

	/**
	* Gets the latest Tilt Angle data
	*/
	static void GetLatestTiltData (SENSORDATA *pdata);	

private:
	
	HMODULE m_hSensorLibrary;		///< Handle to loaded Sensor library

	HANDLE hSensorShake;			///< Handle to opened Accelerometer object
	HANDLE hSensorTilt;				///< Handle to opened TiltAngle object

	static HANDLE hDataShakeReady;	///< Event which class user can wait on for Accelerometer data to be available
	static HANDLE hDataTiltReady;	///< Event which class user can wait on for TiltAngle data to be available
	
	static HANDLE hCancel;			///< Handle to event which is signalled by the destructor to terminate the thread

	static long pScaleShake;		///< Accelerometer scale factor
	static long pScaleTilt;			///< Tilt Angle scale factor

	static SENSORDATA sLatestShakeData;		///< Copy of the latest data from the Accelerometer Sensor library
	static SENSORDATA sLatestTiltData;		///< Copy of the latest data from the TiltAngle Sensor library	

	/**
	* Starts listening to the sensor
	*/
	DWORD StartSensor(SENSOR_TYPE sensorType, HANDLE* phSensor, PFN_SENSOR_DATA_CALLBACK pfnSensorDataCallback, LONG* pscaleFactor);

	/** 
	* Copies latest Accelerometer Sensor data into private copy
	* \param pdata Sensor data to copy 
	*/
	static void SetLatestShakeData (SENSORDATA *pdata);

	/** 
	* Copies latest TiltAngle Sensor data into private copy
	* \param pdata Sensor data to copy 
	*/
	static void SetLatestTiltData (SENSORDATA *pdata);

	//Sensor Data Callback funtions

	/**
	* Accelerometer callback function
	*/
	static DWORD OnReceiveAccelerometerSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop); 

	/**
	* TiltAngle callback function
	*/
	static DWORD OnReceiveTiltAngleSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);	
	
};

