#include "common/RhoDefs.h"
#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/StringConverter.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include "rhoruby/api/RhoRuby.h"
#undef null

using namespace rho;
using namespace rho::common;
using namespace std;

#include "../../platform/shared/qt/rhodes/RhoSimulator.h"
#include <string>

static bool m_isJSApplication = false;
static String m_strRootPath, m_strRhodesPath, m_logPort;
static String m_strHttpProxy;


extern void parseHttpProxyURI(const rho::String &http_proxy);

extern "C" {

    const char* rho_native_rhopath()
    {
        return m_strRootPath.c_str();
    }
}



int main(int argc, char *argv[])
{
    char path[MAX_PATH] = {0};
    GetCurrentDirectoryA(MAX_PATH, path);
    std::string current_path(path);
    m_strRootPath = current_path;
    m_strRootPath += "/rho/";
    m_logPort = rho::String("11000");

    if (m_strHttpProxy.length() > 0) {
        parseHttpProxyURI(m_strHttpProxy);
    } else {
        if (RHOCONF().isExist("http_proxy_url")) {
            parseHttpProxyURI(RHOCONF().getString("http_proxy_url"));
        } else if (RHOCONF().isExist("http_proxy_uri")) {
            parseHttpProxyURI(RHOCONF().getString("http_proxy_uri"));
        }
    }

    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());
    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRootPath);
    RHODESAPP().setJSApplication(m_isJSApplication);
    RHODESAPP().startApp();
    RHODESAPP().waitAppStarted();


    return 0;
}
