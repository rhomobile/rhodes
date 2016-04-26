//
//  DeviceImpl.cpp
#include"DeviceModule.h"
#include "../../../../shared/generated/cpp/DeviceBase.h"

namespace rho 
{
	using namespace apiGenerator;
	using namespace common;

	#undef DEFAULT_LOGCATEGORY
	#define DEFAULT_LOGCATEGORY "Device"

	class CDeviceSingletonImpl: public CDeviceSingletonBase
	{
	public:
		CDeviceSingletonImpl(): CDeviceSingletonBase()
		{
		
		}
	    
		// calibrate - Carries out the screen calibration routine. 
		virtual void calibrate(rho::apiGenerator::CMethodResult& oResult) 
		{
			SetCallback(NULL);
			if (oResult.hasCallback())
			{
				DEBUGMSG(true, (L"Callback"));
				SetCallback(&oResult);
				//Calibrate the device with callback.
				CalibrateTheDeviceWithCallback();	
			}
			else
			{
				//Calibrate the device without callback.
				CalibrateTheDevice();	
			}		
		} 
		// suspend - Puts the device into suspend mode. On suspend, the device goes to hibernation mode. Callback is triggered only for 'failed' status.
		virtual void suspend(rho::apiGenerator::CMethodResult& oResult) 
		{
			SetCallback(NULL);
			if (oResult.hasCallback())
			{
				DEBUGMSG(true, (L"Callback"));
				SetCallback(&oResult);
			}
			//Suspend the device.
			SuspendTheDevice();
		} 
		// powerOff  - Puts the device into power off mode.  In this mode the device will draw no power. Only supported on SB1. Callback is triggered only for 'failed' status.
		virtual void powerOff(rho::apiGenerator::CMethodResult& oResult) 
		{
			SetCallback(NULL);
			if (oResult.hasCallback())
			{
				DEBUGMSG(true, (L"Callback"));
				SetCallback(&oResult);
			}
			//PowerOff the device.
			PowerOffTheDevice();			
		} 
		// idle - Puts the device into idle mode. In this mode, the screen display is turned off. Callback is triggered only for 'failed' status.
        virtual void idle(rho::apiGenerator::CMethodResult& oResult) 
		{
			SetCallback(NULL);
			if (oResult.hasCallback())
			{
				DEBUGMSG(true, (L"Callback"));
				SetCallback(&oResult);
			}
            //Idle the device.
            IdleTheDevice();
        } 
		// wake - In WM/CE, wakes the device from idle state to active state. Callback is triggered only for 'failed' status. In Android, it will turn on the display, if it was off. The callback parameter is ignored in Android platform.
		virtual void wake(rho::apiGenerator::CMethodResult& oResult) 
		{
			SetCallback(NULL);
			if (oResult.hasCallback())
			{
				DEBUGMSG(true, (L"Callback"));
				SetCallback(&oResult);
			}
			//Wake the device.
			WakeTheDevice();            
		} 
		// reboot - In WM/CE, reboots the terminal using either a Warm or Cold software boot (as specified). Note on CE6 devices a 'ColdCAD' boot is required to replicate the Coldboot key sequence, e.g. 1+9+Power on an MC3000. Callback is triggered only for 'failed' status. In Android, it will reboot the device. The callback parameter is ignored in Android platform. 
		virtual void reboot( const rho::String& bootType, rho::apiGenerator::CMethodResult& oResult) 
		{
			SetCallback(NULL);
			if (oResult.hasCallback())
			{
				DEBUGMSG(true, (L"Callback"));
				SetCallback(&oResult);
			}
			//Reboot the device.
			RebootTheDevice(convertToStringW(bootType).c_str());            
		} 

	};

	class CDeviceImpl : public CDeviceBase
	{
	public:
		CDeviceImpl(const rho::String& strID) 
		{

		}
		virtual ~CDeviceImpl() 
		{
		
		}
	};
	    
	////////////////////////////////////////////////////////////////////////

	class CDeviceFactory: public CDeviceFactoryBase    
	{
	public:
		CDeviceFactory()
		{
		
		}
		~CDeviceFactory()
		{

		}        
		IDeviceSingleton* createModuleSingleton()
		{ 
			return new CDeviceSingletonImpl();
		}        
	};    

	extern "C" void Init_Device_extension()
	{
		CDeviceFactory::setInstance( new CDeviceFactory() );
		Init_Device_API();
	}
}

