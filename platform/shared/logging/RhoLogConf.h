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

#ifndef _RHOLOGCONF_H_
#define _RHOLOGCONF_H_

#ifdef __cplusplus

#include "common/RhoStd.h"
//#include "RhoPlainLog.h"
#include "common/RhoMutexLock.h"

namespace rho {

namespace common{ class RhoSettings; }
class LogCategory;

struct ILogSink{
    virtual ~ILogSink(){}
    virtual void writeLogMessage( String& strMsg ) = 0;
    virtual int getCurPos() = 0;
    virtual void clear() = 0;
};

class LogSettings{
    LogSeverity m_nMinSeverity;
    bool        m_bLogToOutput;
    bool        m_bLogToSocket;

    bool        m_bLogToFile;
    String      m_strLogFilePath;
//    String      m_strLogConfFilePath;
    unsigned int m_nMaxLogFileSize;

	String      m_strLogHost;
    String      m_strLogPort;

    bool        m_bLogPrefix;

    String      m_strEnabledCategories, m_strDisabledCategories;
    Vector<String>     m_arExcludeAttribs;

    ILogSink*   m_pFileSink;
    ILogSink*   m_pOutputSink;
    ILogSink*   m_pLogViewSink;
    ILogSink*   m_pSocketSink;

    static common::CMutex m_FlushLock;
    static common::CMutex m_CatLock;

public:
    LogSettings();
    ~LogSettings();

    LogSeverity getMinSeverity()const{ return m_nMinSeverity; }
    void setMinSeverity(LogSeverity nMinSeverity){ m_nMinSeverity = nMinSeverity; }

    bool isLogToOutput()const{ return m_bLogToOutput;}
    void setLogToOutput(bool bLogToOutput){ m_bLogToOutput = bLogToOutput;}

    bool isLogToFile()const{ return m_bLogToFile;}
    void setLogToFile(bool bLogToFile){ m_bLogToFile = bLogToFile;}

    bool isLogToSocket()const{ return m_bLogToSocket;}
    void setLogToSocket(bool bLogToSocket){ m_bLogToSocket = bLogToSocket;}

    const String& getLogFilePath()const{ return m_strLogFilePath;}
    void setLogFilePath(const char* szLogFilePath);

//    const String& getLogConfFilePath()const{ return m_strLogConfFilePath;}
//    void setLogConfFilePath(const char* szLogConfFilePath){ m_strLogConfFilePath = szLogConfFilePath; }

    void setMaxLogFileSize(unsigned int nMaxSize){m_nMaxLogFileSize = nMaxSize; }
    unsigned int getMaxLogFileSize()const{ return m_nMaxLogFileSize; }

    bool isLogPrefix()const{ return m_bLogPrefix;}
	void setLogPrefix(bool bLogPrefix){ m_bLogPrefix = bLogPrefix; }

	const String& getLogHost() const { return m_strLogHost; }
	void setLogHost(const char* szLogHost) { m_strLogHost = rho::String(szLogHost); }

	const String& getLogPort() const { return m_strLogPort; }
	void initRemoteLog();
	void closeRemoteLog();

    void setEnabledCategories( const char* szCatList );
    void setDisabledCategories( const char* szCatList );
    const String& getEnabledCategories(){ return m_strEnabledCategories; }
    const String& getDisabledCategories(){ return m_strDisabledCategories; }
    bool isCategoryEnabled(const LogCategory& cat)const;

    void setExcludeFilter( const String& strExcludeFilter );
    Vector<String>& getExcludeAttribs(){ return m_arExcludeAttribs; }

    void sinkLogMessage( String& strMsg );

    void getLogText(String& strText);
    void getLogTextW(StringW& strTextW);
    int  getLogTextPos();
	
	void setLogView(ILogSink* logView) { 
		m_pLogViewSink = logView; 
	}

	bool hasLogView() {
		return m_pLogViewSink == NULL;
	}

	void clearLog();

    void saveToFile();
    void loadFromConf(rho::common::RhoSettings& oRhoConf);

};

extern LogSettings g_LogSettings;

}

inline rho::LogSettings& LOGCONF(){ return rho::g_LogSettings; }

#else
#include "common/RhoPort.h"
#endif //__cplusplus

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

void rho_logconf_Init(const char* szRootPath, const char* szLogPort);
void rho_logconf_Init_with_separate_user_path(const char* szRootPath, const char* szLogPort, const char* szUserPath);

char* rho_logconf_getText();
int   rho_logconf_getTextPos();

char* rho_logconf_getEnabledCategories();
char* rho_logconf_getDisabledCategories();
int   rho_logconf_getSeverity();

void rho_logconf_setEnabledCategories(const char* categories);
void rho_logconf_setDisabledCategories(const char* categories);
void rho_logconf_setSeverity(int nLevel);

void rho_logconf_saveSettings();
void rho_logconf_freeString(char* str);

int rho_conf_send_log(const char* callback_url);
void rho_conf_clean_log();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOLOGCONF_H_
