#include "generated/cpp/ApplicationBase.h"

namespace rho {

using namespace rho::apiGenerator;

struct Application
{
    static IApplicationSingleton* getApplicationObj(){ return CApplicationFactoryBase::getApplicationSingletonS(); }

    static rho::String databaseFilePath(const rho::String& partitionName){ rho::apiGenerator::CMethodResult oResult(true); 
    getApplicationObj()->databaseFilePath(partitionName, oResult); return oResult.getString(); }
};

}
