/*
 * webview.c
 *
 *  Created on: Mar 12, 2009
 *      Author: Anton
 */

#include "rhodesAppView.h"

extern "C" {
void webview_refresh() {
	if ( g_RhodesAppView )
		g_RhodesAppView->Refresh();
}

void webview_navigate(char* url){
	if ( g_RhodesAppView )
		g_RhodesAppView->LoadUrl(url);
}

char* webview_current_location() {
	if ( g_RhodesAppView )
		return g_RhodesAppView->GetCurrentPageUrl();
	
	return NULL;
}

char* get_current_location() {
	return webview_current_location();
}

}
