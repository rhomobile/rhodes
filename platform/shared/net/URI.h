#pragma once

#include "common/RhoStd.h"

namespace rho {
namespace net {

class URI
{
public:
    static void urlEncode(const String& fullPath, String& strRes);
};

}
}
