package com.rhomobile.printing.zebra.api;

import java.util.List;
import java.util.Map;

import com.rho.printerzebra.IPrinterZebraSingleton;
import com.rhomobile.printing.api.IPrinterDevice;

public interface IZebraPrinter extends IPrinterDevice {

    static final String CONNECTION_TYPE_ANY = IPrinterZebraSingleton.CONNECTION_TYPE_ANY;
    static final String CONNECTION_TYPE_BLUETOOTH = IPrinterZebraSingleton.CONNECTION_TYPE_BLUETOOTH;
    static final String CONNECTION_TYPE_ON_BOARD = IPrinterZebraSingleton.CONNECTION_TYPE_ON_BOARD;
    static final String CONNECTION_TYPE_TCP = IPrinterZebraSingleton.CONNECTION_TYPE_TCP;
    static final String CONNECTION_TYPE_USB = IPrinterZebraSingleton.CONNECTION_TYPE_USB;
    static final String DISCOVERY_MODE_DIRECTED_BROADCAST = IPrinterZebraSingleton.DISCOVERY_MODE_DIRECTED_BROADCAST;
    static final String DISCOVERY_MODE_LOCAL_BROADCAST = IPrinterZebraSingleton.DISCOVERY_MODE_LOCAL_BROADCAST;
    static final String DISCOVERY_MODE_MULTICAST = IPrinterZebraSingleton.DISCOVERY_MODE_MULTICAST;
    static final String DISCOVERY_MODE_SUBNET_SEARCH = IPrinterZebraSingleton.DISCOVERY_MODE_SUBNET_SEARCH;
    static final String ERROR_BAD_FILE_DIRECTORY_ENTRY = IPrinterZebraSingleton.ERROR_BAD_FILE_DIRECTORY_ENTRY;
    static final String ERROR_INVALID_DISCOVERY_HOP_COUNT = IPrinterZebraSingleton.ERROR_INVALID_DISCOVERY_HOP_COUNT;
    static final String ERROR_INVALID_FILE_NAME = IPrinterZebraSingleton.ERROR_INVALID_FILE_NAME;
    static final String ERROR_INVALID_FORMAT_NAME = IPrinterZebraSingleton.ERROR_INVALID_FORMAT_NAME;
    static final String ERROR_INVALID_PRINTER_DRIVE_LETTER = IPrinterZebraSingleton.ERROR_INVALID_PRINTER_DRIVE_LETTER;
    static final String ERROR_INVALID_PRINTER_LANGUAGE = IPrinterZebraSingleton.ERROR_INVALID_PRINTER_LANGUAGE;
    static final String ERROR_MALFORMED_FORMAT_FIELD_NUMBER = IPrinterZebraSingleton.ERROR_MALFORMED_FORMAT_FIELD_NUMBER;
    static final String ERROR_MALFORMED_NETWORK_DISCOVERY_ADDRESS = IPrinterZebraSingleton.ERROR_MALFORMED_NETWORK_DISCOVERY_ADDRESS;
    static final String ERROR_MALFORMED_PRINTER_STATUS_RESPONSE = IPrinterZebraSingleton.ERROR_MALFORMED_PRINTER_STATUS_RESPONSE;
    static final String ERROR_NETWORK_ERROR_DURING_DISCOVERY = IPrinterZebraSingleton.ERROR_NETWORK_ERROR_DURING_DISCOVERY;
    static final String ERROR_NO_CONNECTION = IPrinterZebraSingleton.ERROR_NO_CONNECTION;
    static final String ERROR_READ_FAILURE = IPrinterZebraSingleton.ERROR_READ_FAILURE;
    static final String ERROR_UNKNOWN_PRINTER_LANGUAGE = IPrinterZebraSingleton.ERROR_UNKNOWN_PRINTER_LANGUAGE;
    static final String ERROR_WRITE_FAILURE = IPrinterZebraSingleton.ERROR_WRITE_FAILURE;
    static final String PRINTER_LANGUAGE_CPCL = IPrinterZebraSingleton.PRINTER_LANGUAGE_CPCL;
    static final String PRINTER_LANGUAGE_EPS = IPrinterZebraSingleton.PRINTER_LANGUAGE_EPS;
    static final String PRINTER_LANGUAGE_ZPL = IPrinterZebraSingleton.PRINTER_LANGUAGE_ZPL;
    static final String PRINTER_STATE_IS_BATTERY_LOW = IPrinterZebraSingleton.PRINTER_STATE_IS_BATTERY_LOW;
    static final String PRINTER_STATE_IS_COVER_OPENED = IPrinterZebraSingleton.PRINTER_STATE_IS_COVER_OPENED;
    static final String PRINTER_STATE_IS_DRAWER_OPENED = IPrinterZebraSingleton.PRINTER_STATE_IS_DRAWER_OPENED;
    static final String PRINTER_STATE_IS_HEAD_COLD = IPrinterZebraSingleton.PRINTER_STATE_IS_HEAD_COLD;
    static final String PRINTER_STATE_IS_HEAD_OPEN = IPrinterZebraSingleton.PRINTER_STATE_IS_HEAD_OPEN;
    static final String PRINTER_STATE_IS_HEAD_TOO_HOT = IPrinterZebraSingleton.PRINTER_STATE_IS_HEAD_TOO_HOT;
    static final String PRINTER_STATE_IS_PAPER_OUT = IPrinterZebraSingleton.PRINTER_STATE_IS_PAPER_OUT;
    static final String PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS = IPrinterZebraSingleton.PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS;
    static final String PRINTER_STATE_IS_PAUSED = IPrinterZebraSingleton.PRINTER_STATE_IS_PAUSED;
    static final String PRINTER_STATE_IS_READY_TO_PRINT = IPrinterZebraSingleton.PRINTER_STATE_IS_READY_TO_PRINT;
    static final String PRINTER_STATE_IS_RECEIVE_BUFFER_FULL = IPrinterZebraSingleton.PRINTER_STATE_IS_RECEIVE_BUFFER_FULL;
    static final String PRINTER_STATE_IS_RIBBON_OUT = IPrinterZebraSingleton.PRINTER_STATE_IS_RIBBON_OUT;
    static final String PRINTER_STATE_LABELS_REMAINING_IN_BATCH = IPrinterZebraSingleton.PRINTER_STATE_LABELS_REMAINING_IN_BATCH;
    static final String PRINTER_STATE_LABEL_LENGTH_IN_DOTS = IPrinterZebraSingleton.PRINTER_STATE_LABEL_LENGTH_IN_DOTS;
    static final String PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER = IPrinterZebraSingleton.PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER;
    static final String PRINTER_STATE_PRINT_MODE = IPrinterZebraSingleton.PRINTER_STATE_PRINT_MODE;
    static final String PRINTER_STATUS_ERROR = IPrinterZebraSingleton.PRINTER_STATUS_ERROR;
    static final String PRINTER_STATUS_ERR_IO = IPrinterZebraSingleton.PRINTER_STATUS_ERR_IO;
    static final String PRINTER_STATUS_ERR_MEMORY = IPrinterZebraSingleton.PRINTER_STATUS_ERR_MEMORY;
    static final String PRINTER_STATUS_ERR_NETWORK = IPrinterZebraSingleton.PRINTER_STATUS_ERR_NETWORK;
    static final String PRINTER_STATUS_ERR_NOT_CONNECTED = IPrinterZebraSingleton.PRINTER_STATUS_ERR_NOT_CONNECTED;
    static final String PRINTER_STATUS_ERR_NOT_FOUND = IPrinterZebraSingleton.PRINTER_STATUS_ERR_NOT_FOUND;
    static final String PRINTER_STATUS_ERR_PARAM = IPrinterZebraSingleton.PRINTER_STATUS_ERR_PARAM;
    static final String PRINTER_STATUS_ERR_PROCESSING = IPrinterZebraSingleton.PRINTER_STATUS_ERR_PROCESSING;
    static final String PRINTER_STATUS_ERR_RESPONSE = IPrinterZebraSingleton.PRINTER_STATUS_ERR_RESPONSE;
    static final String PRINTER_STATUS_ERR_TIMEOUT = IPrinterZebraSingleton.PRINTER_STATUS_ERR_TIMEOUT;
    static final String PRINTER_STATUS_ERR_UNSUPPORTED = IPrinterZebraSingleton.PRINTER_STATUS_ERR_UNSUPPORTED;
    static final String PRINTER_STATUS_SUCCESS = IPrinterZebraSingleton.PRINTER_STATUS_SUCCESS;
    static final String PRINTER_TYPE_ANY = IPrinterZebraSingleton.PRINTER_TYPE_ANY;
    static final String PRINTER_TYPE_APD = IPrinterZebraSingleton.PRINTER_TYPE_APD;
    static final String PRINTER_TYPE_EPSON = IPrinterZebraSingleton.PRINTER_TYPE_EPSON;
    static final String PRINTER_TYPE_NATIVE = IPrinterZebraSingleton.PRINTER_TYPE_NATIVE;
    static final String PRINTER_TYPE_ZEBRA = IPrinterZebraSingleton.PRINTER_TYPE_ZEBRA;
    static final String PRINT_MODE_APPLICATOR = IPrinterZebraSingleton.PRINT_MODE_APPLICATOR;
    static final String PRINT_MODE_CUTTER = IPrinterZebraSingleton.PRINT_MODE_CUTTER;
    static final String PRINT_MODE_DELAYED_CUT = IPrinterZebraSingleton.PRINT_MODE_DELAYED_CUT;
    static final String PRINT_MODE_KIOSK = IPrinterZebraSingleton.PRINT_MODE_KIOSK;
    static final String PRINT_MODE_LINERLESS_PEEL = IPrinterZebraSingleton.PRINT_MODE_LINERLESS_PEEL;
    static final String PRINT_MODE_LINERLESS_REWIND = IPrinterZebraSingleton.PRINT_MODE_LINERLESS_REWIND;
    static final String PRINT_MODE_PARTIAL_CUTTER = IPrinterZebraSingleton.PRINT_MODE_PARTIAL_CUTTER;
    static final String PRINT_MODE_PEEL_OFF = IPrinterZebraSingleton.PRINT_MODE_PEEL_OFF;
    static final String PRINT_MODE_REWIND = IPrinterZebraSingleton.PRINT_MODE_REWIND;
    static final String PRINT_MODE_RFID = IPrinterZebraSingleton.PRINT_MODE_RFID;
    static final String PRINT_MODE_TEAR_OFF = IPrinterZebraSingleton.PRINT_MODE_TEAR_OFF;
    static final String PRINT_MODE_UNKNOWN = IPrinterZebraSingleton.PRINT_MODE_UNKNOWN;

  // hash keys used in properties and parameters

    //static final String HK_ADDITIONAL_PRINTER_DISCOVERY_PROPERTIES = IPrinterZebraSingleton.HK_ADDITIONAL_PRINTER_DISCOVERY_PROPERTIES; //additionalPrinterDiscoveryProperties; 
    static final String HK_ADDITIONAL_STRING_OPTION = IPrinterZebraSingleton.HK_ADDITIONAL_STRING_OPTION; //additionalStringOption; 
    static final String HK_BYTE_ARRAY_RESPONCE = IPrinterZebraSingleton.HK_BYTE_ARRAY_RESPONCE; //byteArrayResponce; 
    static final String HK_CONNECTION_TYPE = IPrinterZebraSingleton.HK_CONNECTION_TYPE; //connectionType; 
    static final String HK_DEVICE_ADDRESS = IPrinterZebraSingleton.HK_DEVICE_ADDRESS; //deviceAddress; 
    static final String HK_DEVICE_PORT = IPrinterZebraSingleton.HK_DEVICE_PORT; //devicePort; 
    static final String HK_FILE_NAMES = IPrinterZebraSingleton.HK_FILE_NAMES; //fileNames; 
    static final String HK_HEIGHT = IPrinterZebraSingleton.HK_HEIGHT; //height; 
    static final String HK_IS_INSIDE_FORMAT = IPrinterZebraSingleton.HK_IS_INSIDE_FORMAT; //isInsideFormat; 
    static final String HK_MESSAGE = IPrinterZebraSingleton.HK_MESSAGE; //message; 
    static final String HK_PRINTER_ID = IPrinterZebraSingleton.HK_PRINTER_ID; //printerID; 
    static final String HK_PRINTER_TYPE = IPrinterZebraSingleton.HK_PRINTER_TYPE; //printerType; 
    static final String HK_PROPERTY_WITH_INT_VALUE = IPrinterZebraSingleton.HK_PROPERTY_WITH_INT_VALUE; //propertyWithIntValue; 
    //static final String HK_PRPERTY_WITH_STRING_VALUE = IPrinterZebraSingleton.HK_PRPERTY_WITH_STRING_VALUE; //prpertyWithStringValue; 
    static final String HK_STATUS = IPrinterZebraSingleton.HK_STATUS; //status; 
    static final String HK_STRING_RESPONCE = IPrinterZebraSingleton.HK_STRING_RESPONCE; //stringResponce; 
    static final String HK_TIMEOUT = IPrinterZebraSingleton.HK_TIMEOUT; //timeout; 
    static final String HK_WIDTH = IPrinterZebraSingleton.HK_WIDTH; //width; 
    static final String PROPERTY_CONNECTION_TYPE = IPrinterZebraSingleton.PROPERTY_CONNECTION_TYPE; //connectionType; 
    static final String PROPERTY_DEVICE_ADDRESS = IPrinterZebraSingleton.PROPERTY_DEVICE_ADDRESS; //deviceAddress; 
    static final String PROPERTY_DEVICE_NAME = IPrinterZebraSingleton.PROPERTY_DEVICE_NAME; //deviceName; 
    static final String PROPERTY_DEVICE_PORT = IPrinterZebraSingleton.PROPERTY_DEVICE_PORT; //devicePort; 
    static final String PROPERTY_ID = IPrinterZebraSingleton.PROPERTY_ID; //ID; 
    static final String PROPERTY_IS_CONNECTED = IPrinterZebraSingleton.PROPERTY_IS_CONNECTED; //isConnected; 
    //static final String PROPERTY_MAX_TIMEOUT_FOR_OPEN = IPrinterZebraSingleton.PROPERTY_MAX_TIMEOUT_FOR_OPEN; //maxTimeoutForOpen; 
    //static final String PROPERTY_MAX_TIMEOUT_FOR_READ = IPrinterZebraSingleton.PROPERTY_MAX_TIMEOUT_FOR_READ; //maxTimeoutForRead; 
    static final String PROPERTY_PRINTER_TYPE = IPrinterZebraSingleton.PROPERTY_PRINTER_TYPE; //printerType; 
    //static final String PROPERTY_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS = IPrinterZebraSingleton.PROPERTY_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS; //timeToWaitAfterReadInMilliseconds; 
    //static final String PROPERTY_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS = IPrinterZebraSingleton.PROPERTY_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS; //timeToWaitAfterWriteInMilliseconds; 
    //static final String PROPERTY_TIME_TO_WAIT_FOR_MORE_DATA = IPrinterZebraSingleton.PROPERTY_TIME_TO_WAIT_FOR_MORE_DATA; //timeToWaitForMoreData; 	
	
	
	
	
	
	

	public void storeImage(String printerDriveAndFileName, String imageFilePathOnDevice, int width, int height, IPrinterResponce result);
	
	public void retrieveFileNames(IPrinterResponce result);
	
	public void retrieveFileNamesWithExtensions(List<String> extensions, IPrinterResponce result);
	
	
	public void sendFileContents(String file, IPrinterResponce result);
	
	public void printStoredFormatWithHash(String formatPathOnPrinter, Map<String, String> vars, IPrinterResponce result);
	
	public void printStoredFormatWithArray(String formatPathOnPrinter, List<String> vars, IPrinterResponce result);
	
	
}
