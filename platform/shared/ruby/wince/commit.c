/***
*commit.c - flush buffer to disk
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _commit() - flush buffer to disk
*
*******************************************************************************/

//#include <cruntime.h>
#include <windows.h>
#include <errno.h>
#include <io.h>
//#include <internal.h>
//#include <msdos.h>      /* for FOPEN */
//#include <mtdll.h>
#include <stdlib.h>     /* for _doserrno */

/***
*int _commit(filedes) - flush buffer to disk
*
*Purpose:
*       Flushes cache buffers for the specified file handle to disk
*
*Entry:
*       int filedes - file handle of file
/*
*Exit:
*       returns success code
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _commit (
        int filedes
        )
{
        int retval;

        /* if filedes out of range, complain */
        _CHECK_FH_RETURN( filedes, EBADF, -1 );
        _VALIDATE_RETURN((filedes >= 0 && (unsigned)filedes < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_RETURN((_osfile(filedes) & FOPEN), EBADF, -1);

        _lock_fh(filedes);
        __try {
                if (_osfile(filedes) & FOPEN) {

        if ( !FlushFileBuffers((HANDLE)_get_osfhandle(filedes)) ) {
                retval = GetLastError();
        }
        else {
                retval = 0;     /* return success */
        }

        /* map the OS return code to C errno value and return code */
        if (retval == 0)
                goto good;

        _doserrno = retval;

                }

        errno = EBADF;
        retval = -1;

        _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
good :
        ; }
        __finally {
                _unlock_fh(filedes);
        }
        return (retval);
}
