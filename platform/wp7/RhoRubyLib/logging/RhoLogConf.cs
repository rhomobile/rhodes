using System;
using rho.common;

namespace rho.logging
{
    public class RhoLogConf
    {
    int         m_nMinSeverity = 0;
    boolean     m_bLogToOutput = false;

    boolean     m_bLogToFile = false;
    String      m_strLogFilePath = "";
    int         m_nMaxLogFileSize = 0;

    boolean     m_bLogPrefix = false;

    String      m_strEnabledCategories = "", m_strDisabledCategories = "";

    IRhoLogSink   m_pFileSink = null;
    IRhoLogSink   m_pOutputSink = null;
    Mutex m_FlushLock = new Mutex();
    Mutex m_CatLock = new Mutex();

	public RhoLogConf(){
		m_pFileSink = new rho.logging.RhoLogFileSink(this);
		m_pOutputSink = new rho.logging.RhoLogOutputSink(this);
    }

	public static void close(){ 
		//TODO: should we close log file? some threads may still logging 
	}
	
	RhoConf RHOCONF(){ return RhoConf.getInstance(); }
	
    public void saveToFile()
    {
        RHOCONF().setInt("MinSeverity", getMinSeverity(), true );
        RHOCONF().setBool("LogToOutput", isLogToOutput(), true );
        RHOCONF().setBool("LogToFile", isLogToFile(), true  );
        RHOCONF().setString("LogFilePath", getLogFilePath(), true  );
        RHOCONF().setInt("MaxLogFileSize", getMaxLogFileSize(), true  );
        RHOCONF().setString("LogCategories", getEnabledCategories(), true  );
        RHOCONF().setString("ExcludeLogCategories", getDisabledCategories(), true  );
    }

    public void loadFromConf(RhoConf oRhoConf)
    {
        if ( oRhoConf.isExist( "MinSeverity" ) )
            setMinSeverity( oRhoConf.getInt("MinSeverity") );
        if ( oRhoConf.isExist( "LogToOutput") )
            setLogToOutput( oRhoConf.getBool("LogToOutput") );
        if ( oRhoConf.isExist( "LogToFile") )
            setLogToFile( oRhoConf.getBool("LogToFile"));
        if ( oRhoConf.isExist( "LogFilePath") )
            setLogFilePath( oRhoConf.getString("LogFilePath") );
        if ( oRhoConf.isExist( "MaxLogFileSize") )
            setMaxLogFileSize( oRhoConf.getInt("MaxLogFileSize") );
        if ( oRhoConf.isExist( "LogCategories") )
            setEnabledCategories( oRhoConf.getString("LogCategories") );
        if (oRhoConf.isExist( "ExcludeLogCategories") )
            setDisabledCategories( oRhoConf.getString("ExcludeLogCategories") );
    }
	
    public int getMinSeverity(){ return m_nMinSeverity; }
    public void setMinSeverity(int nMinSeverity){ m_nMinSeverity = nMinSeverity; }

    boolean isLogToOutput(){ return m_bLogToOutput;}
    public void setLogToOutput(boolean bLogToOutput) { m_bLogToOutput = bLogToOutput; }

    boolean isLogToFile(){ return m_bLogToFile;}
    public void setLogToFile(boolean bLogToFile) { m_bLogToFile = bLogToFile; }

    public String getLogFilePath(){ return m_strLogFilePath;}
    public void setLogFilePath(String szLogFilePath)
    {
        if ( m_strLogFilePath.compareTo(szLogFilePath)!=0 ){
            m_strLogFilePath = szLogFilePath; 

            if ( m_pFileSink != null){
            	m_pFileSink.close();
            }
        }
    }

	public void clearLog(){
        if ( m_pFileSink != null){
        	m_pFileSink.clear();
        }
	}

    public void setMaxLogFileSize(int nMaxSize) { m_nMaxLogFileSize = nMaxSize; }
    public int getMaxLogFileSize(){ return m_nMaxLogFileSize; }

    public boolean isLogPrefix(){ return m_bLogPrefix;}
    public void setLogPrefix(boolean bLogPrefix){ m_bLogPrefix = bLogPrefix;}

    public void setEnabledCategories( String szCatList )
    {
        lock(m_CatLock)
        {
            m_strEnabledCategories = szCatList; 
        }
    }
    public void setDisabledCategories( String szCatList )
    { 
        lock(m_CatLock)
        {
            m_strDisabledCategories = szCatList; 
        }
    }
    
    public String getEnabledCategories(){ return m_strEnabledCategories; }
    public String getDisabledCategories(){ return m_strDisabledCategories; }
    
    public boolean isCategoryEnabled(String cat)
    {
        //TODO: Optimize categories search : add map
        lock (m_CatLock)
        {
            if (m_strDisabledCategories.indexOf(cat) >= 0)
                return false;

            if (m_strEnabledCategories.length() == 0)
                return false;

            return m_strEnabledCategories.equals("*") || m_strEnabledCategories.indexOf(cat) >= 0;
        }
    }

    public void sinkLogMessage( String strMsg, boolean bOutputOnly ){
        if ( !bOutputOnly && isLogToFile() )
            m_pFileSink.writeLogMessage(strMsg);

        //Should be at the end
        if ( isLogToOutput() )
            m_pOutputSink.writeLogMessage(strMsg);
    	
    }
    
    public int  getLogTextPos(){
        return m_pFileSink != null ? m_pFileSink.getCurPos() : -1;
    }
    
	public String getLogText(){
		String res = "";
	    boolean bOldSaveToFile = isLogToFile();
	    setLogToFile(false);
    	
    	try{
            res = CRhoFile.readStringFromFile(getLogFilePath());
    	}finally
    	{
    		setLogToFile(bOldSaveToFile);
    	}
		
		return res;
	}
    }
}
