package com.rho.rhoelements;

import java.util.Date;

/**
 * @author Geoff Day (XFH386, Initial Creation)
 *
 */
public class LogEntry
{
	// Severity levels
	public static final int PB_LOG_ERROR = 0;
	public static final int PB_LOG_WARNING = 1;
	public static final int PB_LOG_INFO = 2;
	public static final int PB_LOG_USER = 3;
	public static final int PB_LOG_DEBUG = 4;
	
	int severity, line;
	String comment, function, caller;
	Date date;
	
	public int getSeverity() {return severity;}
	public String getCaller() {return caller;}
	public String getFunction() {return function;}
	public String getComment() {return comment;}
	public int getLine() {return line;}
	public Date getDate() {return date;}
	
	static String[] severityNames = {"ERROR", "WARNING", "INFO", "USER", "DEBUG"};
	public String getSeverityName() {return severityNames[severity];}
	
	public LogEntry(int severity, String caller, String function, String comment, int line)
	{
		this.severity = severity;
		this.caller = caller;
		this.function = function;
		this.comment = comment;
		this.line = line;

		date = new Date();
	}
	
	public LogEntry(int severity, String comment)
	{
		this.severity = severity;
		this.comment = (comment == null ? "" : comment);
		
		date = new Date();
		
		StackTraceElement[] trace = Thread.currentThread().getStackTrace();
		if (trace.length >= 4)
		{
			this.caller = trace[3].getClassName();
			
			// Remove package name (including dot) from start of class name, if present
			if (this.caller.startsWith(Common.packageName))
				this.caller = this.caller.substring(Common.packageName.length() + 1);
			
			this.function = trace[3].getMethodName();
			this.line = trace[3].getLineNumber();
		}
		else
		{
			this.caller = "Unknown";
			this.function = "Unknown";
			this.line = 0;
		}
	}
}
