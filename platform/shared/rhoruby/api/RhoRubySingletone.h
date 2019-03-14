#pragma once
#include "IRhoRuby.h"

namespace rho {
namespace ruby {

class RhoRubySingletone {

public:

    static IRhoRuby* getRhoRuby();

private:

    static IRhoRuby* mRhoRuby;
};

}
}
