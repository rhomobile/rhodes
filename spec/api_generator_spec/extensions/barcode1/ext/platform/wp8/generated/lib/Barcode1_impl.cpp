#include "..\IBarcode1.h"
#include "MethodResultImpl.h"

using namespace Barcode1Runtime;
using namespace rho::apiGenerator;

class CBarcode1Impl: public CModuleBase<IBarcode1>
{
	IBarcode1Impl^ _runtime;
public:
    CBarcode1Impl(const rho::StringW& strID): CModuleBase<IBarcode1>(strID)
    {
        /*m_hashProps.put( L"display", L"LCD");
        m_hashProps.put( L"sound", L"Dolby");*/
    }

    virtual void enable( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult)
	{
		// _runtime->enable(..., ref new CMethodResultImpl((int64)&oResult));
	}
    
	virtual void start(CMethodResult& oResult)
	{
		_runtime->start(ref new CMethodResultImpl((int64)&oResult));
	}

    virtual void stop(CMethodResult& oResult)
	{
		_runtime->stop(ref new CMethodResultImpl((int64)&oResult));
	}

    virtual void disable(CMethodResult& oResult)
	{
		_runtime->disable(ref new CMethodResultImpl((int64)&oResult));
	}

    virtual void take( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult)
	{
		// _runtime->take(..., ref new CMethodResultImpl((int64)&oResult));
	}

	virtual void clearProps(CMethodResult& oResult)
	{
	}

	virtual void registerRuntime(IBarcode1Impl^ runtime)
	{
		_runtime = runtime;
	}
};

class CBarcode1Singleton: public CModuleSingletonBase<IBarcode1Singleton>
{
    ~CBarcode1Singleton(){}
    virtual rho::StringW getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CBarcode1Factory: public CBarcode1FactoryBase
{
    ~CBarcode1Factory(){}
    virtual IBarcode1Singleton* createModuleSingleton();
    virtual IBarcode1* createModuleByID(const rho::StringW& strID);
};

extern "C" void Init_Barcode1_extension()
{
    CBarcode1Factory::setInstance( new CBarcode1Factory() );
    Init_Barcode1_API();
}

IBarcode1* CBarcode1Factory::createModuleByID(const rho::StringW& strID)
{
    return new CBarcode1Impl(strID);
}

IBarcode1Singleton* CBarcode1Factory::createModuleSingleton()
{
    return new CBarcode1Singleton();
}

void CBarcode1Singleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::StringW> arIDs;
    arIDs.addElement(L"SC1");
    arIDs.addElement(L"SC2");

    oResult.set(arIDs);
}

rho::StringW CBarcode1Singleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::StringW>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}
