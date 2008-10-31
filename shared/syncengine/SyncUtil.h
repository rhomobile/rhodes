#ifndef __SYNCUTIL__
#define __SYNCUTIL__

#include "SyncOperation.h"

#if defined(__cplusplus)
extern "C" {
#endif
	
typedef struct {
	int				_source_id;		/* id corresponding to source */
	char*			_source_url;	/* url for the source */
} Source;
	
typedef Source* pSource;
	
pSource SourceCreate(char *source_url, int source_id);

void finalize_src_statements();
int fetch_remote_changes(sqlite3 *database);
int push_remote_changes(pSyncOperation *list, int size);
int get_source_urls_from_database(pSource *list, sqlite3 *database, int max_size);
	
void free_source_list(pSource *list, int length);
	
void SourceRelease(pSource source);

#if defined(__cplusplus)
}
#endif

#endif //__SYNCUTIL__