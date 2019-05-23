#pragma once
#include "IRefCountedObject.h"

namespace rho {
namespace ruby {


typedef enum {
        None,
        // types decorate real ruby objects
        RubyNil,
        Class,
        Object,
        Boolean,
        Integer,
        Float,
        String,
        Array,
        Hash,
        // types for prepare parameters for send to ruby - can be changed instead of read-only base types (used for receive ruby objects from ruby code)
        MutableBoolean,
        MutableInteger,
        MutableFloat,
        MutableString,
        MutableArray,
        MutableHash
}BASIC_TYPES;


// this object decorate real ruby object when you received it from any methods.
// But you also can construct objects for base type for send to ruby code as parameters
// you should release this object manually in both cases - we do not has automatically reference counting !

class IObject : public IRefCountedObject {

public:
    virtual ~IObject() {}

    virtual const char* getClassName() = 0;

    virtual BASIC_TYPES getBasicType() = 0;

};

}
}
