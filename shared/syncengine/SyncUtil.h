#ifndef __SYNCUTIL__
#define __SYNCUTIL__

#include "SyncOperation.h"
#include "Source.h"

#if defined(__cplusplus)
extern "C" {
#endif

void finalize_src_statements();
int fetch_remote_changes(sqlite3 *database);
int push_remote_changes(pSyncOperation *list, int size);
int get_sources_from_database(pSource *list, sqlite3 *database, int max_size);

#if defined(__cplusplus)
}
#endif

#endif //__SYNCUTIL__