
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_cSystem;

VALUE rb_System_getProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getProperty(int argc, VALUE *argv);
VALUE rb_System_getProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getProperties(int argc, VALUE *argv);
VALUE rb_System_getAllProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getAllProperties(int argc, VALUE *argv);
VALUE rb_System_setProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setProperty(int argc, VALUE *argv);
VALUE rb_System_setProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setProperties(int argc, VALUE *argv);
VALUE rb_System_clearAllProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_clearAllProperties(int argc, VALUE *argv);



VALUE rb_System_s_default(VALUE klass);
VALUE rb_System_s_setDefault(VALUE klass, VALUE obj);


VALUE getRuby_System_Module(){ return rb_cSystem; }



void Init_RubyAPI_System(void)
{

    rb_mParent = rb_define_module("Rho");
	rb_cSystem = rb_define_class_under(rb_mParent, "System", rb_cObject);

    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_cSystem);

rb_define_method(rb_cSystem, "getProperty", rb_System_getProperty, -1);
rb_define_singleton_method(rb_cSystem, "getProperty", rb_s_System_def_getProperty, -1);
rb_define_method(rb_cSystem, "getProperties", rb_System_getProperties, -1);
rb_define_singleton_method(rb_cSystem, "getProperties", rb_s_System_def_getProperties, -1);
rb_define_method(rb_cSystem, "getAllProperties", rb_System_getAllProperties, -1);
rb_define_singleton_method(rb_cSystem, "getAllProperties", rb_s_System_def_getAllProperties, -1);
rb_define_method(rb_cSystem, "setProperty", rb_System_setProperty, -1);
rb_define_singleton_method(rb_cSystem, "setProperty", rb_s_System_def_setProperty, -1);
rb_define_method(rb_cSystem, "setProperties", rb_System_setProperties, -1);
rb_define_singleton_method(rb_cSystem, "setProperties", rb_s_System_def_setProperties, -1);
rb_define_method(rb_cSystem, "clearAllProperties", rb_System_clearAllProperties, -1);
rb_define_singleton_method(rb_cSystem, "clearAllProperties", rb_s_System_def_clearAllProperties, -1);



    rb_define_singleton_method(rb_cSystem, "default", rb_System_s_default, 0);
    rb_define_singleton_method(rb_cSystem, "setDefault", rb_System_s_setDefault, 1);



}

