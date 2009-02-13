#ifndef RHO_MEMORY__
#define RHO_MEMORY__

#ifdef _WIN32_WCE

#ifndef _RHO_NO_MEMDEFINES
#undef _CRTDBG_MAP_ALLOC

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

#ifdef __cplusplus
extern "C" {
#endif

void   __cdecl rho_free(void *);
void * __cdecl rho_malloc(size_t);
void * __cdecl rho_calloc(size_t num, size_t size);
size_t __cdecl rho_msize(void *);
void * __cdecl rho_realloc(void *, size_t);

#ifdef __cplusplus
};
#endif

#define free(p) rho_free(p)
#define malloc(s) rho_malloc(s)
#define calloc(num, size) rho_calloc(num,size)
#define _msize( p) rho_msize(p)
#define realloc(p, s) rho_realloc(p,s)

#endif //_RHO_NO_MEMDEFINES

#endif// _WIN32_WCE
#endif  // RHO_MEMORY__