#include "RhoLogConf.h"
#include "RhoLogCat.h"
#include "RhoLogSink.h"
#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "common/RhoConf.h"

namespace rho{
general::CMutex LogSettings::m_FlushLock;
general::CMutex LogSettings::m_CatLock;

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

void LogSettings::getLogTextW(StringW& strTextW){
    general::CRhoFile oFile;
    if ( oFile.open( getLogFilePath().c_str(), general::CRhoFile::OpenReadOnly) )
        oFile.readStringW(strTextW);
}

int LogSettings::getLogTextPos()
{
    return m_pFileSink ? m_pFileSink->getCurPos() : -1;
}

void LogSettings::saveToFile(){
    RHOCONF().setInt("MinSeverity", getMinSeverity() );
    RHOCONF().setBool("LogToOutput", isLogToOutput() );
    RHOCONF().setBool("LogToFile", isLogToFile() );
    RHOCONF().setString("LogFilePath", getLogFilePath() );
    RHOCONF().setInt("MaxLogFileSize", getMaxLogFileSize() );
    RHOCONF().setString("LogCategories", getEnabledCategories() );
    RHOCONF().setString("ExcludeLogCategories", getDisabledCategories() );

    RHOCONF().saveToFile();
}

void LogSettings::loadFromFile(){
    loadFromConf(RHOCONF());
}

void LogSettings::loadFromConf(rho::general::RhoSettings& oRhoConf){
    if ( oRhoConf.isExist( "MinSeverity" ) )
        setMinSeverity( oRhoConf.getInt("MinSeverity") );
    if ( oRhoConf.isExist( "LogToOutput") )
        setLogToOutput( oRhoConf.getBool("LogToOutput") );
    if ( oRhoConf.isExist( "LogToFile") )
        setLogToFile( oRhoConf.getBool("LogToFile"));
    if ( oRhoConf.isExist( "LogFilePath") )
        setLogFilePath( oRhoConf.getString("LogFilePath").c_str() );
    if ( oRhoConf.isExist( "MaxLogFileSize") )
        setMaxLogFileSize( oRhoConf.getInt("MaxLogFileSize") );
    if ( oRhoConf.isExist( "LogCategories") )
        setEnabledCategories( oRhoConf.getString("LogCategories").c_str() );
    if (oRhoConf.isExist( "ExcludeLogCategories") )
        setDisabledCategories( oRhoConf.getString("ExcludeLogCategories").c_str() );
}

void LogSettings::setLogFilePath(const char* szLogFilePath){ 
    if ( m_strLogFilePath.compare(szLogFilePath) != 0 ){
        general::CMutexLock oLock(m_FlushLock);

        m_strLogFilePath = szLogFilePath; 

        if ( m_pFileSink ){
            delete m_pFileSink;
            m_pFileSink = new CLogFileSink(*this);
        }
    }
}

void LogSettings::clearLog(){
    general::CMutexLock oLock(m_FlushLock);

    if ( m_pFileSink ){
        m_pFileSink->clear();
    }

}

void LogSettings::sinkLogMessage( String& strMsg ){
    general::CMutexLock oLock(m_FlushLock);

    if ( isLogToFile() )
        m_pFileSink->writeLogMessage(strMsg);

    //Should be at the end
    if ( isLogToOutput() )
        m_pOutputSink->writeLogMessage(strMsg);
}

bool LogSettings::isCategoryEnabled(const LogCategory& cat)const{
    //TODO: Optimize categories search : add map
    general::CMutexLock oLock(m_CatLock);

    if ( m_strDisabledCategories.length() > 0 && strstr(m_strDisabledCategories.c_str(), cat.getName().c_str() ) != 0 )
        return false;

    if ( m_strEnabledCategories.length() == 0 )
        return false;

    return strcmp(m_strEnabledCategories.c_str(),"*") == 0 || strstr(m_strEnabledCategories.c_str(), cat.getName().c_str() ) != 0;
}

void LogSettings::setEnabledCategories( const char* szCatList ){
    general::CMutexLock oLock(m_CatLock);

    m_strEnabledCategories = szCatList;
}

void LogSettings::setDisabledCategories( const char* szCatList ){
    general::CMutexLock oLock(m_CatLock);

    m_strDisabledCategories = szCatList;
}

}

extern "C" void InitRhoLog(const char* szRootPath){

    InitRhoConf(szRootPath);

	rho::general::CFilePath oLogPath( szRootPath );
	
    //Set defaults
#ifdef RHO_DEBUG
    LOGCONF().setMinSeverity( 0 );
    LOGCONF().setLogToOutput(true);
    LOGCONF().setEnabledCategories("*");
    LOGCONF().setDisabledCategories("");
#else //!RHO_DEBUG
    LOGCONF().setMinSeverity( L_ERROR );
    LOGCONF().setLogToOutput(false);
    LOGCONF().setEnabledCategories("");
#endif//!RHO_DEBUG
	
    LOGCONF().setLogPrefix(true);		
	
    LOGCONF().setLogToFile(true);
    LOGCONF().setLogFilePath( oLogPath.makeFullPath("RhoLog.txt").c_str() );
    LOGCONF().setMaxLogFileSize(1024*50);
	
    LOGCONF().loadFromFile();
}
