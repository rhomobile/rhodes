#ifndef __locking_h__
#define __locking_h__

#define _LK_UNLCK       0       /* unlock the file region */
#define _LK_LOCK        1       /* lock the file region */
#define _LK_NBLCK       2       /* non-blocking lock */
#define _LK_RLCK        3       /* lock for writing */
#define _LK_NBRLCK      4       /* non-blocking lock for writing */

#if !__STDC__
	/* Non-ANSI names for compatibility */
#define LK_UNLCK        _LK_UNLCK
#define LK_LOCK         _LK_LOCK
#define LK_NBLCK        _LK_NBLCK
#define LK_RLCK         _LK_RLCK
#define LK_NBRLCK       _LK_NBRLCK
#endif

#ifdef __cplusplus
extern "C" {
#endif

int _wcelocking (int _FileHandle, int _LockMode, long _NumOfBytes);
#define locking _wcelocking
#define _locking _wcelocking

#ifdef __cplusplus
}
#endif

#endif