/* syncengine.i */
%module SyncEngine
%{
/* Put header files here or function declarations like below */
extern void dosync();
extern void lock_sync_mutex();
extern void unlock_sync_mutex();
extern int login(const char *login, const char *password);
extern int logged_in();
extern void logout();
extern void trigger_sync_db_reset();
extern void set_notification(int source_id, const char *url, char* params);
extern void clear_notification(int source_id);
%}

extern void dosync();
extern void lock_sync_mutex();
extern void unlock_sync_mutex();
extern int login(const char *login, const char *password);
extern int logged_in();
extern void logout();
extern void trigger_sync_db_reset();
extern void set_notification(int source_id, const char *url, char* params);
extern void clear_notification(int source_id);
