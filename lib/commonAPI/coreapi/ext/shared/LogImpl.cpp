#include "generated/cpp/LogBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoFile.h"

extern "C" void rho_conf_show_log();

namespace rho {

using namespace apiGenerator;
using namespace common;

class CLogImpl: public CLogSingletonBase
{
public:

    CLogImpl(): CLogSingletonBase(){}

    virtual void getLevel(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( LOGCONF().getMinSeverity() );
    }

    virtual void setLevel( int value, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGCONF().setMinSeverity(value);
    }

    virtual void getDestination(rho::apiGenerator::CMethodResult& oResult)
    {
        Vector<String> arRes;
        if (LOGCONF().isLogToFile())
            arRes.addElement( DEST_FILE );
        if (LOGCONF().isLogToOutput())
            arRes.addElement(DEST_OUTPUT);
        if (LOGCONF().isLogToSocket())
            arRes.addElement(DEST_URI);

        oResult.set(arRes);
    }

    virtual void setDestination( const rho::Vector<rho::String>& ar, rho::apiGenerator::CMethodResult& oResult)
    {
        bool bLogToFile = false, bLogToOutput = false, bLogToURI = false;
        for( int i = 0; i < (int)ar.size(); i++ )
        {
            if ( ar[i] == DEST_FILE )
                bLogToFile = true;
            else if ( ar[i] == DEST_OUTPUT )
                bLogToOutput = true;
            else if ( ar[i] == DEST_URI )
                bLogToURI = true;
        }

        LOGCONF().setLogToFile(bLogToFile);
        LOGCONF().setLogToOutput(bLogToOutput);
        LOGCONF().setLogToSocket(bLogToURI);
    }

    virtual void getIncludeCategories(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(LOGCONF().getEnabledCategories());
    }

    virtual void setIncludeCategories( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGCONF().setEnabledCategories(value.c_str());
    }

    virtual void getExcludeCategories(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(LOGCONF().getDisabledCategories());
    }

    virtual void setExcludeCategories( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGCONF().setDisabledCategories(value.c_str());
    }

    virtual void getFileSize(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( (int)LOGCONF().getMaxLogFileSize() );
    }

    virtual void setFileSize( int value, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGCONF().setMaxLogFileSize(value);
    }

    virtual void getFilePath(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( LOGCONF().getLogFilePath() );
    }

    virtual void setFilePath( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGCONF().setLogFilePath(value);
    }

    virtual void getMemoryPeriod(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( (int)LOGCONF().getCollectMemoryInfoInterval() );
    }

    virtual void setMemoryPeriod( int value, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGCONF().setCollectMemoryInfoInterval(value);
    }

    virtual void getNetTrace(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getBool("net_trace") );
    }

    virtual void setNetTrace( bool value, rho::apiGenerator::CMethodResult& oResult)
    {
        RHOCONF().setBool("net_trace",value, false);
    }

    virtual void getSkipPost(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getBool("log_skip_post") );
    }

    virtual void setSkipPost( bool value, rho::apiGenerator::CMethodResult& oResult)
    {
        RHOCONF().setBool("log_skip_post",value, false);
    }

    virtual void getExcludeFilter(rho::apiGenerator::CMethodResult& oResult)
    {
        const Vector<rho::String>& log_attr = LOGCONF().getExcludeAttribs();

        size_t len = 0;
        for(Vector<rho::String>::const_iterator iter = log_attr.begin(); iter != log_attr.end(); iter++)
        {
            len += (*iter).length() + 1; // including separator
        }
        
        rho::String str;
        str.reserve(len);
  
        for(Vector<rho::String>::const_iterator iter = log_attr.begin(); iter != log_attr.end(); iter++)
        {
            if (iter != log_attr.begin())
            {
                str += ",";
            }
            str += *iter;
        }
        
        oResult.set( str );
    }

    virtual void setExcludeFilter( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
    {
        //RHOCONF().setString("log_exclude_filter", value, false);
        LOGCONF().setExcludeFilter(value);
    }

    virtual void getDestinationURI(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("Log.destinationURI") );
    }

    virtual void setDestinationURI( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
    {
#if !defined(RHO_NO_RUBY)
        RHOCONF().setString("Log.destinationURI", value, false);
        rho_log_resetup_http_url(value.c_str());
#endif
    }

    virtual void trace( const rho::String& message,  const rho::String& category, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGC(TRACE, (category.length() > 0 ? category.c_str() : "APP") ) + message;
    }

    virtual void info( const rho::String& message,  const rho::String& category, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGC(INFO, (category.length() > 0 ? category.c_str() : "APP") ) + message;
    }

    virtual void warning( const rho::String& message,  const rho::String& category, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGC(WARNING, (category.length() > 0 ? category.c_str() : "APP") ) + message;
    }

    virtual void error( const rho::String& message,  const rho::String& category, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGC(ERROR, (category.length() > 0 ? category.c_str() : "APP") ) + message;
    }

    virtual void fatalError( const rho::String& message,  const rho::String& category, rho::apiGenerator::CMethodResult& oResult)
    {
        LOGC(FATAL, (category.length() > 0 ? category.c_str() : "APP") ) + message;
    }

    virtual void sendLogFile(rho::apiGenerator::CMethodResult& oResult)
    {
        RHODESAPPBASE().sendLog(oResult);
    }

    virtual void showLog(rho::apiGenerator::CMethodResult& oResult)
    {
        rho_conf_show_log();
    }

    virtual void cleanLogFile(rho::apiGenerator::CMethodResult& oResult)
    {
        LOGCONF().clearLog();
    }

    virtual void readLogFile(int limit, rho::apiGenerator::CMethodResult& oResult)
    {
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

            oResult.setCollectionMode(true);
            oResult.set("");
            oResult.setCollectionMode(false);

            oResult.getString().resize(limit);
            char* szStr = (char*)oResult.getString().c_str();

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

        if ( oResult.hasCallback() )
            oResult.callCallback();

    }

};

////////////////////////////////////////////////////////////////////////

class CLogFactory: public CLogFactoryBase
{
public:
    ~CLogFactory(){}

    ILogSingleton* createModuleSingleton()
    { 
        return new CLogImpl(); 
    }
};

}

extern "C" void Init_Log()
{
    rho::CLogFactory::setInstance( new rho::CLogFactory() );
    rho::Init_Log_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoLogApi");
}
