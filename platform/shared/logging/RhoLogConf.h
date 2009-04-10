#ifndef _RHOLOGCONF_H_
#define _RHOLOGCONF_H_

#include "common/RhoStd.h"
#include "RhoPlainLog.h"

namespace rho {

class LogCategory;

struct ILogSink{
    virtual void writeLogMessage( String& strMsg ) = 0;
    virtual int getCurPos() = 0;
};

class LogSettings{
    LogSeverity m_nMinSeverity;
    bool        m_bLogToOutput;

    bool        m_bLogToFile;
    String      m_strLogFilePath;
    unsigned int m_nMaxLogFileSize;

    bool        m_bLogPrefix;

    String      m_strEnabledCategories, m_strDisabledCategories;

    ILogSink*   m_pFileSink;
    ILogSink*   m_pOutputSink;

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

    void saveToFile(const char* szFilePath);
    void saveToString(String& strData);

    void loadFromFile(const char* szFilePath);
    void loadFromString(const char* szSettings);

    void getLogTextW(StringW& strTextW);
    int  getLogTextPos();
private:

    void setPropertyByName(const char* szName, int nNameLen, const char* szValue, int nValueLen );
    void loadProperty( const char* start, int len );
};

extern LogSettings g_LogSettings;

}

inline rho::LogSettings& LOGCONF(){ return rho::g_LogSettings; }
extern "C" void InitRhoLog(const char* szRootPath);

#endif //_RHOLOGCONF_H_
