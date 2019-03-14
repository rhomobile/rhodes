#pragma once
#include "IObject.h"

namespace rho {
namespace ruby {

class IFloat : public IObject {

public:
    virtual ~IFloat() {}

    virtual double getDouble() = 0;

};

}
}
