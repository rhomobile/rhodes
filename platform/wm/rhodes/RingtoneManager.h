#ifndef __RINGTONE_MANAGER__H__
#define __RINGTONE_MANAGER__H__

#include <soundfile.h>
#include <map>

#include "common/RhoConf.h"
#include "common/RhoMutexLock.h"
#include "ext/rho/rhoruby.h"

using namespace std;
using namespace rho;
using namespace rho::common;

class CRingtoneManager 
{
  public:
    static CRingtoneManager& getCRingtoneManager();
        
    void getAllRingtones (map<String, String> &ringtones);
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


extern "C" VALUE ringtone_manager_get_all();
extern "C" void ringtone_manager_stop();
extern "C" void ringtone_manager_play(char* file_name);

#endif
