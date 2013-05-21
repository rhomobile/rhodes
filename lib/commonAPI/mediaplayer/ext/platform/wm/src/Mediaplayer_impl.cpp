#include "../../../shared/generated/cpp/MediaplayerBase.h"

namespace rho {

using namespace apiGenerator;

class CMediaplayerImpl: public CMediaplayerBase
{
public:
    CMediaplayerImpl(const rho::String& strID): CMediaplayerBase()
    {
        m_hashProps.put( "display", "LCD");
        m_hashProps.put( "sound", "Dolby");
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class CMediaplayerSingleton: public CMediaplayerSingletonBase
{
    ~CMediaplayerSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CMediaplayerFactory: public CMediaplayerFactoryBase
{
    ~CMediaplayerFactory(){}
    virtual IMediaplayerSingleton* createModuleSingleton();
    virtual IMediaplayer* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Mediaplayer_extension()
{
    CMediaplayerFactory::setInstance( new CMediaplayerFactory() );
    Init_Mediaplayer_API();
}

IMediaplayer* CMediaplayerFactory::createModuleByID(const rho::String& strID)
{
    return new CMediaplayerImpl(strID);
}

IMediaplayerSingleton* CMediaplayerFactory::createModuleSingleton()
{
    return new CMediaplayerSingleton();
}

void CMediaplayerSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CMediaplayerSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}