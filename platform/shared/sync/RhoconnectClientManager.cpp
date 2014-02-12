#include "RhoconnectClientManager.h"

namespace rho {

namespace sync {

    class RhoconnectClientManager::DummyRhoconnectClientImpl : public IRhoconnectClient {
    public:
        virtual ~DummyRhoconnectClientImpl() {}

        virtual void doSyncSourceByName( const char* srcName ) {}
        virtual void doSyncAllSources( int show_status_popup, const char* query_params, int sync_only_changed_sources ) {}

        virtual bool clientRegisterHaveInstance() { return false; }
        virtual void clientRegisterCreate( const char* szDevicePin ) {}
        virtual void clientRegisterCreate( ) {}
        virtual void clientRegisterDestroy() {}
        virtual void clientRegisterAddLoginListener( ILoginListener* listener ) {}
        virtual const String& clientRegisterGetDevicePin() { return m_strClientRegisterDeviceId; }
        virtual void clientRegisterSetDevicePin(const char* pin ) {}

        virtual void rho_sync_addobjectnotify_bysrcname(const char* szSrcName, const char* szObject) {}
        virtual String syncEnineReadClientID() { return ""; }
        virtual net::IRhoSession* getRhoSession() { return 0; }
        virtual void syncEngineApplyChangedValues(db::CDBAdapter& db) {}
        virtual void syncThreadCreate() {}
        virtual void syncThreadDestroy() {}
        virtual bool haveSyncThread() { return false; }
        virtual unsigned int syncThreadGetThreadID() { return (unsigned int)-1; }
        virtual bool syncEngineNotifyIsReportingEnabled() { return false; }

        virtual int logged_in() { return 0; }
        virtual unsigned long login(const char* szLogin, const char* password, const char* callback) { return 0; }
        virtual void logout() {}
        virtual void stop() {}
        virtual int set_pollinterval( int interval ) { return 0; }
        virtual int get_pollinterval() { return 0; }
        virtual void set_syncserver( const char* syncserver ) {}
        virtual int get_pagesize() { return 0; }
        virtual void set_pagesize(int nPageSize) {}
        virtual int get_lastsync_objectcount(int nSrcID) { return 0; }
        virtual int issyncing() { return 0; }
        virtual void enable_status_popup(int b) {}
        virtual void set_threaded_mode(int b) {}
        virtual void register_push() {}
        virtual void set_ssl_verify_peer(int b) {}
        virtual void setobjectnotify_url(const char* szUrl) {}
        virtual void cleanobjectnotify() {}
        virtual void clear_notification(int srcID) {}
    };

    IMPLEMENT_LOGCLASS(RhoconnectClientManager, "RhoconnectClientManager");

    RhoconnectClientManager::DummyRhoconnectClientImpl RhoconnectClientManager::s_dummyRhoconnectClientImpl;

    IRhoconnectClient* RhoconnectClientManager::m_pImpl = &RhoconnectClientManager::s_dummyRhoconnectClientImpl;

    bool RhoconnectClientManager::m_bClientRegisterCreate = false;

    String RhoconnectClientManager::m_strClientRegisterDeviceId;

    Vector<rho::sync::ILoginListener*> RhoconnectClientManager::m_loginListeners;

    void RhoconnectClientManager::setRhoconnectClientImpl( IRhoconnectClient* impl )
    {
        LOG(INFO) + "setRhoconnectClientImpl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
        m_pImpl = impl;
        if(haveRhoconnectClientImpl())
        {
            for(Vector<rho::sync::ILoginListener*>::iterator It = m_loginListeners.begin(); It != m_loginListeners.end(); ++It)
            {
                m_pImpl->clientRegisterAddLoginListener(*It);
            }
            if(m_strClientRegisterDeviceId.length() != 0)
            {
                m_pImpl->clientRegisterCreate(m_strClientRegisterDeviceId.c_str());
            }
            else if(m_bClientRegisterCreate)
            {
                m_pImpl->clientRegisterCreate();
            }

        }
    }

    bool RhoconnectClientManager::haveRhoconnectClientImpl()
    {
        return ((m_pImpl != 0) && (m_pImpl != (&s_dummyRhoconnectClientImpl)));
    }

    void RhoconnectClientManager::clientRegisterCreate(const char* szDevicePin)
    {
        if(haveRhoconnectClientImpl())
        {
            m_pImpl->clientRegisterCreate(szDevicePin);
        }
        else
        {
            LOG(TRACE) + "Cache ClientRegister device PIN";
            m_strClientRegisterDeviceId = szDevicePin;
        }
    }
    void RhoconnectClientManager::clientRegisterCreate()
    {
        if(haveRhoconnectClientImpl())
        {
            m_pImpl->clientRegisterCreate();
        }
        else
        {
            LOG(TRACE) + "Cache ClientRegister create request";
            m_bClientRegisterCreate = true;
        }
    }
    void RhoconnectClientManager::clientRegisterDestroy()
    {
        if(haveRhoconnectClientImpl())
        {
            m_pImpl->clientRegisterDestroy();
        }
        else
        {
            m_bClientRegisterCreate = false;
            m_strClientRegisterDeviceId = "";
        }
    }
    void RhoconnectClientManager::clientRegisterAddLoginListener(ILoginListener* listener)
    {
        if(haveRhoconnectClientImpl())
        {
            m_pImpl->clientRegisterAddLoginListener(listener);
        }
        else
        {
            LOG(TRACE) + "Cache ClientRegister login listener";
            m_loginListeners.addElement(listener);
        }
    }
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
