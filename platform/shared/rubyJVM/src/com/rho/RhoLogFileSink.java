package com.rho;

import java.io.IOException;
import com.rho.file.SimpleFile;

public class RhoLogFileSink implements IRhoLogSink{

    SimpleFile         m_pFile = null;
    SimpleFile         m_pPosFile = null;
    int                m_nCirclePos = -1;
    int       		   m_nFileLogSize = 0;
	
	private static RhoLogConf m_oLogConf;

	RhoLogFileSink( RhoLogConf conf ){
		m_oLogConf = conf;
	}
	
    public void close(){
    	if ( m_pFile != null ){
    		try{ m_pFile.close(); }catch(IOException exc2){}
    		
    		m_pFile = null;
    	}
    	if ( m_pPosFile != null ){
    		try{ m_pPosFile.close(); }catch(IOException exc2){}
    		m_pPosFile = null;
    	}
    	
    	m_nCirclePos = -1;
    	m_nFileLogSize = 0;
    }
	
    public void clear(){
		try{
	    	close();
	    	
		    SimpleFile oFile = RhoClassFactory.createFile();
		    oFile.delete(m_oLogConf.getLogFilePath());
		    oFile.delete(m_oLogConf.getLogFilePath()+"_pos");
		}catch(Exception exc){
			
		}
    }
    
	private RhoLogConf getLogConf(){
		return m_oLogConf;
	}
	
	public int getCurPos() {
		return m_nCirclePos >= 0 ? m_nCirclePos : m_nFileLogSize;
	}

	static private void log(String txt) {
		System.out.println("RhoLogFileSink: " + txt);
	}
	
	public void writeLogMessage(String strMsg) {
		try{
		    int len = strMsg.length();
	
		    if ( m_pFile == null )    
		        m_pFile = RhoClassFactory.createFile();
	
		    if ( !m_pFile.isOpened() ){
		        m_pFile.open( getLogConf().getLogFilePath(), false, false );
		        m_nFileLogSize = (int)m_pFile.length();
		        loadLogPosition();
		    }
	
		    if ( getLogConf().getMaxLogFileSize() > 0 )
		    {
		        if ( ( m_nCirclePos >= 0 && m_nCirclePos + len > getLogConf().getMaxLogFileSize() ) || 
		             ( m_nCirclePos < 0 && m_nFileLogSize + len > getLogConf().getMaxLogFileSize() ) )
		        {
		            //m_pFile.movePosToStart();
		            m_nFileLogSize = 0;
		            m_nCirclePos = 0;
		        }
		    }
	
		    m_pFile.write( m_nCirclePos >= 0 ? m_nCirclePos : m_nFileLogSize, strMsg.getBytes() );
		    m_pFile.sync();
	
		    if ( m_nCirclePos >= 0 )
		        m_nCirclePos += len;
		    else
		        m_nFileLogSize += len;
	
		    saveLogPosition();
		}catch(Exception exc){
			log(exc.getMessage());
		}
	}

	private void loadLogPosition()throws Exception
	{
	    if ( m_pPosFile == null )
	        m_pPosFile = RhoClassFactory.createFile();

	    if ( !m_pPosFile.isOpened() ){
	        String strPosPath = getLogConf().getLogFilePath() + "_pos";
	        try{
	        	m_pPosFile.open( strPosPath, false, false );
	        }catch(Exception exc){}
	    }

	    String strPos = m_pPosFile.readString();
	    if ( strPos.length() > 0 )
	    	m_nCirclePos = Integer.parseInt(strPos);
	}

	private void saveLogPosition()throws IOException
	{
	    if ( m_nCirclePos < 0 )
	        return;

	    String strPos = Integer.toString(m_nCirclePos);
	    m_pPosFile.write( 0, strPos.getBytes() );
	    m_pPosFile.sync();
	}
	
}
