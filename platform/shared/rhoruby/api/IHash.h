#pragma once

#include "IObject.h"
#include "IArray.h"

namespace rho {
namespace ruby {

class IHash : public IObject {

public:
    virtual ~IHash() {}

    //array of IString
    virtual IArray* getKeys() = 0;

    //return object by key
    virtual IObject* getItem(const char* key) = 0;

};

}
}
