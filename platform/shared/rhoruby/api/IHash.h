#pragma once

#include "IObject.h"
#include "IArray.h"

namespace rho {
namespace ruby {

class IHash : public IObject {

public:
    virtual ~IHash() = 0;

    //array of strings
    virtual IArray* getKeys() = 0;

    //array of strings
    virtual IObject* getItem(const char* key) = 0;

};

}
}
