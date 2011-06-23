#ifndef _RHOSYSTEM_H_
#define _RHOSYSTEM_H_

#include "RhoPort.h"

#if defined OS_LINUX || defined OS_FREEBSD || defined OS_MACOSX
#include <sys/syscall.h>
#endif

namespace rho{
namespace common{

class CSystem{
public:
    CSystem(){}

    static unsigned int getThreadID();
};

inline unsigned int CSystem::getThreadID(){ 
  // On Linux and FreeBSD, we try to use gettid().
#if defined OS_LINUX || defined OS_FREEBSD || defined OS_MACOSX
#ifndef __NR_gettid
#ifdef OS_MACOSX
#define __NR_gettid SYS_gettid
#elif ! defined __i386__
#error "Must define __NR_gettid for non-x86 platforms"
#else
#define __NR_gettid 224
#endif
#endif
  static bool lacks_gettid = false;
  if (!lacks_gettid) {
    pid_t tid = syscall(__NR_gettid);
    if (tid != -1) {
      return tid;
    }
    // Technically, this variable has to be volatile, but there is a small
    // performance penalty in accessing volatile variables and there should
    // not be any serious adverse effect if a thread does not immediately see
    // the value change to "true".
    lacks_gettid = true;
  }
#endif  // OS_LINUX || OS_FREEBSD

  // If gettid() could not be used, we use one of the following.
#if defined OS_LINUX
  return getpid();  // Linux:  getpid returns thread ID when gettid is absent
#elif defined(OS_WINDOWS) || defined(OS_WINCE) || defined(OS_CYGWIN)  || defined(OS_SYMBIAN32)
  return GetCurrentThreadId();
#else
  // If none of the techniques above worked, we use pthread_self().
    return (pid_t)(int64)pthread_self();
#endif
    }

}
}

#endif //_RHOFILEPATH_H_
