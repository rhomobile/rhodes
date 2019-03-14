
#include "MutableIntegerImpl.h"
#include <stdlib.h>


namespace rho {
namespace ruby {


MutableIntegerImpl::MutableIntegerImpl(bool is_mutable) {
    BASIC_TYPES basicType = BASIC_TYPES::Integer;
    if (is_mutable) {
        basicType = BASIC_TYPES::MutableInteger;
    }
    setBasicType(basicType);
    mValue = 0;
}

MutableIntegerImpl::MutableIntegerImpl() {
    mValue = 0;
}


MutableIntegerImpl::~MutableIntegerImpl() {

}

const char* MutableIntegerImpl::getClassName() {
    return "Fixnum";
}

long int MutableIntegerImpl::getLong() {
    return mValue;
}

void MutableIntegerImpl::setLong(long int value) {
    mValue = value;
}

}
}
