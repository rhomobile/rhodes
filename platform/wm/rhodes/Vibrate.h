#ifndef __VIBRATE__H__
#define __VIBRATE__H__

#include <logging/RhoLog.h>
#include <common/RhoThread.h>

using namespace rho;
using namespace rho::common;

class CVibrate :  public CRhoThread
{
    DEFINE_LOGCLASS;
    
  public:
    static CVibrate& getCVibrate();
    
    void toggle ();
    void toggle (int seconds);
    void untoggle ();
    
  private:
    CVibrate ();
    ~CVibrate ();
    
    virtual void run();
    
    int m_nDuration;
    bool m_bToggled;
    
    static void createCVibrate();
    static CVibrate *m_pInstance;    
    static CMutex m_mxLocker;
};

#endif // __VIBRATE__H__
