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

#pragma once

#include "RhoStd.h"
#include "IRhoThreadImpl.h"
#include "AutoPointer.h"

namespace rho {
namespace common {

class CRhoThread : public IRhoRunnable
{
    CAutoPtr<IRhoThreadImpl> m_pImpl;
    static const int TS_NONE = 0;
    static const int TS_WAIT = 1;
    static const int TS_STOPPING = 2;
    static const int TS_RUNNING = 4;

    int m_nState;
public:

    CRhoThread();
    virtual ~CRhoThread(void){};

    virtual void start(EPriority ePriority);
    virtual void stop(unsigned int nTimeoutToKill){ m_nState |= TS_STOPPING; if (m_nState&TS_RUNNING) m_pImpl->stop(nTimeoutToKill); m_nState &= ~TS_STOPPING; }
    virtual int wait(unsigned int nTimeout){ m_nState |= TS_WAIT; int nRet = 0; if ((m_nState&TS_RUNNING) && !(m_nState&TS_STOPPING) ) nRet = m_pImpl->wait(nTimeout); m_nState &= ~TS_WAIT; return nRet; }
    virtual void stopWait(){ /*if (isWaiting())*/ m_pImpl->stopWait(); }
    virtual void sleep(unsigned int nTimeout){ m_pImpl->sleep(nTimeout); }
    virtual void run() = 0;
    virtual void runObject()
    {
        run();
        m_nState = TS_NONE;
    }

    boolean isStopping(){return (m_nState&TS_STOPPING) != 0;}
    boolean isWaiting(){return (m_nState&TS_WAIT) != 0;}
    boolean isAlive(){return (m_nState&TS_RUNNING) != 0;}

};

}
}
