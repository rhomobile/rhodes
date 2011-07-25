/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
