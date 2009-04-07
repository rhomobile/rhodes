#ifndef _RHOFATALERROR_H_
#define _RHOFATALERROR_H_

#include "RhoPort.h"

#ifdef OS_SYMBIAN
#include <e32std.h>
#endif 

namespace rho{
namespace general{

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
            //TODO: debugbreak for OS_MACOSX
            exit(-1);
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
