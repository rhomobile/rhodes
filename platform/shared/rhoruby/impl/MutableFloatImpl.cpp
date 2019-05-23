
#include "MutableFloatImpl.h"
#include <stdlib.h>


namespace rho {
namespace ruby {


MutableFloatImpl::MutableFloatImpl(bool is_mutable) {
    BASIC_TYPES basicType = BASIC_TYPES::Float;
    if (is_mutable) {
        basicType = BASIC_TYPES::MutableFloat;
    }
    setBasicType(basicType);
    mValue = 0.0;
}

MutableFloatImpl::MutableFloatImpl() {
    mValue = 0.0;
}


MutableFloatImpl::~MutableFloatImpl() {

}

const char* MutableFloatImpl::getClassName() {
    return "Float";
}

double MutableFloatImpl::getDouble() {
    return mValue;
}

void MutableFloatImpl::setDouble(double value) {
    mValue = value;
}

}
}
