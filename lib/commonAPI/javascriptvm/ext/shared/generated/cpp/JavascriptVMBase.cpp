#include "JavascriptVMBase.h"
#include "common/RhodesApp.h"


namespace rho {

IMPLEMENT_LOGCLASS(CJavascriptVMSingletonBase, "JavascriptVM");
IMPLEMENT_LOGCLASS(CJavascriptVMBase, "JavascriptVM");

rho::common::CAutoPtr< CJavascriptVMFactoryBase> CJavascriptVMFactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 

////////////////////////////////////////////////

CJavascriptVMBase::CJavascriptVMBase()
{

}

CJavascriptVMSingletonBase::~CJavascriptVMSingletonBase()
{
    CJavascriptVMFactoryBase::setInstance(0);
}


}
