#pragma once

#include "IObject.h"

namespace rho {
namespace ruby {

class IArray : public IObject {

public:
    virtual ~IArray() = 0;

    virtual int getItemsCount() = 0;

    virtual IObject* getItem(int index) = 0;

};

}
}
