#include "stdafx.h"
#include "RingtoneManager.h"
#include "logging/RhoLog.h"

#if defined(_WIN32_WCE)
#include <soundfile.h>
#endif

CRingtoneManager *CRingtoneManager::m_pInstance = NULL;
CMutex CRingtoneManager::m_mxRMLocker;

CRingtoneManager::CRingtoneManager () {}

CRingtoneManager::~CRingtoneManager () {}

void CRingtoneManager::createCRingtoneManager()
{
    static CRingtoneManager instance;
    m_pInstance = &instance;
}

CRingtoneManager &CRingtoneManager::getCRingtoneManager()
{
    m_mxRMLocker.Lock();

    if (m_pInstance != NULL)
        createCRingtoneManager();
    
    m_mxRMLocker.Unlock();
    
    return *m_pInstance;
}


void CRingtoneManager::getAllRingtones (map <String, String> &ringtones)
{
#if defined(_WIN32_WCE)
    SNDFILEINFO *sndFilesList = NULL;
    int filesNum = 0;
    
    SndGetSoundFileList(SND_EVENT_RINGTONELINE1, SND_LOCATION_ALL, &sndFilesList, &filesNum);
    LOG(INFO) + __FUNCTION__ + ": " + filesNum + " found";
    
    for (int i = 0; i < filesNum; i++) {
        if (sndFilesList[i].sstType == SND_SOUNDTYPE_FILE) {
            ringtones.insert(pair<String, String>
                             (String((const char *)sndFilesList->szDisplayName, MAX_PATH),
                              String((const char *)sndFilesList->szPathName, MAX_PATH)));
        }
    }
#endif
}

void CRingtoneManager::play (String ringtone_name)
{
#if defined(_WIN32_WCE)
    HRESULT hr;
    
    stop();
    
    USES_CONVERSION;
    
    hr = SndOpen(A2T(ringtone_name.c_str()), &m_hSound);
    if (hr != S_OK) {
        LOG(WARNING) + "RingtoneManager: failed to open file";
        return;
    }
    hr = SndPlayAsync (m_hSound, 0);
    if (hr != S_OK) {
        LOG(WARNING) + "RingtoneManager: failed to play file";
        return;
    }
#endif
}

void CRingtoneManager::stop()
{
#if defined(_WIN32_WCE)
    HRESULT hr;

    hr = SndClose(m_hSound);
    if (hr != S_OK) {
        LOG(WARNING) + "RingtoneManager: failed to close file";
        return;
    }
    
    hr = SndStop(SND_SCOPE_PROCESS, NULL);
    if (hr != S_OK) {
        LOG(WARNING) + "RingtoneManager: failed to stop playing";
        return;
    }
#endif
}

VALUE ringtone_manager_get_all()
{
    LOG(INFO) + __FUNCTION__;
    
    VALUE retval = createHash();

    std::map <String, String> ringtones;
    CRingtoneManager::getCRingtoneManager().getAllRingtones(ringtones);

    for (map<String, String>::iterator itr = ringtones.begin();
         itr != ringtones.end(); ++itr) {
        addStrToHash(retval,
                     itr->first.c_str(), itr->second.c_str(),
                     strlen(itr->second.c_str()));
    }
    
    return retval;
}

void ringtone_manager_stop()
{
    LOG(INFO) + __FUNCTION__;
    CRingtoneManager::getCRingtoneManager().stop();
}

void ringtone_manager_play(char* file_name)
{
    LOG(INFO) + __FUNCTION__;
    CRingtoneManager::getCRingtoneManager().play(String(file_name));
}
