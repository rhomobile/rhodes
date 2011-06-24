#include <string>
#include "common/RhoPort.h"
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

static String m_strRootPath, m_strRhodesPath, m_logPort;
static String m_strHttpProxy;

extern "C" {
    void parseHttpProxyURI(const String &http_proxy);
    void rho_ringtone_manager_stop();

    const char* rho_native_rhopath() 
    {
        return m_strRootPath.c_str();
    }
}

char* parseToken(const char* start)
{
    int len = strlen(start);
    int nNameLen = 0;
    while (*start==' ') { start++; len--; }

    int i = 0;
    for (i = 0; i < len; i++) {
        if (start[i] == '=') {
            if (i > 0) {
                int s = i-1;
                for (; s >= 0 && start[s]==' '; s--);
                nNameLen = s+1;
                break;
            } else
                break;
        }
    }

    if ( nNameLen == 0 )
        return NULL;

    const char* szValue = start + i+1;
    int nValueLen = 0;

    while (*szValue==' ' || *szValue=='\'' || *szValue=='"' && nValueLen >= 0) { szValue++; }
    while (szValue[nValueLen] && szValue[nValueLen] !='\'' && szValue[nValueLen] != '"') { nValueLen++; }

    //while (nValueLen > 0 && (szValue[nValueLen-1]==' ' || szValue[nValueLen-1]=='\'' || szValue[nValueLen-1]=='"')) nValueLen--;

    char* value = (char*) malloc(nValueLen+2);
    strncpy(value, szValue, nValueLen);
    value[nValueLen] = '\0';

    return value;
}

int main(int argc, char *argv[])
{
    CMainWindow* m_appWindow = CMainWindow::getInstance();

    m_logPort = String("11000");
    for (int i=1; i<argc; ++i) {
        g_strCmdLine += String(argv[i]) + " ";
        if (strnicmp("-log",argv[i],4)==0) {
            char* port = parseToken(argv[i]);
            if (port) {
                    String strLogPort = port;
                    m_logPort = strLogPort;
                    free(port);
            }
        } else if (strnicmp("-http_proxy_url",argv[i],15)==0) {
            char *proxy = parseToken(argv[i]);
            if (proxy) {
                m_strHttpProxy = proxy;
                free(proxy);
            } else
                LOG(WARNING) + "invalid value for \"http_proxy_url\" cmd parameter";
        } else if (strnicmp("-approot",argv[i],8)==0) {
            char* path = parseToken(argv[i]);
            if (path) {
                int len = strlen(path);
                if (!(path[len-1]=='\\' || path[len-1]=='/')) {
                    path[len] = '/';
                    path[len+1]  = 0;
                }
                m_strRootPath = path;
                free(path);
            }
        } else if (strnicmp("-rhodespath",argv[i],11)==0) {
            char* path = parseToken(argv[i]);
            if (path) {
                m_strRhodesPath = path;
                free(path);
            }
        } else {
            String msg = String(argv[i]);
            LOG(WARNING) + "wrong cmd parameter: " + msg;
        }
    }

    // PreMessageLoop:
    rho_logconf_Init(m_strRootPath.c_str(), m_logPort.c_str());
#ifdef RHODES_EMULATOR
    RHOSIMCONF().setAppConfFilePath(CFilePath::join(m_strRootPath, RHO_EMULATOR_DIR"/rhosimconfig.txt").c_str());
    RHOSIMCONF().loadFromFile();
    if ( m_strRhodesPath.length() > 0 )
        RHOSIMCONF().setString("rhodes_path", m_strRhodesPath, false );
    RHOCONF().setString( "rhosim_platform", RHOSIMCONF().getString( "platform"), false);
#endif
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
#ifdef RHODES_EMULATOR
    if (RHOSIMCONF().getString("debug_host").length() > 0)
        RHOCONF().setString("debug_host", RHOSIMCONF().getString("debug_host"), false);
    if (RHOSIMCONF().getString("debug_port").length() > 0)
        RHOCONF().setString("debug_port", RHOSIMCONF().getString("debug_port"), false);
#endif
    rho::common::CRhodesApp::Create(m_strRootPath);

    // Create the main application window
#ifdef RHODES_EMULATOR
    m_appWindow->Initialize(convertToStringW(RHOSIMCONF().getString("app_name")).c_str());
#endif

    RHODESAPP().startApp();

    // Navigate to the "loading..." page
    m_appWindow->navigate(L"about:blank", -1);

    rho_clientregister_create("qt_client");

    // RunMessageLoop:
    m_appWindow->messageLoop();

    // stopping Rhodes application
    rho_ringtone_manager_stop();
    m_appWindow->DestroyUi();
    rho::common::CRhodesApp::Destroy();

    return 0;
}
