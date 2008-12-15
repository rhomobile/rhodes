/*
** 2008 February 09
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This file contains code that is specific to windows.
*/
#include "sqliteInt.h"
#if OS_SYMBIAN               /* This file is used for symbian only */

#define MAX_PATH 260
/*
** A Note About Memory Allocation:
**
** This driver uses malloc()/free() directly rather than going through
** the SQLite-wrappers sqlite3_malloc()/sqlite3_free().  Those wrappers
** are designed for use on embedded systems where memory is scarce and
** malloc failures happen frequently.  Win32 does not typically run on
** embedded systems, and when it does the developers normally have bigger
** problems to worry about than running out of memory.  So there is not
** a compelling need to use the wrappers.
**
** But there is a good reason to not use the wrappers.  If we use the
** wrappers then we will get simulated malloc() failures within this
** driver.  And that causes all kinds of problems for our tests.  We
** could enhance SQLite to deal with simulated malloc failures within
** the OS driver, but the code to deal with those failure would not
** be exercised on Linux (which does not need to malloc() in the driver)
** and so we would have difficulty writing coverage tests for that
** code.  Better to leave the code out, we think.
**
** The point of this discussion is as follows:  When creating a new
** OS layer for an embedded system, if you use this file as an example,
** avoid the use of malloc()/free().  Those routines work ok on windows
** desktops but not so well in embedded systems.
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <e32std.h>
#include <f32file.h>
#include <charconv.h>
#include <bautils.h>
#include <unistd.h>

/*
** Macros used to determine whether or not to use threads.
*/
#if defined(THREADSAFE) && THREADSAFE
# define SQLITE_W32_THREADS 1
#endif

/*
** Include code that is common to all os_*.c files
*/
#include "os_common.h"

/*
** The symbianFile structure is a subclass of sqlite3_file* specific to the win32
** portability layer.
*/

typedef struct symbianFile symbianFile;
struct symbianFile {
	int isOpen;
  unsigned char locktype; /* Type of lock currently held on this file */
  short sharedLockByte;   /* Randomly chosen byte used as a shared lock */
  char fileName[512];
  RFs session;
  RFile file;
};

/*****************************************************************************
** The next group of routines implement the I/O methods specified
** by the sqlite3_io_methods object.
******************************************************************************/

/*
** Close a file.
**
** It is reported that an attempt to close a handle might sometimes
** fail.  This is a very unreasonable result, but windows is notorious
** for being unreasonable so I do not doubt that it might happen.  If
** the close fails, we pause for 100 milliseconds and try again.  As
** many as MX_CLOSE_ATTEMPT attempts to close the handle are made before
** giving up and returning an error.
*/
#define MX_CLOSE_ATTEMPT 3
int winClose(sqlite3_file *id){
  int rc, cnt = 0;
  symbianFile *pFile = (symbianFile*)id;
  pFile->file.Close();
  pFile->session.Close();
  return SQLITE_OK;
}

/*
** Some microsoft compilers lack this definition.
*/
#ifndef INVALID_SET_FILE_POINTER
# define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif

/*
** Read data from a file into a buffer.  Return SQLITE_OK if all
** bytes were read successfully and SQLITE_IOERR if anything goes
** wrong.
*/
int winRead(
  sqlite3_file *id,          /* File to read from */
  void *pBuf,                /* Write content into this buffer */
  int amt,                   /* Number of bytes to read */
  sqlite3_int64 offset       /* Begin reading at this offset */
){
  int rc;
  size_t got;
  symbianFile *pFile = (symbianFile*)id;
  assert( id!=0 );
  SimulateIOError(return SQLITE_IOERR_READ);
  TInt tOffset = (TInt)offset;
  rc = pFile->file.Seek(ESeekStart, tOffset);
  if( rc!= KErrNone){
    return SQLITE_FULL;
  }

  HBufC8* buf = HBufC8::NewL(amt) ;
  TPtr8 ptr = buf->Des();
  TInt readError = pFile->file.Read(ptr, amt);
  if ( readError != KErrNone)
	  {
	    delete buf;
	    return SQLITE_IOERR_READ;
	  }

  got = buf->Length();

  if( got == 0 ){
	  delete buf;
	  TInt size = 0;
	  if (pFile->file.Size(size) != KErrNone) 
	  {
		  return SQLITE_IOERR_READ;
	  }
	  if (size == 0)
	  {
		  return SQLITE_IOERR_SHORT_READ;
	  }
    return SQLITE_IOERR_READ;
 }
  memcpy(pBuf, ptr.Ptr(), got);
  delete buf;
 if( got == amt ){
    return SQLITE_OK;
  }else{
    memset(&((char*)pBuf)[got], 0, amt-got);
    return SQLITE_IOERR_SHORT_READ;
  }
}

/*
** Write data from a buffer into a file.  Return SQLITE_OK on success
** or some other error code on failure.
*/
int winWrite(
  sqlite3_file *id,         /* File to write into */
  const void *pBuf,         /* The bytes to be written */
  int amt,                  /* Number of bytes to write */
  sqlite3_int64 offset      /* Offset into the file to begin writing at */
){
  int rc;
  symbianFile *pFile = (symbianFile*)id;
  assert( id!=0 );
  SimulateIOError(return SQLITE_IOERR_WRITE);
  SimulateDiskfullError(return SQLITE_FULL);
  TInt tOffset = (TInt)offset;
  rc = pFile->file.Seek(ESeekStart, tOffset);
  if( rc!= KErrNone){
    return SQLITE_FULL;
  }

  assert( amt>0 );
  
  rc = SQLITE_OK;
  TPtrC8 ptr((TUint8 *)pBuf,amt);

  if (pFile->file.Write(ptr, amt) != KErrNone) rc = SQLITE_FULL;

  return SQLITE_OK;
}

/*
** Truncate an open file to a specified size
*/
int winTruncate(sqlite3_file *id, sqlite3_int64 nByte){
  symbianFile *pFile = (symbianFile*)id;

  if (pFile->file.SetSize(nByte) != KErrNone)
  {
	  return SQLITE_IOERR; 
  }

  return SQLITE_OK;
}

#ifdef SQLITE_TEST
/*
** Count the number of fullsyncs and normal syncs.  This is used to test
** that syncs and fullsyncs are occuring at the right times.
*/
int sqlite3_sync_count = 0;
int sqlite3_fullsync_count = 0;
#endif

/*
** Make sure all writes to a particular file are committed to disk.
*/
int winSync(sqlite3_file *id, int flags){
  symbianFile *pFile = (symbianFile*)id;
  OSTRACE3("SYNC %d lock=%d\n", pFile->h, pFile->locktype);
#ifdef SQLITE_TEST
  if( flags & SQLITE_SYNC_FULL ){
    sqlite3_fullsync_count++;
  }
  sqlite3_sync_count++;
#endif
  
  TInt retval = pFile->file.Flush();
  if (retval != KErrNone)
  {
  	return SQLITE_IOERR;
  }else{
  	return SQLITE_OK;
  }
}

/*
** Determine the current size of a file in bytes
*/
int winFileSize(sqlite3_file *id, sqlite3_int64 *pSize){
  symbianFile *pFile = (symbianFile*)id;

  TInt size = 0;
  if (pFile->file.Size(size) != KErrNone)
  {
	  return SQLITE_IOERR;
  }

  *pSize = size;

  return SQLITE_OK;
}


/*
** Lock the file with the lock specified by parameter locktype - one
** of the following:
**
**     (1) SHARED_LOCK
**     (2) RESERVED_LOCK
**     (3) PENDING_LOCK
**     (4) EXCLUSIVE_LOCK
**
** Sometimes when requesting one lock state, additional lock states
** are inserted in between.  The locking might fail on one of the later
** transitions leaving the lock state different from what it started but
** still short of its goal.  The following chart shows the allowed
** transitions and the inserted intermediate states:
**
**    UNLOCKED -> SHARED
**    SHARED -> RESERVED
**    SHARED -> (PENDING) -> EXCLUSIVE
**    RESERVED -> (PENDING) -> EXCLUSIVE
**    PENDING -> EXCLUSIVE
**
** This routine will only increase a lock.  The winUnlock() routine
** erases all locks at once and returns us immediately to locking level 0.
** It is not possible to lower the locking level one step at a time.  You
** must go straight to locking level 0.
*/
int winLock(sqlite3_file *id, int locktype){
  int rc = SQLITE_OK;    /* Return code from subroutines */
  int res = 1;           /* Result of a windows lock call */
  int newLocktype;       /* Set pFile->locktype to this value before exiting */
  int gotPendingLock = 0;/* True if we acquired a PENDING lock this time */
  symbianFile *pFile = (symbianFile*)id;

  assert( pFile!=0 );
  OSTRACE5("LOCK %d %d was %d(%d)\n",
          pFile->h, locktype, pFile->locktype, pFile->sharedLockByte);

  // one smartphone only one application can control the database

  TInt size = 0;
  if (pFile->file.Size(size) == KErrNone) return SQLITE_OK;

  return SQLITE_BUSY;
}

/*
** This routine checks if there is a RESERVED lock held on the specified
** file by this or any other process. If such a lock is held, return
** non-zero, otherwise zero.
*/
int winCheckReservedLock(sqlite3_file *id){
  int rc;
  symbianFile *pFile = (symbianFile*)id;
  assert( pFile!=0 );
  if( pFile->locktype>=RESERVED_LOCK ){
    rc = 1;
    OSTRACE3("TEST WR-LOCK %d %d (local)\n", pFile->h, rc);
  }else{
	  TInt size = 0;
	  if (pFile->file.Size(size) == KErrNone) rc = 1;
    OSTRACE3("TEST WR-LOCK %d %d (remote)\n", pFile->h, rc);
  }
  return rc;
}

/*
** Lower the locking level on file descriptor id to locktype.  locktype
** must be either NO_LOCK or SHARED_LOCK.
**
** If the locking level of the file descriptor is already at or below
** the requested locking level, this routine is a no-op.
**
** It is not possible for this routine to fail if the second argument
** is NO_LOCK.  If the second argument is SHARED_LOCK then this routine
** might return SQLITE_IOERR;
*/
int winUnlock(sqlite3_file *id, int locktype){
  int type;
  symbianFile *pFile = (symbianFile*)id;
  int rc = SQLITE_OK;
  assert( pFile!=0 );
  return rc;
}

/*
** Control and query of the open file handle.
*/
int winFileControl(sqlite3_file *id, int op, void *pArg){
  switch( op ){
    case SQLITE_FCNTL_LOCKSTATE: {
      *(int*)pArg = ((symbianFile*)id)->locktype;
      return SQLITE_OK;
    }
  }
  return SQLITE_ERROR;
}

/*
** Return the sector size in bytes of the underlying block device for
** the specified file. This is almost always 512 bytes, but may be
** larger for some devices.
**
** SQLite code assumes this function cannot fail. It also assumes that
** if two files are created in the same file-system directory (i.e.
** a database and its journal file) that the sector size will be the
** same for both.
*/
int winSectorSize(sqlite3_file *id){
  return SQLITE_DEFAULT_SECTOR_SIZE;
}

/*
** Return a vector of device characteristics.
*/
int winDeviceCharacteristics(sqlite3_file *id){
  return 0;
}


/***************************************************************************
** Here ends the I/O methods that form the sqlite3_io_methods object.
**
** The next block of code implements the VFS methods.
****************************************************************************/

void ConvertToUnicode(RFs session, TDes16& aUnicode, const char *str)
{
  CCnvCharacterSetConverter *converter = CCnvCharacterSetConverter::NewL();
  converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierUtf8, session);
  
  TPtrC8 ptr((const unsigned char*)str);

  int state = CCnvCharacterSetConverter::KStateDefault;
  converter->ConvertToUnicode(aUnicode, ptr, state);
}

/*
** Open a file.
*/
int winOpen(
  sqlite3_vfs *pVfs,        /* Not used */
  const char *zName,        /* Name of the file (UTF-8) */
  sqlite3_file *id,         /* Write the SQLite file handle here */
  int flags,                /* Open mode flags */
  int *pOutFlags            /* Status return flags */
){
  symbianFile *pFile = (symbianFile*)id;
  TBuf16<MAX_PATH> filename;
  TInt nPos = 1; 
  _LIT16(Slash,"\\");
  _LIT16(BackSlash,"/");
  
  pFile->isOpen = 0;
  memset(pFile, 0, sizeof(*pFile));
  strcpy(pFile->fileName, zName);
  pFile->session.Connect();
  pFile->session.ShareAuto();
  
  ConvertToUnicode(pFile->session, filename, zName);

  while ( nPos != KErrNotFound )
  {
  	nPos = filename.Find(BackSlash);
  	if ( nPos != KErrNotFound )
  		filename.Replace( nPos, 1, Slash );
  }
  
  int ret = 0;
  if( flags & SQLITE_OPEN_CREATE ){
    if (BaflUtils::FileExists(pFile->session, filename) == 1)
    	{
  	  ret = pFile->file.Open(pFile->session, filename, EFileStream | EFileWrite | EFileShareAny);
    	}
    else
    	{
    	ret = pFile->file.Create(pFile->session, filename, EFileStream | EFileWrite | EFileShareAny);
    	}
  }
  else
  if( flags & SQLITE_OPEN_READWRITE ){
	  ret = pFile->file.Open(pFile->session, filename, EFileStream | EFileWrite | EFileShareAny);
  }else{
	ret = pFile->file.Open(pFile->session, filename, EFileStream | EFileRead | EFileShareAny);
  }

  OpenCounter(+1);

  if (ret != KErrNone)
  {
	  return SQLITE_IOERR;
  }

  pFile->isOpen = 1;
  return SQLITE_OK;
}

/*
** Delete the named file.
**
** Note that windows does not allow a file to be deleted if some other
** process has it open.  Sometimes a virus scanner or indexing program
** will open a journal file shortly after it is created in order to do
** whatever does.  While this other process is holding the
** file open, we will be unable to delete it.  To work around this
** problem, we delay 100 milliseconds and try to delete again.  Up
** to MX_DELETION_ATTEMPTs deletion attempts are run before giving
** up and returning an error.
*/
#define MX_DELETION_ATTEMPTS 5
int winDelete(
  sqlite3_vfs *pVfs,          /* Not used on win32 */
  const char *zFilename,      /* Name of file to delete */
  int syncDir                 /* Not used on win32 */
){
  SimulateIOError(return SQLITE_IOERR_DELETE);
  TBuf16<MAX_PATH> filename;

  RFs session;
  session.Connect();
  ConvertToUnicode(session, filename, zFilename);
  BaflUtils::DeleteFile(session, filename);
  OSTRACE2("DELETE \"%s\"\n", zFilename);
  session.Close();
  return SQLITE_OK;
}

/*
** Check the existance and status of a file.
*/
int winAccess(
  sqlite3_vfs *pVfs,         /* Not used on win32 */
  const char *zFilename,     /* Name of file to check */
  int flags                  /* Type of test to make on this file */
){
  TBuf16<MAX_PATH> filename;

  RFs session;
  session.Connect();
  ConvertToUnicode(session, filename, zFilename);
  int ret = BaflUtils::FileExists(session, filename);
  session.Close();

  return ret;
}


/*
** Create a temporary file name in zBuf.  zBuf must be big enough to
** hold at pVfs->mxPathname characters.
*/
int winGetTempname(sqlite3_vfs *pVfs, int nBuf, char *zBuf){
  static char zChars[] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789";
  int i, j;
  char zTempPath[MAX_PATH+1];
  if( sqlite3_temp_directory ){
    sqlite3_snprintf(MAX_PATH-30, zTempPath, "%s", sqlite3_temp_directory);
  }
  else
  {
  }
  
  for(i=strlen(zTempPath); i>0 && zTempPath[i-1]=='\\'; i--){}
  zTempPath[i] = 0;
  sqlite3_snprintf(nBuf-30, zBuf,
                   "%s\\"SQLITE_TEMP_FILE_PREFIX, zTempPath);
  j = strlen(zBuf);
  sqlite3Randomness(20, &zBuf[j]);
  for(i=0; i<20; i++, j++){
    zBuf[j] = (char)zChars[ ((unsigned char)zBuf[j])%(sizeof(zChars)-1) ];
  }
  zBuf[j] = 0;
  OSTRACE2("TEMP FILENAME: %s\n", zBuf);
  return SQLITE_OK; 
}

/*
** Turn a relative pathname into a full pathname.  Write the full
** pathname into zOut[].  zOut[] will be at least pVfs->mxPathname
** bytes in size.
*/
int winFullPathname(
  sqlite3_vfs *pVfs,            /* Pointer to vfs object */
  const char *zRelative,        /* Possibly relative input path */
  int nFull,                    /* Size of output buffer in bytes */
  char *zFull                   /* Output buffer */
){

  /* WinCE has no concept of a relative pathname, or so I am told. */
  sqlite3_snprintf(pVfs->mxPathname, zFull, "%s", zRelative);
  return SQLITE_OK;
}

  #define winDlOpen  0
  #define winDlError 0
  #define winDlSym   0
  #define winDlClose 0


/*
** Write up to nBuf bytes of randomness into zBuf.
*/
int winRandomness(sqlite3_vfs *pVfs, int nBuf, char *zBuf){

	int i;
	for (i=0; i<nBuf; ++i)
	{
		zBuf[i] = rand() % 255;
	}
	return nBuf;
}


/*
** Sleep for a little while.  Return the amount of time slept.
*/
int winSleep(sqlite3_vfs *pVfs, int microsec){
	return sleep(microsec / 1000);
}

/*
** The following variable, if set to a non-zero value, becomes the result
** returned from sqlite3OsCurrentTime().  This is used for testing.
*/
#ifdef SQLITE_TEST
int sqlite3_current_time = 0;
#endif

/*
** Find the current time (in Universal Coordinated Time).  Write the
** current time and date as a Julian Day number into *prNow and
** return 0.  Return 1 if the time and date cannot be found.
*/
int winCurrentTime(sqlite3_vfs *pVfs, double *prNow){
  double now;

  now = time(NULL);
  *prNow = now;

  return 0;
}


/*
** Return a pointer to the sqlite3DefaultVfs structure.   We use
** a function rather than give the structure global scope because
** some compilers (MSVC) do not allow forward declarations of
** initialized structures.
*/
sqlite3_vfs *sqlite3OsDefaultVfs(void){
  static sqlite3_vfs winVfs = {
    1,                 /* iVersion */
    -1,   /* szOsFile */
    MAX_PATH,          /* mxPathname */
    0,                 /* pNext */
    "symbian",           /* zName */
    0,                 /* pAppData */
    };
  
  winVfs.szOsFile = sizeof(symbianFile);
  return &winVfs;
}

#endif /* OS_SYMBIAN */
