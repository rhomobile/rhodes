#include "..\I<%= $cur_module.name %>.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_<%= $cur_module.name %>_Module();

<% if $cur_module.is_template_default_instance %>
VALUE rb_<%= $cur_module.name %>_s_default(VALUE klass)
{
    rho::StringW strDefaultID = C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, convertToStringA(strDefaultID).c_str() );
}

VALUE rb_<%= $cur_module.name %>_s_set_default(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->setDefaultID(convertToStringW(szID));

    return rho_ruby_get_NIL();
}
<% end %>

<% $cur_module.methods.each do |module_method|%>
<%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC)%>
{
}
<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE%>
<%= api_generator_MakeRubyMethodDecl($cur_module.name + "_def", module_method, true)%>
{
}
<% end %>

<% end %>


VALUE rb_<%= $cur_module.name %>_s_enumerate(int argc, VALUE *argv)
{
    bool bCallInUIThread = false;
    bool bCallInSeparateThread = true;

    CMethodResult oRes;
    oRes.setRubyObjectClass(getRuby_<%= $cur_module.name %>_Module());

    if ( argc == 0 )
    {
        C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->enumerate(oRes);
    }else 
    {
        if ( !rho_ruby_is_string(argv[1]) )
        {
            oRes.setArgError(L"Type error: argument 2 should be String"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(bCallInUIThread);
        oRes.setRubyCallback( getStringFromValue(argv[1]) );
        if ( argc >= 2 )
        {
            if ( !rho_ruby_is_string(argv[1]) )
            {
                oRes.setArgError(L"Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[1]) );
        }

        rho::common::IRhoRunnable* pFunctor = new rho::common::CInstanceClassFunctor1<I<%= $cur_module.name %>Singleton*, void (I<%= $cur_module.name %>Singleton::*)(CMethodResult&), CMethodResult>
                ( C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS(), &I<%= $cur_module.name %>Singleton::enumerate, oRes );

        if ( bCallInUIThread )
            rho_wm_impl_performOnUiThread( pFunctor );
        else if ( bCallInSeparateThread )
            C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->callCommandInThread( pFunctor );
        else
            C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );
    }

    return oRes.toRuby();
}

extern "C" static void
string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::StringW>& ar = *((rho::Vector<rho::StringW>*)(par));
    ar.addElement( convertToStringW(szVal) );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::StringW>& res)
{
    rho_ruby_enum_strary(val, string_iter, &res);
}

static VALUE <%= $cur_module.name %>_getprops(int argc, VALUE *argv, I<%= $cur_module.name %>* pObj)
{
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    bool bCallInUIThread = false;
    bool bCallInSeparateThread = true;

    CMethodResult oRes;
    if ( argc == 0 )
    {
        pObj->getProps(oRes);
    }else if ( argc == 1 )
    {
        if ( rho_ruby_is_NIL(argv[0]) )
        {
            pObj->getProps(oRes);
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            pObj->getPropsWithString( convertToStringW(getStringFromValue(argv[0])), oRes);
        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::StringW> ar;
            getStringArrayFromValue(argv[0], ar);
            pObj->getPropsWithArray(ar, oRes );
        }else
        {
            oRes.setArgError(L"Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
        }
    }else if ( argc >= 2 )
    {
        if ( !rho_ruby_is_string(argv[1]) )
        {
            oRes.setArgError(L"Type error: argument 2 should be String"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(bCallInUIThread);
        oRes.setRubyCallback( getStringFromValue(argv[1]) );
        if ( argc >= 3 )
        {
            if ( !rho_ruby_is_string(argv[2]) )
            {
                oRes.setArgError(L"Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[2]) );
        }

        rho::common::IRhoRunnable* pFunctor = 0;
        if ( rho_ruby_is_NIL(argv[0]) )
        {
            pFunctor = new rho::common::CInstanceClassFunctor1<I<%= $cur_module.name %>*, void (I<%= $cur_module.name %>::*)(CMethodResult&), CMethodResult>
                ( pObj, &I<%= $cur_module.name %>::getProps, oRes );
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            oRes.setStringParam(getStringFromValue(argv[0]));
            pFunctor = new rho::common::CInstanceClassFunctor2<I<%= $cur_module.name %>*, void (I<%= $cur_module.name %>::*)(const rho::StringW&, CMethodResult&), rho::StringW, CMethodResult>
                ( pObj, &I<%= $cur_module.name %>::getPropsWithString, convertToStringW(getStringFromValue(argv[0])), oRes );

        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::StringW> ar;
            getStringArrayFromValue(argv[0], ar);

            pFunctor = new rho::common::CInstanceClassFunctor2<I<%= $cur_module.name %>*, void (I<%= $cur_module.name %>::*)(const rho::Vector<rho::StringW>&, CMethodResult&), rho::Vector<rho::StringW>, CMethodResult>
                ( pObj, &I<%= $cur_module.name %>::getPropsWithArray, ar, oRes );
        }else
        {
            oRes.setArgError(L"Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        if ( bCallInUIThread )
            rho_wm_impl_performOnUiThread( pFunctor );
        else if ( bCallInSeparateThread )
            C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->callCommandInThread( pFunctor );
        else
            C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );


    }else
    {
        oRes.setArgError(L"wrong # of arguments(%d for 2)", argc );
    }

    return oRes.toRuby();
}

VALUE rb_<%= $cur_module.name %>_s_getprops(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();
    I<%= $cur_module.name %>* pObj = C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(strDefaultID);

    return <%= $cur_module.name %>_getprops(argc, argv, pObj);
}

static VALUE <%= $cur_module.name %>_setprops(int argc, VALUE *argv, I<%= $cur_module.name %>* pObj)
{
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    bool bCallInUIThread = false;
    bool bCallInThread = false;
    //TODO:<%= $cur_module.name %>_setprops

    CMethodResult oRes;
/*
    if ( argc == 0 )
    {
        pObj->getProps(oRes);
    }else if ( argc == 1 )
    {
        if ( rho_ruby_is_NIL(argv[0]) )
        {
            pObj->getProps(oRes);
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            pObj->getPropsWithString(getStringFromValue(argv[0]), oRes);
        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::String> ar;
            getStringArrayFromValue(argv[0], ar);
            pObj->getPropsWithArray(ar, oRes );
        }else
        {
            oRes.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
        }
    }else if ( argc >= 2 )
    {
        if ( !rho_ruby_is_string(argv[1]) )
        {
            oRes.setArgError("Type error: argument 2 should be String"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(bCallInUIThread);
        oRes.setRubyCallback( getStringFromValue(argv[1]) );
        if ( argc >= 3 )
        {
            if ( !rho_ruby_is_string(argv[2]) )
            {
                oRes.setArgError("Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[2]) );
        }

        rho::common::IRhoRunnable* pFunctor = 0;
        if ( rho_ruby_is_NIL(argv[0]) )
        {
            pFunctor = new rho::common::CInstanceClassFunctor1<I<%= $cur_module.name %>*, void (I<%= $cur_module.name %>::*)(CMethodResult&), CMethodResult>
                ( pObj, &I<%= $cur_module.name %>::getProps, oRes );
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            oRes.setStringParam(getStringFromValue(argv[0]));
            pFunctor = new rho::common::CInstanceClassFunctor2<I<%= $cur_module.name %>*, void (I<%= $cur_module.name %>::*)(const rho::String&, CMethodResult&), rho::String, CMethodResult>
                ( pObj, &I<%= $cur_module.name %>::getPropsWithString, getStringFromValue(argv[0]), oRes );

        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::String> ar;
            getStringArrayFromValue(argv[0], ar);

            pFunctor = new rho::common::CInstanceClassFunctor2<I<%= $cur_module.name %>*, void (I<%= $cur_module.name %>::*)(const rho::Vector<rho::String>&, CMethodResult&), rho::Vector<rho::String>, CMethodResult>
                ( pObj, &I<%= $cur_module.name %>::getPropsWithArray, ar, oRes );
        }else
        {
            oRes.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        if ( bCallInUIThread )
            rho_wm_impl_performOnUiThread( pFunctor );
        else //call in separate thread
            C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );

    }else
    {
        oRes.setArgError("wrong # of arguments(%d for 2)", argc );
    }
*/
    return oRes.toRuby();
}

VALUE rb_<%= $cur_module.name %>_getprops(int argc, VALUE *argv, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    I<%= $cur_module.name %>* pObj =  C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return <%= $cur_module.name %>_getprops(argc, argv, pObj);
}

VALUE rb_<%= $cur_module.name %>_s_setprops(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();
    I<%= $cur_module.name %>* pObj = C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(strDefaultID);

    return <%= $cur_module.name %>_setprops(argc, argv, pObj);
}

VALUE rb_<%= $cur_module.name %>_setprops(int argc, VALUE *argv, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    I<%= $cur_module.name %>* pObj =  C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return <%= $cur_module.name %>_setprops(argc, argv, pObj);
}

}