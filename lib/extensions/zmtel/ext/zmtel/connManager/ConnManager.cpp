#include "../stdafx.h"

#include "ConnManager.h"

#include "Declare.h"
#include "stdlib.h"

ConnManager* ConnManager::m_connManager = NULL;

ConnManager::ConnManager()
{
    memset(m_phoneNumber, 0, sizeof(m_phoneNumber));
    memset(m_deviceId, 0, sizeof(m_deviceId));
}

ConnManager::~ConnManager()
{
    memset(m_phoneNumber, 0, sizeof(m_phoneNumber));
    memset(m_deviceId, 0, sizeof(m_deviceId));
}

void ConnManager::destroy()
{
    assert(m_connManager);
    delete m_connManager;
    m_connManager = NULL;
}

char* ConnManager::getPhoneNumber(unsigned lineNumber)
{
    if('\0' == m_phoneNumber[0])
    {
        LPTSTR szNumber = new TCHAR[MAX_PHONE_LENGTH];

        SHGetPhoneNumber(szNumber, MAX_PHONE_LENGTH, lineNumber);

        CStringA cStr(szNumber);
        strcpy(m_phoneNumber, cStr.GetBuffer());
    }

    return m_phoneNumber;
}

char* ConnManager::getDeviceID()
{
    if('\0' == m_deviceId)
    {
        getDeviceUniqueID(m_deviceId);
    }

    return m_deviceId;
}

// End of file
