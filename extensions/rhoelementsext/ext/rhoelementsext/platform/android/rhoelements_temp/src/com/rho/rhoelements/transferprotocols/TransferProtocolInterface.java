/**
 * 
 */
package com.rho.rhoelements.transferprotocols;


/**
 * @author FPC843
 *
 */
public interface TransferProtocolInterface
{
	public static final String OK_FILESENT = "OK: File Sent";
	public static final String OK_FILERECEIVED = "OK: File Received";
	public static final String ERROR = "Error Code";
	
	/*
	 * Exception strings 
	 */
	public static final String IOEXCEPTION = "IO Exception";
	/*public static final String FILENOTFOUNDEXCEPTION = "File not found";
	public static final String MALFORMEDURLEXCEPTION = "Malformed URL";
	public static final String AUTHENTICATIONEXCEPTION = "Authentication failed";
	public static final String CONNECTIONCLOSED = "Connection unexpectedly closed";
	public static final String CONNECTEXCEPTION = "Nework is unreachable";
	public static final String HOSTUNKNOWN = "Host unknown";*/
	
	/*Windows error codes emulation*/
	public static final String FILENOTFOUNDEXCEPTION = "2";
	public static final String PATHNOTFOUNDEXCEPTION = "3";
	public static final String MALFORMEDURLEXCEPTION = "123";
	public static final String AUTHENTICATIONEXCEPTION = "Authentication failed";
	public static final String FTPAUTHENTICATIONEXCEPTION = "12014";
	public static final String CONNECTIONCLOSED = "12031";
	public static final String CONNECTEXCEPTION = "12029";
	public static final String HOSTUNKNOWN = "12007";
	public static final String SERVERERRORMESSAGE = "12003";
	public static final String CONNECTIONTIMEOUTEXCEPTION = "12002";
	
	
	/*
	 * Regex for username:password
	 */
	public static final String USERNAMEPASSWORDREGEX = "^[A-Za-z0-9]+:[A-Za-z0-9]*";
	
	/**
	 * Method to perform the transfer.
	 * This will never be called in the main activity queue therefore the method can block.
	 * @return String to specify if the transfer was successful, conforming to the Browser FileTransfer format
	 */
	public abstract String transfer();
}
