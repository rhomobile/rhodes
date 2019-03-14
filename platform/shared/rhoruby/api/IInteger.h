#pragma once

#include "IObject.h"

namespace rho {
namespace ruby {

class IInteger : public IObject {

public:
    virtual ~IInteger() {}

    virtual long int getLong() = 0;

};

}
}
