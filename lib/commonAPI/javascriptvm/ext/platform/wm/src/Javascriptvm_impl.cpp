#include "../../../shared/generated/cpp/JavascriptVMBase.h"

namespace rho {

using namespace apiGenerator;

class CJavascriptVMImpl: public CJavascriptVMBase
{
public:
    CJavascriptVMImpl(): CJavascriptVMBase()
    {
    }


};

class CJavascriptVMSingleton: public CJavascriptVMSingletonBase
{
    ~CJavascriptVMSingleton(){}

    virtual void executeScript( const rho::StringW& script, rho::apiGenerator::CMethodResult& oResult){}
};

class CJavascriptVMFactory: public CJavascriptVMFactoryBase
{
    ~CJavascriptVMFactory(){}
    virtual IJavascriptVMSingleton* createModuleSingleton();
};

extern "C" void Init_JavascriptVM_extension()
{
    CJavascriptVMFactory::setInstance( new CJavascriptVMFactory() );
    Init_JavascriptVM_API();
}

IJavascriptVMSingleton* CJavascriptVMFactory::createModuleSingleton()
{
    return new CJavascriptVMSingleton();
}

}