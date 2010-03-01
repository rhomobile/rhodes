/* geolocation.i */
%module GeoLocation
%{
/* Put header files here or function declarations like below */

	#define latitude rho_geo_latitude
	extern double rho_geo_latitude();

	#define longitude rho_geo_longitude
	extern double rho_geo_longitude();

	#define known_position rho_geo_known_position
	extern int rho_geo_known_position();

	#define set_view_notification rho_geo_set_view_notification
    extern void rho_geo_set_view_notification( const char *url, char* params, int timeout_sec);

	#define set_notification rho_geo_set_notification
    extern void rho_geo_set_notification( const char *url, char* params, int timeout_sec);

%}

%predicate known_position();
%typemap(default) int timeout_sec {
 $1 = 0;
}
%typemap(default) char* params {
 $1 = 0;
}

extern double latitude();
extern double longitude();
extern int    known_position();
extern void set_view_notification( const char *url, char* params, int timeout_sec);
extern void set_notification( const char *url, char* params, int timeout_sec);
