/* webview.i */
%module WebView
%{
extern void rho_webview_refresh(int tab_index);
#define refresh rho_webview_refresh 
extern void rho_webview_navigate(const char* url, int tab_index);
#define navigate rho_webview_navigate
extern char* rho_webview_current_location(int tab_index);
#define current_location rho_webview_current_location
extern char* rho_webview_execute_js(const char* js, int tab_index);
#define execute_js rho_webview_execute_js
extern void rho_webview_set_menu_items(VALUE argv);
#define set_menu_items rho_webview_set_menu_items
extern int rho_webview_active_tab();
#define active_tab rho_webview_active_tab

extern void rho_webview_full_screen_mode(int enable);
#define full_screen_mode rho_webview_full_screen_mode

extern void rho_webview_set_cookie(const char* url, const char* cookie);
#define set_cookie rho_webview_set_cookie

extern void rho_webview_navigate_back();
#define navigate_back rho_webview_navigate_back

extern void rho_webview_save(const char* format, const char* path, int tab_index);
#define save rho_webview_save

%}

%typemap(default) int tab_index {
  $1 = -1;
}


extern void refresh(int tab_index);
extern void navigate(const char* url, int tab_index);
extern const char* current_location(int tab_index);
extern const char* execute_js(const char* js, int tab_index);
extern void set_menu_items(VALUE argv);
extern int active_tab();
extern void full_screen_mode(int enable);
extern void set_cookie(const char* url, const char* cookie);
extern void navigate_back();
extern void save(const char* format, const char* path, int tab_index);
