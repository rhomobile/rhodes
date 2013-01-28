#include "js_helpers.h"

#include "common/RhodesApp.h"

using namespace rho::json;

static rho::Hashtable<rho::String,Func_JS> g_hashJSMethods;

void js_define_method(const char* szMethodPath, Func_JS pFunc )
{
    g_hashJSMethods[szMethodPath] = pFunc;
}

rho::String js_entry_point(const char* szJSON)
{
    rho::String strModule, strMethod, strObjID;
    CJSONEntry oEntry(szJSON);

    if ( oEntry.hasName("Module") )
        strModule = oEntry.getString("Module");
    else
    {
        //return json with exception
        return "{Error}";
    }

    if ( oEntry.hasName("Method") )
        strMethod = oEntry.getString("Method");
    else
    {
        //return json with exception
        return "{Error}";
    }

    if ( oEntry.hasName("ObjID") )
        strObjID = oEntry.getString("ObjID");


    Func_JS pMethod = g_hashJSMethods[strModule + ":" + strMethod];
    if (!pMethod )
    {
        //return json with exception
        return "{Error}";
    }

    CJSONArray oParams(oEntry.getEntry("Params"));
    return pMethod( oParams, strObjID );
}

void rho_http_js_entry_point(void *arg, rho::String const &query )
{
    rho::String res = js_entry_point(query.c_str());
    rho_http_sendresponse(arg, res.c_str());
}
