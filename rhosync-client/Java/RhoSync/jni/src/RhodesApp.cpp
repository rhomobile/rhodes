#include "rhodes/JNIRhodes.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"

namespace rho {
namespace common {

boolean CRhodesApp::isBaseUrl(const String& strUrl)
{
    return String_startsWith(strUrl, getBaseUrl());
}

const String& CRhodesApp::getBaseUrl()
{
    return rho_root_path();
}

}}

RHO_GLOBAL int rho_conf_send_log()
{
    return 0;//RHODESAPP().sendLog();
}
