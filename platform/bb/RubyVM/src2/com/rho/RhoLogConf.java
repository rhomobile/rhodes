package com.rho;

public class RhoLogConf {
    int         m_nMinSeverity = 0;
    boolean     m_bLogToOutput = false;

    boolean     m_bLogToFile = false;
    String      m_strLogFilePath = "";
    String      m_strLogConfFilePath = "";
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
	
	void setLogConfFilePath(String path){ m_strLogConfFilePath = path; }
	String getLogConfFilePath(){ return m_strLogConfFilePath; }
	
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

        if ( m_strDisabledCategories.indexOf(cat) > 0 )
            return false;

        if ( m_strEnabledCategories.length() == 0 )
            return false;

        return m_strEnabledCategories.equals("*") || m_strEnabledCategories.indexOf(cat) > 0;
    }

    void sinkLogMessage( String strMsg ){
        if ( isLogToFile() )
            m_pFileSink.writeLogMessage(strMsg);

        //Should be at the end
        if ( isLogToOutput() )
            m_pOutputSink.writeLogMessage(strMsg);
    	
    }

    public void saveToFile(String szFilePath){
        String strData = saveToString();
    	SimpleFile oFile = null;

    	try{
	        oFile = FileFactory.createFile();
	        
        	oFile.open( szFilePath != null && szFilePath.length()>0 ? szFilePath : this.getLogConfFilePath(),
        			false, false);
        	oFile.truncate(0);
	        oFile.write( 0, strData.getBytes() );
	        oFile.close();
    	}catch(Exception exc){
    		if ( oFile != null )
    			oFile.close();
    	}
        
    }
    
    void loadFromFile(String szFilePath){
    	SimpleFile oFile = null;
    	try{
	        oFile = FileFactory.createFile();
	        oFile.open( szFilePath != null && szFilePath.length()>0 ? szFilePath : this.getLogConfFilePath(),
	        		true, false);
	        
	        if ( oFile.isOpened() ){
	            String strSettings = oFile.readString();
	            oFile.close();
	            loadFromString( strSettings );
	        }
	        
    	}catch(Exception exc){
    		if ( oFile != null )
    			oFile.close();
    	}
    }
    
    String saveToString(){
    	String strData = ""; 
        strData += "MinSeverity=";
        strData += getMinSeverity();
        strData += "\n";

        strData += "LogToOutput=";
        strData += isLogToOutput()?1:0;
        strData += "\n";

        strData += "LogToFile=";
        strData += isLogToFile()?1:0;
        strData += "\n";

        strData += "LogFilePath=";
        strData += getLogFilePath();
        strData += "\n";

        strData += "MaxLogFileSize=";
        strData += getMaxLogFileSize();
        strData += "\n";

        strData += "LogCategories=";
        strData += getEnabledCategories();
        strData += "\n";

        strData += "ExcludeLogCategories=";
        strData += getDisabledCategories();
        strData += "\n";
        
        return strData;
    }
    
    void loadFromString(String szSettings){
		Tokenizer stringtokenizer = new Tokenizer(szSettings, "\n");
		while (stringtokenizer.hasMoreTokens()) {
			String tok = stringtokenizer.nextToken();
			tok = tok.trim();
			if (tok.length() == 0) {
				continue;
			}
			int i = tok.indexOf('=');
			String name;
			String value;
			if (i > 0) {
				name = tok.substring(0, i);
				value = tok.substring(i + 1);
			} else {
				name = tok;
				value = "";
			}
			name = name.trim();
			value = value.trim();
			
			setPropertyByName(name,value);
		}
	}

    void setPropertyByName(String name, String value ){

        if ( name.equalsIgnoreCase("MinSeverity") )
            setMinSeverity(Integer.parseInt(value));
        else if ( name.equalsIgnoreCase("LogToOutput") )
            setLogToOutput( Integer.parseInt(value) == 0 ? false : true);
        else if ( name.equalsIgnoreCase("LogToFile") )
            setLogToFile( Integer.parseInt(value) == 0 ? false : true);
        else if ( name.equalsIgnoreCase("LogFilePath") )
            setLogFilePath( value );
        else if ( name.equalsIgnoreCase("MaxLogFileSize") )
            setMaxLogFileSize( Integer.parseInt(value) );
        else if ( name.equalsIgnoreCase("LogCategories") )
            setEnabledCategories( value );
        else if ( name.equalsIgnoreCase("ExcludeLogCategories") )
            setDisabledCategories( value );
    }
    
    int  getLogTextPos(){
        return m_pFileSink != null ? m_pFileSink.getCurPos() : -1;
    }
    
	public String getLogText(){
		String res = "";
    	SimpleFile oFile = null;
    	try{
	        oFile = FileFactory.createFile();
	        oFile.open( getLogFilePath(), true, false);
	        
	        if ( oFile.isOpened() ){
	            res = oFile.readString();
	            oFile.close();
	        }
	        
    	}catch(Exception exc){
    		if ( oFile != null )
    			oFile.close();
    	}
		
		return res;
	}
    
}
