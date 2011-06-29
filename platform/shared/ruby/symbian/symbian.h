#ifndef SYMBIAN_H
#define SYMBIAN_H 1

#include <sys/types.h>

//#define close _symbian_close
//#define fclose _symbian_fclose
#define Sleep sleep

extern void* alloca ( unsigned int size);// defined in ext/alloca

/*#define HAVE_PIPS 1

#ifdef HAVE_PIPS

#include <sys/signal.h>
#include <fcntl.h>
#include <pthreadtypes.h>

#define HAVE_SOCKADDR_STORAGE

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

int sigfillset(sigset_t *set); 
int sigdelset(sigset_t *set, int signum); 
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset); 
int raise(int sig);
int kill(pid_t pid, int sig);
int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset);
int execv(const char *path, char *const argv[]); 
int pthread_kill(pthread_t thread, int sig);
#endif
*/
#endif
