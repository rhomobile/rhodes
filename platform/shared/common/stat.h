#ifdef stat
#undef stat
#endif
#ifdef lstat
#undef lstat
#endif
#ifdef fstat
#undef fstat
#endif
#include <sys/stat.h>
#if defined(__APPLE__) && !defined(__IPHONE_4_0)
#define stat stat64
#define lstat lstat64
#define fstat fstat64
#endif
