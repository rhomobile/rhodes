#include "RhoPushToken.h"
#include "common/IRhoThreadImpl.h"
#include "sync/SyncThread.h"

namespace rho{
namespace common{
using namespace rho::sync;

#define THREAD_WAIT_TIMEOUT 60

IMPLEMENT_LOGCLASS(CRhoPushToken,"RhoPushToken");

CRhoPushToken* CRhoPushToken::m_pInstance = 0;
	
/*static*/ CRhoPushToken* CRhoPushToken::create(IRhoClassFactory* factory) {
	if ( m_pInstance ) 
		return m_pInstance;
		
	m_pInstance = new CRhoPushToken(factory);
	return m_pInstance;
}
	
CRhoPushToken::CRhoPushToken(common::IRhoClassFactory* factory) : CRhoThread(factory), m_set(false) {
}

bool CRhoPushToken::post_token() {
	LOG(INFO)+"Running push token operation...";
	CSyncThread* pSync = CSyncThread::getInstance();
	if (pSync) {
		db::CDBAdapter& db = pSync->getDBAdapter();
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
