
#include "MutableStringImpl.h"
#include <stdlib.h>
#include "string.h"

namespace rho {
namespace ruby {


MutableStringImpl::MutableStringImpl(bool is_mutable) {
    BASIC_TYPES basicType = BASIC_TYPES::String;
    if (is_mutable) {
        basicType = BASIC_TYPES::MutableString;
    }
    setBasicType(basicType);
}

MutableStringImpl::~MutableStringImpl() {
    if (mString != NULL) {
        free(mString);
    }
}

const char* MutableStringImpl::getClassName() {
    return "String";
}

const char* MutableStringImpl::getUTF8() {
    return (const char*)mString;
}

void MutableStringImpl::setUTF8(const char* utf8_string) {
    if (mString != NULL) {
        free(mString);
    }
    mString = NULL;
    if (utf8_string != NULL) {
        int len = (int)strlen(utf8_string);
        mString = (char*)malloc(len+1);
        strcpy(mString, utf8_string);
    }
}


}
}
