package com.rhomobile.printing.zebra.impl;

import java.io.IOException;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;



import android.hardware.usb.UsbConstants;
import android.os.Looper;

import com.rho.printer.IPrinterSingleton;
import com.rhomobile.printing.api.IPrinterDevice;
import com.rhomobile.printing.api.IPrinterDevice.IPrinterResponce;
import com.rhomobile.printing.zebra.api.IZebraPrinter;
import com.rhomobile.rhodes.util.Utils;
import com.zebra.sdk.printer.ZplPrintMode;
import com.zebra.sdk.printer.PrinterLanguage;
import com.zebra.sdk.comm.BluetoothConnection;
import com.zebra.sdk.comm.Connection;
import com.zebra.sdk.comm.ConnectionException;
import com.zebra.sdk.comm.TcpConnection;
import com.zebra.sdk.comm.UsbConnection;
import com.zebra.sdk.device.ZebraIllegalArgumentException;
import com.zebra.sdk.printer.PrinterStatus;
import com.zebra.sdk.printer.SGD;
import com.zebra.sdk.printer.ZebraPrinter;
import com.zebra.sdk.printer.ZebraPrinterFactory;
import com.zebra.sdk.printer.ZebraPrinterLanguageUnknownException;

public class ZebraPrinterImpl implements IZebraPrinter {

	private String mID = null;
    private Connection mPrinterConnection = null;
    private ZebraPrinter mPrinter = null;	
    private String mZebraAddress = null;
    private String mAdress = null;
    private String mFriendlyName = null;
    private List<String> mSupportedLanguages = null;
    private Map<String,String> mDiscoveryMap = null;
    private String mInitialControlLanguage = "unknown";
    
    
    private class PrintSimpleCommand implements Runnable {
    	
    	private IPrinterResponce mCallback = null;
    	
    	public PrintSimpleCommand(IPrinterResponce listener) {
    		mCallback = listener;
    	}
    	
    	public IPrinterResponce getCallback() {
    		return mCallback;
    	}
    	
        public void run() {
        }

    }
    
    
    private class PrintRawStringCommand implements Runnable {
    	
    	private String mText = null;
    	private IPrinterResponce mCallback = null;
    	
    	public PrintRawStringCommand(String text, IPrinterResponce listener) {
    		mText = text;
    		mCallback = listener;
    	}
    	
        public void run() {
        	
    		if (mPrinter == null) {
    			mCallback.onErrorHash("printer not connected!");
    			return;
    		}
    		if (!mPrinterConnection.isConnected()) {
    			mCallback.onErrorHash("printer not connected!");
    			return;
    		}        	
        	
			PrinterStatus printerStatus = null;
			try {
				printerStatus = mPrinter.getCurrentStatus();
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onErrorHash("error with connection!");
    			return;
			}
			
			if (printerStatus.isReadyToPrint) {
				try {
					
					//mPrinter.sendCommand(cc);
				
					byte[] data_send = mText.getBytes();
					byte[] data_received = null;
					
					data_received = mPrinterConnection.sendAndWaitForResponse(data_send, 2000, 2000, null);
					//mPrinterConnection.write(data_send);
					//mCallback.onSuccess();
					
					Hashtable<String, Object> result = new Hashtable<String, Object>();
					result.put(IPrinterDevice.HK_STATUS, IPrinterDevice.PRINTER_STATUS_SUCCESS);

					if (data_received != null) {
						if (data_received.length > 0) {
							String responceString = new String(data_received);
							result.put(IPrinterDevice.HK_STRING_RESPONCE, responceString);
							int i;
							LinkedList<Object> responceByteArray = new LinkedList<Object>();
							for (i = 0; i < data_received.length; i++) {
								responceByteArray.add(new Integer(data_received[i]));
							}
							result.put(IPrinterDevice.HK_BYTE_ARRAY_RESPONCE, responceByteArray);
						}
					}
					mCallback.onReturnHash(result);
					
				} catch (ConnectionException e) {
					e.printStackTrace();
					mCallback.onErrorHash("error with connection!");
				}
			}
			else {
				mCallback.onErrorHash("printer not ready to print!");
			}					
        }

    }

    private class PrintPrinImageFromFileCommand implements Runnable {
    	
    	private String mPath = null;
    	private int mX = 0;
    	private int mY = 0;
    	private int mW = -1;
    	private int mH = -1;
    	private boolean mInsideFormat = false;
    	private IPrinterResponce mCallback = null;
    	
    	public PrintPrinImageFromFileCommand(String path, int x, int y, int w, int h, boolean insideFormat, IPrinterResponce listener) {
    		mPath = path;
    		mX = x;
    		mY = y;
    		mW = w;
    		mH = h;
    		mInsideFormat = insideFormat;
    		mCallback = listener;
    	}
    	
        public void run() {
    		//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@ printImageFromFileCommand !!!");
    		if (mPrinter == null) {
    			mCallback.onError("printer not connected !");
    			return;
    		}
    		if (!mPrinterConnection.isConnected()) {
    			mCallback.onError("printer not connected !!!");
    			return;
    		}        	
        	
			PrinterStatus printerStatus = null;
			try {
				printerStatus = mPrinter.getCurrentStatus();
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onError("error with connection!");
    			return;
			}
			
			if (printerStatus.isReadyToPrint) {
				try {
					
					//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@@@@@@ PRINT IMAGE :"+mPath);
					mPrinter.printImage(mPath, mX, mY, mW, mH, mInsideFormat);
					//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@@@@@@ PRINT IMAGE FINISHED !!!");
					
					mCallback.onSuccess();
					
				} catch (ConnectionException e) {
					e.printStackTrace();
					mCallback.onError("error with connection !!!");
				} catch (IOException e) {
					e.printStackTrace();
					mCallback.onError("error with Input/Output operation!");
				}
			}
			else {
				mCallback.onError("printer not ready to print!");
			}					
        }

    }
    
    private class StoreImageCommand implements Runnable {
    	
    	private String mPath = null;
    	private String mPathInPrinter = null;
    	private int mW = -1;
    	private int mH = -1;
    	private IPrinterResponce mCallback = null;
    	
    	public StoreImageCommand(String path, String pathInPrinter, int w, int h, IPrinterResponce listener) {
    		mPath = path;
    		mPathInPrinter = pathInPrinter;
    		mW = w;
    		mH = h;
    		mCallback = listener;
    	}
    	
        public void run() {
    		//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@ printImageFromFileCommand !!!");
    		if (mPrinter == null) {
    			mCallback.onError("printer not connected !");
    			return;
    		}
    		if (!mPrinterConnection.isConnected()) {
    			mCallback.onError("printer not connected !!!");
    			return;
    		}        	
        	
			//PrinterStatus printerStatus = null;
			//try {
			//	printerStatus = mPrinter.getCurrentStatus();
			//} catch (ConnectionException e) {
			//	e.printStackTrace();
			//	mCallback.onError("error with connection!");
			//}
			
			try {
				
				//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@@@@@@ STORE IMAGE :"+mPath);
				mPrinter.storeImage(mPathInPrinter, mPath, mW, mH);
				//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@@@@@@ STORE IMAGE FINISHED !!!");
				
				mCallback.onSuccess();
				
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onError("error with connection !!!");
			} catch (IOException e) {
				e.printStackTrace();
				mCallback.onError("error with Input/Output operation!");
			} catch (ZebraIllegalArgumentException e) {
				mCallback.onError("invalid arguments!");
				e.printStackTrace();
			}
        }

    }
    
    
    
   private class RequestStatusCommand implements Runnable {
    	
    	private List<String> mListOfParameters;
    	private IPrinterResponce mCallback = null;
    	
    	public RequestStatusCommand(List<String> listOfParameters, IPrinterResponce listener) {
    		mListOfParameters = listOfParameters;
    		mCallback = listener;
    	}
    	
        public void run() {
    		if (mPrinter == null) {
    			mCallback.onErrorHash("printer not connected !");
    			return;
    		}
    		if (!mPrinterConnection.isConnected()) {
    			mCallback.onErrorHash("printer not connected !!!");
    			return;
    		}        	
    		
    		try {
				SGD.SET("device.languages", "zpl", mPrinterConnection);
			} catch (ConnectionException e1) {
				e1.printStackTrace();
				//mCallback.onErrorHash("error with pre configure printer before request Status operation");
    			//return;			
    		}
        	
			PrinterStatus printerStatus = null;
			try {
				printerStatus = mPrinter.getCurrentStatus();
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onErrorHash("error with connection!");
    			return;
			}
			
			
			Hashtable<String, Object> result = new Hashtable<String, Object>();
			Iterator<String> iter = mListOfParameters.iterator();
			while (iter.hasNext()) {
				String key = iter.next();
				if (key.equals(IPrinterDevice.PRINTER_STATE_IS_READY_TO_PRINT)) {
					result.put(IPrinterDevice.PRINTER_STATE_IS_READY_TO_PRINT, new Boolean(printerStatus.isReadyToPrint));
				}
				// PRINTER_STATE_IS_BATTERY_LOW unsupported
				// PRINTER_STATE_IS_COVER_OPENED unsupported
				// PRINTER_STATE_IS_DRAWER_OPENED unsupported
				// 
				if (key.equals(IPrinterDevice.PRINTER_STATE_IS_PAPER_OUT)) {
					result.put(IPrinterDevice.PRINTER_STATE_IS_PAPER_OUT, new Boolean(printerStatus.isPaperOut));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_IS_HEAD_COLD)) {
					result.put(IZebraPrinter.PRINTER_STATE_IS_HEAD_COLD, new Boolean(printerStatus.isHeadCold));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_IS_HEAD_OPEN)) {
					result.put(IZebraPrinter.PRINTER_STATE_IS_HEAD_OPEN, new Boolean(printerStatus.isHeadOpen));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_IS_HEAD_TOO_HOT)) {
					result.put(IZebraPrinter.PRINTER_STATE_IS_HEAD_TOO_HOT, new Boolean(printerStatus.isHeadTooHot));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS)) {
					result.put(IZebraPrinter.PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS, new Boolean(printerStatus.isPartialFormatInProgress));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_IS_PAUSED)) {
					result.put(IZebraPrinter.PRINTER_STATE_IS_PAUSED, new Boolean(printerStatus.isPaused));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_IS_RECEIVE_BUFFER_FULL)) {
					result.put(IZebraPrinter.PRINTER_STATE_IS_RECEIVE_BUFFER_FULL, new Boolean(printerStatus.isReceiveBufferFull));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_IS_RIBBON_OUT)) {
					result.put(IZebraPrinter.PRINTER_STATE_IS_RIBBON_OUT, new Boolean(printerStatus.isRibbonOut));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_LABEL_LENGTH_IN_DOTS)) {
					result.put(IZebraPrinter.PRINTER_STATE_LABEL_LENGTH_IN_DOTS, new Integer(printerStatus.labelLengthInDots));
				}
				if (key.equals(IZebraPrinter.PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER)) {
					result.put(IZebraPrinter.PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER, new Integer(printerStatus.numberOfFormatsInReceiveBuffer));
				}

				
				if (key.equals(IZebraPrinter.PRINTER_STATE_PRINT_MODE)) {
					String mode = IZebraPrinter.PRINT_MODE_UNKNOWN;
					
					if (printerStatus.printMode.equals(ZplPrintMode.APPLICATOR)) {
						mode = IZebraPrinter.PRINT_MODE_APPLICATOR;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.CUTTER)) {
						mode = IZebraPrinter.PRINT_MODE_CUTTER;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.DELAYED_CUT)) {
						mode = IZebraPrinter.PRINT_MODE_DELAYED_CUT;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.KIOSK)) {
						mode = IZebraPrinter.PRINT_MODE_KIOSK;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.LINERLESS_PEEL)) {
						mode = IZebraPrinter.PRINT_MODE_LINERLESS_PEEL;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.LINERLESS_REWIND)) {
						mode = IZebraPrinter.PRINT_MODE_LINERLESS_REWIND;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.PARTIAL_CUTTER)) {
						mode = IZebraPrinter.PRINT_MODE_PARTIAL_CUTTER;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.PEEL_OFF)) {
						mode = IZebraPrinter.PRINT_MODE_PEEL_OFF;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.REWIND)) {
						mode = IZebraPrinter.PRINT_MODE_REWIND;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.RFID)) {
						mode = IZebraPrinter.PRINT_MODE_RFID;
					}
					if (printerStatus.printMode.equals(ZplPrintMode.TEAR_OFF)) {
						mode = IZebraPrinter.PRINT_MODE_TEAR_OFF;
					}
					
					result.put(IZebraPrinter.PRINTER_STATE_PRINT_MODE, mode);
				}
			}
			
			result.put(IPrinterDevice.HK_STATUS, IPrinterDevice.PRINTER_STATUS_SUCCESS);
			
			mCallback.onReturnHash(result);
        }
    }
    
   private class RetrieveFilesWithExtension implements Runnable {
   	
	   	private List<String> mListOfExtensions;
	   	private IPrinterResponce mCallback = null;
	   	
	   	public RetrieveFilesWithExtension(List<String> listOfExtensions, IPrinterResponce listener) {
	   		mListOfExtensions = listOfExtensions;
	   		mCallback = listener;
	   	}
   	
       public void run() {
	   		if (mPrinter == null) {
	   			mCallback.onErrorHash("printer not connected !");
				return;
			}
			if (!mPrinterConnection.isConnected()) {
				mCallback.onErrorHash("printer not connected !!!");
    			return;
			}      
			try {
				String[] extensions = new String[mListOfExtensions.size()];
				Iterator<String> iter = mListOfExtensions.iterator();
				int i = 0;
				while (iter.hasNext()) {
					extensions[i++] = iter.next();
				}
				
				String[] names = mPrinter.retrieveFileNames(extensions);
	
				Hashtable<String, Object> result = new Hashtable<String, Object>();
				result.put(IPrinterDevice.HK_STATUS, IPrinterDevice.PRINTER_STATUS_SUCCESS);
				
				LinkedList<String> files = new LinkedList<String>();
				
				for (i = 0; i < names.length; i++) {
					files.add(names[i]);
				}
				
				result.put(IZebraPrinter.HK_FILE_NAMES, files);
				
				mCallback.onReturnHash(result);
				
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onErrorHash("error with connection!");
			} catch (ZebraIllegalArgumentException e) {
				e.printStackTrace();
				mCallback.onErrorHash("illegal arguments!");
			}

       }
   }
    
   
   private class SendFileContentCommand implements Runnable {
	   	
	   	private String mPath;
	   	private IPrinterResponce mCallback = null;
	   	
	   	public SendFileContentCommand(String path, IPrinterResponce listener) {
	   		mPath = path;
	   		mCallback = listener;
	   	}
  	
      public void run() {
	   		if (mPrinter == null) {
	   			mCallback.onError("printer not connected !");
				return;
			}
			if (!mPrinterConnection.isConnected()) {
				mCallback.onError("printer not connected !!!");
    			return;
			}      
			try {
				mPrinter.sendFileContents(mPath);
				
				mCallback.onSuccess();
				
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onError("error with connection!");
			}
	  }
  }   

   private class PrintFormatWithArrayCommand implements Runnable {
	   	
	   	private String mFormatPath = null;
	   	private String mVars[] = null;
	   	private IPrinterResponce mCallback = null;
	   	
	   	public PrintFormatWithArrayCommand(String formatPath, List<String> vars, IPrinterResponce listener) {
	   		mFormatPath = formatPath;
	   		
	   		mVars = new String[vars.size()];
	   		int i;
	   		for (i = 0; i < vars.size(); i++) {
	   			mVars[i] = vars.get(i);
	   		}
	   		mCallback = listener;
	   	}
 	
     public void run() {
	   		if (mPrinter == null) {
	   			mCallback.onError("printer not connected !");
				return;
			}
			if (!mPrinterConnection.isConnected()) {
				mCallback.onError("printer not connected !!!");
    			return;
			}      
			PrinterStatus printerStatus = null;
			try {
				printerStatus = mPrinter.getCurrentStatus();
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onError("error with connection!");
    			return;
			}
			
			if (printerStatus.isReadyToPrint) {
				try {

					mPrinter.printStoredFormat(mFormatPath, mVars);
					
					mCallback.onSuccess();
					
				} catch (ConnectionException e) {
					e.printStackTrace();
					mCallback.onError("error with connection !!!");
				}
			}
			else {
				mCallback.onError("printer not ready to print!");
			}					
	  }
  }   
   
   private class PrintFormatWithHashCommand implements Runnable {
	   	
	   	private String mFormatPath = null;
	   	private Map<String, String> mVars = null;
	   	private IPrinterResponce mCallback = null;
	   	
	   	public PrintFormatWithHashCommand(String formatPath, Map<String, String> vars, IPrinterResponce listener) {
	   		mFormatPath = formatPath;
	   		mVars = vars;
	   		mCallback = listener;
	   	}
	
    public void run() {
	   		if (mPrinter == null) {
	   			mCallback.onError("printer not connected !");
				return;
			}
			if (!mPrinterConnection.isConnected()) {
				mCallback.onError("printer not connected !!!");
    			return;
			}      
			PrinterStatus printerStatus = null;
			try {
				printerStatus = mPrinter.getCurrentStatus();
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onError("error with connection!");
    			return;
			}
			
			if (printerStatus.isReadyToPrint) {
				try {
					
					Hashtable<Integer, String> vars = new Hashtable<Integer, String>();
					Iterator<String> it = mVars.keySet().iterator();
					while (it.hasNext()) {
						String key = it.next();
						String value = mVars.get(key);
						Integer intKey = new Integer(Integer.parseInt(key));
						vars.put(intKey, value);
					}
					mPrinter.printStoredFormat(mFormatPath, vars);
					
					mCallback.onSuccess();
					
				} catch (ConnectionException e) {
					e.printStackTrace();
					mCallback.onError("error with connection !!!");
				}
			}
			else {
				mCallback.onError("printer not ready to print!");
			}					
	  }
 }   
  
   
   private class ConnectWithOptionsCommand implements Runnable {
	   	
	   	private long mTimeout;
	   	private long mStartTime;
	   	private IPrinterResponce mCallback = null;
		private Thread mTimeoutThread = null;
		private boolean mIsTimeOver = false;
		private boolean mIsFinished = false;
	   	
	   	public ConnectWithOptionsCommand(long startTime, long timeout, IPrinterResponce listener) {
	   		mStartTime = startTime;
	   		mTimeout = timeout;
	   		mCallback = listener;
	   		mIsFinished = false;
			mTimeoutThread = new Thread( new Runnable() {
				public void run() {
					Looper.prepare();
					try {
						long curTime = System.currentTimeMillis();
						long sleep = mTimeout- (curTime - mStartTime);
						if (sleep > 0 ) {
							Thread.sleep(sleep);
						}
						mIsTimeOver = true;
						synchronized (mCallback) {
							if (!mIsFinished) {
								mIsFinished = true;
								//mCallback.onError("error with connection - timeout");
								mCallback.onReturnStatus(IPrinterDevice.PRINTER_STATUS_ERR_TIMEOUT);
								//mCallback.onReturnStatus(IPrinterDevice.PRINTER_STATUS_ERROR);
								return;
							}
						}
						
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					finally {
						Looper.myLooper().quit();
					}
				}
			});
			mTimeoutThread.start();		
		}
 	
     public void run() {
			try {
				if (mPrinter == null) {
					try {
						Thread.sleep(2000);
						mPrinterConnection.open();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					mPrinter = ZebraPrinterFactory.getInstance(mPrinterConnection);
				}
				else {
					if (!mPrinterConnection.isConnected()) {
						mPrinterConnection.open();
						mPrinter = ZebraPrinterFactory.getInstance(mPrinterConnection);
					}
				}
				synchronized (mCallback) {
					if (!mIsFinished) {
						mIsFinished = true;
					}
					else {
						try{
							Thread.sleep(4000);
							mPrinterConnection.close();
						}
						catch(Exception exp){
							exp.printStackTrace();
						}
						mPrinter = null;
					}
				}
				if (mPrinter == null) {
					mCallback.onError("error with connection - can not receive printer object from Zebra SDK");
					return;
				}
				//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@@@@@@ CONNECT !!!");
				long curTime = System.currentTimeMillis();
				long to_sleep = mTimeout- (curTime - mStartTime);
				if (to_sleep > 0 ) {
					if (checkConnectionInCommandThread()) {
						mCallback.onSuccess();
					}
					else {
						mCallback.onError("error with connection");
					}
				}
				else {
					mCallback.onReturnStatus(IPrinterDevice.PRINTER_STATUS_ERR_TIMEOUT);
					//mCallback.onReturnStatus(IPrinterDevice.PRINTER_STATUS_ERROR);
				}
			} catch (ConnectionException e) {
				e.printStackTrace();
				mCallback.onError("error with connection");
			} catch (ZebraPrinterLanguageUnknownException e) {
				e.printStackTrace();
				mCallback.onError("error with get ZebraPrinter instance");
			}
	  }
 }      
    
   
   
   
   
	public ZebraPrinterImpl(String id, Connection printerConnection, String zebraAddress, String adress, String friendlyName) {
		mID = id;
		mPrinterConnection = printerConnection;
		mPrinter = null;
		mZebraAddress = zebraAddress;
		mAdress = adress;
		mFriendlyName = friendlyName;

		if (friendlyName == null) {
			initPrinter();
		}
		
	}
	
	private void initPrinter() {
		try {
			mPrinterConnection.open();
			mPrinter = ZebraPrinterFactory.getInstance(mPrinterConnection);
			if (mPrinter != null) {
				PrinterLanguage pl = mPrinter.getPrinterControlLanguage();
				if (pl != null) {
					Utils.platformLog("@@@@@ PRINTER LANGUAGE :", pl.toString());
					if (pl == PrinterLanguage.CPCL) {
						mInitialControlLanguage = IPrinterDevice.PRINTER_LANGUAGE_CPCL;
					}
					if (pl == PrinterLanguage.ZPL) {
						mInitialControlLanguage =  IPrinterDevice.PRINTER_LANGUAGE_ZPL;
					}
				}				
			}
			
			mFriendlyName = SGD.GET("device.friendly_name", mPrinterConnection);
			
			if (mPrinterConnection instanceof UsbConnection) {
				UsbConnection con = (UsbConnection)mPrinterConnection;
				mFriendlyName = con.getDeviceName();
			}

			mPrinterConnection.close();
			
			if (mFriendlyName != null) {
				if (mFriendlyName.length() <= 0) {
					mFriendlyName = null;
				}
			}
			
		
		} catch (ConnectionException e) {
			e.printStackTrace();
		} catch (ZebraPrinterLanguageUnknownException e) {
			e.printStackTrace();
		}
		finally {
			mPrinter = null;
		}
		
	}
	
	
	private void initFromDiscoveryPrinter() {
		
	}

	public String getFrienldyName() {
		if (mFriendlyName != null) {
			return mFriendlyName;
		}
		if (mPrinterConnection instanceof UsbConnection) {
			initPrinter();
			if (mFriendlyName != null) {
				return mFriendlyName;
			}
		}
		return "ZebraPrinter:"+getAddress();
	}
	
	
	
	public void connect(IPrinterResponce listener) {
		ZebraTransactionManager.addCommand(new PrintSimpleCommand(listener) {
			public void run() {
				try {
					if (mPrinter == null) {
						mPrinterConnection.open();
						mPrinter = ZebraPrinterFactory.getInstance(mPrinterConnection);
					}
					else {
						if (!mPrinterConnection.isConnected()) {
							mPrinterConnection.open();
							mPrinter = ZebraPrinterFactory.getInstance(mPrinterConnection);
						}
					}
					//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@@@@@@ CONNECT !!!");
					if (checkConnectionInCommandThread()) {
						getCallback().onSuccess();
					}
					else {
						getCallback().onError("error with connection");
					}
				} catch (ConnectionException e) {
					e.printStackTrace();
					getCallback().onError("error with connection");
				} catch (ZebraPrinterLanguageUnknownException e) {
					e.printStackTrace();
					getCallback().onError("error with get ZebraPrinter instance");
				}
			}
		});
	}
	
	public boolean checkConnectionInCommandThread() {
		ZebraPrinter savedPrinter = mPrinter;
		
		try {
			boolean wasConnected = false;
			if (mPrinter == null) {
				mPrinterConnection.open();
				mPrinter = ZebraPrinterFactory.getInstance(mPrinterConnection);
			}
			else {
				if (mPrinterConnection.isConnected()) {
					wasConnected = true;
				}
				else {
					mPrinterConnection.open();
					mPrinter = ZebraPrinterFactory.getInstance(mPrinterConnection);
				}
			}
			if (mPrinter == null) {
				Utils.platformLog("ZebraPrinter.checkConnection", "WARNINGEN2: can not check connection of ["+getZebraAddress()+"]");
				return false;
			}

			PrinterStatus printerStatus = null;
			try {
				printerStatus = mPrinter.getCurrentStatus();
			} catch (ConnectionException e) {
				Utils.platformLog("ZebraPrinter.checkConnection", "error with connection with ["+getZebraAddress()+"]!");
    			return false;
			}
			if (printerStatus == null) {
				Utils.platformLog("ZebraPrinter.checkConnection", "error with connection with ["+getZebraAddress()+"]!");
    			return false;				
			}
			
			
			if (!wasConnected) {
				mPrinterConnection.close();
				mPrinter = null;
			}
			mPrinter = savedPrinter;
		}
		catch (Exception e) {
			mPrinter = null;
			Utils.platformLog("ZebraPrinter.checkConnection", "WARNINGEN3: can not check connection of ["+getZebraAddress()+"]");
			return false;
		}
		Utils.platformLog("ZebraPrinter.checkConnection", "INFONGEN1: check connection of ["+getZebraAddress()+"] is OK !");
		return true;
	}
	
	
	public void connectWithOptions(Map<String, Object> options, IPrinterResponce listener) {
		//connect(listener);
		long timeout = 15000;
		
		if (options != null) {
			Object obj = options.get(IZebraPrinter.HK_TIMEOUT);
			if (obj != null) {
				if (obj instanceof Integer) {
					timeout = ((Integer)obj).intValue();
				}
				else {
					listener.onError("invalid type for timeout parameter - should be Integer !");
					return;
				}
			}
			if (timeout <= 0) {
				listener.onError("invalid timeout !");
				return;
			}
		}
		ZebraTransactionManager.addCommand(new ConnectWithOptionsCommand(System.currentTimeMillis(), timeout, listener));

	}


	public void disconnect(IPrinterResponce listener) {
		//Utils.platformLog("ZEBRA_PRINTER", "@@@@ DISCONNECT !!!");
		//Thread.dumpStack();
		ZebraTransactionManager.addCommand(new PrintSimpleCommand(listener) {
			public void run() {
				try {
					if (mPrinter != null) {
						mPrinterConnection.close();
						mPrinter = null;
					}
					//Utils.platformLog("ZEBRA_PRINTER", "@@@@@@@@@@@@@@@@@ DISCONNECT !!!");
					getCallback().onSuccess();
				} catch (ConnectionException e) {
					e.printStackTrace();
					getCallback().onError("error with connection");
				}
			}
		});
	}

	public void printRawString(String command, Map<String, Object> options, IPrinterResponce listener) {
		ZebraTransactionManager.addCommand(new PrintRawStringCommand(command, listener));		
	}

	
	public String getID() {
		return mID;
	}

	public String getZebraAddress() {
		return mZebraAddress;
	}

	public String getAddress() {
		return mAdress;
	}
	
	public String getPort() {
		if (mPrinterConnection instanceof TcpConnection) {
			return ((TcpConnection)mPrinterConnection).getPortNumber();
		}
		return "0";
	}
	
	public String getControlLanguage() {
		if (mPrinter != null) {
			PrinterLanguage pl = mPrinter.getPrinterControlLanguage();
			if (pl != null) {
				Utils.platformLog("@@@@@@@@@@@@@@@@@ LANGUAGE :", pl.toString());
				if (pl == PrinterLanguage.CPCL) {
					return IPrinterDevice.PRINTER_LANGUAGE_CPCL;
				}
				if (pl == PrinterLanguage.ZPL) {
					return IPrinterDevice.PRINTER_LANGUAGE_ZPL;
				}
				return mInitialControlLanguage;
			}
		}
		return "";
	}

	public String getConnectionType() {
		if (mPrinterConnection instanceof TcpConnection) {
			return IPrinterDevice.CONNECTION_TYPE_TCP;
		}
		if (mPrinterConnection instanceof BluetoothConnection) {
			return IPrinterDevice.CONNECTION_TYPE_BLUETOOTH;
		}
		if (mPrinterConnection instanceof UsbConnection) {
			return IPrinterDevice.CONNECTION_TYPE_USB;
		}
		return IPrinterDevice.CONNECTION_TYPE_ANY;
	}
	
	public boolean isConnected() {
		return (mPrinter != null);
	}

	
	public void printImageFromFile(String path, int x, int y, Map<String, Object> options, IPrinterResponce result) {
		Utils.platformLog("ZEBRA_PRINTER", "@@@@ printImageFromFile !!!");
		
		int width = -1;
		int height = -1;
		boolean isInsideFormat = false;
		
		if (path.toLowerCase().endsWith(".gif") || path.toLowerCase().endsWith(".bmp")) {
			result.onError("unsupported image format");
			return;
		}
		if (options != null) {
			Object obj = options.get(IZebraPrinter.HK_WIDTH);
			if ((obj != null) && (obj instanceof Integer)) {
				width = ((Integer)obj).intValue();
			}
			obj = options.get(IZebraPrinter.HK_HEIGHT);
			if ((obj != null) && (obj instanceof Integer)) {
				height = ((Integer)obj).intValue();
			}
			obj = options.get(IZebraPrinter.HK_IS_INSIDE_FORMAT);
			if ((obj != null) && (obj instanceof Boolean)) {
				isInsideFormat = ((Boolean)obj).booleanValue();
			}
			if ((width == 0) || (height == 0)) {
				result.onError("invalid image size");
				return;
			}
		}
		ZebraTransactionManager.addCommand(new PrintPrinImageFromFileCommand(path, x, y, width, height, isInsideFormat, result));
	}
	
	public List<Object> enumerateSupportedControlLanguages() {
        List<Object> lngs = new LinkedList<Object>();
        // TODO:
        //lngs.add(mInitialControlLanguage);
        //if (mInitialControlLanguage.equals(IPrinterDevice.PRINTER_LANGUAGE_ZPL)) {
        //    lngs.add(IPrinterDevice.PRINTER_LANGUAGE_CPCL);
        //}
        lngs.add(IPrinterDevice.PRINTER_LANGUAGE_ZPL);
        lngs.add(IPrinterDevice.PRINTER_LANGUAGE_CPCL);
        return lngs;
	}

	public void storeImage(String printerDriveAndFileName, String imageFilePathOnDevice, int width, int height, IPrinterResponce result) {
		if (imageFilePathOnDevice.toLowerCase().endsWith(".gif") || imageFilePathOnDevice.toLowerCase().endsWith(".bmp")) {
			result.onError("unsupported image format");
			return;
		}
		if ((width == 0) || (height == 0)) {
			result.onError("invalid image size");
			return;
		}
		ZebraTransactionManager.addCommand(new StoreImageCommand(imageFilePathOnDevice, printerDriveAndFileName, width, height, result));
	}
	
	public void requestState(List<String> listOfParameters, IPrinterResponce result) {
		ZebraTransactionManager.addCommand(new RequestStatusCommand(listOfParameters, result));
	}
	
	public void retrieveFileNames(IPrinterResponce result) {
		ZebraTransactionManager.addCommand(new PrintSimpleCommand(result) {
			public void run() {
	    		if (mPrinter == null) {
	    			getCallback().onErrorHash("printer not connected !");
	    			return;
	    		}
	    		if (!mPrinterConnection.isConnected()) {
	    			getCallback().onErrorHash("printer not connected !!!");
	    			return;
	    		}      
	    		try {
					String[] names = mPrinter.retrieveFileNames();

					Hashtable<String, Object> result = new Hashtable<String, Object>();
					result.put(IPrinterDevice.HK_STATUS, IPrinterDevice.PRINTER_STATUS_SUCCESS);
					
					LinkedList<String> files = new LinkedList<String>();
					
					int i;
					for (i = 0; i < names.length; i++) {
						files.add(names[i]);
					}
					
					result.put(IZebraPrinter.HK_FILE_NAMES, files);
					
					getCallback().onReturnHash(result);
					
				} catch (ConnectionException e) {
					e.printStackTrace();
					getCallback().onErrorHash("error with connection!");
				} catch (ZebraIllegalArgumentException e) {
					e.printStackTrace();
					getCallback().onErrorHash("illegal arguments!");
				}
				
			}
		});
	}
	
	public void retrieveFileNamesWithExtensions(List<String> extensions, IPrinterResponce result) {
		ZebraTransactionManager.addCommand(new RetrieveFilesWithExtension(extensions, result));
	}
	
	
	public void sendFileContents(String file, IPrinterResponce result) {
		ZebraTransactionManager.addCommand(new SendFileContentCommand(file, result));
	}
	
	public void printStoredFormatWithHash(String formatPathOnPrinter, Map<String, String> vars, IPrinterResponce result) {
		ZebraTransactionManager.addCommand(new PrintFormatWithHashCommand(formatPathOnPrinter, vars, result));
	}
	
	public void printStoredFormatWithArray(String formatPathOnPrinter, List<String> vars, IPrinterResponce result) {
		ZebraTransactionManager.addCommand(new PrintFormatWithArrayCommand(formatPathOnPrinter, vars, result));
	}
	
	public void preconfigurePrinterBeforeOperation() {
		if (mPrinterConnection.isConnected()) {
			try {
				SGD.SET("device.languages", "zpl", mPrinterConnection);
			} catch (ConnectionException e1) {
				e1.printStackTrace();
				return;			
			}		
		}
	}
	
	public void replaceConnection(Connection printerConnection) {
		try {
			if (mPrinter != null) {
				mPrinterConnection.close();
				mPrinter = null;
			}
		} catch (ConnectionException e) {
			e.printStackTrace();
		}
		mPrinterConnection = printerConnection;
	}

	
	
}
