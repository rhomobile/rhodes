#include <time.h>
#include "ext/rho/rhoruby.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"

using namespace std;
using namespace rho;

extern "C" {

void rho_conf_show_log()
{
    //TODO: ::PostMessage(getMainWnd(),WM_COMMAND,IDM_LOG,0);
}

void rho_net_impl_network_indicator(int active)
{
    //TODO: rho_net_impl_network_indicator
}

void rho_map_location(char* query)
{
    //TODO: rho_map_location
}

void rho_appmanager_load( void* httpContext, char* szQuery)
{
    //TODO: rho_appmanager_load
}

int rho_net_ping_network(const char* szHost)
{
    //TODO: rho_net_ping_network
    return 1;
}

void parseHttpProxyURI(const rho::String &http_proxy)
{
    // http://<login>:<passwod>@<host>:<port>
    const char *default_port = "8080";

    if (http_proxy.length() < 8) {
        LOG(ERROR) + "invalid http proxy url";
        return;
    }

    int index = http_proxy.find("http://", 0, 7);
    if (index == string::npos) {
        LOG(ERROR) + "http proxy url should starts with \"http://\"";
        return;
    }
    index = 7;

    enum {
        ST_START,
        ST_LOGIN,
        ST_PASSWORD,
        ST_HOST,
        ST_PORT,
        ST_FINISH
    };

    String token, login, password, host, port;
    char c, state = ST_START, prev_state = state;
    int length = http_proxy.length();

    for (int i = index; i < length; i++) {
        c = http_proxy[i];

        switch (state) {
        case ST_START:
            if (c == '@') {
                prev_state = state; state = ST_HOST;
            } else if (c == ':') {
                prev_state = state; state = ST_PASSWORD;
            } else {
                token +=c;
                state = ST_HOST;
            }
            break;
        case ST_HOST:
            if (c == ':') {
                host = token; token.clear();            
                prev_state = state; state = ST_PORT;
            } else if (c == '@') {
                host = token; token.clear();        
                prev_state = state;    state = ST_LOGIN;                    
            } else {
                token += c;
                if (i == (length - 1)) {
                    host = token; token.clear();                                
                }
            }
            break;
        case ST_PORT:
            if (c == '@') {
                port = token; token.clear();            
                prev_state = state; state = ST_LOGIN;
            } else {
                token += c;
                if (i == (length - 1)) {
                    port = token; token.clear();
                }
            }
            break;
        case ST_LOGIN:
            if (prev_state == ST_PORT || prev_state == ST_HOST) {
                login    = host; host.clear();
                password = port; port.clear();
                prev_state = state; state = ST_HOST;
                token += c;
            } else {
                token += c;
                if (i == (length - 1)) {
                    login = token; token.clear();                                
                }
            }
            break;
        case ST_PASSWORD:
            if (c == '@') {
                password = token; token.clear();            
                prev_state = state; state = ST_HOST;
            } else {
                token += c;
                if (i == (length - 1)) {
                    password = token; token.clear();                                
                }
            }
            break;
        default:
            ;
        }
    }

    LOG(INFO) + "Setting up HTTP proxy:";
    LOG(INFO) + "URI: " + http_proxy;
    LOG(INFO) + "HTTP proxy login    = " + login;
    LOG(INFO) + "HTTP proxy password = " + password;
    LOG(INFO) + "HTTP proxy host     = " + host;
    LOG(INFO) + "HTTP proxy port     = " + port;
    
    if (host.length()) {
        RHOCONF().setString ("http_proxy_host", host, false);

        if (port.length()){
            RHOCONF().setString ("http_proxy_port", port, false);
        } else {
            LOG(WARNING) + "there is no proxy port defined";
        }

        if (login.length())
            RHOCONF().setString ("http_proxy_login", login, false);
        if (password.length())
            RHOCONF().setString ("http_proxy_password", password, false);
    } else {
        LOG(ERROR) + "empty host name in HTTP-proxy URL";
    }
}

} //extern "C"
