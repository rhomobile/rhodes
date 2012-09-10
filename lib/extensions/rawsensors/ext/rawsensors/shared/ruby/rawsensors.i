/* rawsensors.i */
%module RawSensorsNative
%{
#include "ruby/ext/rho/rhoruby.h"

extern VALUE rawsensors_get_property(const char* property_name);
extern void rawsensors_set_property(const char* property_name, const char* value);
extern VALUE rawsensors_is_available(const char* sensor_name);
extern void rawsensors_getSensorData();

#define get_property rawsensors_get_property
#define set_property rawsensors_set_property
#define is_available rawsensors_is_available
#define getSensorData rawsensors_getSensorData


%}

extern VALUE get_property(const char* property_name);
extern void set_property(const char* property_name, const char* value);
extern VALUE is_available(const char* sensor_name);
extern void getSensorData();
