/**
 * \file Houses the CRawMotoSensor class
 */
#ifndef RAWMOTOSENSOR_H
#define RAWMOTOSENSOR_H

/******************************************************************************/
#pragma once

/******************************************************************************/
#include "SensorApiDll.h"
#include "SensorEventListener.h"
#include "RawSensor.h"


/**
 * all extension specifics goes into this name space
 * 
 * @author GXV738 (6/19/2013)
 */
namespace sensormodule
{

enum SensorStatus
{
    SENSOR_STATUS_NOT_READY = 0,
    SENSOR_STATUS_READY,
    SENSOR_STATUS_STARTED,
    SENSOR_STATUS_ERROR
};

class CRawMotoSensor : public IRawSensor
{
public:
    CRawMotoSensor(const char* sensorType);
    virtual ~CRawMotoSensor(void);

    virtual int getMinimumGap();
    virtual void setMinimumGap(int minimumGap);
    virtual const char* getType();
    virtual const char* getStatus();
    virtual bool Start();
    virtual bool Stop();

    bool SensorAPIsPresent() const;    
    bool RegisterSensor(ISensorEventListener* eventListener);    

    

    static void EnumerateSensors(rho::Vector<rho::String>& sensorList);

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



protected:
    DWORD OpenSensor(/*SENSOR_TYPE sensorType,*/ PFN_SENSOR_DATA_CALLBACK pfnSensorDataCallback);
    DWORD CloseSensor();

private:
    CRawMotoSensor();
    CRawMotoSensor(const CRawMotoSensor&);
    CRawMotoSensor& operator=(const CRawMotoSensor&);

    void AddEventListener(SENSOR_TYPE sensorType, ISensorEventListener* listener);
    void RemoveEventListener(SENSOR_TYPE sensorType);

    static const char* ConvertToTypeDescription(SENSOR_TYPE sensorType);
    static SENSOR_TYPE ConvertFromTypeDescription(const char* type);
    static const char* ConvertToStatusDescription(SensorStatus sensorStatus);
    static SensorStatus ConvertFromStatusDescription(const char* status);

    CSensorApiDll*      m_sensorApiDll;
    int                 m_minimumInterval;
    SensorStatus        m_status;
    SENSOR_TYPE         m_type;
    DWORD               m_dwQuietTime;
    LONG                m_lScale;
    bool                m_bFirstSample;

    static rho::Hashtable<DWORD, sensormodule::ISensorEventListener*> g_listenerMap;
    //static std::vector<LONG> g_scale;
    static rho::common::CMutex  m_cs;
    

}; // class ends

} // namespace ends

#endif //RAWMOTOSENSOR_H
