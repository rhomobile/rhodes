#include "stdafx.h"
#include "PowerManagement.h"
#include "logging/RhoLog.h"
#include "common/RhodesApp.h"

#include <pm.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "PowerManagement"

extern "C" void createPowerManagementThread()
{  
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
    mqo.dwMaxMessages = 4;
    mqo.cbMaxMessage  = cbPowerMsgSize;
    mqo.bReadAccess   = TRUE;              

    // Create a message queue to receive power notifications
    m_powerMsgQ = CreateMsgQueue(NULL, &mqo);
    if (NULL == m_powerMsgQ) 
    {
        RAWLOG_ERROR1("CreateMsgQueue failed: %x", GetLastError());
        closePowerManQueue();
        return;
    }

    // Request power notifications 
    m_powerNotifications = RequestPowerNotifications(m_powerMsgQ, PBT_TRANSITION);
    if (NULL == m_powerNotifications) 
    {
        RAWLOG_ERROR1("RequestPowerNotifications failed: %x", GetLastError());
        closePowerManQueue();
        return;
    }

    // Wait for a power notification or for the app to exit
    while(WaitForSingleObject(m_powerMsgQ, INFINITE) == WAIT_OBJECT_0)
    {
        DWORD cbRead;
        DWORD dwFlags;
        POWER_BROADCAST *ppb = (POWER_BROADCAST*) new BYTE[cbPowerMsgSize];

        // loop through in case there is more than 1 msg 
        while(ReadMsgQueue(m_powerMsgQ, ppb, cbPowerMsgSize, &cbRead, 
            0, &dwFlags))
        {
            if (ppb->Message == PBT_TRANSITION)
            {
                if (ppb->Flags & POWER_STATE_ON)
                {
                    rho_rhodesapp_callScreenOnCallback();
                    rho_rhodesapp_callScreenUnlockedCallback();
                }
                else if (ppb->Flags & POWER_STATE_UNATTENDED)
                {
                    rho_rhodesapp_callScreenLockedCallback();
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
    if (m_powerNotifications)
        StopPowerNotifications(m_powerNotifications);

    if (m_powerMsgQ)
        CloseMsgQueue(m_powerMsgQ);
}

}
}