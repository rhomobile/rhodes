#pragma once
#include "IObject.h"

namespace rho {
namespace ruby {

class IFloat : public IObject {

public:
    virtual ~IFloat() = 0;

    virtual double getDouble() = 0;

};

}
}
