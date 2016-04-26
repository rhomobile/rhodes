package com.rhomobile.printing.api;

import java.util.List;
import java.util.Map;

import com.rho.printer.IPrinterSingleton;


public interface IPrinterDevice  {

	
/*
    static final String CONNECTION_TYPE_ANY = "CONNECTION_TYPE_ANY";
    static final String CONNECTION_TYPE_BLUETOOTH = "CONNECTION_TYPE_BLUETOOTH";
    static final String CONNECTION_TYPE_ON_BOARD = "CONNECTION_TYPE_ON_BOARD";
    static final String CONNECTION_TYPE_TCP = "CONNECTION_TYPE_TCP";
    static final String PRINTER_LANGUAGE_CPCL = "PRINTER_LANGUAGE_CPCL";
    static final String PRINTER_LANGUAGE_EPS = "PRINTER_LANGUAGE_EPS";
    static final String PRINTER_LANGUAGE_ZPL = "PRINTER_LANGUAGE_ZPL";
    static final String PRINTER_STATE_IS_BATTERY_LOW = "PRINTER_STATE_IS_BATTERY_LOW";
    static final String PRINTER_STATE_IS_COVER_OPENED = "PRINTER_STATE_IS_COVER_OPENED";
    static final String PRINTER_STATE_IS_DRAWER_OPENED = "PRINTER_STATE_IS_DRAWER_OPENED";
    static final String PRINTER_STATE_IS_PAPER_OUT = "PRINTER_STATE_IS_PAPER_OUT";
    static final String PRINTER_STATE_IS_READY_TO_PRINT = "PRINTER_STATE_IS_READY_TO_PRINT";
    static final String PRINTER_TYPE_ANY = "PRINTER_TYPE_ANY";
    static final String PRINTER_TYPE_APD = "PRINTER_TYPE_APD";
    static final String PRINTER_TYPE_EPSON = "PRINTER_TYPE_EPSON";
    static final String PRINTER_TYPE_NATIVE = "PRINTER_TYPE_NATIVE";
    static final String PRINTER_TYPE_ZEBRA = "PRINTER_TYPE_ZEBRA";
    static final String PRINTER_STATUS_ERROR = "PRINTER_STATUS_ERROR";
    static final String PRINTER_STATUS_ERR_IO = "PRINTER_STATUS_ERR_IO";
    static final String PRINTER_STATUS_ERR_MEMORY = "PRINTER_STATUS_ERR_MEMORY";
    static final String PRINTER_STATUS_ERR_NETWORK = "PRINTER_STATUS_ERR_NETWORK";
    static final String PRINTER_STATUS_ERR_NOT_CONNECTED = "PRINTER_STATUS_ERR_NOT_CONNECTED";
    static final String PRINTER_STATUS_ERR_NOT_FOUND = "PRINTER_STATUS_ERR_NOT_FOUND";
    static final String PRINTER_STATUS_ERR_PARAM = "PRINTER_STATUS_ERR_PARAM";
    static final String PRINTER_STATUS_ERR_PROCESSING = "PRINTER_STATUS_ERR_PROCESSING";
    static final String PRINTER_STATUS_ERR_RESPONSE = "PRINTER_STATUS_ERR_RESPONSE";
    static final String PRINTER_STATUS_ERR_TIMEOUT = "PRINTER_STATUS_ERR_TIMEOUT";
    static final String PRINTER_STATUS_ERR_UNSUPPORTED = "PRINTER_STATUS_ERR_UNSUPPORTED";
    static final String PRINTER_STATUS_SUCCESS = "PRINTER_STATUS_SUCCESS";

    static final String HK_ADDITIONAL_STRING_OPTION = "additionalStringOption"; 
    static final String HK_CONNECTION_TYPE = "connectionType"; 
    static final String HK_DEVICE_ADDRESS = "deviceAddress"; 
    static final String HK_DEVICE_PORT = "devicePort"; 
    static final String HK_MESSAGE = "message"; 
    static final String HK_PRINTER_ID = "printerID"; 
    static final String HK_PRINTER_TYPE = "printerType"; 
    static final String HK_PROPERTY_WITH_INT_VALUE = "propertyWithIntValue"; 
    static final String HK_PRPERTY_WITH_STRING_VALUE = "prpertyWithStringValue"; 
    static final String HK_STATUS = "status"; 
    static final String HK_TIMEOUT = "timeout"; 
    static final String PROPERTY_CONNECTION_TYPE = "connectionType"; 
    static final String PROPERTY_CONTROL_LANGUAGE = "controlLanguage"; 
    static final String PROPERTY_DEVICE_ADDRESS = "deviceAddress"; 
    static final String PROPERTY_DEVICE_NAME = "deviceName"; 
    static final String PROPERTY_DEVICE_PORT = "devicePort"; 
    static final String PROPERTY_ID = "ID"; 
    static final String PROPERTY_IS_CONNECTED = "isConnected"; 
    static final String PROPERTY_PRINTER_EVENT_CALLBACK = "printerEventCallback"; 
    static final String PROPERTY_PRINTER_TYPE = "printerType"; 	
*/
	
    static final String CONNECTION_TYPE_ANY = IPrinterSingleton.CONNECTION_TYPE_ANY;
    static final String CONNECTION_TYPE_BLUETOOTH = IPrinterSingleton.CONNECTION_TYPE_BLUETOOTH;
    static final String CONNECTION_TYPE_ON_BOARD = IPrinterSingleton.CONNECTION_TYPE_ON_BOARD;
    static final String CONNECTION_TYPE_TCP = IPrinterSingleton.CONNECTION_TYPE_TCP;
    static final String CONNECTION_TYPE_USB = IPrinterSingleton.CONNECTION_TYPE_USB;
    static final String PRINTER_LANGUAGE_CPCL = IPrinterSingleton.PRINTER_LANGUAGE_CPCL;
    static final String PRINTER_LANGUAGE_EPS = IPrinterSingleton.PRINTER_LANGUAGE_EPS;
    static final String PRINTER_LANGUAGE_ZPL = IPrinterSingleton.PRINTER_LANGUAGE_ZPL;
    static final String PRINTER_STATE_IS_BATTERY_LOW = IPrinterSingleton.PRINTER_STATE_IS_BATTERY_LOW;
    static final String PRINTER_STATE_IS_COVER_OPENED = IPrinterSingleton.PRINTER_STATE_IS_COVER_OPENED;
    static final String PRINTER_STATE_IS_DRAWER_OPENED = IPrinterSingleton.PRINTER_STATE_IS_DRAWER_OPENED;
    static final String PRINTER_STATE_IS_PAPER_OUT = IPrinterSingleton.PRINTER_STATE_IS_PAPER_OUT;
    static final String PRINTER_STATE_IS_READY_TO_PRINT = IPrinterSingleton.PRINTER_STATE_IS_READY_TO_PRINT;
    static final String PRINTER_TYPE_ANY = IPrinterSingleton.PRINTER_TYPE_ANY;
    static final String PRINTER_TYPE_APD = IPrinterSingleton.PRINTER_TYPE_APD;
    static final String PRINTER_TYPE_EPSON = IPrinterSingleton.PRINTER_TYPE_EPSON;
    static final String PRINTER_TYPE_NATIVE = IPrinterSingleton.PRINTER_TYPE_NATIVE;
    static final String PRINTER_TYPE_ZEBRA = IPrinterSingleton.PRINTER_TYPE_ZEBRA;
    static final String PRINTER_STATUS_ERROR = IPrinterSingleton.PRINTER_STATUS_ERROR;
    static final String PRINTER_STATUS_ERR_IO = IPrinterSingleton.PRINTER_STATUS_ERR_IO;
    static final String PRINTER_STATUS_ERR_MEMORY = IPrinterSingleton.PRINTER_STATUS_ERR_MEMORY;
    static final String PRINTER_STATUS_ERR_NETWORK = IPrinterSingleton.PRINTER_STATUS_ERR_NETWORK;
    static final String PRINTER_STATUS_ERR_NOT_CONNECTED = IPrinterSingleton.PRINTER_STATUS_ERR_NOT_CONNECTED;
    static final String PRINTER_STATUS_ERR_NOT_FOUND = IPrinterSingleton.PRINTER_STATUS_ERR_NOT_FOUND;
    static final String PRINTER_STATUS_ERR_PARAM = IPrinterSingleton.PRINTER_STATUS_ERR_PARAM;
    static final String PRINTER_STATUS_ERR_PROCESSING = IPrinterSingleton.PRINTER_STATUS_ERR_PROCESSING;
    static final String PRINTER_STATUS_ERR_RESPONSE = IPrinterSingleton.PRINTER_STATUS_ERR_RESPONSE;
    static final String PRINTER_STATUS_ERR_TIMEOUT = IPrinterSingleton.PRINTER_STATUS_ERR_TIMEOUT;
    static final String PRINTER_STATUS_ERR_UNSUPPORTED = IPrinterSingleton.PRINTER_STATUS_ERR_UNSUPPORTED;
    static final String PRINTER_STATUS_SUCCESS = IPrinterSingleton.PRINTER_STATUS_SUCCESS;

    static final String HK_ADDITIONAL_STRING_OPTION = IPrinterSingleton.HK_ADDITIONAL_STRING_OPTION; 
    static final String HK_STRING_RESPONCE = IPrinterSingleton.HK_STRING_RESPONCE; 
    static final String HK_BYTE_ARRAY_RESPONCE = IPrinterSingleton.HK_BYTE_ARRAY_RESPONCE; 
    static final String HK_CONNECTION_TYPE = IPrinterSingleton.HK_CONNECTION_TYPE; 
    static final String HK_DEVICE_ADDRESS = IPrinterSingleton.HK_DEVICE_ADDRESS; 
    static final String HK_DEVICE_PORT = IPrinterSingleton.HK_DEVICE_PORT; 
    static final String HK_MESSAGE = IPrinterSingleton.HK_MESSAGE; 
    static final String HK_PRINTER_ID = IPrinterSingleton.HK_PRINTER_ID; 
    static final String HK_PRINTER_TYPE = IPrinterSingleton.HK_PRINTER_TYPE; 
    static final String HK_PROPERTY_WITH_INT_VALUE = IPrinterSingleton.HK_PROPERTY_WITH_INT_VALUE; 
    //static final String HK_PRPERTY_WITH_STRING_VALUE = IPrinterSingleton.HK_PRPERTY_WITH_STRING_VALUE; 
    static final String HK_STATUS = IPrinterSingleton.HK_STATUS; 
    static final String HK_TIMEOUT = IPrinterSingleton.HK_TIMEOUT; 
    static final String PROPERTY_CONNECTION_TYPE = IPrinterSingleton.PROPERTY_CONNECTION_TYPE; 
    static final String PROPERTY_DEVICE_ADDRESS = IPrinterSingleton.PROPERTY_DEVICE_ADDRESS; 
    static final String PROPERTY_DEVICE_NAME = IPrinterSingleton.PROPERTY_DEVICE_NAME; 
    static final String PROPERTY_DEVICE_PORT = IPrinterSingleton.PROPERTY_DEVICE_PORT; 
    static final String PROPERTY_ID = IPrinterSingleton.PROPERTY_ID; 
    static final String PROPERTY_IS_CONNECTED = IPrinterSingleton.PROPERTY_IS_CONNECTED; 
    static final String PROPERTY_PRINTER_TYPE = IPrinterSingleton.PROPERTY_PRINTER_TYPE; 
	
	
public interface IPrinterResponce {
	public void onError(String message);
	public void onSuccess();
	public void onReturnHash(Map<String,Object> result);
	public void onErrorHash(String message);
	public void onReturnStatus(String status);
}

public class PrinterResponceBaseImpl implements IPrinterResponce {
	
	private Object mStoredObj = null;
	
	public PrinterResponceBaseImpl() {
		
	}
	
	public PrinterResponceBaseImpl(Object obj) {
		mStoredObj = obj;
	}
	
	public Object getStoredObject() {
		return mStoredObj;
	}
	
	public void onError(String message) {
		
	}

	public void onErrorHash(String message) {
		
	}
	
	
	public void onSuccess() {
		
	}
	
	public void onReturnHash(Map<String,Object> result) {
		
	}
	
	public void onReturnStatus(String status) {
		
	}
}

public String getID();

public void connect(IPrinterResponce listener);

public void connectWithOptions(Map<String, Object> options, IPrinterResponce listener);

public void disconnect(IPrinterResponce listener);

public void printImageFromFile(String path, int x, int y, Map<String, Object> options, IPrinterResponce result);

public void printRawString(String command, Map<String,Object> options, IPrinterResponce listener);

public String getAddress();

public String getFrienldyName();

public String getPort();

public String getControlLanguage();

public String getConnectionType();

public boolean isConnected();

public List<Object> enumerateSupportedControlLanguages();

public void requestState(List<String> listOfParameters, IPrinterResponce result);
	

}
