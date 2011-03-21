#define SQLITE_OS_WIN

using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using DWORD = System.UInt64;
using HANDLE = System.IntPtr;
using i64 = System.Int64;
using sqlite3_int64 = System.Int64;
using u32 = System.UInt32;
using u8 = System.Byte;
using System.IO.IsolatedStorage;


namespace Community.CsharpSqlite
{
  public partial class Sqlite3
  {
    /*
    ** 2004 May 22
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
    *************************************************************************
    **  Included in SQLite3 port to C#-SQLite;  2008 Noah B Hart
    **  C#-SQLite is an independent reimplementation of the SQLite software library
    **
    **  SQLITE_SOURCE_ID: 2010-03-09 19:31:43 4ae453ea7be69018d8c16eb8dabe05617397dc4d
    **
    **  $Header: Community.CsharpSqlite/src/os_win_c.cs,v 6604176a7dbe 2010/03/12 23:35:36 Noah $
    *************************************************************************
    */

    const int INVALID_FILE_ATTRIBUTES = -1;



    private static LockingStrategy lockingStrategy = HelperMethods.IsRunningMediumTrust() ? new MediumTrustLockingStrategy() : new LockingStrategy();

    public static IsolatedStorageFile store = IsolatedStorageFile.GetUserStoreForApplication();    
    
    public partial class sqlite3_file
    {      
        // public HANDLE h;            /* Handle for accessing the file */
      public int locktype;           /* Type of lock currently held on this file */
      public int sharedLockByte;     /* Randomly chosen byte used as a shared lock */
      public DWORD lastErrno;        /* The Windows errno from the last I/O error */
      public DWORD sectorSize;       /* Sector size of the device file is on */

      public IsolatedStorageFileStream fs = null;

      public void Clear()
      {
        pMethods = null;
        //fs = null;
        locktype = 0;
        sharedLockByte = 0;
        lastErrno = 0;
        sectorSize = 0;
      }
    };
      
      static int sqlite3_os_type = 0;


    /*
** Return true (non-zero) if we are running under WinNT, Win2K, WinXP,
** or WinCE.  Return false (zero) for Win95, Win98, or WinME.
**
** Here is an interesting observation:  Win95, Win98, and WinME lack
** the LockFileEx() API.  But we can still statically link against that
** API as long as we don't call it when running Win95/98/ME.  A call to
** this routine is used to determine if the host is Win95/98/ME or
** WinNT/2K/XP so that we will know whether or not we can safely call
** the LockFileEx() API.
*/

    static bool isNT()
    {
      return Environment.OSVersion.Platform >= PlatformID.Win32NT;
    }

    private static bool ExistsFile(string sFilename)
    {
        return store.FileExists(sFilename);
    }

    private static void DeleteFile(string sFilename)
    {
        store.DeleteFile(sFilename);
    }

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
    public static int MX_CLOSE_ATTEMPT = 3;
    static int winClose( sqlite3_file id )    
    {
      bool rc;
      int cnt = 0;
      //sqlite3_file pFile = (sqlite3_file)id;

      Debug.Assert( id != null );
      do
      {
          if (id.fs != null)
          {
              id.fs.Close();
              id.fs = null;
          }
        rc = true;
        //  rc = CloseHandle(pFile.h);
        //  if (!rc && ++cnt < MX_CLOSE_ATTEMPT) Thread.Sleep(100); //, 1) );
      } while ( !rc && ++cnt < MX_CLOSE_ATTEMPT ); //, 1) );
      return rc ? SQLITE_OK : SQLITE_IOERR;
    }

    /*
    ** Some microsoft compilers lack this definition.
    */
#if !INVALID_SET_FILE_POINTER
    const int INVALID_SET_FILE_POINTER = -1;
#endif

    /*
** Read data from a file into a buffer.  Return SQLITE_OK if all
** bytes were read successfully and SQLITE_IOERR if anything goes
** wrong.
*/
    static int winRead(
    sqlite3_file id,           /* File to read from */
    byte[] pBuf,           /* Write content into this buffer */
    int amt,                   /* Number of bytes to read */
    sqlite3_int64 offset       /* Begin reading at this offset */
    )
    {
      long rc;
     // sqlite3_file pFile = id;
      long got;
      //Debug.Assert( fs != null );
      if ( !id.fs.CanRead ) return SQLITE_IOERR_READ;
      try
      {
        rc = id.fs.Seek( offset, SeekOrigin.Begin ); // SetFilePointer(pFile.fs.Name, lowerBits, upperBits, FILE_BEGIN);
      }
      catch ( Exception e )      //            if( rc==INVALID_SET_FILE_POINTER && (error=GetLastError())!=NO_ERROR )
      {
        //pFile.lastErrno = 1;
        return SQLITE_FULL;
      }

      try
      {
        got = id.fs.Read( pBuf, 0, amt ); // if (!ReadFile(pFile.fs.Name, pBuf, amt, got, 0))
      }
      catch ( Exception e )
      {
        //pFile.lastErrno = 1;
        return SQLITE_IOERR_READ;
      }
      if ( got == amt )
      {
        return SQLITE_OK;
      }
      else
      {
        /* Unread parts of the buffer must be zero-filled */
        Array.Clear( pBuf, (int)got, (int)( amt - got ) ); // memset(&((char*)pBuf)[got], 0, amt - got);
        return SQLITE_IOERR_SHORT_READ;
      }
    }

    /*
    ** Write data from a buffer into a file.  Return SQLITE_OK on success
    ** or some other error code on failure.
    */
    static int winWrite(
    sqlite3_file id,          /* File to write into */
    byte[] pBuf,              /* The bytes to be written */
    int amt,                  /* Number of bytes to write */
    sqlite3_int64 offset      /* Offset into the file to begin writing at */
    )
    {
      int rc;
      long wrote = 0;
      //Debug.Assert( id != null );
      //id.fs.Seek( offset, SeekOrigin.Begin );
      id.fs.Seek(offset, SeekOrigin.Begin);
      Debug.Assert( amt > 0 );
      //wrote = id.fs.Position;
      wrote = id.fs.Position;
      try
      {
        Debug.Assert( pBuf.Length >= amt );
        //id.fs.Write( pBuf, 0, amt );
        id.fs.Write(pBuf, 0, amt);
        rc = 1;// Success
        //wrote = id.fs.Position - wrote;
        wrote = id.fs.Position - wrote;
      }
      catch ( IOException e )
      {
        return SQLITE_READONLY;
      }
      if ( rc == 0 || amt > (int)wrote )
      {
        id.lastErrno  = 1;     
        return SQLITE_FULL;
      }
      return SQLITE_OK;
    }

    /*
    ** Truncate an open file to a specified size
    */
    static int winTruncate( sqlite3_file id, sqlite3_int64 nByte )
    {
      Debug.Assert( id != null );
      try
      {
        //id.fs.SetLength( nByte );
          id.fs.SetLength(nByte);
      }
      catch ( IOException e )
      {
         id.lastErrno  = 1;
        return SQLITE_IOERR_TRUNCATE;
      }
      return SQLITE_OK;
    }

    /*
** Make sure all writes to a particular file are committed to disk.
*/
    static int winSync( sqlite3_file id, int flags )
    {
      sqlite3_file pFile = (sqlite3_file)id;
      Debug.Assert( id != null );
      UNUSED_PARAMETER(flags);
      //pFile.fs.Flush();
      id.fs.Flush();
      return SQLITE_OK;
    }

    /*
    ** Determine the current size of a file in bytes
    */
    static int sqlite3_fileSize( sqlite3_file id, ref int pSize )
    {
      Debug.Assert( id != null );
      //pSize = id.fs.CanRead ? (int)id.fs.Length : 0;
      pSize = id.fs.CanRead ? (int)id.fs.Length : 0;
      return SQLITE_OK;
    }


    /*
    ** Acquire a reader lock.
    ** Different API routines are called depending on whether or not this
    ** is Win95 or WinNT.
    */
    static int getReadLock(sqlite3_file pFile)
    {
        int res = 0;
        if (isNT())
        {
            res = lockingStrategy.SharedLockFile(pFile, SHARED_FIRST, SHARED_SIZE);
        }
        if (res == 0)
        {
            pFile.lastErrno = 1;
        }
        return res;
    }

    /*
    ** Undo a readlock
    */
    static int unlockReadLock(sqlite3_file pFile)
    {
        int res = 1;
        if (isNT())
        {
            try
            {
                lockingStrategy.UnlockFile(pFile, SHARED_FIRST, SHARED_SIZE); //     res = UnlockFile(pFilE.h, SHARED_FIRST, 0, SHARED_SIZE, 0);
            }
            catch (Exception e)
            {
                res = 0;
            }
        }
        else
        {
            Debugger.Break(); //    res = UnlockFile(pFilE.h, SHARED_FIRST + pFilE.sharedLockByte, 0, 1, 0);
        }
        if (res == 0)
        {
            pFile.lastErrno = 1;
        }
        return res;
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
    static int winLock( sqlite3_file id, int locktype )
    {
      int rc = SQLITE_OK;         /* Return code from subroutines */
      int res = 1;                /* Result of a windows lock call */
      int newLocktype;            /* Set pFile.locktype to this value before exiting */
      bool gotPendingLock = false;/* True if we acquired a PENDING lock this time */
      sqlite3_file pFile = (sqlite3_file)id;
      DWORD error = NO_ERROR;

      Debug.Assert( id != null );
      /* If there is already a lock of this type or more restrictive on the
** OsFile, do nothing. Don't use the end_lock: exit path, as
** sqlite3OsEnterMutex() hasn't been called yet.
*/
      if ( pFile.locktype >= locktype )
      {
        return SQLITE_OK;
      }

      /* Make sure the locking sequence is correct
      */
      Debug.Assert( pFile.locktype != NO_LOCK || locktype == SHARED_LOCK );
      Debug.Assert( locktype != PENDING_LOCK );
      Debug.Assert( locktype != RESERVED_LOCK || pFile.locktype == SHARED_LOCK );

      /* Lock the PENDING_LOCK byte if we need to acquire a PENDING lock or
      ** a SHARED lock.  If we are acquiring a SHARED lock, the acquisition of
      ** the PENDING_LOCK byte is temporary.
      */
      newLocktype = pFile.locktype;
      if ( pFile.locktype == NO_LOCK
      || ( ( locktype == EXCLUSIVE_LOCK )
      && ( pFile.locktype == RESERVED_LOCK ) )
      )
      {
        int cnt = 3;
        res = 0;
        while ( cnt-- > 0 && res == 0 )//(res = LockFile(pFile.fs.SafeFileHandle.DangerousGetHandle().ToInt32(), PENDING_BYTE, 0, 1, 0)) == 0)
        {
          try
          {
            lockingStrategy.LockFile( pFile, PENDING_BYTE, 1 );
            res = 1;
          }
          catch ( Exception e )
          {
            /* Try 3 times to get the pending lock.  The pending lock might be
            ** held by another reader process who will release it momentarily.
            */
            Thread.Sleep( 1 );
          }
        }
        gotPendingLock = ( res != 0 );
        if ( 0 == res )
        {
                error = 1;
        }
      }

      /* Acquire a shared lock
      */
      if ( locktype == SHARED_LOCK && res != 0 )
      {
        Debug.Assert( pFile.locktype == NO_LOCK );
        res = getReadLock( pFile );
        if ( res != 0 )
        {
          newLocktype = SHARED_LOCK;
        }
        else
        {
                error = 1;
        }
      }

      if ( ( locktype == RESERVED_LOCK ) && res != 0 )
      {
        Debug.Assert( pFile.locktype == SHARED_LOCK );
        try
        {
          lockingStrategy.LockFile( pFile, RESERVED_BYTE, 1 );//res = LockFile(pFile.fs.SafeFileHandle.DangerousGetHandle().ToInt32(), RESERVED_BYTE, 0, 1, 0);
          newLocktype = RESERVED_LOCK;
          res = 1;
        }
        catch ( Exception e )
        {
            res = 0;
            error = 1;
        }
        if ( res != 0 )
        {
          newLocktype = RESERVED_LOCK;
        }
        else
        {
                error = 1;
        }
      }

      /* Acquire a PENDING lock
      */
      if ( locktype == EXCLUSIVE_LOCK && res != 0 )
      {
        newLocktype = PENDING_LOCK;
        gotPendingLock = false;
      }

      /* Acquire an EXCLUSIVE lock
      */
      if ( locktype == EXCLUSIVE_LOCK && res != 0 )
      {
        Debug.Assert( pFile.locktype >= SHARED_LOCK );
        res = unlockReadLock( pFile );
        try
        {
          lockingStrategy.LockFile( pFile, SHARED_FIRST, SHARED_SIZE );
          newLocktype = EXCLUSIVE_LOCK;
          res = 1;
        }
        catch ( Exception e )
        {
          res = 0;
        }
        if ( res != 0 )
        {
          newLocktype = EXCLUSIVE_LOCK;
        }
        else
        {
            error = 1;
            getReadLock( pFile );
        }
      }

      /* If we are holding a PENDING lock that ought to be released, then
      ** release it now.
      */
      if ( gotPendingLock && locktype == SHARED_LOCK )
      {
        lockingStrategy.UnlockFile( pFile, PENDING_BYTE, 1 );
      }

      /* Update the state of the lock has held in the file descriptor then
      ** return the appropriate result code.
      */
      if ( res != 0 )
      {
        rc = SQLITE_OK;
      }
      else
      {
        pFile.lastErrno = error;
        rc = SQLITE_BUSY;
      }
      pFile.locktype = (u8)newLocktype;
      return rc;
    }

    /*
    ** This routine checks if there is a RESERVED lock held on the specified
    ** file by this or any other process. If such a lock is held, return
    ** non-zero, otherwise zero.
    */
    static int winCheckReservedLock( sqlite3_file id, ref int pResOut )
    {
      //int rc;      
      //Debug.Assert( id != null );
      //if ( id.locktype >= RESERVED_LOCK )
      //{
      //  rc = 1;
      //}
      //else
      //{
      //  try
      //  {
      //      lockingStrategy.LockFile(id, RESERVED_BYTE, 1);
      //      lockingStrategy.UnlockFile(id, RESERVED_BYTE, 1);
      //    rc = 1;
      //  }
      //  catch ( IOException e )
      //  { rc = 0; }
      //  rc = 1 - rc; // !rc
      //}
      //pResOut = rc;
      pResOut = 0;
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
    static int winUnlock( sqlite3_file id, int locktype )
    {
      int type;
      sqlite3_file pFile = (sqlite3_file)id;
      int rc = SQLITE_OK;
      Debug.Assert( pFile != null );
      Debug.Assert( locktype <= SHARED_LOCK );
      type = pFile.locktype;
      if ( type >= EXCLUSIVE_LOCK )
      {
        lockingStrategy.UnlockFile( pFile, SHARED_FIRST, SHARED_SIZE ); // UnlockFile(pFilE.h, SHARED_FIRST, 0, SHARED_SIZE, 0);
        if ( locktype == SHARED_LOCK && getReadLock( pFile ) == 0 )
        {
          /* This should never happen.  We should always be able to
          ** reacquire the read lock */
          rc = SQLITE_IOERR_UNLOCK;
        }
      }
      if ( type >= RESERVED_LOCK )
      {
        try
        {
          lockingStrategy.UnlockFile( pFile, RESERVED_BYTE, 1 );// UnlockFile(pFilE.h, RESERVED_BYTE, 0, 1, 0);
        }
        catch ( Exception e ) { }
      }
      if ( locktype == NO_LOCK && type >= SHARED_LOCK )
      {
        unlockReadLock( pFile );
      }
      if ( type >= PENDING_LOCK )
      {
        try
        {
          lockingStrategy.UnlockFile( pFile, PENDING_BYTE, 1 );//    UnlockFile(pFilE.h, PENDING_BYTE, 0, 1, 0);
        }
        catch ( Exception e )
        { }
      }
      pFile.locktype = (u8)locktype;
      return rc;
    }

    /*
    ** Control and query of the open file handle.
    */
    static int winFileControl( sqlite3_file id, int op, ref int pArg )
    {
      switch ( op )
      {
        case SQLITE_FCNTL_LOCKSTATE:
          {
            pArg = ( (sqlite3_file)id ).locktype;
            return SQLITE_OK;
          }
        case SQLITE_LAST_ERRNO:
          {
            pArg = (int)( (sqlite3_file)id ).lastErrno;
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
    static int winSectorSize( sqlite3_file id )
    {
      Debug.Assert( id != null );
      return (int)( id.sectorSize );
    }

    /*
    ** Return a vector of device characteristics.
    */
    static int winDeviceCharacteristics( sqlite3_file id )
    {
      UNUSED_PARAMETER( id );
      return 0;
    }

    /*
    ** This vector defines all the methods that can operate on an
    ** sqlite3_file for win32.
    */
    static sqlite3_io_methods winIoMethod = new sqlite3_io_methods(
    1,                        /* iVersion */
    (dxClose)winClose,
    (dxRead)winRead,
    (dxWrite)winWrite,
    (dxTruncate)winTruncate,
    (dxSync)winSync,
    (dxFileSize)sqlite3_fileSize,
    (dxLock)winLock,
    (dxUnlock)winUnlock,
    (dxCheckReservedLock)winCheckReservedLock,
    (dxFileControl)winFileControl,
    (dxSectorSize)winSectorSize,
    (dxDeviceCharacteristics)winDeviceCharacteristics
    );

    /***************************************************************************
    ** Here ends the I/O methods that form the sqlite3_io_methods object.
    **
    ** The next block of code implements the VFS methods.
    ****************************************************************************/

    /*
    ** Convert a UTF-8 filename into whatever form the underlying
    ** operating system wants filenames in.  Space to hold the result
    ** is obtained from malloc and must be freed by the calling
    ** function.
    */
    static string convertUtf8Filename( string zFilename )
    {
      return zFilename;
    }

    /*
    ** Create a temporary file name in zBuf.  zBuf must be big enough to
    ** hold at pVfs.mxPathname characters.
    */
    static int getTempname(int nBuf, StringBuilder zBuf)
    {
      const string zChars = "abcdefghijklmnopqrstuvwxyz0123456789";
      StringBuilder zRandom = new StringBuilder(20);
      i64 iRandom = 0;
      for (int i = 0; i < 20; i++)
      {
        sqlite3_randomness(1, ref iRandom);
        zRandom.Append((char)zChars[(int)(iRandom % (zChars.Length - 1))]);
      }
      zBuf.Append(Path.GetTempPath() + SQLITE_TEMP_FILE_PREFIX + zRandom.ToString());
      return SQLITE_OK;
    }

    /*
    ** The return value of getLastErrorMsg
    ** is zero if the error message fits in the buffer, or non-zero
    ** otherwise (if the message was truncated).
    */
    static int getLastErrorMsg( int nBuf, ref string zBuf )
    {
        zBuf = "Unknown error";
        return 0;
    }

    /*
    ** Open a file.
    */
    static int winOpen(
    sqlite3_vfs pVfs,       /* Not used */
    string zName,           /* Name of the file (UTF-8) */
    sqlite3_file pFile, /* Write the SQLite file handle here */
    int flags,              /* Open mode flags */
    ref int pOutFlags       /* Status return flags */
    )
    {
        if (pFile.fs != null)
            return SQLITE_OK;
      //HANDLE h;
      //pFile.fs = null;
      //fs = null;
      FileAccess dwDesiredAccess;
      FileShare dwShareMode;
      FileMode dwCreationDisposition;
      string zConverted;                 /* Filename in OS encoding */
      string zUtf8Name = zName;    /* Filename in UTF-8 encoding */
      StringBuilder zTmpname = new StringBuilder( MAX_PATH + 1 );        /* Buffer used to create temp filename */

      Debug.Assert( pFile != null );
      UNUSED_PARAMETER( pVfs );

      /* If the second argument to this function is NULL, generate a
      ** temporary file name to use
      */
      if ( String.IsNullOrEmpty( zUtf8Name ) )
      {
        int rc = getTempname( MAX_PATH + 1, zTmpname );
        if ( rc != SQLITE_OK )
        {
          return rc;
        }
        zUtf8Name = zTmpname.ToString();
      }

      // /* Convert the filename to the system encoding. */
      zConverted = zUtf8Name;// convertUtf8Filename( zUtf8Name );
      if ( String.IsNullOrEmpty( zConverted ) )
      {
        return SQLITE_NOMEM;
      }

      if ( ( flags & SQLITE_OPEN_READWRITE ) != 0 )
      {
        dwDesiredAccess = FileAccess.Read | FileAccess.Write; // GENERIC_READ | GENERIC_WRITE;
      }
      else
      {
        dwDesiredAccess = FileAccess.Read; // GENERIC_READ;
      }
      /* SQLITE_OPEN_EXCLUSIVE is used to make sure that a new file is
      ** created. SQLite doesn't use it to indicate "exclusive access"
      ** as it is usually understood.
      */
      Debug.Assert( 0 == ( flags & SQLITE_OPEN_EXCLUSIVE ) || ( flags & SQLITE_OPEN_CREATE ) != 0 );
      if ( ( flags & SQLITE_OPEN_EXCLUSIVE ) != 0 )
      {
        /* Creates a new file, only if it does not already exist. */
        /* If the file exists, it fails. */
        dwCreationDisposition = FileMode.CreateNew;// CREATE_NEW;
      }
      else if ( ( flags & SQLITE_OPEN_CREATE ) != 0 )
      {
        /* Open existing file, or create if it doesn't exist */
        dwCreationDisposition = FileMode.OpenOrCreate;// OPEN_ALWAYS;
      }
      else
      {
        /* Opens a file, only if it exists. */
        dwCreationDisposition = FileMode.Open;//OPEN_EXISTING;
      }
      dwShareMode = FileShare.Read | FileShare.Write;// FILE_SHARE_READ | FILE_SHARE_WRITE;
      if ( ( flags & SQLITE_OPEN_DELETEONCLOSE ) != 0 )
      {
      }
      else
      {
      }
      /* Reports from the internet are that performance is always
      ** better if FILE_FLAG_RANDOM_ACCESS is used.  Ticket #2699. */
      if ( isNT() )
      {
        int retries = 3;
        //while ( ( pFile.fs == null ) && ( retries > 0 ) )
        while ((pFile.fs == null) && (retries > 0))
          try
          {
            retries--;
            //pFile.fs = new IsolatedStorageFileStream(zConverted, dwCreationDisposition, dwDesiredAccess, dwShareMode, pFile.store);
            pFile.fs = new IsolatedStorageFileStream(zConverted, dwCreationDisposition, dwDesiredAccess, dwShareMode, store);
          }
          catch ( Exception e )
          {
            Thread.Sleep( 100 );
          }

        /* isNT() is 1 if SQLITE_OS_WINCE==1, so this else is never executed.
        ** Since the ASCII version of these Windows API do not exist for WINCE,
        ** it's important to not reference them for WINCE builds.
        */
     }
      //if ( pFile.fs == null 
      if (pFile.fs == null 
          ||
          //!pFile.fs.CanRead
          !pFile.fs.CanRead
          ) 
      {
        if ( ( flags & SQLITE_OPEN_READWRITE ) != 0 )
        {
          return winOpen( pVfs, zName, pFile,
          ( ( flags | SQLITE_OPEN_READONLY ) & ~SQLITE_OPEN_READWRITE ), ref pOutFlags );
        }
        else
        {
          return SQLITE_CANTOPEN_BKPT();
        }
      }
      if ( ( flags & SQLITE_OPEN_READWRITE ) != 0 )
      {
        pOutFlags = SQLITE_OPEN_READWRITE;
      }
      else
      {
        pOutFlags = SQLITE_OPEN_READONLY;
      }
      //}
      pFile.Clear(); // memset(pFile, 0, sizeof(*pFile));
      pFile.pMethods = winIoMethod;
      pFile.lastErrno = NO_ERROR;
      pFile.sectorSize = (ulong)getSectorSize( pVfs, zUtf8Name );
      return SQLITE_OK;
    }

    /*
    ** Delete the named file.
    **
    ** Note that windows does not allow a file to be deleted if some other
    ** process has it open.  Sometimes a virus scanner or indexing program
    ** will open a journal file shortly after it is created in order to do
    ** whatever it does.  While this other process is holding the
    ** file open, we will be unable to delete it.  To work around this
    ** problem, we delay 100 milliseconds and try to delete again.  Up
    ** to MX_DELETION_ATTEMPTs deletion attempts are run before giving
    ** up and returning an error.
    */
    static int MX_DELETION_ATTEMPTS = 5;
    static int winDelete(
    sqlite3_vfs pVfs,         /* Not used on win32 */
    string zFilename,         /* Name of file to delete */
    int syncDir               /* Not used on win32 */
    )
    {
      int cnt = 0;
      int rc;
      int error;
      UNUSED_PARAMETER( pVfs );
      UNUSED_PARAMETER( syncDir );
      string zConverted = convertUtf8Filename( zFilename );
      if ( zConverted == null || zConverted == "" )
      {
        return SQLITE_NOMEM;
      }
      if ( isNT() )
      {
        do
        {
          if (!ExistsFile(zFilename))
          {
            rc = SQLITE_IOERR;
            break;
          }
          try
          {
            DeleteFile( zConverted );
            rc = SQLITE_OK;
          }
          catch ( IOException e )
          {
            rc = SQLITE_IOERR;
            Thread.Sleep( 100 );
          }
        } while ( rc != SQLITE_OK && ++cnt < MX_DELETION_ATTEMPTS );
        /* isNT() is 1 if SQLITE_OS_WINCE==1, so this else is never executed.
        ** Since the ASCII version of these Windows API do not exist for WINCE,
        ** it's important to not reference them for WINCE builds.
        */
      }
      else
      {
        do
        {
          if (!ExistsFile(zFilename))
          {
            rc = SQLITE_IOERR;
            break;
          }
          try
          {
            DeleteFile(zConverted);
            rc = SQLITE_OK;
          }
          catch ( IOException e )
          {
            rc = SQLITE_IOERR;
            Thread.Sleep( 100 );
          }
        } while ( rc != SQLITE_OK && cnt++ < MX_DELETION_ATTEMPTS );

      }
      //free(zConverted);
#if SQLITE_DEBUG
      OSTRACE2( "DELETE \"%s\"\n", zFilename );
#endif
      //return ( ( rc == INVALID_FILE_ATTRIBUTES )
      //&& ( error == ERROR_FILE_NOT_FOUND ) ) ? SQLITE_OK : SQLITE_IOERR_DELETE;
      return rc;
    }

    /*
    ** Check the existence and status of a file.
    */
    static int winAccess(
    sqlite3_vfs pVfs,       /* Not used on win32 */
    string zFilename,       /* Name of file to check */
    int flags,              /* Type of test to make on this file */
    ref int pResOut         /* OUT: Result */
    )
    {
        pResOut = 1;
        return SQLITE_OK;

      //FileAttributes attr = 0; // DWORD attr;
      //int rc = 0;
      //UNUSED_PARAMETER( pVfs );
      //    if (flags == SQLITE_ACCESS_EXISTS)
      //    {
      //        pResOut =store.FileExists(zFilename) ? 1 : 0;
      //        return SQLITE_OK;
      //    }
          
      //    try
      //    {
      //        if (store.DirectoryExists(zFilename))
      //        {
      //            StringBuilder zTmpname = new StringBuilder(255);        /* Buffer used to create temp filename */
      //            getTempname(256, zTmpname);

      //            string zTempFilename;
      //            zTempFilename = zTmpname.ToString();//( SQLITE_TEMP_FILE_PREFIX.Length + 1 );
      //            try
      //            {
      //                IsolatedStorageFileStream fs = store.CreateFile(zTempFilename);
      //                fs.Close();
      //                fs = null;
      //                store.DeleteFile(zTempFilename);
      //                attr = FileAttributes.Normal;
      //            }
      //            catch (IOException e) { attr = FileAttributes.ReadOnly; }
      //        }
      //    }
      //    catch (IOException e)
      //    {
      //    }

      ////  free(zConverted);
      //switch ( flags )
      //{
      //  case SQLITE_ACCESS_READ:
      //  case SQLITE_ACCESS_EXISTS:
      //    rc = attr != 0 ? 1 : 0;// != INVALID_FILE_ATTRIBUTES;
      //    break;
      //  case SQLITE_ACCESS_READWRITE:
      //    rc = attr == 0 ? 0 : (int)( attr & FileAttributes.ReadOnly ) != 0 ? 0 : 1; //FILE_ATTRIBUTE_READONLY ) == 0;
      //    break;
      //  default:
      //    Debug.Assert( "" == "Invalid flags argument" );
      //    rc = 0;
      //    break;
      //}
      //pResOut = rc;
      //return SQLITE_OK;
    }

    /*
    ** Turn a relative pathname into a full pathname.  Write the full
    ** pathname into zOut[].  zOut[] will be at least pVfs.mxPathname
    ** bytes in size.
    */
    static int winFullPathname(
    sqlite3_vfs pVfs,             /* Pointer to vfs object */
    string zRelative,             /* Possibly relative input path */
    int nFull,                    /* Size of output buffer in bytes */
    StringBuilder zFull           /* Output buffer */
    )
    {
        zFull.Append(zRelative);
        return SQLITE_OK;
      int nByte;
      string zOut = null;
      UNUSED_PARAMETER( nFull );
      //convertUtf8Filename(zRelative));
      if ( isNT() )
      {
        try
        {
          zOut = Path.GetFullPath( zRelative ); // was unicodeToUtf8(zTemp);
        }
        catch ( IOException e )
        { zOut = zRelative; }
      }
      else
      {
        Debugger.Break(); // -- Not Running under NT
      }
      if ( zOut != null )
      {
        // sqlite3_snprintf(pVfs.mxPathname, zFull, "%s", zOut);
        if ( zFull.Length > pVfs.mxPathname ) zFull.Length = pVfs.mxPathname;
        zFull.Append( zOut );

        // will happen on exit; was   free(zOut);
        return SQLITE_OK;
      }
      else
      {
        return SQLITE_NOMEM;
      }
    }


    /*
    ** Get the sector size of the device used to store
    ** file.
    */
    static int getSectorSize(
    sqlite3_vfs pVfs,
    string zRelative     /* UTF-8 file name */
    )
    {
      return SQLITE_DEFAULT_SECTOR_SIZE;
    }


    //TODO -- Fix This
    static HANDLE winDlOpen( sqlite3_vfs vfs, string zFilename ) { return new HANDLE(); }
    static int winDlError( sqlite3_vfs vfs, int nByte, ref string zErrMsg ) { return 0; }
    static HANDLE winDlSym( sqlite3_vfs vfs, HANDLE data, string zSymbol ) { return new HANDLE(); }
    static int winDlClose( sqlite3_vfs vfs, HANDLE data ) { return 0; }

    static int winRandomness( sqlite3_vfs pVfs, int nBuf, ref byte[] zBuf )
    {
      int n = 0;
      UNUSED_PARAMETER( pVfs );

byte[] sBuf = BitConverter.GetBytes(System.DateTime.Now.Ticks);
zBuf[0] = sBuf[0];
zBuf[1] = sBuf[1];
zBuf[2] = sBuf[2];
zBuf[3] = sBuf[3];
;// memcpy(&zBuf[n], x, sizeof(x))
n += 16;// sizeof(x);
if ( sizeof( DWORD ) <= nBuf - n )
{
//DWORD pid = GetCurrentProcessId();
    u32 processId;
    processId = 28376023;

put32bits( zBuf, n, processId);//(memcpy(&zBuf[n], pid, sizeof(pid));
n += 4;// sizeof(pid);
}
if ( sizeof( DWORD ) <= nBuf - n )
{
//DWORD cnt = GetTickCount();
System.DateTime dt = new System.DateTime();
put32bits( zBuf, n, (u32)dt.Ticks );// memcpy(&zBuf[n], cnt, sizeof(cnt));
n += 4;// cnt.Length;
}
if ( sizeof( long ) <= nBuf - n )
{
long i;
i = System.DateTime.UtcNow.Millisecond;// QueryPerformanceCounter(out i);
put32bits( zBuf, n, (u32)( i & 0xFFFFFFFF ) );//memcpy(&zBuf[n], i, sizeof(i));
put32bits( zBuf, n, (u32)( i >> 32 ) );
n += sizeof( long );
}
      return n;
    }


    /*
    ** Sleep for a little while.  Return the amount of time slept.
    */
    static int winSleep( sqlite3_vfs pVfs, int microsec )
    {
      Thread.Sleep( ( microsec + 999 ) / 1000 );
      UNUSED_PARAMETER( pVfs );
      return ( ( microsec + 999 ) / 1000 ) * 1000;
    }

    /*
    ** The following variable, if set to a non-zero value, becomes the result
    ** returned from sqlite3OsCurrentTime().  This is used for testing.

    /*
** Find the current time (in Universal Coordinated Time).  Write the
** current time and date as a Julian Day number into prNow and
** return 0.  Return 1 if the time and date cannot be found.
*/
    static int winCurrentTime( sqlite3_vfs pVfs, ref double prNow )
    {
      sqlite3_int64 timeW;   /* Whole days */
      sqlite3_int64 timeF;   /* Fractional Days */

      /* Number of 100-nanosecond intervals in a single day */
      const sqlite3_int64 ntuPerDay =
      10000000 * (sqlite3_int64)86400;

      /* Number of 100-nanosecond intervals in half of a day */
      const sqlite3_int64 ntuPerHalfDay =
      10000000 * (sqlite3_int64)43200;

      timeW = System.DateTime.UtcNow.ToFileTime();
      timeF = timeW % ntuPerDay;          /* fractional days (100-nanoseconds) */
      timeW = timeW / ntuPerDay;          /* whole days */
      timeW = timeW + 2305813;            /* add whole days (from 2305813.5) */
      timeF = timeF + ntuPerHalfDay;      /* add half a day (from 2305813.5) */
      timeW = timeW + ( timeF / ntuPerDay );  /* add whole day if half day made one */
      timeF = timeF % ntuPerDay;          /* compute new fractional days */
      prNow = (double)timeW + ( (double)timeF / (double)ntuPerDay );
      return 0;
    }


    /*
    ** The idea is that this function works like a combination of
    ** GetLastError() and FormatMessage() on windows (or errno and
    ** strerror_r() on unix). After an error is returned by an OS
    ** function, SQLite calls this function with zBuf pointing to
    ** a buffer of nBuf bytes. The OS layer should populate the
    ** buffer with a nul-terminated UTF-8 encoded error message
    ** describing the last IO error to have occurred within the calling
    ** thread.
    **
    ** If the error message is too large for the supplied buffer,
    ** it should be truncated. The return value of xGetLastError
    ** is zero if the error message fits in the buffer, or non-zero
    ** otherwise (if the message was truncated). If non-zero is returned,
    ** then it is not necessary to include the nul-terminator character
    ** in the output buffer.
    **
    ** Not supplying an error message will have no adverse effect
    ** on SQLite. It is fine to have an implementation that never
    ** returns an error message:
    **
    **   int xGetLastError(sqlite3_vfs pVfs, int nBuf, char *zBuf){
    **     Debug.Assert(zBuf[0]=='\0');
    **     return 0;
    **   }
    **
    ** However if an error message is supplied, it will be incorporated
    ** by sqlite into the error message available to the user using
    ** sqlite3_errmsg(), possibly making IO errors easier to debug.
    */
    static int winGetLastError( sqlite3_vfs pVfs, int nBuf, ref string zBuf )
    {
      UNUSED_PARAMETER( pVfs );
      return getLastErrorMsg( nBuf, ref zBuf );
    }

    static sqlite3_vfs winVfs = new sqlite3_vfs(
    1,                              /* iVersion */
    -1, //sqlite3_file.Length,      /* szOsFile */
    MAX_PATH,                       /* mxPathname */
    null,                           /* pNext */
    "win32",                        /* zName */
    0,                              /* pAppData */

    (dxOpen)winOpen,                /* xOpen */
    (dxDelete)winDelete,            /* xDelete */
    (dxAccess)winAccess,            /* xAccess */
    (dxFullPathname)winFullPathname,/* xFullPathname */
    (dxDlOpen)winDlOpen,            /* xDlOpen */
    (dxDlError)winDlError,          /* xDlError */
    (dxDlSym)winDlSym,              /* xDlSym */
    (dxDlClose)winDlClose,          /* xDlClose */
    (dxRandomness)winRandomness,    /* xRandomness */
    (dxSleep)winSleep,              /* xSleep */
    (dxCurrentTime)winCurrentTime,  /* xCurrentTime */
    (dxGetLastError)winGetLastError /* xGetLastError */
    );

    /*
    ** Initialize and deinitialize the operating system interface.
    */
    static int sqlite3_os_init()
    {
      sqlite3_vfs_register( winVfs, 1 );
      return SQLITE_OK;
    }
    static int sqlite3_os_end()
    {
      return SQLITE_OK;
    }

    //
    //          Windows DLL definitions
    //

    const int NO_ERROR = 0;
    /// <summary>
    /// Basic locking strategy for Console/Winform applications
    /// </summary>
    private class LockingStrategy
    {
      public virtual void LockFile( sqlite3_file pFile, long offset, long length )
      {
      }

      public virtual int SharedLockFile( sqlite3_file pFile, long offset, long length )
      {
        return 1;
      }

      public virtual void UnlockFile( sqlite3_file pFile, long offset, long length )
      {
      }
    }

    /// <summary>
    /// Locking strategy for Medium Trust. It uses the same trick used in the native code for WIN_CE
    /// which doesn't support LockFileEx as well.
    /// </summary>
    private class MediumTrustLockingStrategy : LockingStrategy
    {
      public override int SharedLockFile( sqlite3_file pFile, long offset, long length )
      {
        return 1;
      }
    }
  }
  internal static class HelperMethods
  {
    public static bool IsRunningMediumTrust()
    {
      return true;
    }
  }
}
