package com.rho;

import java.io.IOException;
import com.rho.file.SimpleFile;

public class RhoLogConf {
    int         m_nMinSeverity = 0;
    boolean     m_bLogToOutput = false;

    boolean     m_bLogToFile = false;
    String      m_strLogFilePath = "";
    int         m_nMaxLogFileSize = 0;

    boolean     m_bLogPrefix = false;

    String      m_strEnabledCategories = "", m_strDisabledCategories = "";

    IRhoLogSink   m_pFileSink = null;
    IRhoLogSink   m_pOutputSink = null;

	public RhoLogConf(){
		m_pFileSink = new RhoLogFileSink(this);
		m_pOutputSink = new RhoLogOutputSink(this);
    }

	public static void close(){ 
		//TODO: should we close log file? some threads may still logging 
	}
	
	RhoConf RHOCONF(){ return RhoConf.getInstance(); }
	
    public void saveToFile(){
        RHOCONF().setInt("MinSeverity", getMinSeverity() );
        RHOCONF().setBool("LogToOutput", isLogToOutput() );
        RHOCONF().setBool("LogToFile", isLogToFile() );
        RHOCONF().setString("LogFilePath", getLogFilePath() );
        RHOCONF().setInt("MaxLogFileSize", getMaxLogFileSize() );
        RHOCONF().setString("LogCategories", getEnabledCategories() );
        RHOCONF().setString("ExcludeLogCategories", getDisabledCategories() );

        RHOCONF().saveToFile();
    }
    
    void loadFromConf(RhoConf oRhoConf){
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
    void setLogToOutput(boolean bLogToOutput){ m_bLogToOutput = bLogToOutput;}

    boolean isLogToFile(){ return m_bLogToFile;}
    void setLogToFile(boolean bLogToFile){ m_bLogToFile = bLogToFile;}

    String getLogFilePath(){ return m_strLogFilePath;}
    void setLogFilePath(String szLogFilePath)
    {
        if ( !m_strLogFilePath.equals(szLogFilePath) ){
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
    
    void setMaxLogFileSize(int nMaxSize){m_nMaxLogFileSize = nMaxSize; }
    int getMaxLogFileSize(){ return m_nMaxLogFileSize; }

    boolean isLogPrefix(){ return m_bLogPrefix;}
    void setLogPrefix(boolean bLogPrefix){ m_bLogPrefix = bLogPrefix;}

    public synchronized void setEnabledCategories( String szCatList ){m_strEnabledCategories = szCatList; }
    public synchronized void setDisabledCategories( String szCatList ){ m_strDisabledCategories = szCatList; }
    
    public String getEnabledCategories(){ return m_strEnabledCategories; }
    public String getDisabledCategories(){ return m_strDisabledCategories; }
    
    synchronized boolean isCategoryEnabled(String cat){
        //TODO: Optimize categories search : add map

        if ( m_strDisabledCategories.indexOf(cat) >= 0 )
            return false;

        if ( m_strEnabledCategories.length() == 0 )
            return false;

        return m_strEnabledCategories.equals("*") || m_strEnabledCategories.indexOf(cat) >= 0;
    }

    void sinkLogMessage( String strMsg, boolean bOutputOnly ){
        if ( !bOutputOnly && isLogToFile() )
            m_pFileSink.writeLogMessage(strMsg);

        //Should be at the end
        if ( isLogToOutput() )
            m_pOutputSink.writeLogMessage(strMsg);
    	
    }
    
    int  getLogTextPos(){
        return m_pFileSink != null ? m_pFileSink.getCurPos() : -1;
    }
    
	public String getLogText(){
		String res = "";
    	SimpleFile oFile = null;
	    boolean bOldSaveToFile = isLogToFile();
	    setLogToFile(false);
    	
    	try{
	        oFile = RhoClassFactory.createFile();
	        oFile.open( getLogFilePath(), true, false);
	        
	        if ( oFile.isOpened() ){
	            res = oFile.readString();
	            oFile.close();
	        }
	        
    	}catch(Exception exc){
    		if ( oFile != null )
    			try{ oFile.close(); }catch(IOException exc2){}
    	}finally
    	{
    		setLogToFile(bOldSaveToFile);
    	}
		
		return res;
	}

}
