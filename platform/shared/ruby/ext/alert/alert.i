/* alert.i */
%module Alert
%{
#include "ext/rho/rhoruby.h"
extern void alert_show_popup(rho_param *p);
#define show_popup alert_show_popup 
extern void alert_hide_popup();
#define hide_popup alert_hide_popup
extern void alert_vibrate(void*);
#define vibrate alert_vibrate
extern void alert_play_file(char* file_name, ...);
#define play_file alert_play_file 
%}

%typemap(in) (rho_param *p) {
    $1 = rho_param_fromvalue($input);
}

%typemap(freearg) (rho_param *p) {
    rho_param_free($1);
}

extern void show_popup(rho_param *p);
extern void hide_popup();
extern void vibrate(...);
extern void play_file(char* file_name, ...);
