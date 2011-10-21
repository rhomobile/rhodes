/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "symbian.h"

#include <sys/signal.h>
#include <fcntl.h>
#include <pthreadtypes.h>
#include <errno.h>
#include <stdio.h>

char **environ = 0;

typedef void (*sighandler_t)(int);

/*sighandler_t signal(int signum, sighandler_t handler)
{
	return (sighandler_t)0;
}

int sigfillset(sigset_t * set)
{
	return 0;
}

int sigdelset(sigset_t * set, int signum)
{
	return 0;
}

int sigprocmask(int how, const sigset_t * set, sigset_t * oldset)
{
	return 0;
}

int raise(int sig)
{
	return 0;
}

int kill(pid_t pid, int sig)
{
	return 0;
}

int pthread_sigmask(int how, const sigset_t * set, sigset_t * oset)
{
	return -1;
}*/

int pthread_kill(pthread_t thread, int sig)
{
	return -1;
}

/*int sigmask(int signum) {
	return -1;
}

int sigblock(int mask) {
	return -1;
}

int sigsetmask(int mask) {
	return -1;
}*/

int fchown(int fildes, uid_t owner, gid_t group)
{
    return 0;
}

int getrusage(int who, struct rusage *r_usage)
{
    return 0;
}

int
flock(int fd, int oper)
{
    return 0;
}

sighandler_t posix_signal(int signum, sighandler_t handler)
{
    return signal((signum),(handler));
}

int execl(const char * arg1, const char * arg2, ...){
  return 0;
}

int execv(const char *path, char *const argv[]){
  return 0;
}

int _symbian_close(int fd){
    errno = EBADF;
    return 0;
//	return close(fd);
}

int _symbian_fclose(FILE* f){
    errno = EBADF;
    return 0;
//	return close(fd);
}
