#include "js_helpers.h"

#include "common/RhodesApp.h"
#include "net/URI.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "js_helper"

namespace rho
{
namespace apiGenerator
{

using namespace rho::json;


static rho::Hashtable<rho::String,Func_JS> g_hashJSStaticMethods;
static rho::Hashtable<rho::String,Func_JS> g_hashJSInstanceMethods;

static const String ID("id");
static const String METHOD("method");
static const String RHO_CLASS("__rhoClass");
static const String RHO_ID("__rhoID");
static const String RHO_CALLBACK("__rhoCallback");
static const String VM_ID("vmID");
static const String RHO_CALLBACK_PARAM("optParams");

void js_define_static_method(const char* szMethodPath, Func_JS pFunc )
{
    g_hashJSStaticMethods[szMethodPath] = pFunc;
    //RAWTRACE1("Static method: %s", szMethodPath);
}
    
void js_define_instance_method(const char* szMethodPath, Func_JS pFunc )
{
    g_hashJSInstanceMethods[szMethodPath] = pFunc;
    //RAWTRACE1("Instance method: %s", szMethodPath);
}

rho::String js_entry_point(const char* szJSON)
{
    RAWTRACE(szJSON);

    rho::String strReqId, strMethod, strObjID, strCallbackID, strJsVmID, strCallbackParam;
    CJSONEntry oEntry(szJSON);

    if ( !oEntry.hasName(ID) )
    {
        RAWLOG_ERROR1("There is no %s string in JSON request", ID.c_str());
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}";
    }
    strReqId = oEntry.getString(ID.c_str());

    if ( !oEntry.hasName(METHOD) )
    {
        RAWLOG_ERROR1("There is no %s string in JSON request object", METHOD.c_str());
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": " + strReqId + "}";
    }
    strMethod = oEntry.getString(METHOD.c_str());

    if ( oEntry.hasName(RHO_CLASS) )
    {
        RAWTRACE("Parsing module class");
        rho::String strModule = oEntry.getString(RHO_CLASS.c_str());
        strMethod = strModule + ":" + strMethod;
    }

    if ( oEntry.hasName(RHO_ID) )
        strObjID = oEntry.getString(RHO_ID.c_str());

    if ( oEntry.hasName(RHO_CALLBACK) ) {
        RAWTRACE("Parsing callback");

        CJSONEntry oCallback = oEntry.getEntry(RHO_CALLBACK.c_str());

        RAWTRACE1("Got %s JSON object", RHO_CALLBACK.c_str());

        if ( !oCallback.hasName(ID) )
        {
            RAWLOG_ERROR1("There is no %s string in __rhoCallback request", ID.c_str());
            return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": " + strReqId + "}";
        }

        const char* pcszCallbackID = oCallback.getString(ID.c_str());
        const char* pcszJsVmID = oCallback.getString(VM_ID.c_str());
        const char* pcszCallbackParam = oCallback.getString(RHO_CALLBACK_PARAM.c_str());

        if (pcszCallbackID)
            strCallbackID = pcszCallbackID;

        if (pcszJsVmID)
            strJsVmID = pcszJsVmID;//oCallback.getString(pcszJsVmID);

        if (pcszCallbackParam)
            strCallbackParam = pcszCallbackParam;
    }

    String_replace(strMethod, '.', ':');
    Func_JS pMethod = NULL;
    if (strObjID == "0")
    {
        pMethod = g_hashJSStaticMethods[strMethod];
        if (!pMethod)
        {
            RAWLOG_ERROR1("Static API method is not found: %s", strMethod.c_str());
            return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Static method not found.\"}, \"id\": " + strReqId + "}";
        }
    }
    else
    {
        pMethod = g_hashJSInstanceMethods[strMethod];
        if (!pMethod)
        {
            RAWLOG_ERROR1("Instance API method is not found: %s", strMethod.c_str());
            return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Instance method not found.\"}, \"id\": " + strReqId + "}";
        }
    }
    

    CJSONArray oParams(oEntry.getEntry("params"));

    RAWTRACE3("Calling API: object: %s, method: %s, callback id: %s", strObjID.c_str(), strMethod.c_str(), strCallbackID.c_str());
    String methodResult = pMethod( strObjID, oParams, strCallbackID, strJsVmID, strCallbackParam );
    #ifdef RHO_DEBUG
    String res = "{"+methodResult+"}";
    CJSONEntry jsonValidator(res.c_str());
    #endif
    return "{\"jsonrpc\": \"2.0\", " + methodResult + ", \"id\": " + strReqId + "}";
}

void rho_http_js_entry_point(void *arg, rho::String const &query )
{
    rho::String res = js_entry_point(query.c_str());
    rho_http_sendresponse(arg, res.c_str());
}

}
}
