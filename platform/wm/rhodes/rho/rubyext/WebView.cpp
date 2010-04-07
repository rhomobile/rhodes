#include "stdafx.h"

#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"
#include "WebView.h"

/**
 * C interface
 */

 //Sync hook to refresh the web view
void perform_webview_refresh() {
	webview_refresh(0);
}

void webview_set_menu_items(VALUE valMenu) 
{
    rho_rhodesapp_setViewMenu(valMenu); 
}

int webview_active_tab() {
	return 0;
}

char* webview_current_location(int index) {
    return const_cast<char*>(RHODESAPP().getCurrentUrl(index).c_str());
}
