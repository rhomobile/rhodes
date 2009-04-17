#include "sqlite3.h"
#include "os_symbian.h"

#define SYM_MAX_PATH 260
#define SYM_NO_LOCK         0
#define SYM_SHARED_LOCK     1
#define SYM_RESERVED_LOCK   2
#define SYM_PENDING_LOCK    3
#define SYM_EXCLUSIVE_LOCK  4

#ifndef SQLITE_DEFAULT_SECTOR_SIZE
# define SQLITE_DEFAULT_SECTOR_SIZE 512
#endif

#ifndef SQLITE_TEMP_FILE_PREFIX
# define SQLITE_TEMP_FILE_PREFIX "etilqs_"
#endif

#include <assert.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <e32std.h>
#include <f32file.h>
#include <charconv.h>
#include <bautils.h>
#include <unistd.h>

extern "C" 
{
	
/*
** Macros used to determine whether or not to use threads.
*/
#if defined(THREADSAFE) && THREADSAFE
# define SQLITE_W32_THREADS 1
#endif

/*
** The symbianFile structure is a subclass of sqlite3_file* specific to the sym32
** portability layer.
*/

typedef struct symbianFile symbianFile;
struct symbianFile {
  sqlite3_io_methods const *pMethod;  /* Always the first entry */
  
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

#define symDlOpen  0
#define symDlError 0
#define symDlSym   0
#define symDlClose 0

sqlite3_vfs *sqlite3SymVfs(void);

/*
** Close a file.
**
** It is reported that an attempt to close a handle might sometimes
** fail.  This is a very unreasonable result, but symdows is notorious
** for being unreasonable so I do not doubt that it might happen.  If
** the close fails, we pause for 100 milliseconds and try again.  As
** many as MX_CLOSE_ATTEMPT attempts to close the handle are made before
** giving up and returning an error.
*/
#define MX_CLOSE_ATTEMPT 3
int symClose(sqlite3_file *id){
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
int symRead(
  sqlite3_file *id,          /* File to read from */
  void *pBuf,                /* Write content into this buffer */
  int amt,                   /* Number of bytes to read */
  sqlite3_int64 offset       /* Begin reading at this offset */
){
  int rc;
  size_t got;
  symbianFile *pFile = (symbianFile*)id;
  assert( id!=0 );
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
int symWrite(
  sqlite3_file *id,         /* File to write into */
  const void *pBuf,         /* The bytes to be written */
  int amt,                  /* Number of bytes to write */
  sqlite3_int64 offset      /* Offset into the file to begin writing at */
){
  int rc;
  symbianFile *pFile = (symbianFile*)id;
  assert( id!=0 );
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
int symTruncate(sqlite3_file *id, sqlite3_int64 nByte){
  symbianFile *pFile = (symbianFile*)id;

  if (pFile->file.SetSize(nByte) != KErrNone)
  {
	  return SQLITE_IOERR; 
  }

  return SQLITE_OK;
}

/*
** Make sure all writes to a particular file are committed to disk.
*/
int symSync(sqlite3_file *id, int flags){
  symbianFile *pFile = (symbianFile*)id;
  
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
int symFileSize(sqlite3_file *id, sqlite3_int64 *pSize){
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
** of the follosymg:
**
**     (1) SYM_SHARED_LOCK
**     (2) SYM_RESERVED_LOCK
**     (3) SYM_PENDING_LOCK
**     (4) SYM_EXCLUSIVE_LOCK
**
** Sometimes when requesting one lock state, additional lock states
** are inserted in between.  The locking might fail on one of the later
** transitions leaving the lock state different from what it started but
** still short of its goal.  The follosymg chart shows the allowed
** transitions and the inserted intermediate states:
**
**    UNLOCKED -> SHARED
**    SHARED -> RESERVED
**    SHARED -> (PENDING) -> EXCLUSIVE
**    RESERVED -> (PENDING) -> EXCLUSIVE
**    PENDING -> EXCLUSIVE
**
** This routine will only increase a lock.  The symUnlock() routine
** erases all locks at once and returns us immediately to locking level 0.
** It is not possible to lower the locking level one step at a time.  You
** must go straight to locking level 0.
*/
int symLock(sqlite3_file *id, int locktype){
  int rc = SQLITE_OK;    /* Return code from subroutines */
  int res = 1;           /* Result of a symdows lock call */
  int newLocktype;       /* Set pFile->locktype to this value before exiting */
  int gotPendingLock = 0;/* True if we acquired a PENDING lock this time */
  symbianFile *pFile = (symbianFile*)id;

  assert( pFile!=0 );
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
int symCheckReservedLock(sqlite3_file *id, int *pResOut){
  int rc = 0;
  symbianFile *pFile = (symbianFile*)id;
  assert( pFile!=0 );
  if( pFile->locktype>=SYM_RESERVED_LOCK ){
    rc = 1;
  }else{
	  TInt size = 0;
	  if (pFile->file.Size(size) == KErrNone) rc = 1;
  }
  
  *pResOut = rc;
  return SQLITE_OK;
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
int symUnlock(sqlite3_file *id, int locktype){
  int type;
  symbianFile *pFile = (symbianFile*)id;
  int rc = SQLITE_OK;
  assert( pFile!=0 );
  return rc;
}

/*
** Control and query of the open file handle.
*/
int symFileControl(sqlite3_file *id, int op, void *pArg){
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
int symSectorSize(sqlite3_file *id){
  return SQLITE_DEFAULT_SECTOR_SIZE;
}

/*
** Return a vector of device characteristics.
*/
int symDeviceCharacteristics(sqlite3_file *id){
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
  delete converter;
}

int symOpen(
  sqlite3_vfs *pVfs,        /* Not used */
  const char *zName,        /* Name of the file (UTF-8) */
  sqlite3_file *id,         /* Write the SQLite file handle here */
  int flags,                /* Open mode flags */
  int *pOutFlags            /* Status return flags */
);


/*
** Delete the named file.
**
** Note that symdows does not allow a file to be deleted if some other
** process has it open.  Sometimes a virus scanner or indexing program
** will open a journal file shortly after it is created in order to do
** whatever does.  While this other process is holding the
** file open, we will be unable to delete it.  To work around this
** problem, we delay 100 milliseconds and try to delete again.  Up
** to MX_DELETION_ATTEMPTs deletion attempts are run before giving
** up and returning an error.
*/
#define MX_DELETION_ATTEMPTS 5
int symDelete(
  sqlite3_vfs *pVfs,          /* Not used on sym32 */
  const char *zFilename,      /* Name of file to delete */
  int syncDir                 /* Not used on sym32 */
){
  TBuf16<SYM_MAX_PATH> filename;

  RFs session;
  session.Connect();
  ConvertToUnicode(session, filename, zFilename);
  BaflUtils::DeleteFile(session, filename);
  session.Close();
  return SQLITE_OK;
}

/*
** Check the existance and status of a file.
*/
int symAccess(
  sqlite3_vfs *pVfs,         /* Not used on sym32 */
  const char *zFilename,     /* Name of file to check */
  int flags,                  /* Type of test to make on this file */
  int *pResOut
){
  TBuf16<SYM_MAX_PATH> filename;

  RFs session;
  session.Connect();
  ConvertToUnicode(session, filename, zFilename);
  int ret = BaflUtils::FileExists(session, filename);
  session.Close();

  *pResOut = ret;
  
  return SQLITE_OK;
}

int symRandomness(sqlite3_vfs *pVfs, int nBuf, char *zBuf);

int randomByte(void){
  unsigned char t;

  /* All threads share a single random number generator.
  ** This structure is the current state of the generator.
  */
  static struct {
    unsigned char isInit;          /* True if initialized */
    unsigned char i, j;            /* State variables */
    unsigned char s[256];          /* State variables */
  } prng;

  /* Initialize the state of the random number generator once,
  ** the first time this routine is called.  The seed value does
  ** not need to contain a lot of randomness since we are not
  ** trying to do secure encryption or anything like that...
  **
  ** Nothing in this file or anywhere else in SQLite does any kind of
  ** encryption.  The RC4 algorithm is being used as a PRNG (pseudo-random
  ** number generator) not as an encryption device.
  */
  if( !prng.isInit ){
    int i;
    char k[256];
    prng.j = 0;
    prng.i = 0;
    symRandomness(sqlite3_vfs_find(0), 256, k);
    for(i=0; i<256; i++){
      prng.s[i] = i;
    }
    for(i=0; i<256; i++){
      prng.j += prng.s[i] + k[i];
      t = prng.s[prng.j];
      prng.s[prng.j] = prng.s[i];
      prng.s[i] = t;
    }
    prng.isInit = 1;
  }

  /* Generate and return single random byte
  */
  prng.i++;
  t = prng.s[prng.i];
  prng.j += t;
  prng.s[prng.i] = prng.s[prng.j];
  prng.s[prng.j] = t;
  t += prng.s[prng.i];
  return prng.s[t];
}

/*
** Return N random bytes.
*/
void sqlite3Randomness(int N, void *pBuf){
  unsigned char *zBuf = (unsigned char*)pBuf;
  static sqlite3_mutex *mutex = 0;
  if( mutex==0 ){
    mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_PRNG);
  }
  sqlite3_mutex_enter(mutex);
  while( N-- ){
    *(zBuf++) = randomByte();
  }
  sqlite3_mutex_leave(mutex);
}


/*
** Create a temporary file name in zBuf.  zBuf must be big enough to
** hold at pVfs->mxPathname characters.
*/
int symGetTempname(sqlite3_vfs *pVfs, int nBuf, char *zBuf){
  static char zChars[] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789";
  int i, j;
  char zTempPath[SYM_MAX_PATH+1];
  if( sqlite3_temp_directory ){
    sqlite3_snprintf(SYM_MAX_PATH-30, zTempPath, "%s", sqlite3_temp_directory);
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
  return SQLITE_OK; 
}

/*
** Turn a relative pathname into a full pathname.  Write the full
** pathname into zOut[].  zOut[] will be at least pVfs->mxPathname
** bytes in size.
*/
int symFullPathname(
  sqlite3_vfs *pVfs,            /* Pointer to vfs object */
  const char *zRelative,        /* Possibly relative input path */
  int nFull,                    /* Size of output buffer in bytes */
  char *zFull                   /* Output buffer */
){

  /* symCE has no concept of a relative pathname, or so I am told. */
  sqlite3_snprintf(pVfs->mxPathname, zFull, "%s", zRelative);
  return SQLITE_OK;
}

  
/*
** Write up to nBuf bytes of randomness into zBuf.
*/
int symRandomness(sqlite3_vfs *pVfs, int nBuf, char *zBuf){

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
int symSleep(sqlite3_vfs *pVfs, int microsec){
	return sleep(microsec / 1000);
}

/*
** Find the current time (in Universal Coordinated Time).  Write the
** current time and date as a Julian Day number into *prNow and
** return 0.  Return 1 if the time and date cannot be found.
*/
int symCurrentTime(sqlite3_vfs *pVfs, double *prNow){
  double now;

  now = time(NULL);
  *prNow = now;

  return 0;
}

int symGetLastError(sqlite3_vfs *pVfs, int nBuf, char *zBuf){
  return 0;
}

#define SYMBIANVFS(zVfsName, pVfsAppData) {                  \
    1,                    /* iVersion */                    \
	sizeof(symbianFile),     /* szOsFile */                    \
	SYM_MAX_PATH,         /* mxPathname */                  \
    0,                    /* pNext */                       \
    zVfsName,             /* zName */                       \
    (void *)pVfsAppData,  /* pAppData */                    \
    symOpen,             /* xOpen */                       \
    symDelete,           /* xDelete */                     \
    symAccess,           /* xAccess */                     \
    symFullPathname,     /* xFullPathname */               \
    symDlOpen,           /* xDlOpen */                     \
    symDlError,          /* xDlError */                    \
    symDlSym,            /* xDlSym */                      \
    symDlClose,          /* xDlClose */                    \
    symRandomness,       /* xRandomness */                 \
    symSleep,            /* xSleep */                      \
    symCurrentTime,      /* xCurrentTime */                \
    symGetLastError      /* xGetLastError */               \
  }

const sqlite3_io_methods symIoMethod = {
  1,                        /* iVersion */
  symClose,
  symRead,
  symWrite,
  symTruncate,
  symSync,
  symFileSize,
  symLock,
  symUnlock,
  symCheckReservedLock,
  symFileControl,
  symSectorSize,
  symDeviceCharacteristics
};

sqlite3_vfs symVfs = SYMBIANVFS("unix", 0);

sqlite3_vfs *sqlite3SymVfs(void){
	return &symVfs;
}

/*
** Open a file.
*/
int symOpen(
  sqlite3_vfs *pVfs,        /* Not used */
  const char *zName,        /* Name of the file (UTF-8) */
  sqlite3_file *id,         /* Write the SQLite file handle here */
  int flags,                /* Open mode flags */
  int *pOutFlags            /* Status return flags */
){
  symbianFile *pFile = (symbianFile*)id;
  memset(pFile, 0, sizeof(symbianFile));
  
  TBuf16<SYM_MAX_PATH> filename;
  TInt nPos = 1; 
  _LIT16(Slash,"\\");
  _LIT16(BackSlash,"/");
  
  pFile->pMethod = &symIoMethod;
  
  pFile->isOpen = 0;
  
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

  if (ret != KErrNone)
  {
	  return SQLITE_IOERR;
  }

  pFile->isOpen = 1;
  return SQLITE_OK;
}


}
