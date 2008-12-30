#ifndef DEBUG_STAT_H
#define DEBUG_STAT_H 1

//ruby vm statistics
//NOTE: define ENABLE_RUBY_VM_STAT to enable calculation of the statistics 

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_RUBY_VM_STAT

extern int g_collect_stat;

extern unsigned long g_iseq_binread_msec; 
extern unsigned long g_iseq_marshal_load_msec; 
extern unsigned long g_require_compiled_msec;

extern int g_httpd_thread_loaded;
extern int g_sync_thread_loaded;

#endif

#ifdef __cplusplus
}
#endif

#endif