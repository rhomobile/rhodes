#pragma once

namespace rho {
namespace common {

struct IRhoBrowser
{
    virtual ~IRhoBrowser(void){;}

    virtual void navigateUrl(const String& szUrl) = 0;
};

}
}