#ifndef __SYNCUTIL__
#define __SYNCUTIL__

#include "SyncOperation.h"
#include "Source.h"
#include "Utils.h"

#if defined(__cplusplus)
extern "C" {
#endif

void finalize_sync_util_statements();
int fetch_remote_changes(sqlite3 *database, char *client_id);
int push_remote_changes(pSyncOperation *list, int size);
int get_sources_from_database(pSource *list, sqlite3 *database, int max_size);
int get_object_count_from_database(sqlite3 *database);
char *set_client_id(sqlite3 *database, pSource source);
void insert_sync_status(sqlite3 *database, const char *status);

/**
 * Retrieve cookie from database storage
 */
char *get_db_session(char* source_url);

/**
 * Save cookie to the database storage
 */
int set_db_session(char* source_url, char * session);

#if defined(__cplusplus)
}
#endif

#endif //__SYNCUTIL__