/**
 * 
 */
package com.rho.rhoelements.apd;

/**
 * @author FPC843
 * Abstracts the printer which data is sent to
 */
public class ApdConfiguration
{
	private String mBtMac;
	private String mIpAddress;
	private int mIpPort;
	private Language mLanguage;
	private int mModel;
	private int mId;
	private int mMessage;
	private int mRfComm;
	private String mFormatPath;
	private String mTemplatePath;
	private String mLabelIdPath;
	private Transport mTransport;
	
	public static final String IP_ADDRESS_DOTS_REGEX = "\\b(([0-9]{1,3}[.]{1}){3})(([0-9]{1,3}){1})\\b";
	public static final String IP_ADDRESS_NODOTS_REGEX = "\\b(([0-9]{3}){4})\\b";
	public static final String MAC_ADDRESS_COLUMNS_REGEX = "\\b([0-9A-Fa-f]{2}([:])){5}([0-9A-Fa-f]{2})\\b";
	public static final String MAC_ADDRESS_NOCOLUMNS_REGEX = "\\b([0-9A-Fa-f]{2}){6}\\b";
	public static final String PORT_REGEX = "\\b(\\d{1,5})\\b";
	/*
	 * The following regular expressions describe all the possible ways handled by APD to describe a Peripheral Id (PID)
	 * as described the APD Programmer's Manual Annex 5
	 */
	//5-Character PID
	public static final String FIVE_CHAR_ID_WIRED_REGEX = "\\b(0){4}[A-Z]+\\b"; //Wired not supported by Android APD
	public static final String FIVE_CHAR_ID_TOSHIBA_REGEX = "\\b[0-9]{5}\\b";
	/*
	 * As a protocol, SPAN (Symbol Personal Area Network is not supported any longer. APD will return an error if a printer)
	 * with a SPAN id is passed to APD
	 */
	public static final String FIVE_CHAR_ID_SPAN_REGEX = "\\b[0-9A-F]{4}[A-Z]+\\b";
	
	//7-Character PID
	public static final String SEVEN_CHAR_ID_WIRED_REGEX = "\\b(0){4}[A-Z]+[0-9]{2}\\b"; //Wired not supported by Android APD
	public static final String SEVEN_CHAR_ID_SPAN_REGEX = "\\b[0-9A-F]{4}[A-Z]+[0-9]{2}\\b"; //Same considerations about SPAN as above
	
	//4-Character PID
	public static final String FOUR_CHAR_ID_WIRELESS_REGEX = "\\b[B,W]{1}[A-Z]{1}[0-9]{2}";
	public static final String FOUR_CHAR_ID_WIRED_REGEX = "\\b[\\d]{1}[A-Z]{1}[0-9]{2}";
	
	public enum Language {A, //printers using "Escape P" or similar command set
						  B, //not assigned yet
						  C, //printers using "CPCL" command language
						  D, //cognitive printers
						  E, //printers using "EPL" command language
						  F, //not assigned yet
						  G, //not assigned yet
						  H, //not assigned yet
						  I, //only to be used for testing purpose
						  J, //not assigned yet
						  K, //not assigned yet
						  L, //not assigned yet
						  M, //printers using "MPCL" command language
						  N, //O'Neil printers
						  O, //not uses as O can be confused for 0
						  P, //Printronix printers
						  Q, //not assigned yet
						  R, //printers using "RCL" command language
						  S, //Sato printers
						  T, //printers using "TEC" command language
						  U, //user defined printer choice
						  V, //not assigned yet
						  W, //not assigned yet
						  X, //Extech printers
						  Y, //Symbol printers
						  Z, //printers using "ZPL" command language
	}				
	
	public enum Transport {Bluetooth, Wlan}
	
	/*
	 * constructor is private as this class is a singleton 
	 */
	public ApdConfiguration() 
	{
		mBtMac = null;
		mIpAddress = null;
		mIpPort = 0;
		mLanguage = null;
		mModel = 0;
		mId = 0;
		mMessage = 0;
		mRfComm = 0;
		mFormatPath = null;
		mTemplatePath = null;
		mLabelIdPath = null;
	}
	
	/**
	 * @return the rfComm
	 */
	public int getRfComm() {
		return mRfComm;
	}
	/**
	 * @param rfComm the rfComm to set
	 */
	public void setRfComm(int rfComm) {
		this.mRfComm = rfComm;
	}
	/**
	 * @return the btMac
	 */
	public String getBtMac() {
		return mBtMac;
	}
	/**
	 * @param btMac the btMac to set
	 */
	public void setBtMac(String btMac) {
		this.mBtMac = btMac;
	}
	/**
	 * @return the ipAddress
	 */
	public String getIpAddress() {
		return mIpAddress;
	}
	/**
	 * @param ipAddress the ipAddress to set
	 */
	public void setIpAddress(String ipAddress) {
		this.mIpAddress = ipAddress;
	}
	/**
	 * @return the ipPort
	 */
	public int getIpPort() {
		return mIpPort;
	}
	/**
	 * @param ipPort the ipPort to set
	 */
	public void setIpPort(int ipPort) {
		this.mIpPort = ipPort;
	}
	/**
	 * @return the language
	 */
	public Language getLanguage() {
		return mLanguage;
	}
	/**
	 * @param language the language to set
	 */
	public void setLanguage(Language language) {
		this.mLanguage = language;
	}
	/**
	 * @return the model
	 */
	public int getModel() {
		return mModel;
	}
	/**
	 * @param model the model to set
	 */
	public void setModel(int model) {
		this.mModel = model;
	}
	/**
	 * @return the id
	 */
	public int getId() {
		return mId;
	}
	/**
	 * @param id the id to set
	 */
	public void setId(int id) {
		this.mId = id;
	}
	/**
	 * @return the message
	 */
	public int getMessage() {
		return mMessage;
	}
	/**
	 * @param message the message to set
	 */
	public void setMessage(int message) {
		this.mMessage = message;
	}
	
	/**
	 * @return the mFormatPath
	 */
	public String getFormatPath() {
		return mFormatPath;
	}

	/**
	 * @param formatPath the formatPath to set
	 */
	public void setFormatPath(String formatPath) {
		mFormatPath = formatPath;
	}

	/**
	 * @return the mTemplatePath
	 */
	public String getTemplatePath() {
		return mTemplatePath;
	}

	/**
	 * @param templatePath the template path to set
	 */
	public void setTemplatePath(String templatePath) {
		mTemplatePath = templatePath;
	}

	/**
	 * @return the mLabelIdPath
	 */
	public String getLabelIdPath() {
		return mLabelIdPath;
	}

	/**
	 * @param labelIdPath the label id path to set
	 */
	public void setLabelIdPath(String labelIdPath) {
		mLabelIdPath = labelIdPath;
	}

	/**
	 * @return the mTransport
	 */
	public Transport getTransport() {
		return mTransport;
	}

	/**
	 * @param mTransport the mTransport to set
	 */
	public void setTransport(Transport transport) {
		mTransport = transport;
	}
}
