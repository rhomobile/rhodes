#include "stdafx.h"
#include "PowerManagement.h"
#include "logging/RhoLog.h"
#include "common/RhodesApp.h"

#include <pm.h>

//TODO: read: http://msdn.microsoft.com/en-us/library/ms894059.aspx ; there is no  POWER_STATE_UNATTENDED on CE
#ifndef POWER_STATE_UNATTENDED
#define POWER_STATE_UNATTENDED   (DWORD)(0x00400000)        // Unattended state.
#endif 

#define POWER_STATE_BACKLIGHTON  (DWORD)(0x02000000)

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "PowerManagement"

extern "C" void createPowerManagementThread()
{  
    LOG(TRACE) + "start power management thread";
    rho::common::PowerNotificationThread* g_powerManageThread = new rho::common::PowerNotificationThread();
    g_powerManageThread->start(rho::common::IRhoRunnable::epNormal);
};

namespace rho {
namespace common {

void PowerNotificationThread::run()
{
    // size of a POWER_BROADCAST message
    DWORD cbPowerMsgSize = sizeof POWER_BROADCAST + (MAX_PATH * sizeof TCHAR);

    // Initialize our MSGQUEUEOPTIONS structure
    MSGQUEUEOPTIONS mqo;
    mqo.dwSize        = sizeof(MSGQUEUEOPTIONS); 
    mqo.dwFlags       = MSGQUEUE_NOPRECOMMIT;
    mqo.dwMaxMessages = 50;
    mqo.cbMaxMessage  = cbPowerMsgSize;
    mqo.bReadAccess   = TRUE;              

    LOG(TRACE) + "[PMT] Create a message queue to receive power notifications";    
    m_powerMsgQ = CreateMsgQueue(NULL, &mqo);
    if (NULL == m_powerMsgQ) 
    {
        RAWLOG_ERROR1("CreateMsgQueue failed: %x", GetLastError());
        closePowerManQueue();
        return;
    }

    LOG(TRACE) + "[PMT] Request power notifications";
    m_powerNotifications = RequestPowerNotifications(m_powerMsgQ, PBT_TRANSITION);

    if (NULL == m_powerNotifications) 
    {
        RAWLOG_ERROR1("RequestPowerNotifications failed: %x", GetLastError());
        closePowerManQueue();
        return;
    }

    LOG(TRACE) + "[PMT] Wait for a power notification or for the app to exit";
    while(WaitForSingleObject(m_powerMsgQ, INFINITE) == WAIT_OBJECT_0)
    {
        DWORD cbRead;
        DWORD dwFlags;
        POWER_BROADCAST *ppb = reinterpret_cast<POWER_BROADCAST*>(new BYTE[cbPowerMsgSize]);

        // loop through in case there is more than 1 msg 
        while(ReadMsgQueue(m_powerMsgQ, ppb, cbPowerMsgSize, &cbRead, 0, &dwFlags))
        {
            LOG(TRACE) + "[PMT] receive messages: " + cbRead;

            if (ppb->Message == PBT_TRANSITION)
            {
                LOG(TRACE) + "[PMT] receive PBT_TRANSITION message";

//#if defined(OS_PLATFORM_MOTCE)
				DWORD flags;
				if(winversion == 2)
					flags = (POWER_STATE_ON | POWER_STATE_PASSWORD);
				else
					flags = (POWER_STATE_ON | POWER_STATE_PASSWORD | POWER_STATE_BACKLIGHTON);
                //if (ppb->Flags == (POWER_STATE_ON | POWER_STATE_PASSWORD))
//#else
                //if (ppb->Flags == (POWER_STATE_ON | POWER_STATE_PASSWORD | POWER_STATE_BACKLIGHTON))
//#endif
                if (ppb->Flags == flags)
				{
                    LOG(TRACE) + "[PMT] POWER_STATE_ON | POWER_STATE_PASSWORD | POWER_STATE_BACKLIGHTON";
                    rho_rhodesapp_callScreenOnCallback();
                }
                else if (ppb->Flags == (POWER_STATE_ON | POWER_STATE_PASSWORD))
                {
                    LOG(TRACE) + "[PMT] POWER_STATE_ON | POWER_STATE_PASSWORD";
                    rho_rhodesapp_callScreenOffCallback();
                }
                else if (ppb->Flags == POWER_STATE_ON)
                {
                    LOG(TRACE) + "[PMT] POWER_STATE_ON";
                    rho_rhodesapp_callScreenOnCallback();
                }
                else if (ppb->Flags == POWER_STATE_UNATTENDED)
                {
                    LOG(TRACE) + "[PMT] POWER_STATE_UNATTENDED";
                    rho_rhodesapp_callScreenOffCallback();
                }
            }
        }

        delete[] ppb;
    }

    closePowerManQueue();
}

void PowerNotificationThread::closePowerManQueue()
{
    LOG(INFO) + "[PMT] stop power notofication";

    if (m_powerNotifications)
        StopPowerNotifications(m_powerNotifications);

    if (m_powerMsgQ)
        CloseMsgQueue(m_powerMsgQ);
}

}
}