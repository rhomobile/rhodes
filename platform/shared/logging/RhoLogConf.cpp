#include "RhoLogConf.h"
#include "RhoLogCat.h"
#include "RhoLogSink.h"
#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "common/RhoConf.h"
#ifndef RHO_NO_RUBY
#include "ruby/ext/rho/rhoruby.h"
#endif /RHO_NO_RUBY

namespace rho{
common::CMutex LogSettings::m_FlushLock;
common::CMutex LogSettings::m_CatLock;

LogSettings g_LogSettings;

LogSettings::LogSettings(){ 
    m_nMinSeverity = 0; 
    m_bLogToOutput = true; 
    m_bLogToFile = false;

    m_nMaxLogFileSize = 0; 
    m_bLogPrefix = true; 

    m_pFileSink = new CLogFileSink(*this);
    m_pOutputSink = new CLogOutputSink(*this);
    m_pLogViewSink = NULL;
}

LogSettings::~LogSettings(){
    delete m_pFileSink;
    delete m_pOutputSink;
}

void LogSettings::getLogTextW(StringW& strTextW)
{
    boolean bOldSaveToFile = isLogToFile();
    setLogToFile(false);

    common::CRhoFile oFile;
    if ( oFile.open( getLogFilePath().c_str(), common::CRhoFile::OpenReadOnly) )
        oFile.readStringW(strTextW);

    setLogToFile(bOldSaveToFile);
}

void LogSettings::getLogText(String& strText)
{
    boolean bOldSaveToFile = isLogToFile();
    setLogToFile(false);

    common::CRhoFile oFile;
    if ( oFile.open( getLogFilePath().c_str(), common::CRhoFile::OpenReadOnly) )
        oFile.readString(strText);

    setLogToFile(bOldSaveToFile);
}

int LogSettings::getLogTextPos()
{
    return m_pFileSink ? m_pFileSink->getCurPos() : -1;
}

void LogSettings::saveToFile(){
    RHOCONF().setInt("MinSeverity", getMinSeverity() );
    RHOCONF().setBool("LogToOutput", isLogToOutput() );
    RHOCONF().setBool("LogToFile", isLogToFile() );
#if !defined(OS_MACOSX)	
    RHOCONF().setString("LogFilePath", getLogFilePath() );
#endif
    RHOCONF().setInt("MaxLogFileSize", getMaxLogFileSize() );
    RHOCONF().setString("LogCategories", getEnabledCategories() );
    RHOCONF().setString("ExcludeLogCategories", getDisabledCategories() );

    RHOCONF().saveToFile();
}

void LogSettings::loadFromConf(rho::common::RhoSettings& oRhoConf){
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
        common::CMutexLock oLock(m_FlushLock);

        m_strLogFilePath = szLogFilePath; 

        if ( m_pFileSink ){
            delete m_pFileSink;
            m_pFileSink = new CLogFileSink(*this);
        }
    }
}

void LogSettings::clearLog(){
    common::CMutexLock oLock(m_FlushLock);

    if ( m_pFileSink ){
        m_pFileSink->clear();
    }

}

void LogSettings::sinkLogMessage( String& strMsg ){
    common::CMutexLock oLock(m_FlushLock);

    if ( isLogToFile() )
        m_pFileSink->writeLogMessage(strMsg);

    if (m_pLogViewSink)
        m_pLogViewSink->writeLogMessage(strMsg);

    //Should be at the end
    if ( isLogToOutput() )
        m_pOutputSink->writeLogMessage(strMsg);
}

bool LogSettings::isCategoryEnabled(const LogCategory& cat)const{
    //TODO: Optimize categories search : add map
    common::CMutexLock oLock(m_CatLock);

    if ( m_strDisabledCategories.length() > 0 && strstr(m_strDisabledCategories.c_str(), cat.getName().c_str() ) != 0 )
        return false;

    if ( m_strEnabledCategories.length() == 0 )
        return false;

    return strcmp(m_strEnabledCategories.c_str(),"*") == 0 || strstr(m_strEnabledCategories.c_str(), cat.getName().c_str() ) != 0;
}

void LogSettings::setEnabledCategories( const char* szCatList ){
    common::CMutexLock oLock(m_CatLock);

    if ( szCatList && *szCatList )
    	m_strEnabledCategories = szCatList;
    else
    	m_strEnabledCategories = "";
}

void LogSettings::setDisabledCategories( const char* szCatList ){
    common::CMutexLock oLock(m_CatLock);

    if ( szCatList && *szCatList )
    	m_strDisabledCategories = szCatList;
    else
    	m_strDisabledCategories = "";
}

}

extern "C" {

void rho_logconf_Init(const char* szRootPath){

    rho::common::CFilePath oLogPath( szRootPath );

    //Set defaults
#ifdef RHO_DEBUG
    LOGCONF().setMinSeverity( L_TRACE );
    LOGCONF().setLogToOutput(true);
    LOGCONF().setEnabledCategories("*");
    LOGCONF().setDisabledCategories("");
#else //!RHO_DEBUG
    LOGCONF().setMinSeverity( L_ERROR );
    LOGCONF().setLogToOutput(false);
    LOGCONF().setEnabledCategories("");
#endif//!RHO_DEBUG

    LOGCONF().setLogPrefix(true);

    rho::String logPath = oLogPath.makeFullPath("RhoLog.txt");
    LOGCONF().setLogToFile(true);
    LOGCONF().setLogFilePath( logPath.c_str() );
    LOGCONF().setMaxLogFileSize(1024*50);

    rho_conf_Init(szRootPath);

    LOGCONF().loadFromConf(RHOCONF());
}

char* rho_logconf_getText() {
    rho::String strText;
    LOGCONF().getLogText(strText);
	return strdup(strText.c_str());
}

int rho_logconf_getTextPos() {
	return LOGCONF().getLogTextPos();
}

char* rho_logconf_getEnabledCategories() {
	return strdup(LOGCONF().getEnabledCategories().c_str());
}

char* rho_logconf_getDisabledCategories() {
	return strdup(LOGCONF().getDisabledCategories().c_str());
}

int rho_logconf_getSeverity() {
	return LOGCONF().getMinSeverity();
}

void rho_logconf_setEnabledCategories(const char* categories) {
	LOGCONF().setEnabledCategories(categories);
}

void rho_logconf_setDisabledCategories(const char* categories) {
	LOGCONF().setDisabledCategories(categories);
}

void rho_logconf_setSeverity(int nLevel) {
	LOGCONF().setMinSeverity(nLevel);
}

void rho_logconf_saveSettings() {
	 LOGCONF().saveToFile();
}

void rho_logconf_freeString(char* str) {
	free(str);
}

// RhoConf.set_property_by_name
void rho_conf_set_property_by_name(char* name, char* value)
{
	rho_conf_setString(name, value);
	rho_conf_save();

    LOGCONF().loadFromConf(RHOCONF());
}

void rho_conf_clean_log()
{
    LOGCONF().clearLog();
}

#ifndef RHO_NO_RUBY
VALUE rho_conf_get_property_by_name(char* name)
{
	char* szValue = rho_conf_getString(name);

    return rho_ruby_create_string(szValue);
}

VALUE rho_conf_read_log(int limit)
{
    VALUE res = rho_ruby_create_string("");
    bool bOldSaveToFile = LOGCONF().isLogToFile();
    LOGCONF().setLogToFile(false);

    rho::common::CRhoFile oFile;
    if ( oFile.open( LOGCONF().getLogFilePath().c_str(), rho::common::CRhoFile::OpenReadOnly) )
    {
        int nFileSize = oFile.size();
        int nPos = LOGCONF().getLogTextPos();
        int nMaxSize = nFileSize > nPos ? nFileSize : nPos;
        if ( limit <= 0 || limit > nMaxSize)
            limit = nMaxSize;

        res = rho_ruby_create_string_withlen(limit);
        char* szStr = getStringFromValue(res);

        if ( limit <= nPos )
        {
            oFile.setPosTo(nPos-limit);
            oFile.readData(szStr,0,limit);
        }else
        {
            oFile.setPosTo(nFileSize-(limit-nPos));
            int nRead = oFile.readData(szStr,0,limit);

            oFile.setPosTo(0);
            oFile.readData(szStr,nRead,limit-nRead);
        }

    }

    LOGCONF().setLogToFile(bOldSaveToFile);

    return res;
}
#endif //RHO_NO_RUBY

}
