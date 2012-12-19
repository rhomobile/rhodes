
#include "ruby.h"

static VALUE rb_mRho;
static VALUE rb_cBarcode1;

VALUE rb_barcode1_s_enumerate(VALUE klass);
VALUE rb_barcode1_s_getprops(int argc, VALUE *argv);
VALUE rb_barcode1_getprops(int argc, VALUE *argv, VALUE obj);
VALUE rb_barcode1_s_default(VALUE klass);
VALUE rb_barcode1_s_set_default(VALUE klass, VALUE obj);

void Init_RubyAPI_Barcode1(void)
{
    rb_mRho = rb_define_module("Rho");
	rb_cBarcode1 = rb_define_class_under(rb_mRho, "Barcode1", rb_cObject);
	
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_cBarcode1);

    //Class fabric
    rb_define_singleton_method(rb_cBarcode1, "enumerate", rb_barcode1_s_enumerate, 0);

    rb_define_singleton_method(rb_cBarcode1, "default", rb_barcode1_s_default, 0);
    rb_define_singleton_method(rb_cBarcode1, "set_default", rb_barcode1_s_set_default, 1);

    rb_define_singleton_method(rb_cBarcode1, "getProps", rb_barcode1_s_getprops, -1);
    rb_define_method(rb_cBarcode1, "getProps", rb_barcode1_getprops, -1);
}

