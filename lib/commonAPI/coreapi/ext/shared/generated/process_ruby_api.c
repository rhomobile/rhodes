
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_cProcess;

VALUE rb_s_Process_runApplication(int argc, VALUE *argv);
VALUE rb_Process_waitForApplication(int argc, VALUE *argv, VALUE obj);
VALUE rb_Process_closeHandle(int argc, VALUE *argv, VALUE obj);
VALUE rb_Process_getProcessExitCode(int argc, VALUE *argv, VALUE obj);




VALUE getRuby_Process_Module(){ return rb_cProcess; }



void Init_RubyAPI_Process(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    
    tmpParent = rb_mParent;
    rb_mParent = rb_define_module_under(tmpParent, "System");
    

	rb_cProcess = rb_define_class_under(rb_mParent, "Process", rb_cObject);

    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_cProcess);

rb_define_singleton_method(rb_cProcess, "runApplication", rb_s_Process_runApplication, -1);
rb_define_method(rb_cProcess, "waitForApplication", rb_Process_waitForApplication, -1);
rb_define_method(rb_cProcess, "closeHandle", rb_Process_closeHandle, -1);
rb_define_method(rb_cProcess, "getProcessExitCode", rb_Process_getProcessExitCode, -1);





}

