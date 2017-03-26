#include "js_helpers.h"
#include "common/RhodesApp.h"
#include "net/URI.h"
#include "logging/RhoLog.h"
#include "api_generator/Api.h"
#include <vector>


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "js_helper"

namespace rho
{
namespace apiGenerator
{

using namespace rho::json;

static const String ID("id");
static const String METHOD("method");
static const String RHO_CLASS("__rhoClass");
static const String RHO_ID("__rhoID");
static const String RHO_CALLBACK("__rhoCallback");
static const String VM_ID("vmID");
static const String RHO_CALLBACK_PARAM("optParams");

std::vector<ApiHandler<Func_JS>*> g_modules;

void defineJSApiModule(ApiHandler<Func_JS>* pModule)
{
    g_modules.push_back(pModule);
    RAWTRACE2("Define %s, %d", pModule->getModuleId(), g_modules.size() -1);
}

ApiHandler<Func_JS>* getJSApiModule(const std::string& moduleId)
{
    for(std::vector<ApiHandler<Func_JS>*>::iterator moduleIt = g_modules.begin(); moduleIt != g_modules.end(); ++moduleIt)
    {
        if(strcmp((*moduleIt)->getModuleId(), moduleId.c_str()) == 0)
        {
            RAWTRACE2("Module %s, %d", (*moduleIt)->getModuleId(), moduleIt - g_modules.begin());
            return *moduleIt;
        }

    }
    RAWTRACE1("Module is not found: %s", moduleId.c_str());
    return 0;
}

rho::String js_entry_point(const char* szJSON)
{
    RAWTRACE(szJSON);

    rho::String strReqId, strModule, strMethod, strObjID, strCallbackID, strJsVmID, strCallbackParam;
    CJSONEntry oEntry(szJSON);

    if ( !oEntry.hasName(ID) )
    {
        RAWLOG_ERROR1("There is no %s string in JSON request", ID.c_str());
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}";
    }
    strReqId = oEntry.getString(ID.c_str());

    if ( !oEntry.hasName(RHO_CLASS) )
    {
        RAWLOG_ERROR1("There is no %s string in JSON request", RHO_CLASS.c_str());
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": " + strReqId + "}";
    }
    strModule = oEntry.getString(RHO_CLASS.c_str());

    if ( !oEntry.hasName(METHOD) )
    {
        RAWLOG_ERROR1("There is no %s string in JSON request object", METHOD.c_str());
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": " + strReqId + "}";
    }
    strMethod = oEntry.getString(METHOD.c_str());

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

    String_replace(strModule, '.', ':');

    ApiHandler<Func_JS>* pApiHandler = getJSApiModule(strModule);
    if (!pApiHandler)
    {
        RAWLOG_ERROR1("API Module is not found: %s", strModule.c_str());
        return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"API module is not found.\"}, \"id\": " + strReqId + "}";
    }

    Func_JS pMethod = NULL;
    if (strObjID == "0")
    {
        pMethod = pApiHandler->getStaticMethod(strMethod);
        if (!pMethod)
        {
            RAWLOG_ERROR1("Static API method is not found: %s", strMethod.c_str());
            return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Static method is not found.\"}, \"id\": " + strReqId + "}";
        }
    }
    else
    {
        pMethod = pApiHandler->getInstanceMethod(strMethod);
        if (!pMethod)
        {
            RAWLOG_ERROR1("Instance API method is not found: %s", strMethod.c_str());
            return "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Instance method is not found.\"}, \"id\": " + strReqId + "}";
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

static std::map<rho::String, JVM_Callback_Provider> g_jvm_providers;




void setCustomJVMCallbackProvider(const char* jvm_id, JVM_Callback_Provider provider) {
    g_jvm_providers[jvm_id] = provider;
}


int callCustomJVMCallbackProvider(const char* jvm_id, const char* js_code) {
    JVM_Callback_Provider provider = g_jvm_providers[jvm_id];
    if (provider != NULL) {
        provider(js_code);
        return 1;
    }
    return 0;
}
