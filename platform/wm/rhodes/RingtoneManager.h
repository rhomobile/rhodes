#ifndef __RINGTONE_MANAGER__H__
#define __RINGTONE_MANAGER__H__

#include "ext/rho/rhoruby.h"

#if _WIN32_WCE > 0x501

#include "logging/RhoLog.h"
#include "common/RhoMutexLock.h"
#include <soundfile.h>

using namespace rho;
using namespace rho::common;

class CRingtoneManager 
{
    DEFINE_LOGCLASS;

  public:
    static CRingtoneManager& getCRingtoneManager();
        
    void getAllRingtones (Hashtable<String, String> &ringtones);
    void play (String ringtoneName);
    void stop ();
    
  private:
    CRingtoneManager ();
    ~CRingtoneManager ();
    
    CRingtoneManager (CRingtoneManager const&) {}
    CRingtoneManager& operator= (CRingtoneManager const&){}
    
    static void createCRingtoneManager();
    
    static CRingtoneManager *m_pInstance;
    static CMutex m_mxRMLocker;

    HSOUND m_hSound;

};
#endif

extern "C" VALUE rho_ringtone_manager_get_all();
extern "C" void rho_ringtone_manager_stop();
extern "C" void rho_ringtone_manager_play(char* file_name);

#endif
