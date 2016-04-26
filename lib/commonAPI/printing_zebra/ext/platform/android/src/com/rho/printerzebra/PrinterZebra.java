package com.rho.printerzebra;

import java.io.FileDescriptor;
import java.util.List;
import java.util.Map;

import com.rho.printer.IPrinterSingleton;
import com.rho.printer.RhoPrinterResponceBase;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;

import com.rhomobile.printing.zebra.api.IZebraPrinter;
import com.rhomobile.printing.zebra.api.IZebraPrintingProvider;
import com.rhomobile.printing.zebra.api.ZebraPrintingProviderSingletone;

public class PrinterZebra extends PrinterZebraBase implements IPrinterZebra {

	private IZebraPrinter mPrinter = null;
	private String mPrinterID = null;
	private final String PDF_FILE_EXT = "pdf";
	private final String BMP_FILE_EXT = "bmp";
	private MethodResult mEmptyMethodResult = new MethodResult(false);
	
    public PrinterZebra(String id) {
        super(id);
        mPrinterID = id;
        mPrinter = (IZebraPrinter)ZebraPrintingProviderSingletone.getInstance().getPrinterByID(id);
        //Uncommenting this because PROPERTY_ID returns empty string
        setProperty(IPrinterSingleton.PROPERTY_ID, mPrinter.getID(), mEmptyMethodResult);
        adjustProperties();
    }

    private void adjustProperties() {
    	setProperty(IPrinterSingleton.PROPERTY_CONNECTION_TYPE, mPrinter.getConnectionType(), mEmptyMethodResult);
    	setProperty(IPrinterSingleton.PROPERTY_DEVICE_ADDRESS, mPrinter.getAddress(), mEmptyMethodResult);
    	setProperty(IPrinterSingleton.PROPERTY_DEVICE_PORT, mPrinter.getPort(), mEmptyMethodResult);
    	
    	String friendlyName = mPrinter.getFrienldyName();
    	if(friendlyName != null) {
    		friendlyName = friendlyName.toLowerCase();
    		if(friendlyName.contains("zebra")) {
    			setProperty(IPrinterSingleton.PROPERTY_PRINTER_TYPE, IPrinterSingleton.PRINTER_TYPE_ZEBRA, mEmptyMethodResult);
    		} else if(friendlyName.contains("apd")) {
    			setProperty(IPrinterSingleton.PROPERTY_PRINTER_TYPE, IPrinterSingleton.PRINTER_TYPE_APD, mEmptyMethodResult);
    		} else if(friendlyName.contains("epson")) {
    			setProperty(IPrinterSingleton.PROPERTY_PRINTER_TYPE, IPrinterSingleton.PRINTER_TYPE_EPSON, mEmptyMethodResult);
    		} else if(friendlyName.contains("native")) {
    			setProperty(IPrinterSingleton.PROPERTY_PRINTER_TYPE, IPrinterSingleton.PRINTER_TYPE_NATIVE, mEmptyMethodResult);
    		} else {
    			setProperty(IPrinterSingleton.PROPERTY_PRINTER_TYPE, IPrinterSingleton.PRINTER_TYPE_ANY, mEmptyMethodResult);
    		}
    	} else {
    		setProperty(IPrinterSingleton.PROPERTY_PRINTER_TYPE, IPrinterSingleton.PRINTER_TYPE_ANY, mEmptyMethodResult);
    	}
    	
    	//setProperty(IPrinterSingleton.PROPERTY_PRINTER_TYPE, IPrinterSingleton.PRINTER_TYPE_ZEBRA, mEmptyMethodResult);
    	//setProperty(IPrinterSingleton.PROPERTY_ID, mPrinterID, mEmptyMethodResult);
    	//setProperty(IPrinterZebraSingleton.PROPERTY_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS, "10", mEmptyMethodResult);
    	//setProperty(IPrinterZebraSingleton.PROPERTY_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS, "200", mEmptyMethodResult);
    	setProperty(IPrinterZebraSingleton.PROPERTY_DEVICE_NAME, mPrinter.getFrienldyName(), mEmptyMethodResult);
    	setProperty(IPrinterZebraSingleton.PROPERTY_IS_CONNECTED, String.valueOf(mPrinter.isConnected()), mEmptyMethodResult);
    }
    
    private boolean prepareFile(String path) {
    	FileDescriptor fd = RhoFileApi.openFd(path);
    	return fd != null;
    }
    
    public void getIsConnected(IMethodResult result) {
    	setProperty(IPrinterZebraSingleton.PROPERTY_IS_CONNECTED, String.valueOf(mPrinter.isConnected()), mEmptyMethodResult);
        result.set(mPrinter.isConnected()); 
    }
    
    public void getProperty(String propertyName, IMethodResult result) { 
    	if (IPrinterZebraSingleton.PROPERTY_DEVICE_NAME.equals(propertyName)) {
    		//String dn = mPrinter.getFrienldyName();
    		//setProperty(IPrinterZebraSingleton.PROPERTY_DEVICE_NAME, dn, mEmptyMethodResult);
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
    	printImageFromFile(fileURI, 0, 0, options, result);
    	
    }
    
    public void printRawString(String command, Map<String, Object> options, IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.printRawString(command, options, new RhoPrinterResponceBase(result));
    	}   	
    }
    
    public void retrieveFileNames(IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.retrieveFileNames(new RhoPrinterResponceBase(result));
    	}       	
    }
    
    public void retrieveFileNamesWithExtensions(List<String> extensions, IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.retrieveFileNamesWithExtensions(extensions, new RhoPrinterResponceBase(result));
    	}       	
    }
    
    public void sendFileContents(String file, IMethodResult result) {
    	if (!prepareFile(file) ) {
    		result.set(IPrinterSingleton.PRINTER_STATUS_ERROR);
    		return;
    	}
    	if (mPrinter != null) {
    		mPrinter.sendFileContents(file, new RhoPrinterResponceBase(result));
    	} 
    }
    
    public void printStoredFormatWithHash(String formatPathOnPrinter, Map<String, String> vars, IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.printStoredFormatWithHash(formatPathOnPrinter, vars, new RhoPrinterResponceBase(result));
    	}   	
    }
    
    public void printStoredFormatWithArray(String formatPathOnPrinter, List<String> vars, IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.printStoredFormatWithArray(formatPathOnPrinter, vars, new RhoPrinterResponceBase(result));
    	}   	
    }
    
    public void printImageFromFile(String path, int x, int y, Map<String, Object> options, IMethodResult result) {
    	if (!prepareFile(path) ) {
    		result.set(IPrinterSingleton.PRINTER_STATUS_ERROR);
    		return;
    	}
    	if (mPrinter != null) {
    		mPrinter.printImageFromFile(path, x, y, options, new RhoPrinterResponceBase(result));
    	}   	
    }
    
    public void storeImage(String printerDriveAndFileName, String imageFilePathOnDevice, int width, int height, IMethodResult result) {
    	if (!prepareFile(imageFilePathOnDevice) ) {
    		result.set(IPrinterSingleton.PRINTER_STATUS_ERROR);
    		return;
    	}
    	if (mPrinter != null) {
    		mPrinter.storeImage(printerDriveAndFileName, imageFilePathOnDevice, width, height, new RhoPrinterResponceBase(result));
    	}
    }

    public void setPrinterEventCallback(IMethodResult result) {
    	
    }
    
    public void requestState(List<String> listOfParameters, IMethodResult result) {
    	if (mPrinter != null) {
    		mPrinter.requestState(listOfParameters, new RhoPrinterResponceBase(result));
    	}    	
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

    
}
