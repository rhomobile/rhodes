#pragma once

#if defined (_WIN32_WCE)

#include <gpsapi.h>
#include "logging/RhoLog.h"

interface IGPSController
{
    virtual HRESULT SetGPSPosition( GPS_POSITION gps_Position ) = 0;
    virtual HRESULT SetGPSDeviceInfo( GPS_DEVICE gps_Device ) = 0;
};

class CGPSDevice
{
  DEFINE_LOGCLASS;
private:
    //Singleton instance
    static CGPSDevice * s_pInstance;

    //Device handle
    HANDLE m_hGPS_Device;

    //Event for location data updates
    HANDLE m_hNewLocationData;

    //Event for device state changes
    HANDLE m_hDeviceStateChange;

    //Thread's handle and id
    HANDLE m_hThread;
    DWORD m_dwThreadID;

    //Exit event
    HANDLE m_hExitThread;

    //Pointer to sink interface
    IGPSController * m_pController;

private:
    //Our wrapper is singleton make constructor private
    CGPSDevice(void);

    HRESULT StartThread();
    HRESULT StopThread();

    static CGPSDevice * Instance();
    static DWORD WINAPI GPSThreadProc(__opt LPVOID lpParameter);
public:

    virtual ~CGPSDevice(void);

    static HRESULT TurnOn(IGPSController * pController);
    static HRESULT TurnOff();

    static bool isAvailable();
};

class CGPSController : public IGPSController {
    DEFINE_LOGCLASS;
	CRITICAL_SECTION m_critical_section;
	double m_latitude;
	double m_longitude;
	bool   m_knownPosition;
	bool   m_gpsIsOn;
	time_t m_timeout;
    //Singleton instance
    static CGPSController * s_pInstance;

public:
	static CGPSController* Instance();
    static CGPSController* startInstance();
	static void DeleteInstance();
	static void CheckTimeout();
    static void TurnGpsOff();
	virtual ~CGPSController();
    //static void show_geolocation(struct shttpd_arg *arg);

public:
    virtual HRESULT SetGPSPosition( GPS_POSITION gps_Position );
    virtual HRESULT SetGPSDeviceInfo( GPS_DEVICE gps_Device );
	virtual bool IsKnownPosition();
	virtual double GetLatitude();
	virtual double GetLongitude();
	virtual time_t UpdateTimeout();

	virtual void TurnGpsOn();

protected:
	void Lock();
    void Unlock();

private:
	CGPSController();
};

#endif //_WIN32_WCE


