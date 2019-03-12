#include "RubyServerResponceImpl.h"
#include "../api/RhoRubySingletone.h"
#include "../api/IMutableString.h"
#include <stdlib.h>
#include "string.h"

namespace rho {
namespace ruby {

RubyServerResponceImpl::RubyServerResponceImpl(const char* body) {
    mBody = NULL;
    if (body != NULL) {
        int len = (int)strlen(body);
        mBody = (char*)malloc(len+1);
        strcpy(mBody, body);
    }
}

RubyServerResponceImpl::~RubyServerResponceImpl(){
    if (mBody != NULL) {
        free(mBody);
    }
}

IString* RubyServerResponceImpl::getResponceBody() {
    IMutableString* str = (IMutableString*)RhoRubySingletone::getRhoRuby()->makeMutableBaseTypeObject(BASIC_TYPES::MutableString);
    str->setUTF8(mBody);
    return str;
}

}
}
