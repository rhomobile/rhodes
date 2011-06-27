/* alert.i */
%module Alert
%{
#include "ext/rho/rhoruby.h"
extern void alert_show_popup(rho_param *p);
#define show_popup alert_show_popup 
extern void alert_hide_popup();
#define hide_popup alert_hide_popup
extern void alert_vibrate(int vibrate_duration_ms);
#define vibrate alert_vibrate
extern void alert_play_file(char* file_name, char* media_type);
#define play_file alert_play_file 

extern void show_status(char* szTitle, char* szText, char* szHideLabel);
#define show_status rho_alert_show_status

%}

%typemap(in) (rho_param *p) {
    $1 = rho_param_fromvalue($input);
}

%typemap(freearg) (rho_param *p) {
    rho_param_free($1);
}

%typemap(default) int vibrate_duration_ms {
  $1 = 2500;
}

%typemap(default) char* media_type {
  $1 = 0;
}

extern void show_popup(rho_param *p);
extern void hide_popup();
extern void vibrate(int vibrate_duration_ms);
extern void play_file(char* file_name, char* media_type);
extern void show_status(char* szTitle, char* szText, char* szHideLabel);