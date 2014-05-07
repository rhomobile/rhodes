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
#include "../RhoSimulator.h"

using namespace std;
using namespace rho;

extern "C" {

void rho_conf_show_log()
{
    CMainWindow::getInstance()->logCommand();
}

void rho_title_change(const int, const char* strTitle)
{
    CMainWindow::getInstance()->setTitle(strTitle);
}

void rho_net_impl_network_indicator(int active)
{
    //TODO: rho_net_impl_network_indicator
    RAWLOGC_INFO("RhodesImpl", "rho_net_impl_network_indicator() has no implementation " RHO_PLATFORM_NAME_AT ".");
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
    RAWLOGC_INFO("RhodesImpl", "rho_appmanager_load() has no implementation " RHO_PLATFORM_NAME_AT ".");
}

int rho_net_ping_network(const char* szHost)
{
    //TODO: rho_net_ping_network
    RAWLOGC_INFO("RhodesImpl", "rho_net_ping_network() has no implementation " RHO_PLATFORM_NAME_AT ".");
    return 1;
}

void rho_qt_unset_window_proxy()
{
    CMainWindow* m_appWindow = CMainWindow::getInstance();
    if (m_appWindow)
        m_appWindow->unsetProxyCommand();
}

void rho_qt_set_window_proxy(const char* host, const char* port, const char* login, const char* password)
{
    CMainWindow* m_appWindow = CMainWindow::getInstance();
    if (m_appWindow)
        m_appWindow->setProxyCommand(host, port, login, password);
}

} //extern "C"
