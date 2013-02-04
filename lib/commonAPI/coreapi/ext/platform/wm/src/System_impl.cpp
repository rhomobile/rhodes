#include "../generated/ISystem.h"

class CSystemImpl: public CModuleBase<ISystem>
{
public:
    CSystemImpl(const rho::StringW& strID): CModuleBase<ISystem>(strID)
    {
    }

};

class CSystemSingleton: public CModuleSingletonBase<ISystemSingleton>
{
    ~CSystemSingleton(){}

    virtual rho::StringW getInitialDefaultID();
};

class CSystemFactory: public CSystemFactoryBase
{
    ~CSystemFactory(){}
    virtual ISystemSingleton* createModuleSingleton();
    virtual ISystem* createModuleByID(const rho::StringW& strID);
};

extern "C" void Init_System_extension()
{
    CSystemFactory::setInstance( new CSystemFactory() );
    Init_System_API();
}

ISystemSingleton* CSystemFactory::createModuleSingleton()
{
    return new CSystemSingleton();
}

ISystem* CSystemFactory::createModuleByID(const rho::StringW& strID)
{
    return new CSystemImpl(strID);
}

rho::StringW CSystemSingleton::getInitialDefaultID()
{
    return L"1";
}

