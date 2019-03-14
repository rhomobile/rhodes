
#include "MutableBooleanImpl.h"
#include <stdlib.h>


namespace rho {
namespace ruby {


MutableBooleanImpl::MutableBooleanImpl(bool is_mutable) {
    BASIC_TYPES basicType = BASIC_TYPES::Boolean;
    if (is_mutable) {
        basicType = BASIC_TYPES::MutableBoolean;
    }
    setBasicType(basicType);
    mValue = false;
}

MutableBooleanImpl::MutableBooleanImpl() {
    mValue = false;
}


MutableBooleanImpl::~MutableBooleanImpl() {

}

const char* MutableBooleanImpl::getClassName() {
    if (mValue) {
        return "TrueClass";
    }
    return "FalseClass";
}

bool MutableBooleanImpl::getBool() {
    return mValue;
}

void MutableBooleanImpl::setBool(bool value) {
    mValue = value;
}

}
}
