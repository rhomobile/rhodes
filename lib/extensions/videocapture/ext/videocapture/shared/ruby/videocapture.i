/* videocapture.i */
%module VideoCaptureNative
%{
#include "ruby/ext/rho/rhoruby.h"

extern VALUE videocapture_get_property(const char* property_name);
extern void videocapture_set_property(const char* property_name, const char* value);

extern void videocapture_start();
extern void videocapture_stop();
extern void videocapture_cancel();

#define get_property videocapture_get_property
#define set_property videocapture_set_property

#define start videocapture_start
#define stop videocapture_stop
#define cancel videocapture_cancel


%}


extern VALUE get_property(const char* property_name);
extern void set_property(const char* property_name, const char* value);

extern void start();
extern void stop();
extern void cancel();
