using System;
using rho.common;

namespace rho.logging
{
    public class RhoLogFileSink : IRhoLogSink
    {
    CRhoFile         m_pFile = null;
    CRhoFile         m_pPosFile = null;
    int              m_nCirclePos = -1;
    int       	     m_nFileLogSize = 0;
	
	private static RhoLogConf m_oLogConf;

	public RhoLogFileSink( RhoLogConf conf ){
		m_oLogConf = conf;
	}
	
    public void close(){
    	if ( m_pFile != null ){
    		try{ m_pFile.close(); }catch(Exception ){}
    		
    		m_pFile = null;
    	}
    	if ( m_pPosFile != null ){
    		try{ m_pPosFile.close(); }catch(Exception ){}
    		m_pPosFile = null;
    	}
    	
    	m_nCirclePos = -1;
    	m_nFileLogSize = 0;
    }
	
    public void clear(){
		try{
	    	close();
	    	
		    CRhoFile.deleteFile(m_oLogConf.getLogFilePath());
		    CRhoFile.deleteFile(m_oLogConf.getLogFilePath()+"_pos");
		}catch(Exception ){
			
		}
    }
    
	private RhoLogConf getLogConf(){
		return m_oLogConf;
	}
	
	public int getCurPos() {
		return m_nCirclePos >= 0 ? m_nCirclePos : m_nFileLogSize;
	}

    static private void log(String txt) {
		System.Diagnostics.Debug.WriteLine("RhoLogFileSink: " + txt);
	}

	public void writeLogMessage(String strMsg) {
		try{
		    int len = strMsg.length();
	
		    if ( m_pFile == null )    
		        m_pFile = new CRhoFile();
	
		    if ( !m_pFile.isOpened() ){
		        m_pFile.open( getLogConf().getLogFilePath(), CRhoFile.EOpenModes.OpenForAppend );
		        m_nFileLogSize = (int)m_pFile.size();
		        loadLogPosition();
		    }
	
		    if ( getLogConf().getMaxLogFileSize() > 0 )
		    {
		        if ( ( m_nCirclePos >= 0 && m_nCirclePos + len > getLogConf().getMaxLogFileSize() ) || 
		             ( m_nCirclePos < 0 && m_nFileLogSize + len > getLogConf().getMaxLogFileSize() ) )
		        {
		            m_pFile.movePosToStart();
		            m_nFileLogSize = 0;
		            m_nCirclePos = 0;
		        }
		    }

            //int nWritten = m_pFile.writeString(strMsg);
            m_pFile.writeString(strMsg);
            m_pFile.flush();
	
		    if ( m_nCirclePos >= 0 )
		        m_nCirclePos += len;
		    else
		        m_nFileLogSize += len;
	
		    saveLogPosition();
		}catch(Exception exc){
			log(exc.Message);
		}
	}

	private void loadLogPosition()
	{
	    if ( m_pPosFile == null )
	        m_pPosFile = new CRhoFile();

	    if ( !m_pPosFile.isOpened() ){
	        String strPosPath = getLogConf().getLogFilePath() + "_pos";
            m_pPosFile.open( strPosPath, CRhoFile.EOpenModes.OpenForReadWrite );
	    }

        if (!m_pPosFile.isOpened())
            return;

        m_pPosFile.movePosToStart();
	    String strPos = m_pPosFile.readString();
        if (strPos.length() == 0)
            return;

    	m_nCirclePos = int.Parse(strPos);

        if (m_nCirclePos < 0 || m_nCirclePos > (int)m_nFileLogSize)
            m_nCirclePos = -1;

        if (m_nCirclePos >= 0)
            m_pFile.setPosTo(m_nCirclePos);
	}

	private void saveLogPosition()
	{
	    if ( m_nCirclePos < 0 )
	        return;

        if (m_nCirclePos > (int)getLogConf().getMaxLogFileSize())
            return;

        String strPos = m_nCirclePos.ToString();
        for (int i = strPos.length(); i < 10; i++)
            strPos += ' ';

        m_pPosFile.movePosToStart();
        m_pPosFile.writeString(strPos);
        m_pPosFile.flush();
    }
    }

    public class RhoLogOutputSink : IRhoLogSink
    {
        private static RhoLogConf m_oLogConf;

        public RhoLogOutputSink(RhoLogConf conf)
        {
            m_oLogConf = conf;
        }

        public void close()
        {
        }

        private RhoLogConf getLogConf()
        {
            return m_oLogConf;
        }

        public int getCurPos()
        {
            return 0;
        }

        public void writeLogMessage(String strMsg) 
        {
#if DEBUG
            if ( strMsg.charAt(strMsg.length() - 1) != '\n' )
                System.Diagnostics.Debug.WriteLine(strMsg);
            else
                System.Diagnostics.Debug.WriteLine(strMsg.Substring(0, strMsg.length() - 1));
#endif
	    }

        public void clear()
        {

        }
    }
}
