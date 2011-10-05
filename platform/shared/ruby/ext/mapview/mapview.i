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
extern void mapview_set_file_caching_enable(int enable);
#define set_file_caching_enable mapview_set_file_caching_enable
extern int mapview_preload_map_tiles(rho_param* p, const char* callback);
#define preload_map_tiles mapview_preload_map_tiles
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
extern void set_file_caching_enable(int enable);
extern int preload_map_tiles(rho_param* p, const char* callback);

