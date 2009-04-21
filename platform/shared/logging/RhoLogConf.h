#ifndef _RHOLOGCONF_H_
#define _RHOLOGCONF_H_

#include "common/RhoStd.h"
#include "RhoPlainLog.h"
#include "common/RhoMutexLock.h"

namespace rho {

namespace general{ class RhoSettings; }
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

    static general::CMutex m_FlushLock;
    static general::CMutex m_CatLock;

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
    void getLogText(String& strText);

    void clearLog();

    void saveToFile();
    void loadFromConf(rho::general::RhoSettings& oRhoConf);

};

extern LogSettings g_LogSettings;

}

inline rho::LogSettings& LOGCONF(){ return rho::g_LogSettings; }
extern "C" void InitRhoLog(const char* szRootPath);

#endif //_RHOLOGCONF_H_
