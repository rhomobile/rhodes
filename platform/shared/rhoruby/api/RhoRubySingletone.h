#pragma once
#include "IRhoRuby.h"

namespace rho {
namespace ruby {

class RhoRubySingletone {

public:
    ~RhoRubySingletone();
    RhoRubySingletone();

    static IRhoRuby* getRhoRuby();

private:
    static IRhoRuby* mRhoRuby;
};

}
}
