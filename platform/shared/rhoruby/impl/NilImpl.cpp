
#include "NilImpl.h"
#include <stdlib.h>



namespace rho {
namespace ruby {


NilImpl::NilImpl():ObjectImpl(BASIC_TYPES::Nil, NULL) {
}

NilImpl::~NilImpl() {
}

const char* NilImpl::getClassName() {
    return "NilClass";
}

}
}
