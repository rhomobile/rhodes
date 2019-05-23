
#include "MutableHashImpl.h"
#include "MutableArrayImpl.h"
#include "MutableStringImpl.h"
#include <stdlib.h>
#include "string.h"

namespace rho {
namespace ruby {


MutableHashImpl::MutableHashImpl(bool is_mutable) {
    BASIC_TYPES basicType = BASIC_TYPES::Hash;
    if (is_mutable) {
        basicType = BASIC_TYPES::MutableHash;
    }
    setBasicType(basicType);
}

MutableHashImpl::MutableHashImpl() {
}


MutableHashImpl::~MutableHashImpl() {
    std::map<std::string, IObject*>::iterator it = mHash.begin();
    while(it != mHash.end())
    {
        IObject* value = it->second;
        value->release();
        it++;
    }
}

const char* MutableHashImpl::getClassName() {
    return "Hash";
}

IArray* MutableHashImpl::getKeys() {
    CMutableArray* ar = new CMutableArray(false);
    std::map<std::string, IObject*>::iterator it = mHash.begin();
    while(it != mHash.end())
    {
        std::string key = it->first;
        CMutableString* str_key = new CMutableString(false);
        str_key->setUTF8(key.c_str());
        ar->addItem(str_key);
        str_key->release();
        it++;
    }
    return ar;
}

IObject* MutableHashImpl::getItem(const char* key) {
    return mHash[key];
}

void MutableHashImpl::addItem(const char* key, IObject* item) {
    item->addRef();
    mHash[key] = item;
}


}
}
