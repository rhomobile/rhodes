/* nativeviewmanager.i */
%module RhodesNativeViewManager
%{
#include "ext/rho/rhoruby.h"

extern int rho_native_view_manager_create_native_view(const char* viewtype, int tab_index, VALUE params);
extern void rho_native_view_manager_navigate_native_view(int native_view_id, const char* url);
extern void rho_native_view_manager_destroy_native_view(int native_view_id);
 

#define create_native_view rho_native_view_manager_create_native_view
#define navigate_native_view rho_native_view_manager_navigate_native_view
#define destroy_native_view rho_native_view_manager_destroy_native_view
%}

extern int create_native_view(const char* viewtype, int tab_index, VALUE params);
extern void navigate_native_view(int native_view_id, const char* url);
extern void destroy_native_view(int native_view_id);
