
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_cBarcode;

VALUE rb_Barcode_enable(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_enable(int argc, VALUE *argv);
VALUE rb_Barcode_start(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_start(int argc, VALUE *argv);
VALUE rb_Barcode_stop(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_stop(int argc, VALUE *argv);
VALUE rb_Barcode_disable(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_disable(int argc, VALUE *argv);
VALUE rb_Barcode_take(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_take(int argc, VALUE *argv);
VALUE rb_Barcode_getProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_getProperty(int argc, VALUE *argv);
VALUE rb_Barcode_getProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_getProperties(int argc, VALUE *argv);
VALUE rb_Barcode_getAllProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_getAllProperties(int argc, VALUE *argv);
VALUE rb_Barcode_setProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_setProperty(int argc, VALUE *argv);
VALUE rb_Barcode_setProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_setProperties(int argc, VALUE *argv);
VALUE rb_Barcode_clearProps(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode_def_clearProps(int argc, VALUE *argv);
VALUE rb_s_Barcode_enumerate(int argc, VALUE *argv);



VALUE rb_Barcode_s_default(VALUE klass);
VALUE rb_Barcode_s_setDefault(VALUE klass, VALUE obj);


VALUE getRuby_Barcode_Module(){ return rb_cBarcode; }



void Init_RubyAPI_Barcode(void)
{
    rb_mParent = rb_define_module("Rho");
	rb_cBarcode = rb_define_class_under(rb_mParent, "Barcode", rb_cObject);
	
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_cBarcode);

rb_define_method(rb_cBarcode, "enable", rb_Barcode_enable, -1);
rb_define_singleton_method(rb_cBarcode, "enable", rb_s_Barcode_def_enable, -1);
rb_define_method(rb_cBarcode, "start", rb_Barcode_start, -1);
rb_define_singleton_method(rb_cBarcode, "start", rb_s_Barcode_def_start, -1);
rb_define_method(rb_cBarcode, "stop", rb_Barcode_stop, -1);
rb_define_singleton_method(rb_cBarcode, "stop", rb_s_Barcode_def_stop, -1);
rb_define_method(rb_cBarcode, "disable", rb_Barcode_disable, -1);
rb_define_singleton_method(rb_cBarcode, "disable", rb_s_Barcode_def_disable, -1);
rb_define_method(rb_cBarcode, "take", rb_Barcode_take, -1);
rb_define_singleton_method(rb_cBarcode, "take", rb_s_Barcode_def_take, -1);
rb_define_method(rb_cBarcode, "getProperty", rb_Barcode_getProperty, -1);
rb_define_singleton_method(rb_cBarcode, "getProperty", rb_s_Barcode_def_getProperty, -1);
rb_define_method(rb_cBarcode, "getProperties", rb_Barcode_getProperties, -1);
rb_define_singleton_method(rb_cBarcode, "getProperties", rb_s_Barcode_def_getProperties, -1);
rb_define_method(rb_cBarcode, "getAllProperties", rb_Barcode_getAllProperties, -1);
rb_define_singleton_method(rb_cBarcode, "getAllProperties", rb_s_Barcode_def_getAllProperties, -1);
rb_define_method(rb_cBarcode, "setProperty", rb_Barcode_setProperty, -1);
rb_define_singleton_method(rb_cBarcode, "setProperty", rb_s_Barcode_def_setProperty, -1);
rb_define_method(rb_cBarcode, "setProperties", rb_Barcode_setProperties, -1);
rb_define_singleton_method(rb_cBarcode, "setProperties", rb_s_Barcode_def_setProperties, -1);
rb_define_method(rb_cBarcode, "clearProps", rb_Barcode_clearProps, -1);
rb_define_singleton_method(rb_cBarcode, "clearProps", rb_s_Barcode_def_clearProps, -1);
rb_define_singleton_method(rb_cBarcode, "enumerate", rb_s_Barcode_enumerate, -1);



    rb_define_singleton_method(rb_cBarcode, "default", rb_Barcode_s_default, 0);
    rb_define_singleton_method(rb_cBarcode, "setDefault", rb_Barcode_s_setDefault, 1);



}

