#include "js_helpers.h"

#include "common/RhodesApp.h"
#include "net/URI.h"

namespace rho
{
namespace apiGenerator
{

using namespace rho::json;


static rho::Hashtable<rho::String,Func_JS> g_hashJSMethods;

void js_define_method(const char* szMethodPath, Func_JS pFunc )
{
    g_hashJSMethods[szMethodPath] = pFunc;
}

rho::String js_entry_point(const char* szJSON)
{
    rho::String strReqId, strMethod, strObjID;
    CJSONEntry oEntry(szJSON);

	if ( !oEntry.hasName("id") )
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}";
	strReqId = oEntry.getString("id");

	if ( !oEntry.hasName("method") )
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": " + strReqId + "}";
    strMethod = oEntry.getString("method");

	if ( oEntry.hasName("module") ) {
		rho::String strModule = oEntry.getString("module");
		strMethod = strModule + ":" + strMethod;
	}

    if ( oEntry.hasName("ObjID") )
        strObjID = oEntry.getString("ObjID");

	String_replace(strMethod, '.', ':');
    Func_JS pMethod = g_hashJSMethods[strMethod];
    if (!pMethod)
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Method not found.\"}, \"id\": " + strReqId + "}";

    CJSONArray oParams(oEntry.getEntry("params"));
    return "{\"jsonrpc\": \"2.0\", " + pMethod( oParams, strObjID ) + ", \"id\": " + strReqId + "}";
}

void rho_http_js_entry_point(void *arg, rho::String const &query )
{
	rho::String data = rho::net::URI::urlDecode(query);
    rho::String res = js_entry_point(data.c_str());
    rho_http_sendresponse(arg, res.c_str());
}

}
}