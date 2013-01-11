#pragma once

#include "rhodes.h"

#include <string>

namespace rhoelements {


class MethodResultJni
{
    jhobject m_obj;

    const char* m_callBackUrl;
    const char* m_callBackData;

public:
    MethodResultJni(jobject jObj) : m_callBackUrl(0), m_callBackData(0) {};

    void setCallBack(const char* url, const char* data) { m_callBackUrl = url; m_callBackData = data; }
    bool hasCallBackUrl() { return m_callBackUrl != 0; }

    operator jobject();

    VALUE enumerateRubyObjects();
    std::string enumerateJSObjects();

    VALUE toRuby();
    std::string toJson();

    void setError(const char* msg);
    void setArgError(const char *fmt, ...);
};

}
