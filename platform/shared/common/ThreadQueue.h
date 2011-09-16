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

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"

namespace rho {
namespace common {

class CThreadQueue : public common::CRhoThread
{
protected:
    static const unsigned int QUEUE_POLL_INTERVAL_SECONDS  = 300;
    static const unsigned int QUEUE_POLL_INTERVAL_INFINITE  = (unsigned int)(-1);
    static const unsigned int QUEUE_STARTUP_INTERVAL_SECONDS  = 10;

public:
    DEFINE_BASELOGCLASS

    struct IQueueCommand
    {
        virtual ~IQueueCommand(){};
        virtual boolean equals(const IQueueCommand& cmd) = 0;
        virtual String toString() = 0;
        virtual void cancel(){}
    };

private:

	int           m_nPollInterval;
   	common::CMutex m_mxStackCommands;
	LinkedListPtr<IQueueCommand*> m_stackCommands;
    IQueueCommand* m_pCurCmd;

    boolean m_bNoThreaded;
public:
    CThreadQueue();

    ~CThreadQueue(void);

    virtual void addQueueCommand(IQueueCommand* pCmd);
    virtual void addQueueCommandToFront(IQueueCommand* pCmd);
	virtual void run();
    virtual void stop(unsigned int nTimeoutToKill);

	void setPollInterval(int nInterval);
    int  getPollInterval()const{ return m_nPollInterval;}

    boolean isNoThreadedMode(){ return m_bNoThreaded; }
    void setNonThreadedMode(boolean b){m_bNoThreaded = b;}

    void cancelCurrentCommand();
    common::CMutex& getCommandLock(){ return m_mxStackCommands; }
    IQueueCommand* getCurCommand(){ return m_pCurCmd; }
    LinkedListPtr<IQueueCommand*>& getCommands(){ return m_stackCommands; }

    int getCommandsCount();	
protected:
    virtual int getLastPollInterval(){ return 0;}
    virtual void processCommand(IQueueCommand* pCmd) = 0;
    virtual boolean isSkipDuplicateCmd() { return false; }
    virtual void onTimeout(){}

    boolean isAlreadyExist(IQueueCommand *pCmd);

    void processCommands();
    void processCommandBase(IQueueCommand* pCmd);

    void addQueueCommandInt(IQueueCommand* pCmd);
    void addQueueCommandToFrontInt(IQueueCommand* pCmd);

    boolean isNoCommands();
};

}
}
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
#ifdef __cplusplus
};
#endif //__cplusplus


