#ifndef _RHORUBY_H_
#define _RHORUBY_H_

#include "common/RhoStd.h"
#include "net/INetRequest.h"
#include "ruby/ext/rho/rhoruby.h"
#include "common/RhoError.h"

namespace rho {
struct _CRhoRuby {

static String getMessageText(const char* szName);
static String getErrorText(int nError);
static int    getErrorFromResponse(net::INetResponse& resp);

};
extern const _CRhoRuby& RhoRuby;
}

#endif //_RHORUBY_H_
