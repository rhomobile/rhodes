
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_mNetworkAccess;

VALUE rb_s_NetworkAccess_cancel(int argc, VALUE *argv);
VALUE rb_s_NetworkAccess_downloadFile(int argc, VALUE *argv);
VALUE rb_s_NetworkAccess_get(int argc, VALUE *argv);
VALUE rb_s_NetworkAccess_post(int argc, VALUE *argv);
VALUE rb_s_NetworkAccess_uploadFile(int argc, VALUE *argv);




VALUE getRuby_NetworkAccess_Module(){ return rb_mNetworkAccess; }



static void _free_class_object(void *p)
{
    ruby_xfree(p);
}

static VALUE _allocate_class_object(VALUE klass)
{
    VALUE valObj = 0;
    char ** ppString = NULL;
    void* pData = ALLOC(void*);
    memset( pData, 0, sizeof(pData) );
    
	valObj = Data_Wrap_Struct(klass, 0, _free_class_object, pData);

    Data_Get_Struct(valObj, void *, (void**)ppString);
    *ppString = xmalloc(10);
    sprintf(*ppString, "%X", valObj);

    return valObj;
}

void Init_RubyAPI_NetworkAccess(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    

	rb_mNetworkAccess = rb_define_class_under(rb_mParent, "NetworkAccess", rb_cObject);

	rb_define_alloc_func(rb_mNetworkAccess, _allocate_class_object);
    //Constructor should be not available in case of static members
    //rb_undef_alloc_func(rb_mNetworkAccess);

    rb_define_singleton_method(rb_mNetworkAccess, "cancel", rb_s_NetworkAccess_cancel, -1);
    rb_define_singleton_method(rb_mNetworkAccess, "downloadFile", rb_s_NetworkAccess_downloadFile, -1);
    rb_define_singleton_method(rb_mNetworkAccess, "get", rb_s_NetworkAccess_get, -1);
    rb_define_singleton_method(rb_mNetworkAccess, "post", rb_s_NetworkAccess_post, -1);
    rb_define_singleton_method(rb_mNetworkAccess, "uploadFile", rb_s_NetworkAccess_uploadFile, -1);





    rb_const_set(rb_mNetworkAccess, rb_intern("AUTH_BASIC"), rb_str_new2("basic") );



}

