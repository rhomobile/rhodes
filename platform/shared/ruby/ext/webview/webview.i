/* webview.i */
%module WebView
%{
extern void webview_refresh(int index);
#define refresh webview_refresh 
extern void webview_navigate(char* url, int index);
#define navigate webview_navigate
extern char* webview_current_location(int index);
#define current_location webview_current_location
extern char* webview_execute_js(char* js, int index);
#define execute_js webview_execute_js
extern void webview_set_menu_items(VALUE argv);
#define set_menu_items webview_set_menu_items
extern int webview_active_tab();
#define active_tab webview_active_tab
%}

%typemap(default) int index {
  $1 = 0;
}

extern void refresh(int index);
extern void navigate(char* url, int index);
extern char* current_location(int index);
extern char* execute_js(char* js, int index);
extern void set_menu_items(VALUE argv);
extern int active_tab();
