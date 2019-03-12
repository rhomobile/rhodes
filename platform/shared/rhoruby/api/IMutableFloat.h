#pragma once
#include "IFloat.h"

namespace rho {
namespace ruby {

class IMutableFloat : public IFloat {

public:
    virtual ~IMutableFloat() = 0;

    virtual void setDouble(double value) = 0;

};

}
}
