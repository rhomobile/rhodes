/* mapview.i */
%module MapView
%{
#include "ext/rho/rhoruby.h"
extern void mapview_create(rho_param *p);
#define create mapview_create
extern void mapview_close();
#define close mapview_close
extern VALUE mapview_state_started();
#define state_started mapview_state_started
extern double mapview_state_center_lat();
#define state_center_lat mapview_state_center_lat
extern double mapview_state_center_lon();
#define state_center_lon mapview_state_center_lon
%}

%typemap(in) (rho_param *p) {
    $1 = rho_param_fromvalue($input);
}

%typemap(freearg) (rho_param *p) {
    rho_param_free($1);
}

extern void create(rho_param *p);
extern void close();
extern VALUE state_started();
extern double state_center_lat();
extern double state_center_lon();

