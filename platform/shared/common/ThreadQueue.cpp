#include "ThreadQueue.h"

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

void CThreadQueue::addQueueCommand(CQueueCommand* pCmd)
{ 
    LOG(INFO) + "addCommand: " + pCmd->toString();
	{
        synchronized(m_mxStackCommands);

		boolean bExist = false;
        if ( isSkipDuplicateCmd() )
        {
		    for ( int i = 0; i < (int)m_stackCommands.size(); i++ )
		    {
			    if ( m_stackCommands.get(i)->equals(*pCmd) )
			    {
				    bExist = true;
				    break;
			    }
		    }
        }

		if ( !bExist )
    		m_stackCommands.add(pCmd);
	}

    if ( isNoThreadedMode()  )
        processCommands();
    else
	    stopWait(); 
}

void CThreadQueue::run()
{
	LOG(INFO) + "Starting main routine...";

	int nLastPollInterval = getLastPollInterval();
	while( !isStopped() )
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

        if ( nWait >= 0 && !isStopped() && isNoCommands() )
		{
            LOG(INFO) + "ThreadQueue blocked for " + nWait + " seconds...";
            wait(nWait);
        }
        nLastPollInterval = 0;

        if ( !isStopped() )
    		processCommands();
	}
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
	while(!isStopped() && !isNoCommands())
	{
		common::CAutoPtr<CQueueCommand> pCmd = null;
    	{
        	synchronized(m_mxStackCommands);
    		pCmd = (CQueueCommand*)m_stackCommands.removeFirst();
    	}
		
		processCommand(pCmd);
	}
}

void CThreadQueue::setPollInterval(int nInterval)
{ 
    m_nPollInterval = nInterval; 
    stopWait();
}

};
};

