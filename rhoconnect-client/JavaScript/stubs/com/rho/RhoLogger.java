package com.rho;

import com.google.gwt.core.client.GWT;

public class RhoLogger {
	public static final boolean RHO_STRIP_LOG = false;
	public static final long EVENT_GUID = 0x4c9c8411d87982f2L;
	public static String LOGFILENAME = "RhoLog.txt";

	private String m_loggerName;
	
	public RhoLogger(String name){
		m_loggerName = name;
	}

	public void ERROR(String message) {
		GWT.log("ERROR("+ m_loggerName +"): "+ message);
	}

	public void INFO(String message) {
		GWT.log("INFO("+ m_loggerName +"): "+ message);
	}

	public void ERROR(String message, Exception exc) {
		GWT.log("ERROR("+ m_loggerName +"): "+ message);
		GWT.log("EXCEPTION("+ m_loggerName +"): "+ exc.getMessage());
	}

	public void TRACE(String message) {
		GWT.log("TRACE("+ m_loggerName +"): "+ message);
	}
}
