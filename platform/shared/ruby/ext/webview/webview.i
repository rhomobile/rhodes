/* webview.i */
%module WebView
%{
extern void webview_refresh();
#define refresh webview_refresh 
extern void webview_navigate(char* url);
#define navigate webview_navigate
extern char* webview_current_location();
#define current_location webview_current_location
extern char* execute_js(char* js);
#define execute_js webview_execute_js
extern void set_menu_items(VALUE argv);
#define set_menu_items webview_set_menu_items
%}

extern void refresh();
extern void navigate(char* url);
extern char* current_location();
extern char* execute_js(char* js);
extern void set_menu_items(VALUE argv);