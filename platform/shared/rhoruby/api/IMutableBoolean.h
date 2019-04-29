#pragma once

#include "IBoolean.h"

namespace rho {
namespace ruby {

class IMutableBoolean : public IBoolean {

public:
    virtual ~IMutableBoolean() {}

    virtual void setBool(bool value) = 0;

};

}
}
