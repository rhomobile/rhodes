/* geolocation.i */
%module GeoLocation
%{
#include "ext/rho/rhoruby.h"
/* Put header files here or function declarations like below */

#define latitude rho_geo_latitude
extern double rho_geo_latitude();

#define longitude rho_geo_longitude
extern double rho_geo_longitude();

#define accuracy rho_geo_accuracy
extern float rho_geo_accuracy();

#define known_position rho_geo_known_position
extern int rho_geo_known_position();

#define set_view_notification rho_geo_set_view_notification
extern void rho_geo_set_view_notification( const char *url, char* params, int timeout_sec);

#define set_notification rho_geo_set_notification
extern void rho_geo_set_notification( const char *url, char* params, int timeout_sec);

#define haversine_distance rho_geo_haversine_distance
extern double rho_geo_haversine_distance(double lat1, double lon1, double lat2, double lon2);

#define turnoff rho_geoimpl_turngpsoff
extern void rho_geoimpl_turngpsoff();

#define do_geocoding rho_geoimpl_do_geocoding
extern void rho_geoimpl_do_geocoding(rho_param* p, const char* callback, int callback_tag);

%}

%predicate known_position();
%typemap(default) int timeout_sec {
 $1 = -1;
}
%typemap(default) char* params {
 $1 = 0;
}
%typemap(default) int callback_tag {
 $1 = 0;
}

%typemap(in) (rho_param *p) {
    $1 = rho_param_fromvalue($input);
}

%typemap(freearg) (rho_param *p) {
    rho_param_free($1);
}

extern double latitude();
extern double longitude();
extern float  accuracy();
extern int    known_position();
extern void   set_view_notification( const char *url, char* params, int timeout_sec);
extern void   set_notification( const char *url, char* params, int timeout_sec);
extern double haversine_distance(double lat1, double lon1, double lat2, double lon2);
extern void   turnoff();
extern void do_geocoding(rho_param* p, const char* callback, int callback_tag);

