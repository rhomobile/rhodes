
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_mNetworkAccess;

VALUE rb_s_NetworkAccess_cancel(int argc, VALUE *argv);
VALUE rb_s_NetworkAccess_downloadFile(int argc, VALUE *argv);
VALUE rb_s_NetworkAccess_get(int argc, VALUE *argv);
VALUE rb_s_NetworkAccess_post(int argc, VALUE *argv);
VALUE rb_s_NetworkAccess_uploadFile(int argc, VALUE *argv);




VALUE getRuby_NetworkAccess_Module(){ return rb_mNetworkAccess; }



void Init_RubyAPI_NetworkAccess(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    

	rb_mNetworkAccess = rb_define_class_under(rb_mParent, "NetworkAccess", rb_cObject);

    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    //rb_undef_alloc_func(rb_mNetworkAccess);

    rb_define_singleton_method(rb_mNetworkAccess, "cancel", rb_s_NetworkAccess_cancel, -1);
    rb_define_singleton_method(rb_mNetworkAccess, "downloadFile", rb_s_NetworkAccess_downloadFile, -1);
    rb_define_singleton_method(rb_mNetworkAccess, "get", rb_s_NetworkAccess_get, -1);
    rb_define_singleton_method(rb_mNetworkAccess, "post", rb_s_NetworkAccess_post, -1);
    rb_define_singleton_method(rb_mNetworkAccess, "uploadFile", rb_s_NetworkAccess_uploadFile, -1);





    rb_const_set(rb_mNetworkAccess, rb_intern("AUTH_BASIC"), rb_str_new2("basic") );



}

