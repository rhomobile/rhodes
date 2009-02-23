/* webview.i */
%module WebView
%{
extern void webview_refresh();
#define refresh webview_refresh 
extern void webview_navigate(char* url);
#define navigate webview_navigate
%}

extern void refresh();
extern void navigate(char* url);