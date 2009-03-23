#ifndef __SYNCBLOB__
#define __SYNCBLOB__

#include "SyncOperation.h"

#if defined(__cplusplus)
extern "C" {
#endif
/*
#define SyncBlob_SYNC_STAGE_NONE  0
#define SyncBlob_SYNC_STAGE  1
#define SyncBlob_POSTSYNC_STAGE  2
#define SyncBlob_REQUEST_STAGE  3
void SyncBlob_insertOp(pSyncObject ref, char *client_id, int stage);
*/

int SyncBlob_extractBlobs(pSyncOperation* op_list, int op_list_count, pSyncOperation** opblob_list);
int SyncBlob_pushRemoteBlobs(pSyncOperation *list, int size);
void SyncBlob_DeleteCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs);

#if defined(__cplusplus)
}
#endif

#endif //__SYNCBLOB__
