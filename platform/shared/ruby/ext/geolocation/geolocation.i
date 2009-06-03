/* geolocation.i */
%module GeoLocation
%{
/* Put header files here or function declarations like below */
extern double latitude();
extern double longitude();
%}

extern double _geo_latitude();
extern double _geo_longitude();
