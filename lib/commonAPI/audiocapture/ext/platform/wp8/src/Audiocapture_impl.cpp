#include "../../../shared/generated/cpp/AudiocaptureBase.h"

namespace rho {

using namespace apiGenerator;

class CAudiocaptureImpl: public CAudiocaptureBase
{
public:
    CAudiocaptureImpl(const rho::String& strID): CAudiocaptureBase()
    {
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class CAudiocaptureSingleton: public CAudiocaptureSingletonBase
{
    ~CAudiocaptureSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CAudiocaptureFactory: public CAudiocaptureFactoryBase
{
    ~CAudiocaptureFactory(){}
    virtual IAudiocaptureSingleton* createModuleSingleton();
    virtual IAudiocapture* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Audiocapture_extension()
{
    CAudiocaptureFactory::setInstance( new CAudiocaptureFactory() );
    Init_Audiocapture_API();
}

IAudiocapture* CAudiocaptureFactory::createModuleByID(const rho::String& strID)
{
    return new CAudiocaptureImpl(strID);
}

IAudiocaptureSingleton* CAudiocaptureFactory::createModuleSingleton()
{
    return new CAudiocaptureSingleton();
}

void CAudiocaptureSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CAudiocaptureSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}