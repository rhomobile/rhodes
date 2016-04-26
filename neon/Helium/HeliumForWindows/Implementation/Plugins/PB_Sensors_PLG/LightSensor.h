/**
* \file Light Sensor header file
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include "../../common/Public/PBPlugin.h"
#include "SensorAPI.h"

/******************************************************************************/
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

/******************************************************************************/
static TCHAR* tcLightSensorEventNames[] = 
{
	L"LightSensorValue",
	NULL// Light Sensor Value	
};

// Sensor Event Values
static long lSensorValue;

/**
* CLightSensor - The class that contains the Sensor related information provided by Sensor API
*/
class CLightSensor 
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
	CLightSensor				(int iInstanceID, PBModule* pModule);

	/**
	*  Used to free memory and  values
	*/
	~CLightSensor	();

	/**
	*  Sets the URI to be used in the Raw Sensors Event
	*/
	BOOL SetLightSensorURI	(LPCWSTR tcNavURI);

	/**
	*  Opens a particular Sensor and registers for data events
	*/
	DWORD RegisterSensor ();

	/**
	*  Closes a particular Sensor and deregisters for data events
	*/
	DWORD UnregisterSensor	();

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

	static WCHAR*		m_szLightSensorURI;	///<  Registered URI for Raw Sensor event (blob data)
	static PBModule*	m_pModule;			///<  Pointer to the CLightSensorModule object owning this CLightSensor instance.  Used for logging and navigation.	
	static int			m_iInstanceID;		///<  Unique identifier of the CLightSensorModule object owning this CLightSensor instance.  Used for navigation.

	static int			m_iMinimumInterval;	///<  Minimum Interval specified by the user
	static long			m_dwQuietStart;		///<  System tick count when the minimum interval was started	
	static LONG			pScale;

	HMODULE				m_hSensorLibrary;	///<  Handle to the loaded Sensor library
	HANDLE				hSensor;		///< Handle to opened IST objects	

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

	//Sensor Data Callback funtion	
	static DWORD OnReceiveAmbientLightSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop);	
};
