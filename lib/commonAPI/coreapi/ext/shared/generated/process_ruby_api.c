
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_mProcess;

VALUE rb_s_Process_runApplication(int argc, VALUE *argv);
VALUE rb_Process_waitForApplication(int argc, VALUE *argv, VALUE obj);
VALUE rb_Process_closeHandle(int argc, VALUE *argv, VALUE obj);
VALUE rb_Process_getProcessExitCode(int argc, VALUE *argv, VALUE obj);




VALUE getRuby_Process_Module(){ return rb_mProcess; }



void Init_RubyAPI_Process(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    
    tmpParent = rb_mParent;
    rb_mParent = rb_define_module_under(tmpParent, "System");
    

	rb_mProcess = rb_define_module_under(rb_mParent, "Process");

    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    //rb_undef_alloc_func(rb_mProcess);

    rb_define_module_function(rb_mProcess, "runApplication", rb_s_Process_runApplication, -1);
    rb_define_method(rb_mProcess, "waitForApplication", rb_Process_waitForApplication, -1);
    rb_define_method(rb_mProcess, "closeHandle", rb_Process_closeHandle, -1);
    rb_define_method(rb_mProcess, "getProcessExitCode", rb_Process_getProcessExitCode, -1);







//TODO: support module aliases
    rb_const_set(rb_mKernel, rb_intern("Process"), rb_mProcess );
}

