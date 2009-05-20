#ifndef _ASSERT_H_
#define _ASSERT_H_

#ifdef  NDEBUG

#define assert(_Expression)     ((void)0)

#else

//void assert( int expression );
#include "logging/RhoLog.h"

#define assert(exp) RHO_ASSERT(exp)

#endif //NDEBUG

#endif //_ASSERT_H_

