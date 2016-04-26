package com.rho.printer;

import java.io.FileDescriptor;
import java.util.List;
import java.util.Map;
import java.io.File;


import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.printing.api.IPrinterDevice;
import com.rhomobile.printing.api.PrintingManagerSingletone;


public class Printer extends PrinterBase implements IPrinter {
    
	private IPrinterDevice mPrinter = null;
	private String mPrinterID = null;
	private final String PDF_FILE_EXT = "pdf";
	private final String BMP_FILE_EXT = "bmp";
	private MethodResult mEmptyMethodResult = null;

	
    public Printer(String id) {
        super(id);
    	mEmptyMethodResult = new MethodResult(false);
        mPrinterID = id;
        mPrinter = PrintingManagerSingletone.getInstance().getPrinterByID(mPrinterID);
        adjustProperties();
    }

    
    public void disconnect(IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.disconnect(new RhoPrinterResponceBase(result));
    	}
    }
    
    public void printFile(String fileURI, Map<String, Object> options, IMethodResult result) {
    	//Added this condition to display proper error for unsupported files
    	if(fileURI.lastIndexOf(".") != -1 && fileURI.lastIndexOf(".") != 0) {
    		String temp = fileURI.substring(fileURI.lastIndexOf(".")+1);
    		if(temp.equals(PDF_FILE_EXT) || temp.equals(BMP_FILE_EXT)) {
    			result.set(IPrinterSingleton.PRINTER_STATUS_ERR_UNSUPPORTED);
        		return;
    		}
    	}
    	if (!prepareFile(fileURI) ) {
    		result.set(IPrinterSingleton.PRINTER_STATUS_ERROR);
    		return;
    	}
    	// UNSUPPORTED !!!
    	printImageFromFile(fileURI,  0, 0, options, result);
    }
    
    public void printRawString(String command, Map<String, Object> options, IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.printRawString(command, options, new RhoPrinterResponceBase(result));
    	}
    }

    public void setPrinterEventCallback(IMethodResult result) {
    	
    }
    
    public void requestState(List<String> listOfParameters, IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.requestState(listOfParameters, new RhoPrinterResponceBase(result));
    	}       	
    }
    
    private void adjustProperties() {
    	setProperty(IPrinterSingleton.PROPERTY_CONNECTION_TYPE, mPrinter.getConnectionType(), mEmptyMethodResult);
    	setProperty(IPrinterSingleton.PROPERTY_DEVICE_ADDRESS, mPrinter.getAddress(), mEmptyMethodResult);
    	setProperty(IPrinterSingleton.PROPERTY_DEVICE_PORT, mPrinter.getPort(), mEmptyMethodResult);
    	//TODO: replace Zebra to get from manager
    	setProperty(IPrinterSingleton.PROPERTY_PRINTER_TYPE, IPrinterSingleton.PRINTER_TYPE_ZEBRA, mEmptyMethodResult);
    	//Uncommenting this because PROPERTY_ID returns empty string
    	setProperty(IPrinterSingleton.PROPERTY_ID, mPrinterID, mEmptyMethodResult);
    	setProperty(IPrinterSingleton.PROPERTY_DEVICE_NAME, mPrinter.getFrienldyName(), mEmptyMethodResult);
    	setProperty(IPrinterSingleton.PROPERTY_IS_CONNECTED, String.valueOf(mPrinter.isConnected()), mEmptyMethodResult);
    }
    
    private boolean prepareFile(String path) {
    	FileDescriptor fd = RhoFileApi.openFd(path);
    	return fd != null;
    }  
    
    public void getIsConnected(IMethodResult result) {
    	setProperty(IPrinterSingleton.PROPERTY_IS_CONNECTED, String.valueOf(mPrinter.isConnected()), mEmptyMethodResult);
        result.set(mPrinter.isConnected()); 
    }
    
    public void getProperty(String propertyName, IMethodResult result) { 
    	if (IPrinterSingleton.PROPERTY_DEVICE_NAME.equals(propertyName)) {
    		//String dn = mPrinter.getFrienldyName();
    		//setProperty(IPrinterSingleton.PROPERTY_DEVICE_NAME, dn, mEmptyMethodResult);
    		//result.set(dn);
    	}
    	if (IPrinterSingleton.PROPERTY_IS_CONNECTED.equals(propertyName)) {
    		if (mPrinter.isConnected()) {
    			result.set("true");
    			return;
    		}
    		else {
    			result.set("false");
    			return;
    		}
    	}
    	super.getProperty(propertyName, result);
    }

	@Override
	public void enumerateSupportedControlLanguages(IMethodResult result) {
    	if (mPrinter != null) {
    		result.set(mPrinter.enumerateSupportedControlLanguages());
    	}    
    }

	@Override
	public void connect(IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.connect(new RhoPrinterResponceBase(result));
    	}		
	}

	@Override
	public void connectWithOptions(Map<String, Object> options,
			IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.connectWithOptions(options, new RhoPrinterResponceBase(result));
    	}		
		
	}

	@Override
	public void printImageFromFile(String path, int x, int y,
			Map<String, Object> options, IMethodResult result) {
    	if (!prepareFile(path) ) {
    		result.set(IPrinterSingleton.PRINTER_STATUS_ERROR);
    		return;
    	}
    	if (mPrinter != null) {
    		mPrinter.printImageFromFile(path, x, y, options, new RhoPrinterResponceBase(result));
    	}  
    }     
    
    
}
