#include "common/RhodesApp.h"
#include "../wp8/rhoruntime/common/RhoConvertWP8.h"
#include "../../../shared/generated/cpp/InstrumentationBase.h"
#include "Instrumentation_MethodResultImpl.h"

using namespace InstrumentationRuntime;
using namespace rho::apiGenerator;

namespace rho {

class CInstrumentationImpl: public CInstrumentationBase
{
    IInstrumentationImpl^ _runtime;
public:
    CInstrumentationImpl(const rho::String& strID): CInstrumentationBase()
    {
        // TODO: implement implementation constructor
    }

    virtual void registerRuntime(IInstrumentationImpl^ runtime)
    {
        _runtime = runtime;
    }

    virtual void simulate_key_event_code(int keycode, CMethodResult& oResult)
    {
        _runtime->simulate_key_event_code(keycode, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void simulate_key_event_string(const rho::String& str, CMethodResult& oResult)
    {
        Platform::String^ _str = rho::convertStringWP8(str);
        _runtime->simulate_key_event_string(_str, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void simulate_touch_event(int event_type, int x, int y, CMethodResult& oResult)
    {
        _runtime->simulate_touch_event(event_type, x, y, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void screen_capture(const rho::String& pszFilename, CMethodResult& oResult)
    {
        Platform::String^ _pszFilename = rho::convertStringWP8(pszFilename);
        _runtime->screen_capture(_pszFilename, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void get_allocated_memory(CMethodResult& oResult)
    {
        _runtime->get_allocated_memory(ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void delete_file(const rho::String& str, CMethodResult& oResult)
    {
        Platform::String^ _str = rho::convertStringWP8(str);
        _runtime->delete_file(_str, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void file_exists(const rho::String& str, CMethodResult& oResult)
    {
        Platform::String^ _str = rho::convertStringWP8(str);
        _runtime->file_exists(_str, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void re_simulate_navigation(CMethodResult& oResult)
    {
        _runtime->re_simulate_navigation(ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void getProperty(const rho::String& propertyName, CMethodResult& oResult)
    {
        Platform::String^ _propertyName = rho::convertStringWP8(propertyName);
        _runtime->getProperty(_propertyName, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void getProperties(const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult)
    {
        Windows::Foundation::Collections::IVectorView<Platform::String^>^ _arrayofNames = rho::convertArrayWP8(arrayofNames);
        _runtime->getProperties(_arrayofNames, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void getAllProperties(CMethodResult& oResult)
    {
        _runtime->getAllProperties(ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void setProperty(const rho::String& propertyName, const rho::String& propertyValue, CMethodResult& oResult)
    {
        Platform::String^ _propertyName = rho::convertStringWP8(propertyName);
        Platform::String^ _propertyValue = rho::convertStringWP8(propertyValue);
        _runtime->setProperty(_propertyName, _propertyValue, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void setProperties(const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
    {
        Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ _propertyMap = rho::convertHashWP8(propertyMap);
        _runtime->setProperties(_propertyMap, ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }

    virtual void clearAllProperties(CMethodResult& oResult)
    {
        _runtime->clearAllProperties(ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }
};

class CInstrumentationSingleton: public CInstrumentationSingletonBase
{
    IInstrumentationSingletonImpl^ _runtime;
public:
    ~CInstrumentationSingleton(){}

    virtual void registerRuntime(IInstrumentationSingletonImpl^ runtime)
    {
        _runtime = runtime;
    }

    virtual void enumerate(CMethodResult& oResult)
    {
        _runtime->enumerate(ref new CInstrumentationMethodResultImpl((int64)&oResult));
    }


    virtual rho::String getDefaultID(){return "1";} // TODO: implement getDefaultID
    virtual rho::String getInitialDefaultID(){return "1";} // TODO: implement getInitialDefaultID
    virtual void setDefaultID(const rho::String& strID){} // TODO: implement setDefaultID
    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor){} // TODO: implement addCommandToQueue
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor){} // TODO: implement callCommandInThread
};

class CInstrumentationFactory: public CInstrumentationFactoryBase
{
    ~CInstrumentationFactory(){}
    virtual IInstrumentationSingleton* createModuleSingleton();
    virtual IInstrumentation* createModuleByID(const rho::String& strID);
};

IInstrumentation* CInstrumentationFactory::createModuleByID(const rho::String& strID)
{
    return new CInstrumentationImpl(strID);
}

IInstrumentationSingleton* CInstrumentationFactory::createModuleSingleton()
{
    return new CInstrumentationSingleton();
}

}

extern "C" void Init_Instrumentation()
{
    rho::CInstrumentationFactory::setInstance( new rho::CInstrumentationFactory() );
    rho::Init_Instrumentation_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoInstrumentationApi");
}
