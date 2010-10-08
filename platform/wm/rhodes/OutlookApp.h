#pragma once

#include "logging/RhoLog.h"

struct IPOutlookApp;
class COutlookApp
{
    DEFINE_LOGCLASS;

	IPOutlookApp* m_outlookApp;

    static COutlookApp* m_pInstance;
    COutlookApp();
public:
    ~COutlookApp();

    static COutlookApp* Create();
    static void Destroy();
    static COutlookApp* getInstance(){ return m_pInstance; }

    IPOutlookApp* getApp(){ return m_outlookApp;}
};
