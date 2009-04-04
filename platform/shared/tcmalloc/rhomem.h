#ifndef RHO_MEMORY__
#define RHO_MEMORY__

//#if defined( _WIN32_WCE ) || defined(__SYMBIAN32__)

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

#ifdef __cplusplus
};
#endif

//#define _recalloc(p, n, s) rho_realloc(p, n*s)
#define free(p) rho_free(p)
#define malloc(s) rho_malloc(s)
#define calloc(num, size) rho_calloc(num,size)
#define _msize( p) rho_msize(p)
#define realloc(p, s) rho_realloc(p,s)

#define strdup(s) rho_strdup(s)
#define _strdup(s) rho_strdup(s)

#endif //_RHO_NO_MEMDEFINES

//#endif// _WIN32_WCE

#endif  // RHO_MEMORY__
