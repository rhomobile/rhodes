#include "IntentImplBase.h"

namespace rho {

using namespace rho::apiGenerator;

struct Intent
{
    static IIntentSingleton* getIntentObj(){ return CIntentFactoryBase::getIntentSingletonS(); }
    static void addApplicationMessage(const rho::String& appName, const rho::String& msg) { static_cast<CIntentImplBase*>(getIntentObj())->addApplicationMessage(appName, msg); }
};

}
