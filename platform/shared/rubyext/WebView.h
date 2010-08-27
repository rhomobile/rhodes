#ifndef _RHOWEBVIEW_H_
#define _RHOWEBVIEW_H_

//#include "../ruby/include/ruby.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void rho_webview_refresh(int index);
void rho_webview_navigate(const char* url, int index);
void rho_webview_navigate_back();
const char* rho_webview_execute_js(const char* js, int index);
const char* rho_webview_current_location(int index);
int rho_webview_active_tab();
void rho_webview_set_menu_items(unsigned long valMenu);
void rho_webview_full_screen_mode(int enable);

#ifdef __cplusplus
};
#endif //__cplusplus

#endif //_RHOWEBVIEW_H_
