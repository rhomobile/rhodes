#ifndef __RHO_UNIVERSAL_LOCK
#define __RHO_UNIVERSAL_LOCK


#if !defined(_WIN32_WCE)

#define INIT_LOCK(name)\
static int __g_mutex_init_##name = 0;\
pthread_mutex_t __g_mutex_##name = PTHREAD_MUTEX_INITIALIZER;

#define LOCK(name) {if(!__g_mutex_init_##name){pthread_mutex_init(&__g_mutex_##name, NULL);__g_mutex_init_##name=1;} pthread_mutex_lock(&__g_mutex_##name);}
#define UNLOCK(name) pthread_mutex_unlock(&__g_mutex_##name);

#else
#define INIT_LOCK(name)\
static int __g_cs_init_##name = 0;\
CRITICAL_SECTION __g_cs_##name;

#define LOCK(name) {if(!__g_cs_init_##name){InitializeCriticalSection(&__g_cs_##name);__g_cs_init_##name=1;} EnterCriticalSection(&__g_cs_##name);}
#define UNLOCK(name) LeaveCriticalSection(&__g_cs_##name);
#endif

#endif //__RHO_UNIVERSAL_LOCK