
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_mJavascriptVM;

VALUE rb_s_JavascriptVM_executeScript(int argc, VALUE *argv);




VALUE getRuby_JavascriptVM_Module(){ return rb_mJavascriptVM; }



void Init_RubyAPI_JavascriptVM(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    

	rb_mJavascriptVM = rb_define_class_under(rb_mParent, "JavascriptVM", rb_cObject);

    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    //rb_undef_alloc_func(rb_mJavascriptVM);

    rb_define_singleton_method(rb_mJavascriptVM, "executeScript", rb_s_JavascriptVM_executeScript, -1);








}

