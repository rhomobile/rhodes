#include "AnsSync.h"
#include <common/RhoConf.h>
#include "PushClient.h"
using namespace RhoIPC;

CAnsSync* CAnsSync::m_pInstance = 0;

CAnsSync* CAnsSync::Create() 
{
	if ( m_pInstance ) 
		return m_pInstance;

	m_pInstance = new CAnsSync();
	return m_pInstance;
}

void CAnsSync::Destroy()
{
	if ( m_pInstance )
		delete m_pInstance;

	m_pInstance = 0;
}

CAnsSync::CAnsSync()
{
	CThreadQueue::setLogCategory(LogCategory("RhoConnectPush"));
	setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
	start(epLow);
}

CAnsSync::~CAnsSync()
{
	//wait till all commands will be sent to server
	CRhoThread::stop(2000);
}

void CAnsSync::sendCommand(int cmd)
{
	addQueueCommand(new AnsCommand(cmd));
}


void CAnsSync::processCommand(IQueueCommand* pCmd)
{
	AnsCommand *cmd = (AnsCommand *)pCmd;
	if (!cmd)
		return;

	if(cmd->m_cmd == Register)
	{
		LOG(INFO) + "process command register";	
		PushClient::Register();
	}
	else if(cmd->m_cmd == Unregister)
	{
		LOG(INFO) + "process command unregister";		
		PushClient::Unregister();
	}
}