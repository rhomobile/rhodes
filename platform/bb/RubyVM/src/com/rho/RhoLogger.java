package com.rho;

import java.util.Calendar;

import net.rim.device.api.system.EventLogger;

public class RhoLogger {
	public static final boolean RHO_STRIP_LOG = false;
	public static final long EVENT_GUID = 0x4c9c8411d87982f2L;

	private static final int L_TRACE = 0;
	private static final int L_INFO = 1;
	private static final int L_WARNING = 2;
	private static final int L_ERROR = 3;
	private static final int L_FATAL = 4;
	//private static final int L_NUM_SEVERITIES = 5;
	
	private String LogSeverityNames[] = { "TRACE", "INFO", "WARNING", "ERROR", "FATAL" };
	
	private String m_category;
	private static RhoLogConf m_oLogConf = new RhoLogConf();
	private String m_strMessage;
	private int    m_severity;
	private static String m_SinkLock = "";
	private static IRhoRubyHelper m_sysInfo;
	public static String LOGFILENAME = "RhoLog.txt";
	
	public RhoLogger(String name){
		m_category = name;
	}

	public static RhoLogConf getLogConf(){
		return m_oLogConf;
	}
	
	static
	{
	    EventLogger.register(EVENT_GUID, "RHODESAPP", EventLogger.VIEWER_STRING);
	}
	
	public String getLogCategory(){ return m_category; }
	public void setLogCategory(String category){ m_category = category; }
	
	public static void close(){ RhoLogConf.close(); }
	
	private boolean isEnabled(){
	    if ( m_severity >= getLogConf().getMinSeverity() ){
	        if ( m_category.length() == 0 || m_severity >= L_ERROR )
	            return true;

	        return getLogConf().isCategoryEnabled(m_category);
	    }

	    return false;
	
	}
	
	private String get2FixedDigit(int nDigit){
		if ( nDigit > 9 )
			return Integer.toString(nDigit);
			
		return "0" + Integer.toString(nDigit);
	}
	
	private String getLocalTimeString(){
		Calendar time = Calendar.getInstance();
		String strTime = "";
		strTime += 
			get2FixedDigit(time.get(Calendar.MONTH) + 1) + "/" + 
			get2FixedDigit(time.get(Calendar.DATE)) + "/" +
			time.get(Calendar.YEAR) + " " + 
			get2FixedDigit(time.get(Calendar.HOUR_OF_DAY)) + ":" + 
			get2FixedDigit(time.get(Calendar.MINUTE)) +	":" + 
			get2FixedDigit(time.get(Calendar.SECOND));
			
			//if ( false ) //comment this to show milliseconds
				strTime += ":" + get2FixedDigit(time.get(Calendar.MILLISECOND));
			
		//Date date = time.getTime();
		return strTime;
	}
	
	private String makeStringSize(String str, int nSize)
	{
		if ( str.length() >= nSize )
			return str.substring(0, nSize);
		else {
			String res = "";
			for( int i = 0; i < nSize - str.length(); i++ )
				res += ' ';
			
			res += str;
			
			return res;
		}
	}
	
	private String getThreadField(){
		String strThread = Thread.currentThread().getName();
		if ( strThread.startsWith("Thread-"))
		{
			try {
				int nThreadID = Integer.parseInt(strThread.substring(7));
				
				return Integer.toHexString(nThreadID);
			}catch(Exception exc){}
		}
		
		return strThread;
	}
	
	private void addPrefix(){
	    //(log level, local date time, thread_id, file basename, line)
	    //I time f5d4fbb0 category|

	    if ( m_severity == L_FATAL )
	    	m_strMessage += LogSeverityNames[m_severity];
	    else
	    	m_strMessage += LogSeverityNames[m_severity].charAt(0);

	    m_strMessage += " " + getLocalTimeString() + ' ' + makeStringSize(getThreadField(),8) + ' ' +
	    	makeStringSize(m_category,15) + "| ";
	}

	private void logMessage( int severity, String msg ){
		logMessage(severity, msg, null, false );
	}
	private void logMessage( int severity, String msg, Throwable e ){
		logMessage(severity, msg, e, false );
	}
	
	private void logMessage( int severity, String msg, Throwable e, boolean bOutputOnly ){
		m_severity = severity;
		if ( !isEnabled() )
			return;
		
		m_strMessage = "";
	    if ( getLogConf().isLogPrefix() )
	        addPrefix();
		
	    if ( msg != null )
	    	m_strMessage += msg;
	    
	    if ( e != null )
	    {
	    	m_strMessage += (msg != null && msg.length() > 0 ? ";" : "") + e.getClass().getName() + ": ";
	    	
	    	String emsg = e.getMessage();
	    	if ( emsg != null )
	    		m_strMessage += emsg;
	    }
	    
		if (m_strMessage.length() > 0 || m_strMessage.charAt(m_strMessage.length() - 1) != '\n')
			m_strMessage += '\n';
			
		if ( bOutputOnly )
		{
			System.out.print(m_strMessage);
			System.out.flush();
		}else
		{
		    synchronized( m_SinkLock ){
		    	getLogConf().sinkLogMessage( m_strMessage, bOutputOnly );
			    if ( (isSimulator() || m_severity == L_FATAL) && e != null ){
					//TODO: redirect printStackTrace to our log
					//e.printStackTrace();
			    }
		    }
		}
	    if ( m_severity == L_FATAL )
	    	processFatalError();
	}

	static boolean isSimulator()
	{
		return m_sysInfo.isSimulator();
	}
	
	protected void processFatalError(){
    	if ( isSimulator() )
    		throw new RuntimeException();
    	
    	System.exit(0);
	}
	
	public void TRACE(String message) {
		logMessage( L_TRACE, message);
	}
	public void TRACE(String message,Throwable e) {
		logMessage( L_TRACE, message, e );
	}
	
	public void INFO(String message) {
		logMessage( L_INFO, message);
	}
	
	public void INFO_OUT(String message) {
		logMessage( L_INFO, message, null, true);
	}

	public void INFO_EVENT(String message) 
	{
    	EventLogger.logEvent(EVENT_GUID, (m_category + ": " + message).getBytes());
		
		INFO_OUT(message);
	}
	
	public void WARNING(String message) {
		logMessage( L_WARNING, message);
	}
	public void ERROR(String message) {
		logMessage( L_ERROR, message);
	}
	public void ERROR(Throwable e) {
		logMessage( L_ERROR, "", e );
	}
	public void ERROR(String message,Throwable e) {
		logMessage( L_ERROR, message, e );
	}
	public void ERROR_OUT(String message,Throwable e) {
		logMessage( L_ERROR, message, e, true );
		//System.out.print(m_category + ": " + message + ". " + ( e != null ? e.getClass().getName() : "Exception") + 
		//		(e != null ? e.getMessage() : "" ) +"\n");
		if ( e != null )
			e.printStackTrace();
		
		System.out.flush();
	}

	public void ERROR_EVENT(String message,Throwable e) 
	{
		ERROR_OUT(message, e);
		
    	EventLogger.logEvent(EVENT_GUID, m_strMessage.getBytes());
	}
	
	public void FATAL(String message) {
		logMessage( L_FATAL, message);
	}
	public void FATAL(Throwable e) {
		logMessage( L_FATAL, "", e);
	}
	public void FATAL(String message, Throwable e) {
		logMessage( L_FATAL, message, e);
	}
	
	public void ASSERT(boolean exp, String message) {
		if ( !exp )
			logMessage( L_FATAL, message);
	}
	
	public static String getLogText(){
		return m_oLogConf.getLogText();
	}
	
	public static int getLogTextPos(){
		return m_oLogConf.getLogTextPos();
	}
	
	public static void clearLog(){
	    synchronized( m_SinkLock ){
	    	getLogConf().clearLog();
	    }
	}
	
    public static void InitRhoLog()throws Exception
    {
    	m_sysInfo = RhoClassFactory.createRhoRubyHelper();
        RhoConf.InitRhoConf();
        
        //Set defaults
    	m_oLogConf.setLogPrefix(true);		
    	
    	m_oLogConf.setLogToFile(true);
        
		if ( isSimulator() ) {
			m_oLogConf.setMinSeverity( L_INFO );
			m_oLogConf.setLogToOutput(true);
			m_oLogConf.setEnabledCategories("*");
			m_oLogConf.setDisabledCategories("");
	    	m_oLogConf.setMaxLogFileSize(0);//No limit
		}else{
			m_oLogConf.setMinSeverity( L_ERROR );
			m_oLogConf.setLogToOutput(false);
			m_oLogConf.setEnabledCategories("");
	    	m_oLogConf.setMaxLogFileSize(1024*50);
		}
		
    	if ( RhoConf.getInstance().getRhoRootPath().length() > 0 )
	    	m_oLogConf.setLogFilePath( RhoConf.getInstance().getRhoRootPath() + LOGFILENAME );

        //load configuration if exist
    	//
    	//m_oLogConf.saveToFile("");
    	//
    	RhoConf.getInstance().loadConf();
    	m_oLogConf.loadFromConf(RhoConf.getInstance());
    }
    
}
