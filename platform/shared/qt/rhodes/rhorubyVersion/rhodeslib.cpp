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
#include <iostream>
#include <mutex>
#include <condition_variable>

static bool m_isJSApplication = false;
static String m_strRootPath;

std::mutex waitGuard;
std::condition_variable waitVar;

extern void parseHttpProxyURI(const rho::String &http_proxy);

extern "C" {

    const char* rho_native_rhopath()
    {
        return m_strRootPath.c_str();
    }
}

#ifdef WINDOWS_PLATFORM
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

DLL_EXPORT std::string executeRubyMethodWithJSON(const char* full_class_name, const char* method_name, const char* parameters)
{
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rr->loadModel(full_class_name);
    rho::ruby::IString* ruby_string = rr->executeRubyMethodWithJSON(full_class_name, method_name, parameters);
    if(!ruby_string) return "(null)";

    std::string result = ruby_string->getUTF8();
    RAWLOG_INFO1("result: %s", result.c_str());
    ruby_string->release();
    return result;
}


DLL_EXPORT void rholib_init(const std::string& rootPath, const std::string& userPath, const std::string& runTimePath,
                 const std::string& logPath,  const std::string& httpProxy, const std::string& logPort, bool isJs)
{
    m_strRootPath = rootPath;
    std::string _logPort;
    if(logPort.empty())
        _logPort = "11000";

    if (httpProxy.length() > 0) {
        parseHttpProxyURI(httpProxy);
    } else {
        if (RHOCONF().isExist("http_proxy_url")) {
            parseHttpProxyURI(RHOCONF().getString("http_proxy_url"));
        } else if (RHOCONF().isExist("http_proxy_uri")) {
            parseHttpProxyURI(RHOCONF().getString("http_proxy_uri"));
        }
    }

    rho_logconf_Init(logPath.c_str(), rootPath.c_str(), logPort.c_str());
    rho::common::CRhodesApp::Create(rootPath, userPath, runTimePath);
    RHODESAPP().setJSApplication(isJs);
    RHODESAPP().startApp();
    RHODESAPP().waitAppStarted();

    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby(true);
    rr->init();


}

DLL_EXPORT void executeInRubyThread(rho::ruby::IRunnable* runnable)
{
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rr->executeInRubyThread(runnable);
}

DLL_EXPORT std::string getRubyServerURL()
{
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IString* str = rr->getRubyServerURL();
    std::string url = str->getUTF8();
    return url;
}


int main(int argc, char *argv[])
{
    char path[MAX_PATH] = {0};
    GetCurrentDirectoryA(MAX_PATH, path);
    std::string current_path(path);
    m_strRootPath = current_path;
    m_strRootPath += "/rho/";

    rholib_init(m_strRootPath, m_strRootPath, m_strRootPath, m_strRootPath, "", "", false);

    std::unique_lock<std::mutex> lock(waitGuard);

    class InitTest : public rho::ruby::IRunnable
    {
    public:
        InitTest() {}

        void run()
        {
            std::string result = executeRubyMethodWithJSON("Model1", "fillModelByPredefinedSet", nullptr);
        }
    } InitTask;

    class Test1 : public rho::ruby::IRunnable
    {
    public:
        Test1() {}

        void run()
        {
            std::string result = executeRubyMethodWithJSON("Model1", "receiveAllItemAsArrayOfHashesWithParams",
                                      "[{\"key1_array\":[\"param_array1_item1_string_value\",\"param_array1_item2_string_value\"],\"key2_integer\":1234567,\"key3_bool\":true},0.123450]");
            waitVar.notify_one();
        }
    } Test1Task;

    executeInRubyThread(&InitTask);
    executeInRubyThread(&Test1Task);


    waitVar.wait(lock);

    return 0;
}
