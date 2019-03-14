
#include "MutableArrayImpl.h"
#include <stdlib.h>
#include "string.h"

namespace rho {
namespace ruby {


MutableArrayImpl::MutableArrayImpl(bool is_mutable) {
    BASIC_TYPES basicType = BASIC_TYPES::Array;
    if (is_mutable) {
        basicType = BASIC_TYPES::MutableArray;
    }
    setBasicType(basicType);
}

MutableArrayImpl::MutableArrayImpl() {
}


MutableArrayImpl::~MutableArrayImpl() {
    int i;
    for (i = 0; i < mVector.size(); i++) {
        mVector[i]->release();
    }
}

const char* MutableArrayImpl::getClassName() {
    return "Array";
}

int MutableArrayImpl::getItemsCount() {
    return (int)mVector.size();
}

IObject* MutableArrayImpl::getItem(int index) {
    if ((index < 0) && ( mVector.size() <= index)) {
        return NULL;
    }
    return mVector[index];
}

void MutableArrayImpl::addItem(IObject* item) {
    mVector.push_back(item);
}
    
void MutableArrayImpl::reserve(int capacity) {
    mVector.reserve(capacity);
}

}
}
