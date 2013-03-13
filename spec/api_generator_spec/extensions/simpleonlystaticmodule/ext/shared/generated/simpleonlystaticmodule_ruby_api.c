
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_mSimpleOnlyStaticModule;

VALUE rb_s_SimpleOnlyStaticModule_calcSumm(int argc, VALUE *argv);
VALUE rb_s_SimpleOnlyStaticModule_joinStrings(int argc, VALUE *argv);
VALUE rb_s_SimpleOnlyStaticModule_getPlatform(int argc, VALUE *argv);
VALUE rb_s_SimpleOnlyStaticModule_showAlertFromUIThread(int argc, VALUE *argv);




VALUE getRuby_SimpleOnlyStaticModule_Module(){ return rb_mSimpleOnlyStaticModule; }



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

    Data_Get_Struct(valObj, char *, ppString);
    *ppString = xmalloc(10);
    sprintf(*ppString, "%X", valObj);

    return valObj;
}

void Init_RubyAPI_SimpleOnlyStaticModule(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    
    tmpParent = rb_mParent;
    rb_mParent = rb_define_module_under(tmpParent, "Examples");
    

	rb_mSimpleOnlyStaticModule = rb_define_class_under(rb_mParent, "SimpleOnlyStaticModule", rb_cObject);

	rb_define_alloc_func(rb_mSimpleOnlyStaticModule, _allocate_class_object);
    //Constructor should be not available in case of static members
    //rb_undef_alloc_func(rb_mSimpleOnlyStaticModule);

    rb_define_singleton_method(rb_mSimpleOnlyStaticModule, "calcSumm", rb_s_SimpleOnlyStaticModule_calcSumm, -1);
    rb_define_singleton_method(rb_mSimpleOnlyStaticModule, "joinStrings", rb_s_SimpleOnlyStaticModule_joinStrings, -1);
    rb_define_singleton_method(rb_mSimpleOnlyStaticModule, "getPlatform", rb_s_SimpleOnlyStaticModule_getPlatform, -1);
    rb_define_singleton_method(rb_mSimpleOnlyStaticModule, "showAlertFromUIThread", rb_s_SimpleOnlyStaticModule_showAlertFromUIThread, -1);








}

