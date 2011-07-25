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

#if _WIN32_WCE > 0x501
#include <nled.h>
#include "Vibrate.h"
#include <common/RhodesApp.h>

IMPLEMENT_LOGCLASS(CVibrate, "Vibrate");

CVibrate *CVibrate::m_pInstance = NULL;
CMutex CVibrate::m_mxLocker;

CVibrate::CVibrate () : CRhoThread(), m_bToggled(false)
{
}

CVibrate::~CVibrate ()
{
    untoggle();
}

void CVibrate::createCVibrate()
{
    static CVibrate instance;
    m_pInstance = &instance;
}

CVibrate &CVibrate::getCVibrate()
{
    m_mxLocker.Lock();

    if (!m_pInstance)
        createCVibrate();
    
    m_mxLocker.Unlock();
    
    return *m_pInstance;
}

void CVibrate::toggle(int ms)
{
    m_nDuration_ms = ms;
    start(epNormal);
}

void CVibrate::untoggle()
{
    NLED_SETTINGS_INFO settings;
    settings.LedNum= 1; 
    settings.OffOnBlink= 0; 
    NLedSetDevice (NLED_SETTINGS_INFO_ID, &settings);
    m_bToggled = false;
	stop(0);
}

void CVibrate::run() 
{
    NLED_SETTINGS_INFO settings;

    settings.LedNum= 1;
    NLedGetDeviceInfo(NLED_SETTINGS_INFO_ID,  &settings);

    if (!m_bToggled && settings.OffOnBlink == 0) {
        settings.LedNum= 1; 
        settings.OffOnBlink= 1;
        NLedSetDevice (NLED_SETTINGS_INFO_ID, &settings);
        m_bToggled = true;        
        wait (m_nDuration_ms/1000);
		untoggle();
    }
}

#endif // _WIN32_WCE
