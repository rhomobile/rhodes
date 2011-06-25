#ifndef _RHOFATALERROR_H_
#define _RHOFATALERROR_H_

#include "RhoPort.h"

#if defined(OS_SYMBIAN)
#include <e32std.h>
#endif 

namespace rho{
namespace common{

class CRhoFatalError{
public:
    static void processFatalError(){
        #ifdef RHO_DEBUG

        #if defined (OS_WINDOWS)
            //__debugbreak();
            DebugBreak();
        #elif defined (OS_WINCE)
            //NKDbgPrintfW ?
            DebugBreak();
        #elif defined(OS_SYMBIAN)
            User::Invariant();
//            exit(-1);
        #elif defined(OS_MACOSX)
//			__assert_rtn(__func__, __FILE__, __LINE__,"RhoFatalError");
        #else
            exit(-1);
        #endif

        #else //!RHO_DEBUG
            exit(-1);
        #endif //!RHO_DEBUG
    }

};

}
}

#endif //_RHOFATALERROR_H_
