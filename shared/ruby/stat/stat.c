#include "stat.h"

//ruby vm statistics definition
unsigned long g_iseq_binread_sec = 0; //time lapsed in iseq_binread (in seconds)
unsigned long g_iseq_binread_usec = 0; //time lapsed in iseq_binread (in microseconds) 

unsigned long g_iseq_marshal_load_sec = 0; //time lapsed in marshal load (in seconds)
unsigned long g_iseq_marshal_load_usec = 0; //time lapsed in marshal load (in microseconds)

unsigned long g_require_compiled_sec = 0;
unsigned long g_require_compiled_usec = 0;

int g_collect_stat = 0; 