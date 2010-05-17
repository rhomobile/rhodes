#ifndef RHO_MEMORY__
#define RHO_MEMORY__

//#ifndef __APPLE__
#if defined( _WIN32_WCE ) || defined(__SYMBIAN32__)
#include <stdlib.h>

#ifndef _RHO_NO_MEMDEFINES
#undef _CRTDBG_MAP_ALLOC

#if (defined( _WIN32_WCE ) || defined( WIN32 )) && !defined( _SIZE_T_DEFINED)
typedef unsigned int size_t;
#define _SIZE_T_DEFINED  1
#endif

#if defined(__SYMBIAN32__) && !defined( _SIZE_T_DECLARED)
typedef unsigned int size_t;
#define _SIZE_T_DECLARED  1
#endif

#if defined(__APPLE__) && !defined( _SIZE_T)
typedef unsigned int size_t;
#define _SIZE_T  1
#endif

#ifdef __cplusplus
extern "C" {
#endif

void    rho_free(void *);
void *  rho_malloc(size_t);
void *  rho_calloc(size_t num, size_t size);
size_t  rho_msize(void *);
void *  rho_realloc(void *, size_t);
char *  rho_strdup(const char *);
wchar_t *  rho_wcsdup(const wchar_t * str);
void    sys_free(void *);
#ifdef __cplusplus
}
#endif

#undef free
#undef malloc
#undef calloc
#undef _msize
#undef realloc
#undef strdup
#undef _strdup
#undef _wcsdup

//#define _recalloc(p, n, s) rho_realloc(p, n*s)
#define free(p) rho_free(p)
#define malloc(s) rho_malloc(s)
#define calloc(num, size) rho_calloc(num,size)
#define _msize( p) rho_msize(p)
#define realloc(p, s) rho_realloc(p,s)

#define strdup(s) rho_strdup(s)
#define _strdup(s) rho_strdup(s)
#define _wcsdup(s) rho_wcsdup(s)

#ifndef __SYMBIAN32__

#ifdef __cplusplus
void* cpp_alloc(size_t size, bool nothrow);

#include <new>

#ifndef __THROW
#define __THROW throw()
#endif

inline void* operator new(size_t size) {
  void* p = cpp_alloc(size, false);
  return p;
}

inline void operator delete(void* p) __THROW {
  rho_free(p);
}

inline  void* operator new[](size_t size) {
  void* p = cpp_alloc(size, false);
  return p;
}

inline void operator delete[](void* p) __THROW {
  rho_free(p);
}
/*
#ifndef __NOTHROW_T_DEFINED
#define __NOTHROW_T_DEFINED
namespace std {
        struct nothrow_t {};

        extern const nothrow_t nothrow;
};
#endif //__NOTHROW_T_DEFINED
*/

inline void operator delete(void* p, const std::nothrow_t&) __THROW {
  rho_free(p);
}
inline void* operator new[](size_t size, const std::nothrow_t&) __THROW {
  void* p = cpp_alloc(size, true);
  return p;
}
inline void operator delete[](void* p, const std::nothrow_t&) __THROW {
  rho_free(p);
}
inline void* operator new(size_t size, const std::nothrow_t&) __THROW {
  void* p = cpp_alloc(size, true);
  return p;
}

#endif// __cplusplus

#endif

#endif //_RHO_NO_MEMDEFINES

//#endif// _WIN32_WCE
#endif

#endif  // RHO_MEMORY__
