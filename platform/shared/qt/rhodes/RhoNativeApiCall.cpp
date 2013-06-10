#include "RhoNativeApiCall.h"
#include "logging/RhoLog.h"
#include "net/URI.h"
#include "api_generator/js_helpers.h"

void RhoNativeApiCall::populateJavaScriptWindowObject()
{
    m_frame->addToJavaScriptWindowObject("__rhoNativeApiCall", this, QScriptEngine::ScriptOwnership);
}

const QString RhoNativeApiCall::apiCall(const QString& msg)
{
    //RAWLOGC_INFO1("RhoNativeApiCall", "JS API call: %s", msg.toStdString().c_str());
    rho::String data = rho::net::URI::urlDecode(msg.toStdString().c_str());
    rho::String res = rho::apiGenerator::js_entry_point(data.c_str());
    return QString(res.c_str());
    // how to return JS-object instance: (QVariant) return m_frame->evaluateJavascript("__rhoNativeApiCall;");
}
