#include "common/RhoAppAdapter.h"
#include "ruby/ext/rho/rhoruby.h"

namespace rho {
const _CRhoAppAdapter& RhoAppAdapter = _CRhoAppAdapter();

/*static*/ String _CRhoAppAdapter::getMessageText(const char* szName)
{
    return rho_ruby_getMessageText(szName);
}

/*static*/ String _CRhoAppAdapter::getErrorText(int nError)
{
    return rho_ruby_getErrorText(nError);
}

/*static*/ int  _CRhoAppAdapter::getErrorFromResponse(net::INetResponse& resp)
{
    if ( !resp.isResponseRecieved())
        return ERR_NETWORK;

    if ( resp.isUnathorized() )
    	return ERR_UNATHORIZED;

    if ( !resp.isOK() )
    	return ERR_REMOTESERVER;

    return ERR_NONE;
}

/*static*/ void  _CRhoAppAdapter::loadServerSources(const String& strSources)
{
    rho_ruby_loadserversources(strSources.c_str());
}

/*static*/ String _CRhoAppAdapter::getRhoDBVersion()
{
	return rho_ruby_getRhoDBVersion();
}
	
}
