
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_cBarcode;

VALUE rb_enable(int argc, VALUE *argv, VALUE instance);
VALUE rb_start(VALUE instance);
VALUE rb_stop(VALUE instance);
VALUE rb_disable(VALUE instance);
VALUE rb_take(int argc, VALUE *argv, VALUE instance);
VALUE rb_getProperty(VALUE instance, VALUE obj);
VALUE rb_getProperties(VALUE instance, VALUE obj);
VALUE rb_getAllProperties(VALUE instance, VALUE obj);
VALUE rb_setProperty(int argc, VALUE *argv, VALUE instance);
VALUE rb_setProperties(VALUE instance, VALUE obj);
VALUE rb_clearProps(VALUE instance);
VALUE rb_s_enumerate(VALUE klass);
VALUE rb_s_default(VALUE klass);
VALUE rb_s_setDefault(VALUE klass, VALUE obj);


VALUE getRuby_Barcode_Module(){ return rb_cBarcode; }

void Init_RubyAPI_Barcode(void)
{
    rb_mParent = rb_define_module("Rho");
	rb_cBarcode = rb_define_class_under(rb_mParent, "Barcode", rb_cObject);
	
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_cBarcode);

    rb_define_method(rb_cBarcode, "enable", rb_enable, -1);
    rb_define_method(rb_cBarcode, "start", rb_start, 0);
    rb_define_method(rb_cBarcode, "stop", rb_stop, 0);
    rb_define_method(rb_cBarcode, "disable", rb_disable, 0);
    rb_define_method(rb_cBarcode, "take", rb_take, -1);
    rb_define_method(rb_cBarcode, "getProperty", rb_getProperty, 1);
    rb_define_method(rb_cBarcode, "getProperties", rb_getProperties, 1);
    rb_define_method(rb_cBarcode, "getAllProperties", rb_getAllProperties, 1);
    rb_define_method(rb_cBarcode, "setProperty", rb_setProperty, -1);
    rb_define_method(rb_cBarcode, "setProperties", rb_setProperties, 1);
    rb_define_method(rb_cBarcode, "clearProps", rb_clearProps, 0);
    rb_define_singleton_method(rb_cBarcode, "enumerate", rb_s_enumerate, 0);
    rb_define_singleton_method(rb_cBarcode, "default", rb_s_default, 0);
    rb_define_singleton_method(rb_cBarcode, "setDefault", rb_s_setDefault, 1);




}

