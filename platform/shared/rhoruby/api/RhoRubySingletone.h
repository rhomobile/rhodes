#pragma once
#include "IRhoRuby.h"

namespace rho {
namespace ruby {

class RhoRubySingletone {

public:

    static IRhoRuby* getRhoRuby(bool lateInit = false);

private:

    static IRhoRuby* mRhoRuby;
};

}
}
