package com.rhomobile.printing.zebra.impl;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.nsd.NsdManager.DiscoveryListener;
import android.os.Looper;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;

import com.rho.printerzebra.IPrinterZebraSingleton;
import com.rhomobile.printing.api.IPrinterDevice;
import com.rhomobile.printing.api.ISearchPrinterListener;
import com.rhomobile.printing.zebra.api.IZebraPrintingProvider;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.Utils;
import com.zebra.sdk.comm.BluetoothConnection;
import com.zebra.sdk.comm.Connection;
import com.zebra.sdk.comm.ConnectionException;
import com.zebra.sdk.comm.TcpConnection;
import com.zebra.sdk.printer.PrinterStatus;
import com.zebra.sdk.printer.SGD;
import com.zebra.sdk.printer.ZebraPrinter;
import com.zebra.sdk.printer.ZebraPrinterFactory;
import com.zebra.sdk.printer.ZebraPrinterLanguageUnknownException;
import com.zebra.sdk.printer.discovery.BluetoothDiscoverer;
import com.zebra.sdk.printer.discovery.DiscoveredPrinter;
import com.zebra.sdk.printer.discovery.DiscoveryException;
import com.zebra.sdk.printer.discovery.DiscoveryHandler;
import com.zebra.sdk.printer.discovery.NetworkDiscoverer;
import com.zebra.sdk.printer.discovery.DiscoveredPrinterUsb;
import com.zebra.sdk.printer.discovery.UsbDiscoverer;

public class ZebraPrintingProviderImpl implements IZebraPrintingProvider {

	private ArrayList<ZebraPrinterImpl> mPrinters = null;
	private static int ourCounter = 1;
	private String mac_adress = null;//MAC_ADDRESS:00:22:58:38:81:B9
    	private String friendly_name = null;//FRIENDLY_NAME:Zebra220
	
	private static void debugLogging(String s) {
		Utils.platformLog("ZebraPrintingProvider $$$", s);
	}
	
	
	private interface IZebraPrinterSearchPrinterListenerBridge extends ISearchPrinterListener {
	
		public void forceStop();
		public void forceStopAndFinish();
		
	}
	
	private ArrayList<IZebraPrinterSearchPrinterListenerBridge>mDiscoveryListeners = null;

    private static final String ACTION_USB_PERMISSION = "com.android.rhomobile.zebra.printing.USB_PERMISSION";
    private PendingIntent mPermissionIntent;
    
    private UsbManager mUsbManager = null;
    
    private BroadcastReceiver mUsbReceiver = null;
    
	
	public ZebraPrintingProviderImpl() {
		debugLogging("ZebraPrintingProviderImpl INIT !!!");
		mPrinters = new ArrayList<ZebraPrinterImpl>();
		mDiscoveryListeners = new ArrayList<IZebraPrinterSearchPrinterListenerBridge>();
		
        // Register broadcast receiver that catches USB permission intent
		try {
		    mUsbReceiver = new BroadcastReceiver() {
		        public void onReceive(Context context, Intent intent) {
		            String action = intent.getAction();
		            if (ACTION_USB_PERMISSION.equals(action)) {
		                synchronized (this) {
		                    UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
		                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
		                        if (device != null) {
		                        	//TODO: mark device as has permission
		                        }
		                    }
		                }
		            }
		        }
		    };	
			mUsbManager = (UsbManager) RhodesActivity.safeGetInstance().getSystemService(Context.USB_SERVICE);
        	mPermissionIntent = PendingIntent.getBroadcast(RhodesActivity.safeGetInstance(), 0, new Intent(ACTION_USB_PERMISSION), 
        		PendingIntent.FLAG_IMMUTABLE);
        	IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        	RhodesActivity.safeGetInstance().registerReceiver(mUsbReceiver, filter);	
		}
		catch(Exception e) {
			e.printStackTrace();
			debugLogging("Exception during register broadcast receiver !!!");
		}
	}
	
	
	
	
	
	
	
	private void addDiscoveryListeners(IZebraPrinterSearchPrinterListenerBridge listener) {
		synchronized (mDiscoveryListeners) {
			mDiscoveryListeners.add(listener);
		}
	}
	
	private void removeDiscoveryListeners(IZebraPrinterSearchPrinterListenerBridge listener) {
		synchronized (mDiscoveryListeners) {
			mDiscoveryListeners.remove(listener);
		}
	}
	
	private class ZebraPrinterTCPConnect implements Runnable {
		
		private String mAdress = null;
		private String mPort = null;
		private ISearchPrinterListener mListener = null;
		
		public ZebraPrinterTCPConnect(String adress, String port, ISearchPrinterListener listener) {
			mAdress = adress;
			mPort = port;
			mListener = listener;
		}
		
		public void run() {
			
			try {
				Thread.sleep(10);
			} catch (InterruptedException e1) {

			}
			
			TcpConnection connection = new TcpConnection(mAdress, Integer.parseInt(mPort));
			
   		    String zAdr = connection.toString();
			 IPrinterDevice p = getPrinterByZebraAdress(zAdr);
			 if (p != null) {
				 mListener.onFound(p.getID());
				 mListener.onFinished();
				 return;
			 }

			 
			try {
				connection.open();
				connection.close();
				
				 Connection c = connection;
				 String adr = null;
					 if (c instanceof TcpConnection) {
					adr = ((TcpConnection)c).getAddress();
				 }
				 if (c instanceof BluetoothConnection) {
						adr = ((BluetoothConnection)c).getMACAddress();
				 }	            	 

				ZebraPrinterImpl printer = new ZebraPrinterImpl("ZEBRA_PRINTER_"+String.valueOf(ourCounter++), connection, zAdr, adr, null);
				mPrinters.add(printer);
				
				mListener.onFound(printer.getID());
				mListener.onFinished();
				
			} catch (ConnectionException e) {
				e.printStackTrace();
				mListener.onNotConnected("cannot connect to defined TCP adress:port: ["+mAdress+"]:["+mPort+"]");
			}			
		}
	}

	private class ZebraPrinterBluetoothConnect implements Runnable {
		
		private String mAdress = null;
		private ISearchPrinterListener mListener = null;
		
		public ZebraPrinterBluetoothConnect(String adress, ISearchPrinterListener listener) {
			mAdress = adress;
			mListener = listener;
		}
		
		public void run() {
			
			try {
				Thread.sleep(10);
			} catch (InterruptedException e1) {

			}			
			
			//Utils.platformLog("@@@@ ZebraPrinterBluetoothConnect Address :", mAdress);
			
			BluetoothConnection connection = new BluetoothConnection(mAdress);
   		    String zAdr = connection.toString();
			 IPrinterDevice p = getPrinterByZebraAdress(zAdr);
			 if (p != null) {
					//Utils.platformLog("@@@@ ZebraPrinterBluetoothConnect :", "return printer from list");

   			    mListener.onFound(p.getID());
				mListener.onFinished();
				 return;
			 }
			
			try {
				connection.open();
				//Utils.platformLog("@@@@ ZebraPrinterBluetoothConnect try{ :", "open passed");
				connection.close();
				//Utils.platformLog("@@@@ ZebraPrinterBluetoothConnect try{ :", "close passed");

				 Connection c = connection;
				 String adr = null;
					 if (c instanceof TcpConnection) {
					adr = ((TcpConnection)c).getAddress();
				 }
				 if (c instanceof BluetoothConnection) {
						adr = ((BluetoothConnection)c).getMACAddress();
				 }	            	 
					//Utils.platformLog("@@@@ ZebraPrinterBluetoothConnect try{ adr:", adr);


				ZebraPrinterImpl printer = new ZebraPrinterImpl("ZEBRA_PRINTER_"+String.valueOf(ourCounter++), connection, zAdr, adr, null);
				mPrinters.add(printer);
				//Utils.platformLog("@@@@ ZebraPrinterBluetoothConnect try{ :", "new printer created");

				
				mListener.onFound(printer.getID());
				mListener.onFinished();
				
			} catch (ConnectionException e) {
				e.printStackTrace();
				mListener.onNotConnected("cannot connect to defined Bluetooth adress: ["+mAdress+"]");
			}			
		}
	}
	
	private class ZebraPrinterDiscoverPrinterCommand implements Runnable {

		private Connection mConnection = null;
		private ISearchPrinterListener mListener = null;
		
		public ZebraPrinterDiscoverPrinterCommand(Connection c, ISearchPrinterListener l) {
			mConnection = c;
			mListener = l;
		}
		
		
		public void run() {
			debugLogging("ZebraPrinterDiscoverPrinterCommand :: START !!!");
			 if (mConnection == null) {
				 return;
			 }
	       	 String zAdr = mConnection.toString();
	       	 String adr = null;
	    		 if (mConnection instanceof TcpConnection) {
	   			adr = ((TcpConnection)mConnection).getAddress();
	   		 }
	   		 if (mConnection instanceof BluetoothConnection) {
	       		adr = ((BluetoothConnection)mConnection).getMACAddress();
	   		 }	            	 
	   		 IPrinterDevice p = getPrinterByZebraAdress(zAdr);
	       	 if (p != null) {
	       		 //mListener.onFound(p.getID());
	       		debugLogging("ZebraPrinterDiscoverPrinterCommand :: can not get ZabraPrinter by adress !!!");
	       		 return;
	       	 }
      	 
			try {
				mConnection.open();
				SGD.SET("device.languages", "zpl", mConnection);
				PrinterStatus printerStatus = null;
				ZebraPrinter zPrinter = ZebraPrinterFactory.getInstance(mConnection);
				printerStatus = zPrinter.getCurrentStatus();
				if (printerStatus == null) {
					mConnection.close();
					debugLogging("ZebraPrinterDiscoverPrinterCommand :: can not get printer status !!!");
					return;
				}
			} catch (ConnectionException e) {
				debugLogging("ZebraPrinterDiscoverPrinterCommand :: connection exception !!!");
				e.printStackTrace();
  			return;
			} catch (ZebraPrinterLanguageUnknownException e) {
				e.printStackTrace();
				debugLogging("ZebraPrinterDiscoverPrinterCommand :: unknown language exception !!!");
				return;
			}

			debugLogging("ZebraPrinterDiscoverPrinterCommand :: printer checked OK !!!");

			 ZebraPrinterImpl prn = new ZebraPrinterImpl("ZEBRA_PRINTER_"+String.valueOf(ourCounter++), mConnection, zAdr, adr, null);
			 //this is Zebra printer !
			 mPrinters.add(prn);
			 mListener.onFound(prn.getID());
			
		}
		
	}
	
	
	private class ZebraPrinterTCPDiscovery implements Runnable {
		
		private ISearchPrinterListener mListener = null;
		private int mTimeout = 30000;
		private long mStartTimeInMillis = 0;
		
		public ZebraPrinterTCPDiscovery(int timeout, long startTime, ISearchPrinterListener listener) {
			mTimeout = timeout;
			mStartTimeInMillis = startTime;
			mListener = listener;
		}
		
		public void run() {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e1) {

			}
			
			debugLogging("ZebraPrinterTCPDiscovery START !!!");
			
			// firstly check already founded printers
			Iterator<ZebraPrinterImpl> iterator = mPrinters.iterator();
			while (iterator.hasNext()) {
				ZebraPrinterImpl prn = iterator.next();
				if (IPrinterDevice.CONNECTION_TYPE_TCP.equals(prn.getConnectionType())) {
					if (prn.checkConnectionInCommandThread()) {
						mListener.onFound(prn.getID());
					}
				}
			}				
			

	         DiscoveryHandler discoveryHandler = new DiscoveryHandler() {
	 
	             public void foundPrinter(DiscoveredPrinter printer) {
	            	 Connection c = printer.getConnection();
	            	 ZebraTransactionManager.addCommandBeforeAll(new ZebraPrinterDiscoverPrinterCommand(c, mListener));
	             }
	 
	             public void discoveryFinished() {
	            	 mListener.onFinished();
	             }
	 
	             public void discoveryError(String message) {
	                 mListener.onError(message);
	             }
	         };
	         try {
	        	 NetworkDiscoverer.localBroadcast(discoveryHandler, mTimeout);
	             //NetworkDiscoverer.findPrinters(discoveryHandler);
	         } catch (DiscoveryException e) {
	             e.printStackTrace();
	             mListener.onError("catch DiscoveryException !");
	         }			
	
		}
	}
	
	
	
    class UsbDiscoveryHandler implements DiscoveryHandler {
        public List<DiscoveredPrinterUsb> printers;

        public UsbDiscoveryHandler() {
            printers = new LinkedList<DiscoveredPrinterUsb>();
        }

        public void foundPrinter(final DiscoveredPrinter printer) {
            printers.add((DiscoveredPrinterUsb) printer);
        }

        public void discoveryFinished() {
        }

        public void discoveryError(String message) {
        }
    }	

	private class ZebraPrinterUSBDiscovery implements Runnable {
		
		private ISearchPrinterListener mListener = null;
		private int mTimeout = 30000;
		private long mStartTimeInMillis = 0;
		
		public ZebraPrinterUSBDiscovery(int timeout, long startTime, ISearchPrinterListener listener) {
			mTimeout = timeout;
			mStartTimeInMillis = startTime;
			mListener = listener;
		}
		
		public void run() {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e1) {

			}
			
			debugLogging("ZebraPrinterUSBDiscovery START !!!");
			
			// firstly check already founded printers
			Iterator<ZebraPrinterImpl> iterator = mPrinters.iterator();
			while (iterator.hasNext()) {
				ZebraPrinterImpl prn = iterator.next();
				if (IPrinterDevice.CONNECTION_TYPE_USB.equals(prn.getConnectionType())) {
					if (prn.checkConnectionInCommandThread()) {
						mListener.onFound(prn.getID());
					}
				}
			}				
			

	         try {
	        	 UsbDiscoveryHandler handler = new UsbDiscoveryHandler();
                 UsbDiscoverer.findPrinters(RhodesService.getContext(), handler);
                 try {
                     if (handler.printers != null && handler.printers.size() > 0)
                     {
                    	 Iterator<DiscoveredPrinterUsb> it = handler.printers.iterator();
                    	 while (it.hasNext()) {
                    		 DiscoveredPrinterUsb usbPrinter = it.next();
                    		 mUsbManager.requestPermission(usbPrinter.device, mPermissionIntent);
                    		 Connection con = usbPrinter.getConnection();
                    		 
                 			 debugLogging("USB printer FOUND !!!");
                 			 
                 			 String zAdr = con.toString();
                 			ZebraPrinterImpl prn = new ZebraPrinterImpl("ZEBRA_PRINTER_"+String.valueOf(ourCounter++), con, zAdr, zAdr, null/*usbPrinter.device.getDeviceName()*/);
                 			 //this is Zebra printer !
                 			 mPrinters.add(prn);
                 			/*ZebraPrinterImpl prn = null;
                 			 
                 			 ZebraPrinterImpl old_p = (ZebraPrinterImpl)getPrinterByZebraAdress(zAdr);
                 			 if (old_p != null) {
                 				 old_p.replaceConnection(con);
                 				 prn = old_p;
                 			 }
                 			 else {
                 				 prn = new ZebraPrinterImpl("ZEBRA_PRINTER_"+String.valueOf(ourCounter++), con, zAdr, zAdr, nullusbPrinter.device.getDeviceName());
               			     	//this is Zebra printer !
               			     	mPrinters.add(prn);
                 			 }*/
               			     mListener.onFound(prn.getID());
                    		 
                    		 
                    	 }
                     }
                     
                     mListener.onFinished();
                     
                 } catch (Exception e) {
    	             e.printStackTrace();
    	             mListener.onError("catch Exception during enumerate discovered USB printers !");
                 }	        	 
             } catch (Exception e) {
	             e.printStackTrace();
	             mListener.onError("catch Exception during USB Discovering !");
             }	        	 
		}
	}
	
	
	
	
	
	private class ZebraPrinterSearchPrinterListenerBridgeImpl implements IZebraPrinterSearchPrinterListenerBridge {
		
		private ISearchPrinterListener mListener = null;
		private long mStartTime = 0;
		private long mTimeout = 0;
		private int mFinishCount = 0;
		private int onFinishCounter = 0;
		private boolean mIsAcive = true;
		private Thread mTimeoutThread = null;
		
		
		public ZebraPrinterSearchPrinterListenerBridgeImpl(ISearchPrinterListener realListener, long startTimeInMilliseconds, long timeoutInMilliseconds, int finishCount) {
			mListener = realListener;
			mStartTime = startTimeInMilliseconds;
			mTimeout = timeoutInMilliseconds;
			mFinishCount = finishCount;
			
			debugLogging("ZebraPrinterSearchPrinterListenerBridgeImpl("+String.valueOf(mFinishCount)+") START !");
			
			mTimeoutThread = new Thread( new Runnable() {
				public void run() {
					Looper.prepare();
					try {
						long curTime = System.currentTimeMillis();
						long to_sleep = mTimeout - (curTime - mStartTime);
						if (to_sleep > 0) {
							Thread.sleep(mTimeout - (curTime - mStartTime));
						}
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					finally {
						//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "timeout reached !");
						forceStopAndFinish();
						Looper.myLooper().quit();
					}
				}
			});
			mTimeoutThread.start();		
			addDiscoveryListeners(this);
		}
		
		private void checkTime() {
			//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "checkTime()");

			if (!mIsAcive) {
				//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "inactive !");
				return;
			}
			long curTimeInMillis = System.currentTimeMillis();
			if ((curTimeInMillis - mStartTime) >= mTimeout) {
				//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "timeout reached 2 !");
				forceStopAndFinish();
			}
		}

		public void onFound(String printer_id) {
			//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "onFound("+printer_id+")");

			checkTime();
			if (!mIsAcive) {
				return;
			}
			mListener.onFound(printer_id);
		}

		public void onError(String message) {
			//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "onError("+message+")");
			checkTime();
			if (!mIsAcive) {
				return;
			}
			forceStop();
			mListener.onError(message);
		}

		public void onFinished() {
			//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "onFinished()");

			onFinishCounter++;
			checkTime();
			if (!mIsAcive) {
				return;
			}
			if (onFinishCounter >= mFinishCount) {
				//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "onFound() -> last finished !");

				forceStop();
				mListener.onFinished();
			}
		}
		
		public void onNotConnected(String message) {
			//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "onNotConnected("+message+")");

			// must not be called !!!
			onError(message);
		}		
		
		public void forceStop() {
			//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "forceStop()");
			if (mIsAcive) {
				mIsAcive = false;
				removeDiscoveryListeners(this);
			}
		}

		public void forceStopAndFinish() {
			//Utils.platformLog("@@@@ ZebraPrinterSearchPrinterListenerBridgeImpl", "forceStopAndFinish()");

			if (mIsAcive) {
				forceStop();
				mListener.onFinished();
			}
		}
		
	}
	
	
	private class ZebraPrinterBluetoothDiscovery implements Runnable {
		
		private ISearchPrinterListener mListener = null;
		private int mTimeout = 30000;
		private long mStartTimeInMillis = 0;
		
		public ZebraPrinterBluetoothDiscovery(int timeout, long startTime, ISearchPrinterListener listener) {
			mTimeout = timeout;
			mStartTimeInMillis = startTime;
			mListener = listener;
		}
		
		public void run() {
			
			try {
				Thread.sleep(10);
			} catch (InterruptedException e1) {

			}
			
			debugLogging("ZebraPrinterBluetoothDiscovery START !!!");

			//Utils.platformLog("@@@@ ZebraPrinterBluetoothDiscovery : ", "start");
			
			// firstly check already founded printers
			/*Iterator<ZebraPrinterImpl> iterator = mPrinters.iterator();
			while (iterator.hasNext()) {
				ZebraPrinterImpl prn = iterator.next();
				if (IPrinterDevice.CONNECTION_TYPE_BLUETOOTH.equals(prn.getConnectionType())) {
					if (prn.checkConnectionInCommandThread()) {
						//Utils.platformLog("@@@@ ZebraPrinterBluetoothDiscovery : ", "return printer from list");
						mListener.onFound(prn.getID());
					}
				}
			}*/			
			
	         DiscoveryHandler discoveryHandler = new DiscoveryHandler() {
	 
	             public void foundPrinter(DiscoveredPrinter printer) {
	            	 //Utils.platformLog("@@@@ ZebraPrinterBluetoothDiscovery : DiscoveryHandler", "foundPrinter");
                     debugLogging("foundPrinter >>>>>>>>>>>>>>");

                     java.util.Map<String,String> map = printer.getDiscoveryDataMap();
                     Iterator<String> iter = map.keySet().iterator();

                     //String mac_adress = null;//MAC_ADDRESS:00:22:58:38:81:B9
                     //String friendly_name = null;//FRIENDLY_NAME:Zebra220
                     while (iter.hasNext()) {
                         String key = iter.next();
                         String value = map.get(key);
                         if (key.equals("MAC_ADDRESS")) {
                             mac_adress = value;
                         }
                         if (key.equals("FRIENDLY_NAME")) {
                             friendly_name = value;
                         }
                         debugLogging("                "+key+":"+value);
                     }
                     debugLogging("foundPrinter <<<<<<<<<<<<<<<");
	            	 final Connection c = printer.getConnection();
	            	 //ZebraTransactionManager.addCommandBeforeAll(new ZebraPrinterDiscoverPrinterCommand(c, mListener));
       			     new Thread(new Runnable() {
							
					@Override
					public void run() {
						// TODO Auto-generated method stub
						ZebraPrinterImpl prn = new ZebraPrinterImpl("ZEBRA_PRINTER_"+String.valueOf(ourCounter++), c, c.toString(), mac_adress, friendly_name/*usbPrinter.device.getDeviceName()*/);
	       			     //this is Zebra printer !
	       			     		mPrinters.add(prn);
			     		     	mListener.onFound(prn.getID());
							}
							}).start();
	             }
	 
	             public void discoveryFinished() {
	            	 Utils.platformLog("@@@@ ZebraPrinterBluetoothDiscovery : DiscoveryHandler", "discoveryFinished");
	            	 if (ZebraTransactionManager.hasContainCommandsWithClass(ZebraPrinterDiscoverPrinterCommand.class)) {
	            		 ZebraTransactionManager.addCommand(new Runnable() {
	            			 public void run() {
	            				 mListener.onFinished(); 
	            			 }
	            		 });
	            	 }
	            	 else {
	            		 mListener.onFinished();
	            	 }
	             }
	 
	             public void discoveryError(String message) {
	            	 //Utils.platformLog("@@@@ ZebraPrinterBluetoothDiscovery : DiscoveryHandler", "discoveryError = "+message);
	                 mListener.onError(message);
	             }
	         };
	         try {
	        	 BluetoothDiscoverer.findPrinters(RhodesActivity.getContext(), discoveryHandler);
	        	 //Utils.platformLog("@@@@ ZebraPrinterBluetoothDiscovery : DiscoveryHandler", "start Zebra discovering");
	             //NetworkDiscoverer.findPrinters(discoveryHandler);
	         } catch (ConnectionException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				mListener.onError("catch ConnectionException !");
			}			
	
		}
	}
	
	
	@Override
	public String getType() {
		return IPrinterDevice.PRINTER_TYPE_ZEBRA;
	}
	
	public List<Object> getPrintersIDs() {
        List<Object> ids = new LinkedList<Object>();
		Iterator<ZebraPrinterImpl> iterator = mPrinters.iterator();
		while (iterator.hasNext()) {
			ZebraPrinterImpl prn = iterator.next();
			ids.add(prn.getID());
		}        
        return ids;			
	}
	
	
	private boolean isNetworkAvailable() {
		ConnectivityManager connectivityManager = (ConnectivityManager)RhodesActivity.getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
		return activeNetworkInfo != null && activeNetworkInfo.isConnected();
	}	
	

	public boolean searchPrinters(Map<String, Object> options, ISearchPrinterListener listener) {
		String connectionType = IPrinterDevice.CONNECTION_TYPE_ANY;
		String deviceAdress = null;
		String deviceConnectionPort = "6101";
		String printerType = null;
		int timeout = 30000;
		boolean btOnBoard = false;
		boolean btEnabled = false;
		boolean netEnabled = isNetworkAvailable();
		
		if (options != null) {
			
			Object valueObj = options.get(IPrinterDevice.HK_PRINTER_TYPE);
			if ((valueObj != null) && (valueObj instanceof String)) {
				printerType = (String)valueObj;
				if ((!printerType.equals(IPrinterDevice.PRINTER_TYPE_ZEBRA)) && (!printerType.equals(IPrinterDevice.PRINTER_TYPE_ANY)) ) {
					//listener.onError("incorrect printer type for search");
					//Utils.platformLog("@@@@ searchPrinters : ERROR", "incorrect printer type for search");
					return false; 
				}
			}
			valueObj = options.get(IPrinterDevice.HK_CONNECTION_TYPE);
			if ((valueObj != null) && (valueObj instanceof String)) {
				connectionType = (String)valueObj;
			}
			valueObj = options.get(IPrinterDevice.HK_DEVICE_ADDRESS);
			if (valueObj == null) {
				valueObj = options.get("address");
			}
			if ((valueObj != null) && (valueObj instanceof String)) {
				deviceAdress = (String)valueObj;
			}
			valueObj = options.get(IPrinterDevice.HK_DEVICE_PORT);
			if (valueObj == null) {
				valueObj = options.get("port");
			}
			if ((valueObj != null) && (valueObj instanceof Integer)) {
				deviceConnectionPort = ((Integer)valueObj).toString();
			}
			valueObj = options.get(IPrinterDevice.HK_TIMEOUT);
			if ((valueObj != null) && (valueObj instanceof Integer)) {
				timeout = ((Integer)valueObj).intValue();
				if (timeout <= 0) {
					listener.onError("ivalid timeout");
					return true;
				}
			}
		}
		
		//Utils.platformLog("@@@@ searchPrinters : options", options.toString());
		//Utils.platformLog("@@@@ searchPrinters : connectionType", connectionType);
		//Utils.platformLog("@@@@ searchPrinters : deviceAdress", deviceAdress);
		//Utils.platformLog("@@@@ searchPrinters : deviceConnectionPort", deviceConnectionPort);
		
		long curTimeInMillis = System.currentTimeMillis();
		
		BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		if (mBluetoothAdapter != null) {
			btOnBoard = true;
		    if (mBluetoothAdapter.isEnabled()) {
		        btEnabled = true;
		    }
		}
		
		if (connectionType.equals(IPrinterDevice.CONNECTION_TYPE_TCP) && (deviceAdress != null) && (deviceConnectionPort != null)) {
			// process connection to TCP adress
			if (!netEnabled) {
				listener.onError("TCP Network is not available!");
				return true;				
			}
			//Utils.platformLog("@@@@ searchPrinters :", "001");
			ZebraPrinterSearchPrinterListenerBridgeImpl listenerBridge = new ZebraPrinterSearchPrinterListenerBridgeImpl(listener, curTimeInMillis, timeout, 1);
			ZebraTransactionManager.addCommand(new ZebraPrinterTCPConnect(deviceAdress, deviceConnectionPort, listenerBridge));
			return true;
		}
		
		
		//process connection to Bluetooth adress
		if (connectionType.equals(IPrinterDevice.CONNECTION_TYPE_BLUETOOTH) && (deviceAdress != null)) {
			// process connection to TCP adress
			if (!btOnBoard) {
				listener.onError("Bluetooth is not exist on device !");
				return true;				
			}
			if (!btEnabled) {
				listener.onError("Bluetooth is not enabled !");
				return true;				
			}
			//Utils.platformLog("@@@@ searchPrinters :", "002");
			ZebraPrinterSearchPrinterListenerBridgeImpl listenerBridge = new ZebraPrinterSearchPrinterListenerBridgeImpl(listener, curTimeInMillis, timeout, 1);
			ZebraTransactionManager.addCommand(new ZebraPrinterBluetoothConnect(deviceAdress, listenerBridge));
			return true;
		}
		
		// discover printers (not adress defined)
		if (connectionType.equals(IPrinterDevice.CONNECTION_TYPE_ANY)) {
			int count = 0;
			if (netEnabled) {
				count = count + 1;
			}
			else {
				debugLogging("NETWORK IS NOT ENABLED !!!");
			}
			if (btOnBoard && btEnabled) {
				count = count + 1;
			}
			else {
				if (!btOnBoard) {
					debugLogging("BLUETOOTH IS NOT ON BOARD !!!");
				}
				if (!btEnabled) {
					debugLogging("BLUETOOTH IS NOT ENABLED !!!");
				}
			}
			count = count+1; //USB
			ZebraPrinterSearchPrinterListenerBridgeImpl listenerBridge = new ZebraPrinterSearchPrinterListenerBridgeImpl(listener, curTimeInMillis, timeout, count);
			if (((!btOnBoard) || (!btEnabled)) && (!netEnabled)) {
				//listener.onError("Bluetooth and TCP network is not available both !");
				debugLogging("BLUETOOTH AND TCP IS NOT ENABLED BOTH !!!");
				//return true;
			}
			if (netEnabled) {
				//Utils.platformLog("@@@@ searchPrinters :", "003");

				ZebraTransactionManager.addCommand(new ZebraPrinterTCPDiscovery(timeout, curTimeInMillis, listenerBridge));
			}
			if (btOnBoard && btEnabled) {
				//Utils.platformLog("@@@@ searchPrinters :", "004");

				ZebraTransactionManager.addCommand(new ZebraPrinterBluetoothDiscovery(timeout, curTimeInMillis, listenerBridge));
			}
			ZebraTransactionManager.addCommand(new ZebraPrinterUSBDiscovery(timeout, curTimeInMillis, listenerBridge));
			return true;
		}

		if (connectionType.equals(IPrinterDevice.CONNECTION_TYPE_USB)) {
			ZebraPrinterSearchPrinterListenerBridgeImpl listenerBridge = new ZebraPrinterSearchPrinterListenerBridgeImpl(listener, curTimeInMillis, timeout, 1);
			ZebraTransactionManager.addCommand(new ZebraPrinterUSBDiscovery(timeout, curTimeInMillis, listenerBridge));
			return true;
		}
		if (connectionType.equals(IPrinterDevice.CONNECTION_TYPE_TCP)) {
			if (!netEnabled) {
				listener.onError("TCP Network is not available!");
				return true;				
			}
			//Utils.platformLog("@@@@ searchPrinters :", "005");

			ZebraPrinterSearchPrinterListenerBridgeImpl listenerBridge = new ZebraPrinterSearchPrinterListenerBridgeImpl(listener, curTimeInMillis, timeout, 1);
			ZebraTransactionManager.addCommand(new ZebraPrinterTCPDiscovery(timeout, curTimeInMillis, listenerBridge));
			return true;
		}
		if (connectionType.equals(IPrinterDevice.CONNECTION_TYPE_BLUETOOTH)) {
			if (!btOnBoard) {
				listener.onError("Bluetooth is not exist on device !");
				return true;
			}
			if (!btEnabled) {
				listener.onError("Bluetooth is not enabled !");
				return true;
			}
			//Utils.platformLog("@@@@ searchPrinters :", "006");

			ZebraPrinterSearchPrinterListenerBridgeImpl listenerBridge = new ZebraPrinterSearchPrinterListenerBridgeImpl(listener, curTimeInMillis, timeout, 1);
			ZebraTransactionManager.addCommand(new ZebraPrinterBluetoothDiscovery(timeout, curTimeInMillis, listenerBridge));
			return true;
		}
		listener.onError("incorrect arguments for search");
		return true;
	}

	@Override
	public void stopSearching() {
		// TODO Auto-generated method stub
		Object[] list_ar = null;
		synchronized (mDiscoveryListeners) {
			list_ar = mDiscoveryListeners.toArray();
		}
		if (list_ar == null) {
			return;
		}
		int i;
		for (i = 0; i < list_ar.length; i++) {
			((IZebraPrinterSearchPrinterListenerBridge)list_ar[i]).forceStop();
		}
	}

	@Override
	public IPrinterDevice getPrinterByID(String printerID) {
		if (printerID == null) {
			return null;
		}
		//String fixed_id = printerID;
		//if (fixed_id.startsWith("RHOPRINTER_")) {
		//	fixed_id = fixed_id.substring("RHOPRINTER_".length());
		//}		
		Iterator<ZebraPrinterImpl> iterator = mPrinters.iterator();
		while (iterator.hasNext()) {
			ZebraPrinterImpl prn = iterator.next();
			if (printerID.equals(prn.getID())) {
				return prn;
			}
		}
		
		return null;
	}

	public IPrinterDevice getPrinterByZebraAdress(String address) {
		if (address == null) {
			return null;
		}
		Iterator<ZebraPrinterImpl> iterator = mPrinters.iterator();
		while (iterator.hasNext()) {
			ZebraPrinterImpl prn = iterator.next();
			if (address.equals(prn.getZebraAddress())) {
				return prn;
			}
		}
		
		return null;
	}





}
