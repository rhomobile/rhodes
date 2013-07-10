#pragma once
#include "SensorApi.h"
#include "RhoStd.h"

namespace sensormodule
{
class CSensorApiDll
{
public:
    CSensorApiDll(void);
    ~CSensorApiDll(void);

    DWORD SensorOpen(OPEN_MODE eMode, SENSOR_INFO_T& ptSensorInfo);
    DWORD SensorClose();
    DWORD SensorRegisterDataNotification(NOTIFIER_TYPE eNotifierType, LPVOID lpNotifier, DWORD dwNotifyThreshold);
    DWORD SensorDeregisterDataNotification();
    DWORD SensorStartSampling();
    DWORD SensorStopSampling();
    DWORD SensorGetProperty(SENSOR_PROPERTY eProperty, LONG &nValue);
    DWORD SensorSetProperty(SENSOR_PROPERTY eProperty, LONG  nValue);    

    
    bool SensorAPIsPresent() const;
    operator HANDLE() const;

    // static helpers
    static DWORD Enumerate(rho::Vector<SENSOR_TYPE>& sensorList);
    static void ReadyForOpen(SENSOR_TYPE sensorType, SENSOR_INFO_T& ptSensorInfo);

protected:
    DWORD LoadModule();
    DWORD UnloadModule();
private:
    CSensorApiDll(const CSensorApiDll&);
    CSensorApiDll& operator=(const CSensorApiDll&);

    HMODULE m_hSensorLibrary;
    HANDLE  m_hSensor;
};
} // namespace ends