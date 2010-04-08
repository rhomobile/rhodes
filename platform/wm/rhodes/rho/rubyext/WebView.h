#ifndef __WEB_VIEW__H__
#define __WEB_VIEW__H__

extern "C" void webview_refresh(int index);
extern "C" void perform_webview_refresh();
extern "C" void webview_navigate(char* url, int index);
extern "C" char* webview_execute_js(char* js, int index);
extern "C" void webview_set_menu_items(VALUE valMenu);
extern "C" int webview_active_tab();
extern "C" char* webview_current_location(int index);

#endif //__WEB_VIEW__H__
