#ifndef __SYNCUTIL__
#define __SYNCUTIL__

#if defined(__cplusplus)
extern "C" {
#endif

int fetch_remote_changes(pSyncObject *list);
int push_remote_changes(pSyncOperation *list, int size);

#if defined(__cplusplus)
}
#endif

#endif //__SYNCUTIL__