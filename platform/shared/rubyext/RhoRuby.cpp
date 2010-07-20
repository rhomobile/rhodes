#include "RhoRuby.h"
#include "ruby/ext/rho/rhoruby.h"

namespace rho {
const _CRhoRuby& RhoRuby = _CRhoRuby();

/*static*/ String _CRhoRuby::getMessageText(const char* szName)
{
    return rho_ruby_getMessageText(szName);
}

/*static*/ String _CRhoRuby::getErrorText(int nError)
{
    return rho_ruby_getErrorText(nError);
}

/*static*/ int  _CRhoRuby::getErrorFromResponse(net::INetResponse& resp)
{
    if ( !resp.isResponseRecieved())
        return RHO_ERR_NETWORK;

    if ( resp.isUnathorized() )
    	return RHO_ERR_UNATHORIZED;

    if ( !resp.isOK() )
    	return RHO_ERR_REMOTESERVER;

    return RHO_ERR_NONE;
}

}

extern "C" char* rho_error_getErrorText(int nError)
{
    return strdup(rho::RhoRuby.getErrorText(nError).c_str());
}
