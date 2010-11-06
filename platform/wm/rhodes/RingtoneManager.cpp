#include "stdafx.h"
#include "RingtoneManager.h"

#if _WIN32_WCE > 0x501

IMPLEMENT_LOGCLASS(CRingtoneManager, "RingtoneManager");
CRingtoneManager *CRingtoneManager::m_pInstance = NULL;
CMutex CRingtoneManager::m_mxRMLocker;

CRingtoneManager::CRingtoneManager () : m_hSound(0) {}

CRingtoneManager::~CRingtoneManager () {}

void CRingtoneManager::createCRingtoneManager()
{
    static CRingtoneManager instance;
    m_pInstance = &instance;
}

CRingtoneManager &CRingtoneManager::getCRingtoneManager()
{
    m_mxRMLocker.Lock();

    if (!m_pInstance)
        createCRingtoneManager();
    
    m_mxRMLocker.Unlock();
    
    return *m_pInstance;
}


void CRingtoneManager::getAllRingtones (Hashtable<String, String>& ringtones)
{
    SNDFILEINFO *sndFilesList = NULL;
    int filesNum = 0;
    
    //TODO: check result
    SndGetSoundFileList(SND_EVENT_RINGTONELINE1, SND_LOCATION_ALL, &sndFilesList, &filesNum);
    LOG(INFO) + __FUNCTION__ + ": " + filesNum + " found";

    USES_CONVERSION;  
    for (int i = 0; i < filesNum; i++) {
        SNDFILEINFO& sndFile = sndFilesList[i];
        if (sndFile.sstType == SND_SOUNDTYPE_FILE) 
            ringtones.put( convertToStringA(sndFile.szDisplayName), convertToStringA(sndFile.szPathName));
    }
}

void CRingtoneManager::play (String ringtone_name)
{
    HRESULT hr;
    
    stop();
    
    USES_CONVERSION;
    
    hr = SndOpen(A2T(ringtone_name.c_str()), &m_hSound);
    if (hr != S_OK || !m_hSound) {
        //TODO: log extended error
        LOG(ERROR) + "RingtoneManager: failed to open file";
        return;
    }
    hr = SndPlayAsync (m_hSound, 0);
    if (hr != S_OK) {
        //TODO: log extended error
        LOG(ERROR) + "RingtoneManager: failed to play file";
        return;
    }
}

void CRingtoneManager::stop()
{
    HRESULT hr;

    if (!m_hSound)
        return;

    hr = SndClose(m_hSound);
    if (hr != S_OK) {
        //TODO: log extended error
        LOG(ERROR) + "RingtoneManager: failed to close file";
        return;
    }
    m_hSound = 0;

    hr = SndStop(SND_SCOPE_PROCESS, NULL);
    if (hr != S_OK) {
        //TODO: log extended error
        LOG(ERROR) + "RingtoneManager: failed to stop playing";
        return;
    }
}

#endif //_WIN32_WCE

extern "C"
{
VALUE rho_ringtone_manager_get_all()
{
    LOG(INFO) + __FUNCTION__;
    
    CHoldRubyValue retval(rho_ruby_createHash());

#if _WIN32_WCE > 0x501

    Hashtable<String, String> ringtones;
    CRingtoneManager::getCRingtoneManager().getAllRingtones(ringtones);

    for (Hashtable<String, String>::iterator itr = ringtones.begin();
         itr != ringtones.end(); ++itr) {
        addStrToHash( retval, itr->first.c_str(), itr->second.c_str() );
    }
#endif
    
    return retval;
}

void rho_ringtone_manager_stop()
{
#if _WIN32_WCE > 0x501
    LOG(INFO) + __FUNCTION__;
    CRingtoneManager::getCRingtoneManager().stop();
#endif
}

void rho_ringtone_manager_play(char* file_name)
{
#if _WIN32_WCE > 0x501
    LOG(INFO) + __FUNCTION__;
    CRingtoneManager::getCRingtoneManager().play(String(file_name));
#endif
}

}