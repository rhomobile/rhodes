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
    virtual void writeLogMessage( String& strMsg ) = 0;
    virtual int getCurPos() = 0;
    virtual void clear() = 0;
};

class LogSettings{
    LogSeverity m_nMinSeverity;
    bool        m_bLogToOutput;

    bool        m_bLogToFile;
    String      m_strLogFilePath;
//    String      m_strLogConfFilePath;
    unsigned int m_nMaxLogFileSize;

    bool        m_bLogPrefix;

    String      m_strEnabledCategories, m_strDisabledCategories;

    ILogSink*   m_pFileSink;
    ILogSink*   m_pOutputSink;
    ILogSink*   m_pLogViewSink;

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

    const String& getLogFilePath()const{ return m_strLogFilePath;}
    void setLogFilePath(const char* szLogFilePath);

//    const String& getLogConfFilePath()const{ return m_strLogConfFilePath;}
//    void setLogConfFilePath(const char* szLogConfFilePath){ m_strLogConfFilePath = szLogConfFilePath; }

    void setMaxLogFileSize(unsigned int nMaxSize){m_nMaxLogFileSize = nMaxSize; }
    unsigned int getMaxLogFileSize()const{ return m_nMaxLogFileSize; }

    bool isLogPrefix()const{ return m_bLogPrefix;}
    void setLogPrefix(bool bLogPrefix){ m_bLogPrefix = bLogPrefix;}

    void setEnabledCategories( const char* szCatList );
    void setDisabledCategories( const char* szCatList );
    const String& getEnabledCategories(){ return m_strEnabledCategories; }
    const String& getDisabledCategories(){ return m_strDisabledCategories; }
    bool isCategoryEnabled(const LogCategory& cat)const;

    void sinkLogMessage( String& strMsg );

    void getLogText(String& strText);
    void getLogTextW(StringW& strTextW);
    int  getLogTextPos();
	
	void setLogView(ILogSink* logView) { 
		m_pLogViewSink = logView; 
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

void rho_logconf_Init(const char* szRootPath);

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

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOLOGCONF_H_
