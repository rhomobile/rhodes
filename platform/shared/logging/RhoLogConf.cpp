#include "RhoLogConf.h"
#include "RhoLogCat.h"
#include "RhoLogSink.h"
#include "common/StringConverter.h"
#include "common/RhoFile.h"

namespace rho{
LogSettings g_LogSettings;

LogSettings::LogSettings(){ 
    m_nMinSeverity = 0; 
    m_bLogToOutput = true; 
    m_bLogToFile = false;

    m_nMaxLogFileSize = 0; 
    m_bLogPrefix = true; 

    m_pFileSink = new CLogFileSink(*this);
    m_pOutputSink = new CLogOutputSink(*this);
}

LogSettings::~LogSettings(){
    delete m_pFileSink;
    delete m_pOutputSink;
}

void LogSettings::saveToFile(const char* szFilePath){
    String strData;
    saveToString(strData);

    general::CRhoFile oFile;
    oFile.open( szFilePath, general::CRhoFile::OpenForWrite);
    oFile.write( strData.c_str(), strData.size() );
}

void LogSettings::loadFromFile(const char* szFilePath){
    general::CRhoFile oFile;
    if ( oFile.open( szFilePath, general::CRhoFile::OpenReadOnly) ){
        String strSettings;
        oFile.readString(strSettings);
        loadFromString( strSettings.c_str() );
    }
}

void LogSettings::loadFromString(const char* szSettings){
    if ( !szSettings && !*szSettings )
        return;

    const char* start = szSettings;

    while(start!=0){
        int len = 0;

        const char* end = end = strchr(start,'\n');
        if (end){
            if ( end > start && *(end-1) == '\r' )
                len = end-start-1;
            else
                len = end-start;
        }else {
            len = (int)strlen(start);
        }

        loadProperty( start, len );

        start = end ? end + 1 : end;
    }
}

void LogSettings::loadProperty( const char* start, int len ){
    int nNameLen = 0;
    while(*start==' '){ start++; len--;}

    int i = 0;
    for( i = 0; i < len; i++ ){
        if ( start[i] == '=' ){
            if ( i > 0 ){
                int s = i-1;
                for(; s >= 0 && start[s]==' '; s-- );

                nNameLen = s+1;
                break;
            }else 
                break;
        }
    }

    if ( nNameLen == 0 )
        return;

    const char* szValue = start + i+1;
    int nValueLen = len - (i+1);

    while(*szValue==' '&& nValueLen >= 0 ){ szValue++; nValueLen--;}
    while(nValueLen > 0 && szValue[nValueLen-1]==' ') nValueLen--;

    setPropertyByName(start, nNameLen, szValue, nValueLen );
}

void LogSettings::setPropertyByName(const char* szName, int nNameLen, const char* szValue, int nValueLen ){
    String name(szName,nNameLen);
    String value(szValue,nValueLen);

    if ( strcasecmp(name.c_str(), "MinSeverity") == 0 )
        setMinSeverity(atoi(value.c_str()));
    else if ( strcasecmp(name.c_str(), "LogToOutput") == 0 )
        setLogToOutput( atoi(value.c_str()) == 0 ? false : true);
    else if ( strcasecmp(name.c_str(), "LogToFile") == 0 )
        setLogToFile( atoi(value.c_str()) == 0 ? false : true);
    else if ( strcasecmp(name.c_str(), "LogFilePath") == 0 )
        setLogFilePath( value.c_str() );
    else if ( strcasecmp(name.c_str(), "MaxLogFileSize") == 0 )
        setMaxLogFileSize( atoi(value.c_str()) );
    else if ( strcasecmp(name.c_str(), "LogCategories") == 0 )
        setEnabledCategories( value.c_str() );
    else if ( strcasecmp(name.c_str(), "ExcludeLogCategories") == 0 )
        setDisabledCategories( value.c_str() );

}

void LogSettings::saveToString(String& strData){
    strData += "MinSeverity=";
    strData += general::convertToStringA(getMinSeverity());
    strData += "\n";

    strData += "LogToOutput=";
    strData += general::convertToStringA(isLogToOutput());
    strData += "\n";

    strData += "LogToFile=";
    strData += general::convertToStringA(isLogToFile());
    strData += "\n";

    strData += "LogFilePath=";
    strData += getLogFilePath();
    strData += "\n";

    strData += "MaxLogFileSize=";
    strData += general::convertToStringA(getMaxLogFileSize());
    strData += "\n";

    strData += "LogCategories=";
    strData += getEnabledCategories();
    strData += "\n";

    strData += "ExcludeLogCategories=";
    strData += getDisabledCategories();
    strData += "\n";

}

void LogSettings::setLogFilePath(const char* szLogFilePath){ 
    if ( m_strLogFilePath.compare(szLogFilePath) != 0 ){
        m_strLogFilePath = szLogFilePath; 

        if ( m_pFileSink ){
            delete m_pFileSink;
            m_pFileSink = new CLogFileSink(*this);
        }
    }
}

void LogSettings::sinkLogMessage( const char* data, unsigned int len ){
    if ( isLogToOutput() )
        m_pOutputSink->writeLogMessage(data, len);

    if ( isLogToFile() )
        m_pFileSink->writeLogMessage(data, len);
}

bool LogSettings::isCategoryEnabled(const LogCategory& cat)const{
    //TODO: Optimize categories search : add map

    if ( m_strDisabledCategories.length() > 0 && strstr(m_strDisabledCategories.c_str(), cat.getName().c_str() ) != 0 )
        return false;

    if ( m_strEnabledCategories.length() == 0 )
        return false;

    return strcmp(m_strEnabledCategories.c_str(),"*") == 0 || strstr(m_strEnabledCategories.c_str(), cat.getName().c_str() ) != 0;
}

void LogSettings::setEnabledCategories( const char* szCatList ){

    m_strEnabledCategories = szCatList;
}

void LogSettings::setDisabledCategories( const char* szCatList ){

    m_strDisabledCategories = szCatList;
}

}
