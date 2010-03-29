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
        return RhoRuby.ERR_NETWORK;

    if ( resp.isUnathorized() )
    	return RhoRuby.ERR_UNATHORIZED;

    if ( !resp.isOK() )
    	return RhoRuby.ERR_REMOTESERVER;

    return RhoRuby.ERR_NONE;
}

}
