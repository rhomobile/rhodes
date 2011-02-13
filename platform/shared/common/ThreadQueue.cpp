#include "ThreadQueue.h"

//1. when stop thread - cancel current command. Add cancelCurrentCommand to ThreadQueue and call it from stop

namespace rho {
namespace common {

CThreadQueue::CThreadQueue(common::IRhoClassFactory* factory) : CRhoThread(factory)
{
    m_nPollInterval = QUEUE_POLL_INTERVAL_SECONDS;
    m_bNoThreaded = false;

    m_ptrFactory = factory;
}

CThreadQueue::~CThreadQueue(void)
{
}

boolean CThreadQueue::isAlreadyExist(IQueueCommand *pCmd)
{
    boolean bExist = false;
    if ( isSkipDuplicateCmd() )
    {
        for (int i = 0; i < (int)m_stackCommands.size(); ++i)
        {
            if (m_stackCommands.get(i)->equals(*pCmd))
            {
                LOG(INFO) + "Command already exists in queue. Skip it.";
                bExist = true;
                break;
            }
        }
    }

    return bExist;
}

void CThreadQueue::addQueueCommandInt(IQueueCommand* pCmd)
{
    LOG(INFO) + "addCommand: " + pCmd->toString();

    synchronized(m_mxStackCommands);

    if ( !isAlreadyExist(pCmd) )
        m_stackCommands.add(pCmd);
}

void CThreadQueue::addQueueCommandToFrontInt(IQueueCommand *pCmd)
{
    LOG(INFO) + "addCommand to front: " + pCmd->toString();

    synchronized(m_mxStackCommands);

    if (!isAlreadyExist(pCmd))
        m_stackCommands.addToFront(pCmd);
}

void CThreadQueue::addQueueCommand(IQueueCommand* pCmd)
{ 
    addQueueCommandInt(pCmd);

    if ( isNoThreadedMode()  )
        processCommands();
    else if ( isAlive() )
	    stopWait(); 
}

void CThreadQueue::addQueueCommandToFront(IQueueCommand* pCmd)
{
    addQueueCommandToFrontInt(pCmd);

    if ( isNoThreadedMode() )
        processCommands();
    else if ( isAlive() )
        stopWait();
}

void CThreadQueue::run()
{
	LOG(INFO) + "Starting main routine...";

	int nLastPollInterval = getLastPollInterval();
	while( !isStopping() )
	{
        unsigned int nWait = m_nPollInterval > 0 ? m_nPollInterval : QUEUE_POLL_INTERVAL_INFINITE;

        if ( m_nPollInterval > 0 && nLastPollInterval > 0 )
        {
            int nWait2 = m_nPollInterval - nLastPollInterval;
            if ( nWait2 <= 0 )
                nWait = QUEUE_STARTUP_INTERVAL_SECONDS;
            else
                nWait = nWait2;
        }

        if ( nWait >= 0 && !isStopping() && isNoCommands() )
		{
            LOG(INFO) + "ThreadQueue blocked for " + nWait + " seconds...";
            if ( wait(nWait) == 1 )
                onTimeout();
        }
        nLastPollInterval = 0;

        if ( !isStopping() )
    		processCommands();
	}

    LOG(INFO) + "Thread shutdown";
}

boolean CThreadQueue::isNoCommands()
{
	boolean bEmpty = false;
	synchronized(m_mxStackCommands)
    {		
		bEmpty = m_stackCommands.isEmpty();
	}

	return bEmpty;
}

void CThreadQueue::processCommands()//throws Exception
{
	while(!isStopping() && !isNoCommands())
	{
		common::CAutoPtr<IQueueCommand> pCmd = null;
    	{
        	synchronized(m_mxStackCommands);
    		pCmd = (IQueueCommand*)m_stackCommands.removeFirst();
    	}
		
		processCommand(pCmd);
	}
}

void CThreadQueue::setPollInterval(int nInterval)
{ 
    m_nPollInterval = nInterval; 
    if ( isAlive() )
        stopWait();
}

};
};

