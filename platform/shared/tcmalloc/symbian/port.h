#ifndef RHO_SYM_PORT__
#define RHO_SYM_PORT__

#include "config.h"
#include "maybe_threads.h"

inline void * operator new(unsigned int, void *_Where)
        {return (_Where); }
inline void  operator delete(void *, void *)
        {return; }

class SpinLock {
 public:
  SpinLock();
  // Used for global SpinLock vars (see base/spinlock.h for more details).
  enum StaticInitializer { LINKER_INITIALIZED };
  SpinLock(StaticInitializer);

  // It's important SpinLock not have a destructor: otherwise we run
  // into problems when the main thread has exited, but other threads
  // are still running and try to access a main-thread spinlock.  This
  // means we leak mutex_ (we should call DeleteCriticalSection()
  // here).  However, I've verified that all SpinLocks used in
  // perftools have program-long scope anyway, so the leak is
  // perfectly fine.  But be aware of this for the future!

  void Lock();
  void Unlock();

  // Used in assertion checks: assert(lock.IsHeld()) (see base/spinlock.h).
  inline bool IsHeld() const {
    // This works, but probes undocumented internals, so I've commented it out.
    // c.f. http://msdn.microsoft.com/msdnmag/issues/03/12/CriticalSections/
    //return mutex_.LockCount>=0 && mutex_.OwningThread==GetCurrentThreadId();
    return true;
  }
 private:
  //void InitializeMutex() {
  //    pthread_mutex_init(&mutex_, NULL);
  //}

  int initialize_token_;
  pthread_mutex_t mutex_;
};

class SpinLockHolder {  // Acquires a spinlock for as long as the scope lasts
 private:
  SpinLock* lock_;
 public:
  inline explicit SpinLockHolder(SpinLock* l) : lock_(l) { l->Lock(); }
  inline ~SpinLockHolder() { lock_->Unlock(); }
};

#define BASE_SPINLOCK_H_ 1

void _sym_assert(int exp);
#undef ASSERT
#define ASSERT(exp) _sym_assert(exp)
#endif //RHO_SYM_PORT__
