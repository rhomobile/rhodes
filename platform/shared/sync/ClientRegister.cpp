#include "ClientRegister.h"
#include "sync/SyncThread.h"
#include "common/RhoConf.h"

namespace rho{
namespace sync{

using namespace rho::common;
using namespace rho::db;

#define THREAD_WAIT_TIMEOUT 10

IMPLEMENT_LOGCLASS(CClientRegister,"ClientRegister");

CClientRegister* CClientRegister::m_pInstance = 0;
	
/*static*/ CClientRegister* CClientRegister::Create(common::IRhoClassFactory* factory,const char* device_pin) 
{
	if ( m_pInstance ) 
		return m_pInstance;

	m_pInstance = new CClientRegister(factory, device_pin);
	return m_pInstance;
}

/*static*/ void CClientRegister::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CClientRegister::CClientRegister(common::IRhoClassFactory* factory,const char* device_pin) : CRhoThread(factory) 
{
	m_sysInfo = factory->createSystemInfo();

	m_strDevicePin = device_pin;
	m_NetRequest = factory->createNetRequest();
    m_nPollInterval = POLL_INTERVAL_SECONDS;

    delete factory;

    startUp();
}

CClientRegister::~CClientRegister()
{
	m_NetRequest->cancel();
	
    stop(WAIT_BEFOREKILL_SECONDS);
    m_pInstance = null;
}

void CClientRegister::startUp() 
{	
    if ( RHOCONF().getString("syncserver").length() > 0 )
    {
    	start(epLow);
        stopWait();
    }
}

void CClientRegister::run() 
{	
    LOG(INFO)+"ClientRegister start";
	while(!isStopping()) 
	{
        if ( CSyncThread::getInstance() != null )
		{
			if ( doRegister(CSyncThread::getSyncEngine()) )
				break;
		}
		
		LOG(INFO)+"Waiting for "+ m_nPollInterval+ " sec to try again to register client";
		wait(m_nPollInterval);
	}
    LOG(INFO)+"ClientRegister thread shutdown";
}

String CClientRegister::getRegisterBody(const String& strClientID)
{
	int port = RHOCONF().getInt("push_port");

    return CSyncThread::getSyncEngine().getProtocol().getClientRegisterBody( strClientID, m_strDevicePin, 
        port > 0 ? port : DEFAULT_PUSH_PORT, m_sysInfo->getPlatform());
}

boolean CClientRegister::doRegister(CSyncEngine& oSync) 
{
	String session = oSync.loadSession();
	if ( session.length() == 0 )
    {
        m_nPollInterval = POLL_INTERVAL_INFINITE;
		return false;
    }
    m_nPollInterval = POLL_INTERVAL_SECONDS;    		

	String client_id = oSync.loadClientID();
	if ( client_id.length() == 0 )
		return false;

    DBResult( res, CDBAdapter::getUserDB().executeSQL("SELECT token,token_sent from client_info") );
    if ( !res.isEnd() ) {
		String token = res.getStringByIdx(0); 
		int token_sent = res.getIntByIdx(1);
		if ( m_strDevicePin.compare(token) == 0 && token_sent > 0 ) 
		{
			//token in db same as new one and it was already send to the server
			//so we do nothing
			return true; 
		}
    }
	String strBody = getRegisterBody(client_id);
    NetResponse(resp, getNet().pushData( oSync.getProtocol().getClientRegisterUrl(), strBody, &oSync ));
	if( resp.isOK() )
    {
//				try {
			CDBAdapter::getUserDB().executeSQL("UPDATE client_info SET token_sent=?, token=?", 1, m_strDevicePin );
//				} catch(Exception ex) {
//					LOG.ERROR("Error saving token_sent to the DB...");
//				}	
		LOG(INFO)+"Registered client sucessfully...";
		return true;
	} else {
		LOG(INFO)+"Network error POST-ing device pin to the server...";
	}
	
	return false;
}

}
}
