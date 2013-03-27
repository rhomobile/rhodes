#include "js_helpers.h"

#include "common/RhodesApp.h"
#include "net/URI.h"

namespace rho
{
namespace apiGenerator
{

using namespace rho::json;


static rho::Hashtable<rho::String,Func_JS> g_hashJSMethods;

static const String ID("id");
static const String METHOD("method");
static const String RHO_CLASS("__rhoClass");
static const String RHO_ID("__rhoID");
static const String RHO_CALLBACK("__rhoCallback");
static const String VM_ID("vmID");

void js_define_method(const char* szMethodPath, Func_JS pFunc )
{
    g_hashJSMethods[szMethodPath] = pFunc;
}

rho::String js_entry_point(const char* szJSON)
{
    rho::String strReqId, strMethod, strObjID, strCallbackID, strJsVmID;
    CJSONEntry oEntry(szJSON);

	if ( !oEntry.hasName(ID) )
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}";
	strReqId = oEntry.getString(ID.c_str());

	if ( !oEntry.hasName(METHOD) )
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": " + strReqId + "}";
    strMethod = oEntry.getString(METHOD.c_str());

	if ( oEntry.hasName(RHO_CLASS) ) {
		rho::String strModule = oEntry.getString(RHO_CLASS.c_str());
		strMethod = strModule + ":" + strMethod;
	}

    if ( oEntry.hasName(RHO_ID) )
        strObjID = oEntry.getString(RHO_ID.c_str());

    if ( oEntry.hasName(RHO_CALLBACK) ) {
        CJSONEntry oCallback = oEntry.getEntry(RHO_CALLBACK.c_str());

        if (!oCallback.hasName(ID) || !oCallback.hasName(VM_ID))
            return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": " + strReqId + "}";

        strCallbackID = oCallback.getString(ID.c_str());
        strJsVmID = oCallback.getString(VM_ID.c_str());
    }

    String_replace(strMethod, '.', ':');
    Func_JS pMethod = g_hashJSMethods[strMethod];
    if (!pMethod)
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Method not found.\"}, \"id\": " + strReqId + "}";

    CJSONArray oParams(oEntry.getEntry("params"));
    return "{\"jsonrpc\": \"2.0\", " + pMethod( strObjID, oParams, strCallbackID, strJsVmID ) + ", \"id\": " + strReqId + "}";
}

void rho_http_js_entry_point(void *arg, rho::String const &query )
{
	rho::String data = rho::net::URI::urlDecode(query);
    rho::String res = js_entry_point(data.c_str());
    rho_http_sendresponse(arg, res.c_str());
}

}
}
