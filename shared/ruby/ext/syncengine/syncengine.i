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
%}

extern void dosync();
extern void lock_sync_mutex();
extern void unlock_sync_mutex();
extern int login(const char *login, const char *password);
extern int logged_in();
extern void logout();
