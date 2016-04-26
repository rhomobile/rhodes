#pragma once

#include "gpswrapperobserver.h"

#define GPSINFO_WAIT_TIMEOUT		30000		// Wait timeout
#define GPSINFO_EVENT_COUNT			2			// Number of events to wait for
#define GPS_HOR_ACCURACY_MT			5			//Approximate horizontal accuracy of the GPS hw in meters
#define GPS_VER_ACCURACY_MT			20			//Approximate vertical accuracy of the GPS hw in meters
const float KNOT_TO_MTS = 0.514444444444444;    //Knots in meters/sec

class CGpsWrapper
{
public:
	CGpsWrapper(IGpsWrapperObserver& observer, bool bGeolocationEnabled);
	~CGpsWrapper(void);
	/**
	* Load the GPS Library
	*
	* \return TRUE if successful, FALSE otherwise
	*/
	BOOL Init();
	/**
	* Format of the callback function specified by the core for the engine to use
	* to request periodic location updates.
	*
	* \param highAccuracy whether high accuracy results are required or not
	*
	* \return S_OK for success
	*/
	LRESULT StartLocationUpdates(EngineLocationInterface* locationInterface,
                                 BOOL highAccuracy);
	/**
	* Format of the callback function specified by the core for the engine to use
	* to stop periodic location updates.
	*
	* \return S_OK for success
	*/
	LRESULT StopLocationUpdates(EngineLocationInterface* locationInterface);
	/**
	 * Format of the callback function specified by the core for the engine to use
	 * to query the last cached location.
	 *
	 * \param location EngineLocation structure to contain results
	 *
	 * \return S_OK if cached location available
	 */
	LRESULT GetCachedLocation(EngineLocationInterface* locationInterface,
										  EngineLocation* location);
public: //members
	static EngineLocation* s_pEkCachedPosition; //To be passed back to WebKit
	static EngineLocationError s_lastErrorCode; //Last error code set by the gpswrapper
	static LPCTSTR s_lastErrorMessage; //Last error message, which is a string explanation of the error code

private: //methods
	//Start the GPS device and create the listening thread
	BOOL Start();
		//Stop the GPS device and cleanup
	BOOL Stop();
	//Creates the listening thread
	void ListenToGpsEvents(void);
	//Actual listening thread
	static DWORD GpsListener(LPVOID pParam);
	//Convert some of the GPS data to the expected units
	void ConvertGpsData(GPS_POSITION position);
	//Set the last error class properties
	static void SetLastError(EngineLocationError errorCode, LPCTSTR errorMessage);
private: //members
	DWORD m_dwThreadId;
	HANDLE m_hThreadHandle;
		
	HANDLE m_hGpsHandle;
	HANDLE m_hLocationDataChangedEvent;
	HANDLE m_hDeviceStateChangedEvent;
	IGpsWrapperObserver& mObserver;
	DWORD m_dwLastError;
	HINSTANCE m_pGpsIdDll;
	BOOL m_bThreadActive;
	BOOL m_bIsEnabled;
};

extern "C"
{
static CGpsWrapper* g_pGpsWrapperInstance;

//C wrapper around CGpsWrapper::StartLocationUpdates
extern LRESULT startLocationUpdates(EngineLocationInterface* locationInterface,
							 BOOL highAccuracy);
//C wrapper around CGpsWrapper::StopLocationUpdates
extern LRESULT stopLocationUpdates(EngineLocationInterface* locationInterface);

//C wrapper around CGpsWrapper::GetCachedLocation
extern LRESULT getCachedLocation(EngineLocationInterface* locationInterface,
						  EngineLocation* location);
}