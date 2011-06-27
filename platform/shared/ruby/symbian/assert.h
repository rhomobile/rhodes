#ifndef _ASSERT_H_
#define _ASSERT_H_

#if defined( NDEBUG ) || !defined( DEBUG )

#define assert(_Expression)     ((void)0)

#else

//void assert( int expression );
//#include "logging/RhoPlainLog.h"

#define assert(exp) RHO_ASSERT(exp)

#endif //NDEBUG

#endif //_ASSERT_H_

