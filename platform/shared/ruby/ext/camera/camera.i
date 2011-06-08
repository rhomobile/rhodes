/* system.i */
%module Camera
%{
#include "ext/rho/rhoruby.h"

extern void take_picture(char* callback_url, rho_param *options_hash);
extern void choose_picture(char* callback_url);

extern VALUE get_camera_info(const char* camera_type);

%}

%typemap(default) (rho_param *options_hash) {
 $1 = NULL;
}

%typemap(in) (rho_param *options_hash) {
  $1 = rho_param_fromvalue($input);
}

%typemap(freearg) (rho_param *options_hash) {
  rho_param_free($1);
}


extern void take_picture(char* callback_url, rho_param *options_hash );
extern void choose_picture(char* callback_url);

extern VALUE get_camera_info(const char* camera_type);
