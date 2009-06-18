/* syncengine.i */
%module SyncEngine
%{
/* Put header files here or function declarations like below */
	extern void rho_sync_doSyncAllSources();
	extern void rho_sync_lock();
	extern void rho_sync_unlock();
	extern int rho_sync_login(const char *login, const char *password);
	extern int rho_sync_logged_in();
	extern void rho_sync_logout();
	extern void rho_sync_db_reset();
	extern void rho_sync_set_notification(int source_id, const char *url, char* params);
	extern void rho_sync_clear_notification(int source_id);
	extern void rho_sync_set_pollinterval(int interval);
%}

extern void rho_sync_doSyncAllSources();
extern void rho_sync_lock();
extern void rho_sync_unlock();
extern int rho_sync_login(const char *login, const char *password);
extern int rho_sync_logged_in();
extern void rho_sync_logout();
extern void rho_sync_db_reset();
extern void rho_sync_set_notification(int source_id, const char *url, char* params);
extern void rho_sync_clear_notification(int source_id);
extern void rho_sync_set_pollinterval(int interval);
