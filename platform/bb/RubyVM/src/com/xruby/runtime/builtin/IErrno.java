/**
 * Copyright 2005-2007 Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

public interface IErrno{
    int    ENOTEMPTY    = 1;
    int    ERANGE       = 2;
    int    ESPIPE       = 3;
    int    ENFILE       = 4;
    int    EXDEV        = 5;
    int    ENOMEM       = 6;
    int    E2BIG        = 7;
    int    ENOENT       = 8;
    int    ENOSYS       = 9;
    int    EDOM         = 10;
    int    ENOSPC       = 11;
    int    EINVAL       = 42;
    int    EEXIST       = 43;
    int    EAGAIN       = 44;
    int    ENXIO        = 45;
    int    EILSEQ       = 46;
    int    ENOLCK       = 47;
    int    EPIPE        = 48;
    int    EFBIG        = 49;
    int    EISDIR       = 50;
    int    EBUSY        = 51;
    int    ECHILD       = 52;
    int    EIO          = 53;
    int    EPERM        = 54;
    int    EDEADLOCK    = 55;
    int    ENAMETOOLONG = 56;
    int    EMLINK       = 57;
    int    ENOTTY       = 58;
    int    ENOTDIR      = 59;
    int    EFAULT       = 60;
    int    EBADF        = 61;
    int    EINTR        = 62;
    int    EWOULDBLOCK  = 63;
    int    EDEADLK      = 64;
    int    EROFS        = 65;
    int    EMFILE       = 66;
    int    ENODEV       = 67;
    int    EACCES       = 68;
    int    ENOEXEC      = 69;
    int    ESRCH        = 70;
    int    ECONNREFUSED = 71;
    int    ECONNRESET   = 72;
    int    EADDRINUSE   = 73;
}
