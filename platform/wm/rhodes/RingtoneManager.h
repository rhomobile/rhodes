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

#ifndef __RINGTONE_MANAGER__H__
#define __RINGTONE_MANAGER__H__

#include "ext/rho/rhoruby.h"

#if _WIN32_WCE > 0x501 && !defined( OS_PLATFORM_CE )

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
