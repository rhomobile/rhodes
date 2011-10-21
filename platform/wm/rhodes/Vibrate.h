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

    void toggle (int seconds);
    void untoggle ();

  private:
    CVibrate ();
    ~CVibrate ();

    virtual void run();

    int m_nDuration_ms;
    bool m_bToggled;

    static void createCVibrate();
    static CVibrate *m_pInstance;
    static CMutex m_mxLocker;
};

#endif // __VIBRATE__H__
