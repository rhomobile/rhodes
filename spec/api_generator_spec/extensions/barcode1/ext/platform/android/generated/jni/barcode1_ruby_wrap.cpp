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
    RAWTRACE1("barcode1_getprops(id=%s) >>>>>>>>>>>>>>>>>>>>>>", id.c_str());

    MethodResultJni result;
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return result.toRuby();
    }

    CBarcode1 barcode(id);
    if(argc == 0)
    {
        RAWTRACE("Get all props sync");
        barcode.getProps(result);
    }
    else
    {
        if(argc >= 2)
        {
            RAWTRACE("Get props to callback");
            if(!rho_ruby_is_string(argv[1]))
            {
                RAWLOG_ERROR("Type error: argument 2 should be String");
                result.setArgError("Type error: argument 2 should be String"); //see SWIG Ruby_Format_TypeError
                return result.toRuby();
            }

            if(argc >= 3)
            {
                RAWTRACE("Get props to callback with data");
                if(!rho_ruby_is_string(argv[2]))
                {
                    RAWLOG_ERROR("Type error: argument 3 should be String");
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

        if(rho_ruby_is_NIL(argv[0]))
        {
            RAWTRACE("Get all props async");
            barcode.getProps(result);
        }
        else if(rho_ruby_is_string(argv[0]))
        {
            RAWTRACE("Get exact prop");
            jhstring jhName = rho_cast<jstring>(argv[0]);
            barcode.getProps(jhName.get(), result);
        }
        else if(rho_ruby_is_array(argv[0]))
        {
            RAWTRACE("Get list of props");
            jhobject jhNames = rho_cast<jobject>(argv[0]);
            barcode.getProps(jhNames.get(), result);
        }
        else
        {
            RAWLOG_ERROR("Type error: argument 1 should be nil, String or Array");
            result.setArgError("Type error: argument 1 should be nil, String or Array"); //see SWIG Ruby_Format_TypeError
        }
    }
    VALUE res = result.toRuby();
    RAWTRACE1("barcode1_getprops(id=%s) end <<<<<<<<<<<<<<<<<<<<<<<<<<<<", id.c_str());
    return res;
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

VALUE rb_Barcode1_s_enumerate(VALUE klass)
{
    RAWTRACE("rb_barcode1_s_enumerate");

    MethodResultJni result;
    CBarcode1::enumerate(result);
    return result.enumerateRubyObjects(klass);
}

VALUE rb_Barcode1_s_default(VALUE klass)
{
    RAWTRACE("rb_barcode1_s_default");
    return rho_ruby_create_object_with_id(klass, CBarcode1::getDefaultID().c_str());
}

VALUE rb_Barcode1_s_setDefault(VALUE klass, VALUE valObj)
{
    RAWTRACE("rb_barcode1_s_set_default");

    const char* szID = rho_ruby_get_object_id(valObj);
    CBarcode1::setDefaultID(szID);

    return rho_ruby_get_NIL();
}

VALUE rb_Barcode1_s_getprops(int argc, VALUE *argv)
{
    RAWTRACE("rb_barcode1_s_getprops");

    rho::String id = CBarcode1::getDefaultID();
    return barcode1_getprops(argc, argv, id);
}

VALUE rb_Barcode1_getprops(int argc, VALUE *argv, VALUE valObj)
{
    RAWTRACE("rb_barcode1_getprops");

    rho::String id = rho_ruby_get_object_id(valObj);
    return barcode1_getprops(argc, argv, id);
}

VALUE rb_Barcode1_s_setprops(int argc, VALUE *argv)
{
    RAWTRACE("rb_barcode1_s_setprops");

    rho::String id = CBarcode1::getDefaultID();
    return barcode1_setprops(argc, argv, id);
}

VALUE rb_Barcode1_setprops(int argc, VALUE *argv, VALUE valObj)
{
    RAWTRACE("rb_barcode1_s_setprops");

    rho::String id = rho_ruby_get_object_id(valObj);
    return barcode1_setprops(argc, argv, id);
}

}
