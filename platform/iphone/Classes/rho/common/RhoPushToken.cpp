#include "RhoPushToken.h"
#include "common/IRhoThreadImpl.h"
#include "sync/SyncThread.h"
#include "common/RhoConf.h"

namespace rho{
namespace common{
using namespace rho::sync;

#define THREAD_WAIT_TIMEOUT 10

IMPLEMENT_LOGCLASS(CRhoPushToken,"RhoPushToken");

CRhoPushToken* CRhoPushToken::m_pInstance = 0;
	
/*static*/ CRhoPushToken* CRhoPushToken::create(IRhoClassFactory* factory) {
	if ( m_pInstance ) 
		return m_pInstance;
		
	m_pInstance = new CRhoPushToken(factory);
	return m_pInstance;
}
	
CRhoPushToken::CRhoPushToken(common::IRhoClassFactory* factory) : CRhoThread(factory), m_set(false) {
	m_NetRequest = factory->createNetRequest();
}

bool CRhoPushToken::post_token() {
	LOG(INFO)+"Running push token operation...";
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
				String strBody = "client_id=" + client_id + "&device_pin=" + m_token + "&device_type=iPhone&device_port=0";
				String strUrl = rho_conf_getString("syncserver");
				if (m_NetRequest->pushData(strUrl + "clientregister", strBody)) {
					//save token db and set reset flag to false
					db.executeSQL( "UPDATE client_info SET reset=?, token=?", 0, m_token.c_str() );	
					return true;
				}
			}
		}
	}
	return false;
}		
	
void CRhoPushToken::run() {	
	int nWait = THREAD_WAIT_TIMEOUT;
	while(true) {
		if (m_set && post_token()) {
			break;
		}
		LOG(INFO) + "Token blocked for " + nWait + " seconds...";
		wait(nWait);
	}
}

void CRhoPushToken::set(char* data) { 
	m_token = data;
	m_set = true;
	//TBD: check if thread is already started
	start(epLow);
}
	
}
}

extern "C" { 
using namespace rho::common;	
void rho_push_token_create() {
    CRhoPushToken::create(new CRhoClassFactory);
}
void rho_push_token_set(char* data) {
	CRhoPushToken::getInstance()->set(data);
}	
};
