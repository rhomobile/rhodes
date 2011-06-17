#include <time.h>
#include <string>
#include "ruby/ext/rho/rhoruby.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/StringConverter.h"
#include "sync/ClientRegister.h"
#include "common/RhoFilePath.h"
#undef null
#include <QtGui/QApplication>
#include "impl/MainWindowImpl.h"

using namespace rho;
using namespace rho::common;
using namespace std;

static String g_strCmdLine;

extern "C" {
    void parseHttpProxyURI(const String &http_proxy);
    void rho_ringtone_manager_stop();
}

int main(int argc, char *argv[])
{
    CMainWindow m_appWindow;
    String m_strRootPath, m_strRhodesPath, m_logPort;
    String m_strHttpProxy;

    //TODO: ParseCommandLine
    m_logPort = String("11000"); // "log"
    m_strRootPath = String("/Volumes/MacOSWork/Rhomobile/rhodes-system-api-samples/"); // "approot"
    m_strRhodesPath = String("/Volumes/MacOSWork/Rhomobile/rhodes"); // "rhodespath"
    // m_strHttpProxy: "http_proxy_url"
    // copy all arguments to: g_strCmdLine

    // PreMessageLoop:
    rho_logconf_Init(m_strRootPath.c_str(), m_logPort.c_str());
    RHOSIMCONF().setAppConfFilePath(CFilePath::join(m_strRootPath, RHO_EMULATOR_DIR"/rhosimconfig.txt").c_str());
    RHOSIMCONF().loadFromFile();
    if ( m_strRhodesPath.length() > 0 )
        RHOSIMCONF().setString("rhodes_path", m_strRhodesPath, false );
    RHOCONF().setString( "rhosim_platform", RHOSIMCONF().getString( "platform"), false);

    if ( !rho_rhodesapp_canstartapp(g_strCmdLine.c_str(), " /-,") )
    {
        LOG(INFO) + "This is hidden app and can be started only with security key.";
        return 1;
    }

    LOG(INFO) + "Rhodes started";
    if (m_strHttpProxy.length() > 0) {
        parseHttpProxyURI(m_strHttpProxy);
    } else {
        if (RHOCONF().isExist("http_proxy_url")) {
            parseHttpProxyURI(RHOCONF().getString("http_proxy_url"));
        }
    }

    if (RHOSIMCONF().getString("debug_host").length() > 0)
        RHOCONF().setString("debug_host", RHOSIMCONF().getString("debug_host"), false);
    if (RHOSIMCONF().getString("debug_port").length() > 0)
        RHOCONF().setString("debug_port", RHOSIMCONF().getString("debug_port"), false);

    /*
    //Check for bundle directory is exists.
    HANDLE hFind;
    WIN32_FIND_DATA wfd;
        
    // rootpath + "rho/"
    if (m_strRootPath.at(m_strRootPath.length()-1) == '/') {
        hFind = FindFirstFile(convertToStringW(m_strRootPath.substr(0, m_strRootPath.find_last_of('/'))).c_str(), &wfd);
    } else if (m_strRootPath.at(m_strRootPath.length()-1) == '\\') {
        //delete all '\' from the end of the pathname
        int i = m_strRootPath.length();
        for ( ; i != 1; i--) {
            if (m_strRootPath.at(i-1) != '\\')
                break;
        }
        hFind = FindFirstFile(convertToStringW(m_strRootPath.substr(0, i)).c_str(), &wfd);
    }

    if (INVALID_HANDLE_VALUE == hFind || !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        int last = 0, pre_last = 0;
        last = getRhoRootPath().find_last_of('\\');
        pre_last = getRhoRootPath().substr(0, last).find_last_of('\\');
        String appName = getRhoRootPath().substr(pre_last + 1, last - pre_last - 1);

        String messageText = "Bundle directory \"" + 
            m_strRootPath.substr(0, m_strRootPath.find_last_of('/')) + 
            "\" is  missing\n";

        LOG(INFO) + messageText;
        int msgboxID = MessageBox(NULL,
            convertToStringW(messageText).c_str(),
            convertToStringW(appName).c_str(),
            MB_ICONERROR | MB_OK);

        return 2;
    }
    */

    rho::common::CRhodesApp::Create(m_strRootPath);

    // Create the main application window
    m_appWindow.Initialize(convertToStringW(RHOSIMCONF().getString("app_name")).c_str());

    RHODESAPP().startApp();

    // Navigate to the "loading..." page
    m_appWindow.navigate(L"about:blank", -1 );

    // Show the main application window
    //m_appWindow.ShowWindow();

    rho_clientregister_create("win32_client");

    // RunMessageLoop:
    m_appWindow.messageLoop();

    // stopping Rhodes application
    rho_ringtone_manager_stop();
    m_appWindow.DestroyUi();
    rho::common::CRhodesApp::Destroy();
    //TODO: net::CNetRequestImpl::deinitConnection();

    return 0;
}
