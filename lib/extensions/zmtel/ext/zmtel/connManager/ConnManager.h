#ifndef __CONN_MANAGER_H__
#define __CONN_MANAGER_H__

#include <assert.h>

#define MAX_PHONE_LENGTH                        (100U)
#define DEVICE_ID_LENGTH                        (20U)

class ConnManager {
public: // Methods
    static void destroy();

    static ConnManager* instance() 
    {
        if(NULL == m_connManager)
        {
            m_connManager = new ConnManager();
        }
        return m_connManager;
    }

    char* getPhoneNumber(unsigned lineNumber);

    char* getDeviceID();

private: // Constructor and Destructor
    ConnManager();

    ~ConnManager();

private: // Data members
    // Single instance
    static ConnManager*             m_connManager;


    char                            m_phoneNumber[MAX_PHONE_LENGTH];
    char                            m_deviceId[DEVICE_ID_LENGTH];
};
#endif // __CONN_MANAGER_H__

// End of file
