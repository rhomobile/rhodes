
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_mSimpleOnlyStaticModule;

VALUE rb_s_SimpleOnlyStaticModule_calcSumm(int argc, VALUE *argv);
VALUE rb_s_SimpleOnlyStaticModule_joinStrings(int argc, VALUE *argv);
VALUE rb_s_SimpleOnlyStaticModule_getPlatform(int argc, VALUE *argv);
VALUE rb_s_SimpleOnlyStaticModule_showAlertFromUIThread(int argc, VALUE *argv);




VALUE getRuby_SimpleOnlyStaticModule_Module(){ return rb_mSimpleOnlyStaticModule; }



void Init_RubyAPI_SimpleOnlyStaticModule(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    
    tmpParent = rb_mParent;
    rb_mParent = rb_define_module_under(tmpParent, "Examples");
    

	rb_mSimpleOnlyStaticModule = rb_define_class_under(rb_mParent, "SimpleOnlyStaticModule", rb_cObject);

    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    //rb_undef_alloc_func(rb_mSimpleOnlyStaticModule);

    rb_define_singleton_method(rb_mSimpleOnlyStaticModule, "calcSumm", rb_s_SimpleOnlyStaticModule_calcSumm, -1);
    rb_define_singleton_method(rb_mSimpleOnlyStaticModule, "joinStrings", rb_s_SimpleOnlyStaticModule_joinStrings, -1);
    rb_define_singleton_method(rb_mSimpleOnlyStaticModule, "getPlatform", rb_s_SimpleOnlyStaticModule_getPlatform, -1);
    rb_define_singleton_method(rb_mSimpleOnlyStaticModule, "showAlertFromUIThread", rb_s_SimpleOnlyStaticModule_showAlertFromUIThread, -1);







//TODO: support module aliases
    rb_const_set(rb_mKernel, rb_intern("SimpleOnlyStaticModule"), rb_mSimpleOnlyStaticModule );
}

