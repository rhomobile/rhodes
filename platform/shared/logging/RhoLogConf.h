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
#include "common/RhoTime.h"
#include "common/RhoThread.h"
#include "common/RhoConf.h"

namespace rho {

class LogCategory;

struct ILogSink{
    virtual ~ILogSink(){}
    virtual void writeLogMessage( String& strMsg ) = 0;
    virtual int getCurPos() = 0;
    virtual void clear() = 0;
};

class IMemoryInfoCollector
{
public:
    virtual ~IMemoryInfoCollector() {}
    virtual String collect() = 0;
};

class LogSettings : public common::IRhoSettingsListener {
    
    class MemoryInfoCollectorThread : public common::CRhoThread
    {
        unsigned int            m_collectMemoryIntervalMilliseconds;
        IMemoryInfoCollector*   m_pCollector;
        LogSettings&            m_logSettings;
        
        mutable common::CMutex  m_accessLock;
    public:
        MemoryInfoCollectorThread( LogSettings& logSettings );
        virtual void run();
        
        void setCollectMemoryInfoInterval( unsigned int interval );
        unsigned int getCollectMemoryInfoInterval(){ return m_collectMemoryIntervalMilliseconds; }
        void setMemoryInfoCollector( IMemoryInfoCollector* memInfoCollector );
        
        boolean willCollect() const;
    };
    
    LogSeverity m_nMinSeverity;
    bool        m_bLogToOutput;
    bool        m_bLogToSocket;

    bool        m_bLogToFile;
    String      m_strLogFilePath;
//    String      m_strLogConfFilePath;
    unsigned int m_nMaxLogFileSize;
    
    MemoryInfoCollectorThread* m_pMemoryCollectorThread;

	String      m_strLogURL;

    bool        m_bLogPrefix;
    
    bool        m_bIsInitialized;

    String      m_strEnabledCategories, m_strDisabledCategories;
    Vector<String>     m_arExcludeAttribs;

    ILogSink*   m_pFileSink;
    ILogSink*   m_pOutputSink;
    ILogSink*   m_pLogViewSink;
    ILogSink*   m_pSocketSink;
    
    Hashtable<ILogSink*, bool> m_pAuxSinks;
    IMemoryInfoCollector* m_pMemoryInfoCollector;

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
    void setLogToSocket(bool bLogToSocket);

    const String& getLogFilePath()const{ return m_strLogFilePath;}
    void setLogFilePath(const String& logFilePath);

//    const String& getLogConfFilePath()const{ return m_strLogConfFilePath;}
//    void setLogConfFilePath(const char* szLogConfFilePath){ m_strLogConfFilePath = szLogConfFilePath; }

    void setMaxLogFileSize(unsigned int nMaxSize){m_nMaxLogFileSize = nMaxSize; }
    unsigned int getMaxLogFileSize()const{ return m_nMaxLogFileSize; }

    bool isLogPrefix()const{ return m_bLogPrefix;}
	void setLogPrefix(bool bLogPrefix){ m_bLogPrefix = bLogPrefix; }
    
    bool isReadyForLogging() { return m_bIsInitialized;}

	const String& getLogURL() const { return m_strLogURL; }
	void setLogURL(const char* szLogURL);

	//void initRemoteLog();
    void reinitRemoteLog();
	void closeRemoteLog();

    void setEnabledCategories( const char* szCatList );
    void setDisabledCategories( const char* szCatList );
    const String& getEnabledCategories(){ return m_strEnabledCategories; }
    const String& getDisabledCategories(){ return m_strDisabledCategories; }
    bool isCategoryEnabled(const LogCategory& cat)const;
    
    void setCollectMemoryInfoInterval( unsigned int interval );
    void setMemoryInfoCollector( IMemoryInfoCollector* memInfoCollector );
    unsigned int getCollectMemoryInfoInterval();

    void setExcludeFilter( const String& strExcludeFilter );
    Vector<String>& getExcludeAttribs(){ return m_arExcludeAttribs; }

    void sinkLogMessage( String& strMsg );

    void getLogText(String& strText);
    void getLogTextW(StringW& strTextW);
    int  getLogTextPos();

    unsigned int getLogFileSize();

    void getLogFileText(int linearPos, int maxSize, String& strText, int refCircularPos = -1);

	void setLogView(ILogSink* logView) { 
		m_pLogViewSink = logView; 
	}

	bool hasLogView() {
		return m_pLogViewSink == NULL;
	}

	void clearLog();

    void saveToFile();
    void loadFromConf(rho::common::RhoSettings& oRhoConf);
    
    void addAuxSink( ILogSink* sink );
    void removeAuxSink( ILogSink* sink );

private:
	void internalSinkLogMessage( String& strMsg );
    virtual void onSettingUpdated( const String& name, const String& newVal );

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

void rho_logconf_Init(const char* szLogPath, const char* szRootPath, const char* szLogPort);
void rho_logconf_Init_with_separate_user_path(const char* szLogPath, const char* szRootPath, const char* szLogPort, const char* szUserPath);

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
int rho_conf_send_log_in_same_thread();

void rho_conf_clean_log();
    
void rho_log_resetup_http_url(const char* http_log_url);
    
bool rho_log_system_is_ready();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOLOGCONF_H_
