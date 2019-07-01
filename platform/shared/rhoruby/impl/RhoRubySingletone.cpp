
#include "../api/RhoRubySingletone.h"
#include "RhoRubyImpl.h"
#include <stdlib.h>

namespace rho {
namespace ruby {

IRhoRuby* RhoRubySingletone::mRhoRuby = NULL;

IRhoRuby* RhoRubySingletone::getRhoRuby(bool lateInit) {
    if (mRhoRuby == NULL) {
        mRhoRuby = new RhoRubyImpl(lateInit);
    }
    return mRhoRuby;
}


}
}
