#include "../../../shared/generated/cpp/KeyboardBase.h"

namespace rho {

using namespace apiGenerator;

class CKeyboardImpl: public CKeyboardBase
{
public:
    CKeyboardImpl(const rho::String& strID): CKeyboardBase()
    {
    }

    virtual void getPlatformName(rho::apiGenerator::CMethodResult& oResult) {
         oResult.set("UWP");
    }

    virtual void calcSumm( int a,  int b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
    
    virtual void joinStrings( const rho::String& a,  const rho::String& b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
};

class CKeyboardSingleton: public CKeyboardSingletonBase
{
    ~CKeyboardSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CKeyboardFactory: public CKeyboardFactoryBase
{
    ~CKeyboardFactory(){}
    virtual IKeyboardSingleton* createModuleSingleton();
    virtual IKeyboard* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Keyboard_extension()
{
    CKeyboardFactory::setInstance( new CKeyboardFactory() );
    Init_Keyboard_API();
}

IKeyboard* CKeyboardFactory::createModuleByID(const rho::String& strID)
{
    return new CKeyboardImpl(strID);
}

IKeyboardSingleton* CKeyboardFactory::createModuleSingleton()
{
    return new CKeyboardSingleton();
}

void CKeyboardSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CKeyboardSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}