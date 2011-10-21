#include <stdlib.h>
#include <libc/sys/unistd.h>
#include "port.h"
#include <stdio.h>

static SpinLock alloc_lock(SpinLock::LINKER_INITIALIZED);
pthread_mutex_t __initmutex = PTHREAD_MUTEX_INITIALIZER;

SpinLock::SpinLock() : initialize_token_(0), mutex_( __initmutex )
{
  pthread_mutex_init(&mutex_, NULL);
}

SpinLock::SpinLock(StaticInitializer) : initialize_token_(0), mutex_( __initmutex )
{
  pthread_mutex_init(&mutex_, NULL);
}

void SpinLock::Lock()
{
	pthread_mutex_lock(&mutex_);
}

void SpinLock::Unlock() {
    pthread_mutex_unlock(&mutex_);
}

static long long g_nTotalMemory = 0;
// This is mostly like MmapSysAllocator::Alloc, except it does these weird
// munmap's in the middle of the page, which is forbidden in windows.
extern void* TCMalloc_SystemAlloc(size_t size, size_t *actual_size,
                                  size_t alignment) {
  // Safest is to make actual_size same as input-size.
  if (actual_size) {
    *actual_size = size;
  }

  SpinLockHolder sh(&alloc_lock);
  // Align on the pagesize boundary
  const int pagesize = getpagesize();
  if (alignment < pagesize) alignment = pagesize;
  size = ((size + alignment - 1) / alignment) * alignment;

  // Ask for extra memory if alignment > pagesize
  size_t extra = 0;
  if (alignment > pagesize) {
    extra = alignment - pagesize;
  }

  //void* result = VirtualAlloc(0, size + extra,
  //                            MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
  void* result = malloc(size + extra);//new unsigned char[size + extra];//malloc(size + extra);

  g_nTotalMemory += size + extra;
  printf("TCMALLOC: VirtualAlloc = %d; Total: %d(Mb)\n", size + extra, g_nTotalMemory/(1024*1024));

  if (result == NULL)
    return NULL;

  // Adjust the return memory so it is aligned
  uintptr_t ptr = reinterpret_cast<uintptr_t>(result);
/*  size_t adjust = 0;
  if ((ptr & (alignment - 1)) != 0) {
    adjust = alignment - (ptr & (alignment - 1));
  }

  ptr += adjust;*/
  return reinterpret_cast<void*>(ptr);
}

void TCMalloc_SystemRelease(void* start, size_t length) {
    //TODO: TCMalloc_SystemRelease
}

void _sym_assert(int exp)
{
    if ( !exp)
        exit(-1);
}
