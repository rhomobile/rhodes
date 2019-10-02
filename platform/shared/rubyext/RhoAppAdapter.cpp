/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "common/RhoAppAdapter.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"
#include "sync/RhoconnectClientManager.h"

namespace rho 
{

const _CRhoAppAdapter& RhoAppAdapter = _CRhoAppAdapter();

/*static*/ String _CRhoAppAdapter::getMessageText(const char* szName)
{
	if ( (!sync::RhoconnectClientManager::haveRhoconnectClientImpl()) || (!sync::RhoconnectClientManager::syncEngineNotifyIsReportingEnabled()) )
        return String();
#ifndef RHO_NO_RUBY_API
    return rho_ruby_getMessageText(szName);
#else
    return String();
#endif
}

/*static*/ String _CRhoAppAdapter::getErrorText(int nError)
{
//    if ( sync::CSyncThread::getInstance()->isNoThreadedMode() || !sync::CSyncThread::getInstance()->getSyncEngine().getNotify().isReportingEnabled() )
	if ( (!sync::RhoconnectClientManager::haveRhoconnectClientImpl()) || (!sync::RhoconnectClientManager::syncEngineNotifyIsReportingEnabled()) )
        return String();
#ifndef RHO_NO_RUBY_API
    return rho_ruby_getErrorText(nError);
#else
    return String();
#endif
}

/*static*/ int  _CRhoAppAdapter::getErrorFromResponse(const NetResponse& resp)
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
#ifndef RHO_NO_RUBY_API
    if (rho_ruby_is_started())
        rho_ruby_loadserversources(strSources.c_str());
#endif
}

/*static*/ void  _CRhoAppAdapter::loadAllSyncSources()
{
#ifndef RHO_NO_RUBY_API
    if (rho_ruby_is_started())
        rho_ruby_loadallsyncsources();
#endif
}

/*static*/ const char* _CRhoAppAdapter::getRhoDBVersion()
{
#ifndef RHO_NO_RUBY_API
    if (rho_ruby_is_started())
	    return rho_ruby_getRhoDBVersion();
    else
#endif
        return "3.22.0";
}

/*static*/ void  _CRhoAppAdapter::resetDBOnSyncUserChanged()
{
#ifndef RHO_NO_RUBY_API
    if (rho_ruby_is_started())
        rho_ruby_reset_db_on_sync_user_changed();
#endif
}

/*static*/ bool  _CRhoAppAdapter::callCallbackOnSyncUserChanged()
{
    return RHODESAPP().getApplicationEventReceiver()->onSyncUserChanged(); 
}

} // end of rho
