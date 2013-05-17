#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/SensorsBase.h"
#include "Sensors.h"

namespace rho {
	using namespace apiGenerator;
	using namespace common;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Sensors"

class CSensorsImpl: public IRhoExtension, CSensorsBase
{
public:
    CSensorsImpl(const rho::String& strID): CSensorsBase()
    {
        //m_hashProps.put( "display", "LCD");
        //m_hashProps.put( "sound", "Dolby");
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class CSensorsSingleton: public CSensorsSingletonBase
{
    ~CSensorsSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
	virtual void watchAccelerometer( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopAccelerometer(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchTiltAngle( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopTiltAngle(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchDeviceOrientation( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopDeviceOrientation(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchMotion( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopMotion(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watcheCompass( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopeCompass(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchMagnetometer( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopMagnetometer(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchGyroscope( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopGyroscope(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchAmbientLight( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopAmbientLight(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchProximity( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopProximity(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchProximityLongRange( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopProximityLongRange(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchPressure( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopPressure(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchTemperature( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopTemperature(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchHumidity( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopHumidity(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchGravity( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopGravity(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchLinearAcceleration( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopLinearAcceleration(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchRotation( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopRotation(rho::apiGenerator::CMethodResult& oResult){ }
    virtual void watchOrientation( int minimumGap, rho::apiGenerator::CMethodResult& oResult){ }
    virtual void stopOrientation(rho::apiGenerator::CMethodResult& oResult){ }
};

class CSensorsFactory: public CSensorsFactoryBase
{
    ~CSensorsFactory(){}
    virtual ISensorsSingleton* createModuleSingleton();
    virtual ISensors* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Sensors_extension()
{
    CSensorsFactory::setInstance( new CSensorsFactory() );
    Init_Sensors_API();
}

/*ISensors* CSensorsFactory::createModuleByID(const rho::String& strID)
{
    return new CSensorsImpl(strID);
}*/

ISensorsSingleton* CSensorsFactory::createModuleSingleton()
{
    return new CSensorsSingleton();
}

void CSensorsSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CSensorsSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}