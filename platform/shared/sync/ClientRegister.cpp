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

#include "ClientRegister.h"
#include "SyncThread.h"
#include "ILoginListener.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"

rho::String rho_sysimpl_get_phone_id();

namespace rho{
namespace sync{

using namespace rho::common;
using namespace rho::db;

#define THREAD_WAIT_TIMEOUT 10

IMPLEMENT_LOGCLASS(CClientRegister,"ClientRegister");

CClientRegister* CClientRegister::m_pInstance = 0;
bool CClientRegister::s_sslVerifyPeer = true;
VectorPtr<ILoginListener*> CClientRegister::s_loginListeners;
	
/*static*/ CClientRegister* CClientRegister::Get()
{
    if (!m_pInstance)
    {
        m_pInstance = new CClientRegister();
        m_pInstance->m_strDevicePin = QueryDevicePin();
    }
    return m_pInstance;
}

/*static*/ CClientRegister* CClientRegister::Create()
{
    if (!m_pInstance)
    {
        m_pInstance = new CClientRegister();
        String devicePin = QueryDevicePin();
        if (devicePin.length() == 0)
        {
            String session = CSyncThread::getSyncEngine().loadSession();
            if (session.length() > 0)
            {
                m_pInstance->setRhoconnectCredentials("", "", session);
            }
        }
        m_pInstance->setDevicehPin(devicePin);
    }
    return m_pInstance;
}

/*static*/ CClientRegister* CClientRegister::Create(const char* pin)
{
    Get()->setDevicehPin(pin);
    return m_pInstance;
}

/*static*/ void CClientRegister::Stop()
{
    if(m_pInstance)
    {
        m_pInstance->doStop();
    }
}

/*static*/ void CClientRegister::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

/*static*/ void CClientRegister::SetSslVerifyPeer(boolean b)
{
    s_sslVerifyPeer = b;
    if (m_pInstance)
        m_pInstance->m_NetRequest.setSslVerifyPeer(b);
}


/*static*/void CClientRegister::AddLoginListener(ILoginListener* listener)
{
    s_loginListeners.addElement(listener);
}

/*static*/ String CClientRegister::QueryDevicePin()
{
    IDBResult res = CDBAdapter::getUserDB().executeSQL("SELECT token from client_info");
    return res.isEnd() ? "" : res.getStringByIdx(0);
}

CClientRegister::CClientRegister() : m_nPollInterval(POLL_INTERVAL_SECONDS)
{
    m_NetRequest.setSslVerifyPeer(s_sslVerifyPeer);

}

CClientRegister::~CClientRegister()
{
    doStop();
    m_pInstance = null;
}
void CClientRegister::setRhoconnectCredentials(const String& user, const String& pass, const String& session)
{
    LOG(INFO) + "New Sync credentials - user: " + user + ", sess: " + session;

    doStop();
    reset();
    for(VectorPtr<ILoginListener*>::iterator I = s_loginListeners.begin(); I != s_loginListeners.end(); ++I)
    {
        (*I)->onLogin(user, pass, session);
    }
}

void CClientRegister::dropRhoconnectCredentials(const String& session)
{
    doStop();
    for(VectorPtr<ILoginListener*>::iterator I = s_loginListeners.begin(); I != s_loginListeners.end(); ++I)
    {
        (*I)->onLogout(session);
    }
    reset();
}

void CClientRegister::setDevicehPin(const String& pin)
{
    m_strDevicePin = pin;
    startUp();
}

void CClientRegister::startUp()
{
    if(isAlive() || isWaiting())
    {
        doStop();
    }
    if ( RHOCONF().getString("syncserver").length() > 0 )
    {
        LOG(INFO) + "Starting ClientRegister...";

        start(epLow);
        stopWait();
    }
}

void CClientRegister::run()
{
    unsigned i = 0;
    LOG(INFO)+"ClientRegister is started";
	while(!isStopping()) 
	{
	    i++;
        LOG(INFO)+"Try to register: " + i;
        if ( CSyncThread::getInstance() != null )
		{
			if ( doRegister(CSyncThread::getSyncEngine()) )
			{
			    LOG(INFO)+"Registered: " + i;
				break;
			}
		} else
		    LOG(INFO)+"SyncThread is not ready";
		
		LOG(INFO)+"Waiting for "+ m_nPollInterval+ " sec to try again to register client";
		wait(m_nPollInterval);
	}
    LOG(INFO)+"ClientRegister thread shutdown";
}

String CClientRegister::getRegisterBody(const String& strClientID)
{
	IRhoPushClient* pushClient = RHODESAPP().getDefaultPushClient();
	int port = RHOCONF().getInt("push_port");

	String body = CSyncThread::getSyncEngine().getProtocol().getClientRegisterBody( strClientID, m_strDevicePin,
        port > 0 ? port : DEFAULT_PUSH_PORT, rho_rhodesapp_getplatform(), rho_sysimpl_get_phone_id(),
        /*device_push_type*/ (0 != pushClient) ? pushClient->getType() : "" /*it means native push type*/);

	LOG(INFO)+"getRegisterBody() BODY is: " + body;
	return body;

	/*
    if(m_isAns)
		body = CSyncThread::getSyncEngine().getProtocol().getClientAnsRegisterBody( strClientID, m_strDevicePin,
			port > 0 ? port : DEFAULT_PUSH_PORT, rho_rhodesapp_getplatform(), rho_sysimpl_get_phone_id() );
	else
		body = CSyncThread::getSyncEngine().getProtocol().getClientRegisterBody( strClientID, m_strDevicePin,
			port > 0 ? port : DEFAULT_PUSH_PORT, rho_rhodesapp_getplatform(), rho_sysimpl_get_phone_id() );
	*/

}

boolean CClientRegister::doRegister(CSyncEngine& oSync)
{
    String session = oSync.loadSession();
    if ( session.length() == 0 )
    {
        m_nPollInterval = POLL_INTERVAL_INFINITE;
        LOG(INFO)+"Session is empty, do register later";
        return false;
    }
    if ( m_strDevicePin.length() == 0 )
    {
        m_nPollInterval = POLL_INTERVAL_INFINITE;
        LOG(INFO)+"Device PUSH pin is empty, do register later";
        return false;
    }
    m_nPollInterval = POLL_INTERVAL_SECONDS;

	String client_id = oSync.loadClientID();
	if ( client_id.length() == 0 )
	{
        LOG(WARNING)+"Sync client_id is empty, do register later";
		return false;
	}

    IDBResult res = CDBAdapter::getUserDB().executeSQL("SELECT token,token_sent from client_info");
    if ( !res.isEnd() ) {
		String token = res.getStringByIdx(0); 
		int token_sent = res.getIntByIdx(1) && !RHOCONF().getBool("register_push_at_startup");
		if ( m_strDevicePin.compare(token) == 0 && token_sent > 0 ) 
		{
			//token in db same as new one and it was already send to the server
			//so we do nothing
			return true; 
		}
    }
	String strBody = getRegisterBody(client_id);
    NetResponse resp = getNet().pushData( oSync.getProtocol().getClientRegisterUrl(), strBody, &oSync );
	if( resp.isOK() )
    {
//				try {
			CDBAdapter::getUserDB().executeSQL("UPDATE client_info SET token_sent=?, token=?", 1, m_strDevicePin );
//				} catch(Exception ex) {
//					LOG.ERROR("Error saving token_sent to the DB...");
//				}	
		LOG(INFO)+"Registered client sucessfully...";
		return true;
	}

	LOG(WARNING)+"Network error: "+ resp.getRespCode();
	return false;
}

void CClientRegister::doStop()
{
    LOG(INFO) + "Stopping ClientRegister...";

    m_NetRequest.cancel();
    stop(WAIT_BEFOREKILL_SECONDS);
}

void CClientRegister::reset()
{
    m_nPollInterval = POLL_INTERVAL_SECONDS;
    m_strDevicePin = "";
}

}
}
