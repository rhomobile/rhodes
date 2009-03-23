#include "stat.h"

//ruby vm statistics definition
unsigned long g_iseq_binread_sec = 0; 
unsigned long g_iseq_binread_msec = 0; 

unsigned long g_iseq_marshal_load_sec = 0;
unsigned long g_iseq_marshal_load_msec = 0;

unsigned long g_require_compiled_sec = 0;
unsigned long g_require_compiled_msec = 0;

int g_collect_stat = 0; 

int g_httpd_thread_loaded = 0;

int g_sync_thread_loaded = 0;