/* webview.i */
%module WebView
%{
extern void webview_refresh();
#define refresh webview_refresh 
%}

extern void refresh();
