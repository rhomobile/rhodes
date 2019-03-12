#pragma once

#include "IObject.h"

namespace rho {
namespace ruby {

class IInteger : public IObject {

public:
    virtual ~IInteger() = 0;

    virtual long int getInt() = 0;

};

}
}
