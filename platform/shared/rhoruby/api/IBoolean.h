#pragma once
#include "IObject.h"

namespace rho {
namespace ruby {

class IBoolean : public IObject {

public:
    virtual ~IBoolean() {}

    virtual bool getBool() = 0;

};

}
}
