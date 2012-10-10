/* audiocapture.i */
%module AudioCaptureNative
%{
#include "ruby/ext/rho/rhoruby.h"

extern VALUE audiocapture_get_property(const char* property_name);
extern void audiocapture_set_property(const char* property_name, const char* value);

extern void audiocapture_start();
extern void audiocapture_stop();
extern void audiocapture_cancel();

#define get_property audiocapture_get_property
#define set_property audiocapture_set_property

#define start audiocapture_start
#define stop audiocapture_stop
#define cancel audiocapture_cancel


%}


extern VALUE get_property(const char* property_name);
extern void set_property(const char* property_name, const char* value);

extern void start();
extern void stop();
extern void cancel();
