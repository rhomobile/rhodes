#pragma once

#include "rhodes.h"

#include <string>

namespace rhoelements {


class MethodResultJni
{
    jhobject m_obj;
public:
    MethodResultJni(jobject jObj);
    operator jhobject();

    VALUE enumerateRubyObjects();
    std::string enumerateJSObjects();

    VALUE toRuby();
    std::string toJson();
};

}
