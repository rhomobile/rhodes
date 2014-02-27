#pragma once

#ifdef __cplusplus


#include "common/RhoStd.h"
#include "net/INetRequest.h"
#include "db/DBAdapter.h"
#include "sync/ILoginListener.h"
#include "logging/RhoLog.h"

namespace  rho {
	
namespace sync {

class IRhoconnectClient {
public:
	virtual ~IRhoconnectClient() {}
	
	virtual void doSyncSourceByName( const char* srcName ) = 0;
	virtual void doSyncAllSources( int show_status_popup, const char* query_params, int sync_only_changed_sources ) = 0;

        virtual bool clientRegisterHaveInstance() = 0;
	virtual void clientRegisterCreate( const char* szDevicePin ) = 0;
	virtual void clientRegisterCreate( ) = 0;
	virtual void clientRegisterDestroy() = 0;
	virtual void clientRegisterAddLoginListener( ILoginListener* listener ) = 0;
	virtual const String& clientRegisterGetDevicePin() = 0;
        virtual void clientRegisterSetDevicePin(const char* pin ) = 0;

	virtual void rho_sync_addobjectnotify_bysrcname(const char* szSrcName, const char* szObject) = 0;
	virtual String syncEnineReadClientID() = 0;
	virtual net::IRhoSession* getRhoSession() = 0;
	virtual void syncEngineApplyChangedValues(db::CDBAdapter& db) = 0;
	virtual void syncThreadCreate() = 0;
	virtual void syncThreadDestroy() = 0;
	virtual bool haveSyncThread() = 0;
	virtual unsigned int syncThreadGetThreadID() = 0;
	virtual bool syncEngineNotifyIsReportingEnabled() = 0;
	
	virtual int logged_in() = 0;
	virtual unsigned long login(const char* szLogin, const char* password, const char* callback) = 0;
	virtual void logout() = 0;
	virtual void stop() = 0;
	virtual int set_pollinterval( int interval ) = 0;
	virtual int get_pollinterval() = 0;
	virtual void set_syncserver( const char* syncserver ) = 0;
	virtual int get_pagesize() = 0;
	virtual void set_pagesize(int nPageSize) = 0;
	virtual int get_lastsync_objectcount(int nSrcID) = 0;
	virtual int issyncing() = 0;
	virtual void enable_status_popup(int b) = 0;
	virtual void set_threaded_mode(int b) = 0;
	virtual void register_push() = 0;
	virtual void set_ssl_verify_peer(int b) = 0;
	virtual void setobjectnotify_url(const char* szUrl) = 0;
	virtual void cleanobjectnotify() = 0;
	virtual void clear_notification(int srcID) = 0;

};

class RhoconnectClientManager {
	class DummyRhoconnectClientImpl;
	
	DEFINE_LOGCLASS;

	static IRhoconnectClient* m_pImpl;
	static DummyRhoconnectClientImpl s_dummyRhoconnectClientImpl;
	static bool m_bClientRegisterCreate;
	static String m_strClientRegisterDeviceId;
	static Vector<rho::sync::ILoginListener*> m_loginListeners;
	
public:
	static void setRhoconnectClientImpl(IRhoconnectClient* impl);
	static bool haveRhoconnectClientImpl();

	
	static void doSyncSourceByName(const char* srcName) { m_pImpl->doSyncSourceByName(srcName); }
	static void doSyncAllSources( int show_status_popup, const char* query_params, int sync_only_changed_sources ) { m_pImpl->doSyncAllSources(show_status_popup, query_params, sync_only_changed_sources ); }

        static bool clientRegisterHaveInstance() { return m_pImpl->clientRegisterHaveInstance(); }
	static void clientRegisterCreate( const char* szDevicePin );
	static void clientRegisterCreate();
	static void clientRegisterDestroy();
	static void clientRegisterAddLoginListener( ILoginListener* listener );
	static const String& clientRegisterGetDevicePin() { return m_pImpl->clientRegisterGetDevicePin(); }
	static void clientRegisterSetDevicePin(const char* pin ) { return m_pImpl->clientRegisterSetDevicePin(pin); }


	static void rho_sync_addobjectnotify_bysrcname(const char* szSrcName, const char* szObject) { m_pImpl->rho_sync_addobjectnotify_bysrcname(szSrcName, szObject); }
	static String syncEnineReadClientID() { return m_pImpl->syncEnineReadClientID(); }
	static net::IRhoSession* getRhoSession() { return m_pImpl->getRhoSession(); }
	static void syncEngineApplyChangedValues(db::CDBAdapter& db) { m_pImpl->syncEngineApplyChangedValues(db); }
	static bool haveSyncThread() { return m_pImpl->haveSyncThread(); }
	static unsigned int syncThreadGetThreadID() { return m_pImpl->syncThreadGetThreadID(); }
	static void syncThreadCreate() { m_pImpl->syncThreadCreate(); }
	static void syncThreadDestroy() { m_pImpl->syncThreadDestroy(); }
	static bool syncEngineNotifyIsReportingEnabled() { return m_pImpl->syncEngineNotifyIsReportingEnabled(); }
	static int rho_sync_issyncing() {
        if (m_pImpl == NULL) {
            return -1;
        }

        return m_pImpl->issyncing();
    }
	
	static int logged_in() { return m_pImpl->logged_in(); }
	static unsigned long login(const char* szLogin, const char* password, const char* callback) { return m_pImpl->login(szLogin,password,callback); }
	static void logout() { m_pImpl->logout(); }
	static void stop() { m_pImpl->stop(); }
	static int set_pollinterval( int interval ) { return m_pImpl->set_pollinterval(interval); }
	static int get_pollinterval() { return m_pImpl->get_pollinterval(); }
	static void set_syncserver( const char* syncserver ) { m_pImpl->set_syncserver(syncserver); }
	static int get_pagesize() { return m_pImpl->get_pagesize(); }
	static void set_pagesize(int nPageSize) { m_pImpl->set_pagesize(nPageSize); }
	static int get_lastsync_objectcount(int nSrcID) { return m_pImpl->get_lastsync_objectcount(nSrcID); }
	static int issyncing() { return m_pImpl->issyncing(); }
	static void enable_status_popup(int b) { m_pImpl->enable_status_popup(b); }
	static void set_threaded_mode(int b) { m_pImpl->set_threaded_mode(b); }
	static void register_push() { m_pImpl->register_push(); }
	static void set_ssl_verify_peer(int b) { m_pImpl->set_ssl_verify_peer(b); }
	static void setobjectnotify_url(const char* szUrl) { m_pImpl->setobjectnotify_url(szUrl); }
	static void cleanobjectnotify() { m_pImpl->cleanobjectnotify(); }
	static void clear_notification(int srcID) { m_pImpl->clear_notification(srcID); }
};

}
}

#endif

#ifdef __cplusplus
extern "C" {
#endif

	int rho_rcclient_have_rhoconnect_client();
	int rho_rcclient_issyncing();
	void rho_rcclient_doSyncSourceByName(const char* srcName);
	void rho_rcclient_doSyncAllSources(int show_status_popup, const char* query_params, int sync_only_changed_sources);
	void rho_clientregister_create(const char* szDevicePin);

#ifdef __cplusplus
}
#endif
