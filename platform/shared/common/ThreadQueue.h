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

    struct CQueueCommand
    {
        virtual ~CQueueCommand(){};
        virtual boolean equals(const CQueueCommand& cmd) = 0;
        virtual String toString() = 0;
    };

private:

    common::CAutoPtr<common::IRhoClassFactory> m_ptrFactory;
	int           m_nPollInterval;
   	common::CMutex m_mxStackCommands;
	LinkedListPtr<CQueueCommand*> m_stackCommands;

    boolean m_bNoThreaded;
public:
    CThreadQueue(common::IRhoClassFactory* factory);

    ~CThreadQueue(void);

    virtual void addQueueCommand(CQueueCommand* pCmd);
	virtual void run();

	void setPollInterval(int nInterval);
    int  getPollInterval()const{ return m_nPollInterval;}

    boolean isNoThreadedMode(){ return m_bNoThreaded; }
    void setNonThreadedMode(bool b){m_bNoThreaded = b;}

    common::IRhoClassFactory* getFactory(){ return m_ptrFactory; }
protected:
    virtual int getLastPollInterval(){ return 0;}
    virtual void processCommand(CQueueCommand* pCmd) = 0;
    virtual boolean isSkipDuplicateCmd() { return false; }

    virtual void processCommands();

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


