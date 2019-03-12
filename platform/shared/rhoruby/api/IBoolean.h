#pragma once
#include "IObject.h"

namespace rho {
namespace ruby {

class IBoolean : public IObject {

public:
    virtual ~IBoolean() = 0;

    virtual bool getBool() = 0;

};

}
}
