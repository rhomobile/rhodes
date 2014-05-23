/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "stdafx.h"
#include "RingtoneManager.h"

#if _WIN32_WCE > 0x499// && !defined( OS_PLATFORM_MOTCE )
typedef HRESULT (WINAPI* LPFN_SND_CLOSE_T)(__in HSOUND);
typedef HRESULT (WINAPI* LPFN_SND_PLAYASYNC_T)(__in HSOUND, DWORD);
typedef HRESULT (WINAPI* LPFN_SND_OPEN_T)(__in LPCTSTR,  __out HSOUND*);
typedef HRESULT (WINAPI* LPFN_SND_GETSOUNDFILELIST_T)(SND_EVENT, DWORD, SNDFILEINFO**, int*);
typedef HRESULT (WINAPI* LPFN_SND_STOP_T)(__in SND_SCOPE, __reserved HSOUND);



extern "C" LPFN_SND_STOP_T lpfn_snd_stop;
extern "C" LPFN_SND_CLOSE_T lpfn_snd_close;
extern "C" LPFN_SND_PLAYASYNC_T lpfn_snd_playasync;
extern "C" LPFN_SND_OPEN_T lpfn_snd_open;
extern "C" LPFN_SND_GETSOUNDFILELIST_T lpfn_snd_getsndflist;
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
    lpfn_snd_getsndflist(SND_EVENT_RINGTONELINE1, SND_LOCATION_ALL, &sndFilesList, &filesNum);
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
    
    StringW ringtone_nameW = convertToStringW(ringtone_name);

    hr = lpfn_snd_open( ringtone_nameW.c_str(), &m_hSound);
    if (hr != S_OK || !m_hSound) {
        //TODO: log extended error
        LOG(ERROR) + "RingtoneManager: failed to open file";
        return;
    }
    hr = lpfn_snd_playasync(m_hSound, 0);
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

    hr = lpfn_snd_close(m_hSound);
    if (hr != S_OK) {
        //TODO: log extended error
        LOG(ERROR) + "RingtoneManager: failed to close file";
        return;
    }
    m_hSound = 0;

    hr = lpfn_snd_stop(SND_SCOPE_PROCESS, NULL);
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

#if _WIN32_WCE > 0x499// && !defined( OS_PLATFORM_MOTCE )
if(RHO_IS_WMDEVICE)
{
    Hashtable<String, String> ringtones;
    CRingtoneManager::getCRingtoneManager().getAllRingtones(ringtones);

    for (Hashtable<String, String>::iterator itr = ringtones.begin();
         itr != ringtones.end(); ++itr) {
        addStrToHash( retval, itr->first.c_str(), itr->second.c_str() );
    }
}
#endif
    
    return retval;
}

void rho_ringtone_manager_stop()
{
#if _WIN32_WCE > 0x499// && !defined( OS_PLATFORM_MOTCE )
	if(RHO_IS_WMDEVICE)
	{
		LOG(INFO) + __FUNCTION__;
		CRingtoneManager::getCRingtoneManager().stop();
	}
#endif
}

void rho_ringtone_manager_play(char* file_name)
{
#if _WIN32_WCE > 0x499// && !defined( OS_PLATFORM_MOTCE )
	if(RHO_IS_WMDEVICE)
	{
		LOG(INFO) + __FUNCTION__;
		CRingtoneManager::getCRingtoneManager().play(String(file_name));
	}
#endif
}

}