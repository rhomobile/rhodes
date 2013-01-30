#include "RhoconnectClientManager.h"

namespace rho {

namespace sync {
	RhoconnectClientManager::DummyRhoconnectClientImpl RhoconnectClientManager::s_dummyRhoconnectClientImpl;
	
	IRhoconnectClient* RhoconnectClientManager::m_pImpl = &RhoconnectClientManager::s_dummyRhoconnectClientImpl;
}
	
}

extern "C" {
	
	int rho_rcclient_have_rhoconnect_client() {
		return rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()?1:0;
	}
	
	int rho_rcclient_issyncing() {
		return rho::sync::RhoconnectClientManager::rho_sync_issyncing();
	}
	
	void rho_rcclient_doSyncSourceByName(const char* srcName) {
		rho::sync::RhoconnectClientManager::doSyncSourceByName(srcName);
	}
	
	void rho_rcclient_doSyncAllSources( int show_status_popup, const char* query_params, int sync_only_changed_sources ) {
		rho::sync::RhoconnectClientManager::doSyncAllSources(show_status_popup, query_params, sync_only_changed_sources);
	}
}