#if defined(__APPLE__) && !defined(__arm__) && !defined(__IPHONE_4_0)
/* Simulator 3.2 or less */
#define RHO_IPHONE_SIMULATOR_3
#endif

#ifdef RHO_IPHONE_SIMULATOR_3
#ifdef stat
#undef stat
#endif
#ifdef lstat
#undef lstat
#endif
#ifdef fstat
#undef fstat
#endif
#endif

#include <sys/stat.h>

#ifdef RHO_IPHONE_SIMULATOR_3
/* 
 * map stat functions and structure to theirs 64-bit analogues to be binary
 * compatible with iPhone 4 x86/x86_64 application - in iPhone 4 SDK stat
 * become 64-bit only so enabling such mapping we could run applications built
 * with 3.x SDK on iPhone 4 simulator
 * This is not required for iPhone devices - there stat was always 64-bit.
 */
#define stat stat64
#define lstat lstat64
#define fstat fstat64
#endif
