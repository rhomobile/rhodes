#pragma once

#include "IBoolean.h"

namespace rho {
namespace ruby {

class IMutableBoolean : public IBoolean {

public:
    virtual ~IMutableBoolean() = 0;

    virtual void setBool(bool value) = 0;

};

}
}
