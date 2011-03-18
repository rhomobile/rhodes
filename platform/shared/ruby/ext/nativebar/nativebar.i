/* nativebar.i */
%module NativeBar
%{
#include "ext/rho/rhoruby.h"

extern void create_nativebar(int bar_type, rho_param *p);
#define create create_nativebar
extern void remove_nativebar();
#define remove remove_nativebar
extern void nativebar_switch_tab(int index);
#define switch_tab nativebar_switch_tab
extern VALUE nativebar_started();
#define started nativebar_started

extern void create_native_toolbar(int bar_type, rho_param *p);
extern void remove_native_toolbar();
extern void create_native_tabbar(int bar_type, rho_param *p);
extern void remove_native_tabbar();
extern void native_tabbar_switch_tab(int index);
extern void native_tabbar_set_tab_badge(int index,char *val);

extern int native_tabbar_get_current_tab();
%}

%typemap(in) (rho_param *p) {
  $1 = rho_param_fromvalue($input);
}

%typemap(freearg) (rho_param *p) {
  rho_param_free($1);
}

extern void create(int bar_type, rho_param *p);
extern void remove();
extern void switch_tab(int index);
extern VALUE started();


extern void create_native_toolbar(int bar_type, rho_param *p);
extern void remove_native_toolbar();
extern void create_native_tabbar(int bar_type, rho_param *p);
extern void remove_native_tabbar();
extern void native_tabbar_switch_tab(int index);
extern void native_tabbar_set_tab_badge(int index,char *val);
extern int native_tabbar_get_current_tab();
