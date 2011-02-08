#include "common/RhoAppAdapter.h"
#include "sync/SyncThread.h"
#include "ruby/ext/rho/rhoruby.h"

namespace rho {
const _CRhoAppAdapter& RhoAppAdapter = _CRhoAppAdapter();

/*static*/ String _CRhoAppAdapter::getMessageText(const char* szName)
{
    if ( sync::CSyncThread::getInstance()->isNoThreadedMode() || !sync::CSyncThread::getInstance()->getSyncEngine().getNotify().isReportingEnabled() )
        return String();

    return rho_ruby_getMessageText(szName);
}

/*static*/ String _CRhoAppAdapter::getErrorText(int nError)
{
    if ( sync::CSyncThread::getInstance()->isNoThreadedMode() || !sync::CSyncThread::getInstance()->getSyncEngine().getNotify().isReportingEnabled() )
        return String();

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

/*static*/ void  _CRhoAppAdapter::loadAllSyncSources()
{
    rho_ruby_loadallsyncsources();
}

/*static*/ const char* _CRhoAppAdapter::getRhoDBVersion()
{
	return rho_ruby_getRhoDBVersion();
}

/*static*/ void  _CRhoAppAdapter::resetDBOnSyncUserChanged()
{
    rho_ruby_reset_db_on_sync_user_changed();
}

}
