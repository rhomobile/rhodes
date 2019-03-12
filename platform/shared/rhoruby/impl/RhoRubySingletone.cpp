
#include "../api/RhoRubySingletone.h"
#include "RhoRubyImpl.h"

namespace rho {
namespace ruby {

RhoRubySingletone::~RhoRubySingletone() {

}

RhoRubySingletone::RhoRubySingletone() {
    mRhoRuby = new RhoRubyImpl();
}

IRhoRuby* RhoRubySingletone::getRhoRuby() {
    return mRhoRuby;
}


}
}
