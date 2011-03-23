using System;
using rho.logging;

namespace rho.common
{
    public class RhoLogger
    {
        public static boolean RHO_STRIP_LOG = false;

        private static int L_TRACE = 0;
	    private static int L_INFO = 1;
	    private static int L_WARNING = 2;
	    private static int L_ERROR = 3;
	    private static int L_FATAL = 4;
	
	    private String[] LogSeverityNames = { "TRACE", "INFO", "WARNING", "ERROR", "FATAL" };
	
	    private String m_category;
	    private static RhoLogConf m_oLogConf = new RhoLogConf();
	    private String m_strMessage;
	    private int    m_severity;
	    private static Mutex m_SinkLock = new Mutex();
	    //private static IRhoRubyHelper m_sysInfo;
	    public static String LOGFILENAME = "RhoLog.txt";

        public RhoLogger(String name)
        {
		    m_category = name;
	    }

        public static RhoLogConf getLogConf(){
		    return m_oLogConf;
	    }

        public String getLogCategory() { return m_category; }
        public void setLogCategory(String category) { m_category = category; }

        public static void close() { RhoLogConf.close(); }

        private boolean isEnabled()
        {
            if (m_severity >= getLogConf().getMinSeverity())
            {
                if (m_category.length() == 0 || m_severity >= L_ERROR)
                    return true;

                return getLogConf().isCategoryEnabled(m_category);
            } 

            return true;
        }

        private String get2FixedDigit(int nDigit)
        {
		    if ( nDigit > 9 )
		    	return nDigit.ToString();
			
		    return "0" + nDigit.ToString();
	    }
	
	    private String getLocalTimeString()
        {
            DateTime time = DateTime.Now;

		    String strTime = "";
		    strTime += 
			    get2FixedDigit(time.Month) + "/" + 
			    get2FixedDigit(time.Day) + "/" +
			    time.Year + " " + 
			    get2FixedDigit(time.Hour) + ":" + 
			    get2FixedDigit(time.Minute) +	":" + 
			    get2FixedDigit(time.Second);
			
			    //if ( false ) //comment this to show milliseconds
				    strTime += ":" + get2FixedDigit(time.Millisecond);
			
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

	    private String makeStringSizeEnd(String str, int nSize)
	    {
		    if ( str.length() >= nSize )
			    return str.substring(str.length()-nSize, str.length());
		    else {
			    String res = "";
			    for( int i = 0; i < nSize - str.length(); i++ )
				    res += ' ';
			
			    res += str;
			
			    return res;
		    }
	    }
	
	    private String getThreadField()
        {
		    String strThread = System.Threading.Thread.CurrentThread.Name;
            if (strThread == null || strThread.length() == 0)
                strThread = System.Threading.Thread.CurrentThread.ManagedThreadId.ToString();

		    return strThread;
	    }
	
	    private void addPrefix()
        {
	        //(log level, local date time, thread_id, file basename, line)
	        //I time f5d4fbb0 category|

	        if ( m_severity == L_FATAL )
	    	    m_strMessage += LogSeverityNames[m_severity];
	        else
	    	    m_strMessage += LogSeverityNames[m_severity].charAt(0);

	        m_strMessage += " " + getLocalTimeString() + ' ' + makeStringSizeEnd(getThreadField(),9) + ' ' +
	    	    makeStringSize(m_category,15) + "| ";
	    }

        private void logMessage( int severity, String msg ){
	        logMessage(severity, msg, null, false );
        }
        private void logMessage( int severity, String msg, Exception e ){
	        logMessage(severity, msg, e, false );
        }

        private void logMessage(int severity, String msg, Exception e, boolean bOutputOnly)
        {
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
	    	    m_strMessage += (msg != null && msg.length() > 0 ? ";" : "") + e.GetType().FullName + ": ";
	    	
	    	    String emsg = e.Message;
	    	    if ( emsg != null )
	    		    m_strMessage += emsg;

                String trace = e.StackTrace;
                if (trace != null)
                    m_strMessage += ";TRACE: \n" + trace;
	        }
	    
		    if (m_strMessage.length() > 0 || m_strMessage.charAt(m_strMessage.length() - 1) != '\n')
			    m_strMessage += '\n';
			
		    if ( bOutputOnly )
		    {
                System.Diagnostics.Debug.WriteLine(m_strMessage);
		    }else
		    {
		        lock( m_SinkLock ){
		    	    getLogConf().sinkLogMessage( m_strMessage, bOutputOnly );
		        }
		    }
	        if ( m_severity == L_FATAL )
	    	    processFatalError();
        }

        static boolean isSimulator()
        {
            return Microsoft.Devices.Environment.DeviceType == Microsoft.Devices.DeviceType.Emulator;
        }

        protected void processFatalError()
        {
            if (isSimulator())
                throw new Exception("Fatal error.");

            System.Threading.Thread.CurrentThread.Abort();
        }

        public void TRACE(String message)
        {
            logMessage(L_TRACE, message);
        }
        public void TRACE(String message, Exception e)
        {
            logMessage(L_TRACE, message, e);
        }

        public void INFO(String message)
        {
            logMessage(L_INFO, message);
        }

        public void INFO_OUT(String message)
        {
            logMessage(L_INFO, message, null, true);
        }

        public void INFO_EVENT(String message)
        {
            //EventLogger.logEvent(EVENT_GUID, (m_category + ": " + message).getBytes());

            INFO_OUT(message);
        }

        public void WARNING(String message)
        {
            logMessage(L_WARNING, message);
        }
        public void ERROR(String message)
        {
            logMessage(L_ERROR, message);
        }
        public void ERROR(Exception e)
        {
            logMessage(L_ERROR, "", e);
        }
        public void ERROR(String message, Exception e)
        {
            logMessage(L_ERROR, message, e);
        }
        public void ERROR_OUT(String message, Exception e) 
        {
		    logMessage( L_ERROR, message, e, true );
	    }

        public void ERROR_EVENT(String message, Exception e)
        {
            ERROR_OUT(message, e);

            //EventLogger.logEvent(EVENT_GUID, m_strMessage.getBytes());
        }

        public void FATAL(String message)
        {
            logMessage(L_FATAL, message);
        }
        public void FATAL(Exception e)
        {
            logMessage(L_FATAL, "", e);
        }
        public void FATAL(String message, Exception e)
        {
            logMessage(L_FATAL, message, e);
        }

        public void ASSERT(boolean exp, String message)
        {
            if (!exp)
                logMessage(L_FATAL, message);
        }

        public static String getLogText(){
		    return m_oLogConf.getLogText();
	    }
	
	    public static int getLogTextPos(){
		    return m_oLogConf.getLogTextPos();
	    }
	
	    public static void clearLog(){
	        lock( m_SinkLock ){
	    	    getLogConf().clearLog();
	        }
	    }
	
        public static void InitRhoLog()
        {
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

    public class RhoEmptyLogger : RhoLogger
    {
        public RhoEmptyLogger() : base("") { }
    }
}
