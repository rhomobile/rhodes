
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_cBarcode1;

VALUE rb_Barcode1_enable(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_enable(int argc, VALUE *argv);
VALUE rb_Barcode1_start(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_start(int argc, VALUE *argv);
VALUE rb_Barcode1_stop(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_stop(int argc, VALUE *argv);
VALUE rb_Barcode1_disable(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_disable(int argc, VALUE *argv);
VALUE rb_Barcode1_take(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_take(int argc, VALUE *argv);
VALUE rb_Barcode1_getProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_getProperty(int argc, VALUE *argv);
VALUE rb_Barcode1_getProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_getProperties(int argc, VALUE *argv);
VALUE rb_Barcode1_getAllProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_getAllProperties(int argc, VALUE *argv);
VALUE rb_Barcode1_setProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_setProperty(int argc, VALUE *argv);
VALUE rb_Barcode1_setProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_setProperties(int argc, VALUE *argv);
VALUE rb_Barcode1_clearProps(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Barcode1_def_clearProps(int argc, VALUE *argv);
VALUE rb_s_Barcode1_enumerate(int argc, VALUE *argv);



VALUE rb_Barcode1_s_default(VALUE klass);
VALUE rb_Barcode1_s_setDefault(VALUE klass, VALUE obj);


VALUE getRuby_Barcode1_Module(){ return rb_cBarcode1; }



void Init_RubyAPI_Barcode1(void)
{
    rb_mParent = rb_define_module("Rho");
	rb_cBarcode1 = rb_define_class_under(rb_mParent, "Barcode1", rb_cObject);
	
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_cBarcode1);

rb_define_method(rb_cBarcode1, "enable", rb_Barcode1_enable, -1);
rb_define_singleton_method(rb_cBarcode1, "enable", rb_s_Barcode1_def_enable, -1);
rb_define_method(rb_cBarcode1, "start", rb_Barcode1_start, -1);
rb_define_singleton_method(rb_cBarcode1, "start", rb_s_Barcode1_def_start, -1);
rb_define_method(rb_cBarcode1, "stop", rb_Barcode1_stop, -1);
rb_define_singleton_method(rb_cBarcode1, "stop", rb_s_Barcode1_def_stop, -1);
rb_define_method(rb_cBarcode1, "disable", rb_Barcode1_disable, -1);
rb_define_singleton_method(rb_cBarcode1, "disable", rb_s_Barcode1_def_disable, -1);
rb_define_method(rb_cBarcode1, "take", rb_Barcode1_take, -1);
rb_define_singleton_method(rb_cBarcode1, "take", rb_s_Barcode1_def_take, -1);
rb_define_method(rb_cBarcode1, "getProperty", rb_Barcode1_getProperty, -1);
rb_define_singleton_method(rb_cBarcode1, "getProperty", rb_s_Barcode1_def_getProperty, -1);
rb_define_method(rb_cBarcode1, "getProperties", rb_Barcode1_getProperties, -1);
rb_define_singleton_method(rb_cBarcode1, "getProperties", rb_s_Barcode1_def_getProperties, -1);
rb_define_method(rb_cBarcode1, "getAllProperties", rb_Barcode1_getAllProperties, -1);
rb_define_singleton_method(rb_cBarcode1, "getAllProperties", rb_s_Barcode1_def_getAllProperties, -1);
rb_define_method(rb_cBarcode1, "setProperty", rb_Barcode1_setProperty, -1);
rb_define_singleton_method(rb_cBarcode1, "setProperty", rb_s_Barcode1_def_setProperty, -1);
rb_define_method(rb_cBarcode1, "setProperties", rb_Barcode1_setProperties, -1);
rb_define_singleton_method(rb_cBarcode1, "setProperties", rb_s_Barcode1_def_setProperties, -1);
rb_define_method(rb_cBarcode1, "clearProps", rb_Barcode1_clearProps, -1);
rb_define_singleton_method(rb_cBarcode1, "clearProps", rb_s_Barcode1_def_clearProps, -1);
rb_define_singleton_method(rb_cBarcode1, "enumerate", rb_s_Barcode1_enumerate, -1);



    rb_define_singleton_method(rb_cBarcode1, "default", rb_Barcode1_s_default, 0);
    rb_define_singleton_method(rb_cBarcode1, "setDefault", rb_Barcode1_s_setDefault, 1);



}

