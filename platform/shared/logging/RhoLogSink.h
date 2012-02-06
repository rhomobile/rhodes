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

#ifndef _RHOLOGSINK_H_
#define _RHOLOGSINK_H_

#include "RhoLogConf.h"
#include "net/RawSocket.h"
#include "net/AsyncHttp.h"
#include "common/RhodesApp.h"

namespace rho {
namespace common {
    class CRhoFile;
}

class CLogFileSink : public ILogSink{
    common::CRhoFile* m_pFile;
    common::CRhoFile* m_pPosFile;
    const LogSettings& m_oLogConf;
    int                m_nCirclePos;

    unsigned int       m_nFileLogSize;

    const LogSettings& getLogConf()const{ return m_oLogConf; }

public:
    CLogFileSink(const LogSettings& oSettings);

    void writeLogMessage( String& strMsg );
    int getCurPos();
    void clear();
private:
    void loadLogPosition();
    void saveLogPosition();
};

class CLogOutputSink : public ILogSink{
    const LogSettings& m_oLogConf;

public:
    CLogOutputSink(const LogSettings& oSettings) : m_oLogConf(oSettings){
	}

    void writeLogMessage( String& strMsg );
    int getCurPos(){ return -1; }
    void clear(){}
};

class CLogSocketSink : public ILogSink, common::CAppCallbacksQueue{
    const LogSettings& m_oLogConf;
    String m_addrHost;
	net::CAsyncHttp* m_aHttp;

public:
    CLogSocketSink(const LogSettings& oSettings); 
	virtual ~CLogSocketSink(){}

    void writeLogMessage( String& strMsg );
    int getCurPos(){ return -1; }
    void clear(){}
};
  
}

#endif //_RHOLOGSINK_H_
