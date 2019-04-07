
#include "NilImpl.h"
#include <stdlib.h>

#include "../../ruby/ext/rho/rhoruby.h"
#include "../../ruby/include/ruby.h"

namespace rho {
namespace ruby {


NilImpl::NilImpl():ObjectImpl(BASIC_TYPES::RubyNil, NULL, rho_ruby_get_NIL()) {
}

NilImpl::~NilImpl() {
}

const char* NilImpl::getClassName() {
    return "NilClass";
}

}
}
