/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "common/RhoPort.h"
#include "ext/rho/rhoruby.h"
#include "rubyext/WebView.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#include "MainWindowImpl.h"

using namespace std;
using namespace rho;

extern "C" {

void rho_conf_show_log()
{
    CMainWindow::getInstance()->logCommand();
}

void rho_net_impl_network_indicator(int active)
{
    //TODO: rho_net_impl_network_indicator
    RAWLOGC_INFO("RhodesImpl", "rho_net_impl_network_indicator() has no implementation in RhoSimulator.");
}

void rho_map_location(char* query)
{
    String url = "http://maps.google.com/?";
    url += query;
    rho_webview_navigate(url.c_str(), 0);
}

void rho_appmanager_load( void* httpContext, char* szQuery)
{
    //TODO: rho_appmanager_load
    RAWLOGC_INFO("RhodesImpl", "rho_appmanager_load() has no implementation in RhoSimulator.");
}

int rho_net_ping_network(const char* szHost)
{
    //TODO: rho_net_ping_network
    RAWLOGC_INFO("RhodesImpl", "rho_net_ping_network() has no implementation in RhoSimulator.");
    return 1;
}

void parseHttpProxyURI(const rho::String &http_proxy)
{
    // http://<login>:<passwod>@<host>:<port>
    const char *default_port = "8080";

    if (http_proxy.length() < 8) {
        RAWLOGC_ERROR("RhodesImpl", "invalid http proxy url");
        return;
    }

    int index = http_proxy.find("http://", 0, 7);
    if (index == string::npos) {
        RAWLOGC_ERROR("RhodesImpl", "http proxy url should starts with \"http://\"");
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

    RAWLOGC_INFO("RhodesImpl", "Setting up HTTP proxy:");
    RAWLOGC_INFO1("RhodesImpl", "URI: %s", http_proxy.c_str());
    RAWLOGC_INFO1("RhodesImpl", "HTTP proxy login    = %s", login.c_str());
    RAWLOGC_INFO1("RhodesImpl", "HTTP proxy password = %s", password.c_str());
    RAWLOGC_INFO1("RhodesImpl", "HTTP proxy host     = %s", host.c_str());
    RAWLOGC_INFO1("RhodesImpl", "HTTP proxy port     = %s", port.c_str());

    if (host.length()) {
        RHOCONF().setString ("http_proxy_host", host, false);

        if (port.length()){
            RHOCONF().setString ("http_proxy_port", port, false);
        } else {
            RAWLOGC_INFO("RhodesImpl", "there is no proxy port defined");
        }

        if (login.length())
            RHOCONF().setString ("http_proxy_login", login, false);
        if (password.length())
            RHOCONF().setString ("http_proxy_password", password, false);
    } else {
        RAWLOGC_ERROR("RhodesImpl", "empty host name in HTTP-proxy URL");
    }
}

} //extern "C"
