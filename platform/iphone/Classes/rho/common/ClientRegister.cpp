#include "ClientRegister.h"
#include "common/IRhoThreadImpl.h"
#include "sync/SyncThread.h"
#include "common/RhoConf.h"

namespace rho{
namespace common{
using namespace rho::sync;

#define THREAD_WAIT_TIMEOUT 10

IMPLEMENT_LOGCLASS(CClientRegister,"RhoPushToken");

CClientRegister* CClientRegister::m_pInstance = 0;
	
/*static*/ CClientRegister* CClientRegister::create(IRhoClassFactory* factory,const char* device_pin) {
	if ( m_pInstance ) 
		return m_pInstance;
	m_pInstance = new CClientRegister(factory, device_pin);
	return m_pInstance;
}
	
CClientRegister::CClientRegister(common::IRhoClassFactory* factory,const char* device_pin) : CRhoThread(factory) {
	m_NetRequest = factory->createNetRequest();
	m_token = device_pin;
	m_set = true;
	start(epLow);
}

bool CClientRegister::post_token() {
	LOG(INFO)+"Running register client operation...";
	CSyncThread* pSync = CSyncThread::getInstance();
	if (pSync&&m_NetRequest) {
		db::CDBAdapter& db = pSync->getDBAdapter();		
		DBResult( res, db.executeSQL("SELECT client_id, token, token_sent from client_info limit 1") );
		if ( !res.isEnd() ) {
			String client_id = res.getStringByIdx(0);
			if (client_id.size()>0) {
				int token_sent = res.getIntByIdx(2);
				String token = res.getStringByIdx(1); 
				if ( (m_token.compare(token) == 0) && (token_sent == 0) ) {
					//token in db same as new one and it was already send to the server
					//so we do nothing
					return true; 
				}
				//Send token to the server
				String strUrl = rho_conf_getString("syncserver");
				if (strUrl.size() > 0) {
					String strBody = "client_id=" + client_id + "&device_pin=" + m_token + "&device_type=Iphone&device_port=0";
					if (m_NetRequest->pushData(strUrl + "clientregister", strBody)) {
						//save token db and set reset flag to false
						db.executeSQL( "UPDATE client_info SET reset=?, token=?", 0, m_token.c_str() );
						LOG(INFO)+"Registered client sucessfully...";
						return true;
					} else {
						LOG(INFO)+"Network error POST-ing device pin to the server...";
					}
				} else {
					LOG(ERROR)+"Can't register client because syncserver url is not configured...";
					return true; //stop client registration
				}
			}
		}
	}
	return false;
}		
	
void CClientRegister::run() {	
	int nWait = THREAD_WAIT_TIMEOUT;
	LOG(INFO)+"Started client registration...";
	while(true) {
		if (m_set && post_token()) {
			break;
		}
		LOG(INFO) + "Register client blocked for " + nWait + " seconds...";
		wait(nWait);
	}
	LOG(INFO)+"Stoped client registration...";
}

}
}

extern "C" { 
using namespace rho::common;	
void rho_client_register(const char* device_pin) {
    CClientRegister::create(new CRhoClassFactory, device_pin);
}
};
