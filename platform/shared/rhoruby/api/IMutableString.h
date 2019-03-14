#pragma once
#include "IString.h"

namespace rho {
namespace ruby {

class IMutableString : public IString {

public:
    virtual ~IMutableString() {}

    virtual void setUTF8(const char* utf8_string) = 0;

};

}
}
