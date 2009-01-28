#include "ruby/ruby.h"

static VALUE mGeoLocation;

VALUE wrap_geo_latitude(int argc, VALUE *argv, VALUE self) {
  double result;
  VALUE vresult = Qnil;
  
  if ((argc < 0) || (argc > 0)) {
    rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc); goto fail;
  }
  result = (double)geo_latitude();
  vresult = rb_float_new((double)(result));
  return vresult;
fail:
  return Qnil;
}

VALUE wrap_geo_longitude(int argc, VALUE *argv, VALUE self) {
  double result;
  VALUE vresult = Qnil;
  
  if ((argc < 0) || (argc > 0)) {
    rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc); goto fail;
  }
  result = (double)geo_longitude();
  vresult = rb_float_new((double)(result));
  return vresult;
fail:
  return Qnil;
}

VALUE wrap_geo_known_position(int argc, VALUE *argv, VALUE self) {
  int result;
  VALUE vresult = Qnil;
  
  if ((argc < 0) || (argc > 0)) {
    rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc); goto fail;
  }
  result = (int)geo_known_position();
  vresult = LONG2NUM((int)(result));
  return vresult;
fail:
  return Qnil;
}

#ifdef __cplusplus
extern "C"
#endif
void Init_GeoLocation(void) {
  size_t i;
  
  mGeoLocation = rb_define_module("GeoLocation");
  
  rb_define_module_function(mGeoLocation, "latitude", wrap_geo_latitude, -1);
  rb_define_module_function(mGeoLocation, "longitude", wrap_geo_longitude, -1);
  rb_define_module_function(mGeoLocation, "known_position?", wrap_geo_known_position, -1);
}

