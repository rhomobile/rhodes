#include "Barcode1.h"

#include "MethodResultJni.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1RUBY"

#include "ext/rho/rhoruby.h"
#include "rhodes/JNIRhoRuby.h"

using namespace rhoelements;

static VALUE barcode1_getprops(int argc, VALUE *argv, const rho::String& id)
{
    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        return result.toRuby();
    }

    CBarcode1 barcode(id);
    if(argc == 0)
    {
        barcode.getProps(result);
    }
    else if(argc <= 1)
    {
        if(argc >= 2)
        {
            if(!rho_ruby_is_string(argv[1]))
            {
                result.setArgError("Type error: argument 2 should be String"); //see SWIG Ruby_Format_TypeError
                return result.toRuby();
            }

            if(argc >= 3)
            {
                if(!rho_ruby_is_string(argv[2]))
                {
                    result.setArgError("Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
                    return result.toRuby();
                }
                result.setCallBack(getStringFromValue(argv[1]), getStringFromValue(argv[2]));
            }
            else
            {
                result.setCallBack(getStringFromValue(argv[1]), 0);
            }
        }

        if(rho_ruby_is_string(argv[0]))
        {
            jhstring jhName = rho_cast<jstring>(argv[0]);
            barcode.getProps(jhName, result);
        }
        else if(rho_ruby_is_array(argv[0]))
        {
            jhobject jhNames = rho_cast<jobject>(argv[0]);
            barcode.getProps(jhNames, result);
        }
        else
        {
            result.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
        }
    }
    return result.toRuby();
}

static VALUE barcode1_setprops(int argc, VALUE* argv, const rho::String& id)
{
    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        return result.toRuby();
    }

    CBarcode1 barcode(id);
//    if(argc == 0)
//    {
//        barcode.getProps(result);
//    }
//    else if(argc <= 1)
//    {
//        if(argc >= 2)
//        {
//            if(!rho_ruby_is_string(argv[1]))
//            {
//                result.setArgError("Type error: argument 2 should be String"); //see SWIG Ruby_Format_TypeError
//                return result.toRuby();
//            }
//
//            if(argc >= 3)
//            {
//                if(!rho_ruby_is_string(argv[2]))
//                {
//                    result.setArgError("Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
//                    return result.toRuby();
//                }
//                result.setCallBack(getStringFromValue(argv[1]), getStringFromValue(argv[2]));
//            }
//            else
//            {
//                result.setCallBack(getStringFromValue(argv[1]), 0);
//            }
//        }
//
//        if(rho_ruby_is_string(argv[0]))
//        {
//            barcode.getProps(rho_cast<jhstring>(argv[0]), result);
//        }
//        else if(rho_ruby_is_array(argv[0]))
//        {
//            barcode.getProps(rho_cast<jhobject>(argv[0]), result);
//        }
//        else
//        {
//            result.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
//        }
//    }

    return result.toRuby();
}

extern "C"
{

VALUE rb_barcode1_s_enumerate(VALUE klass)
{
    MethodResultJni result;
    CBarcode1::enumerate(result);
    return result.enumerateRubyObjects(klass);
}

VALUE rb_barcode1_s_default(VALUE klass)
{
    return rho_ruby_create_object_with_id(klass, CBarcode1::getDefaultID().c_str());
}

VALUE rb_barcode1_s_set_default(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id(valObj);
    CBarcode1::setDefaultID(szID);

    return rho_ruby_get_NIL();
}

VALUE rb_barcode1_s_getprops(int argc, VALUE *argv)
{
    rho::String id = CBarcode1::getDefaultID();
    return barcode1_getprops(argc, argv, id);
}

VALUE rb_barcode1_getprops(int argc, VALUE *argv, VALUE valObj)
{
    rho::String id = rho_ruby_get_object_id(valObj);
    return barcode1_getprops(argc, argv, id);
}

VALUE rb_barcode1_s_setprops(int argc, VALUE *argv)
{
    rho::String id = CBarcode1::getDefaultID();
    return barcode1_setprops(argc, argv, id);
}

VALUE rb_barcode1_setprops(int argc, VALUE *argv, VALUE valObj)
{
    rho::String id = rho_ruby_get_object_id(valObj);
    return barcode1_setprops(argc, argv, id);
}

}
