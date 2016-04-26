package com.rho.rhoelements.services;

import org.json.JSONArray;

import com.rho.rhoelements.Common;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

/**
 * 
 * @author FPC843
 * Factory class returning the appropriate scanner service based on whether the device supports emdk or not
 */
public class ScannerService extends Service
{
    public static final String SCANNER_TRIGGER_EVENT = "com.rho.rhoelements.services.ScannerService.SCAN_TRIGGER";
    static private IScannerImplementation scannerServiceImplementation;
     
    //This is the object that receives interactions from clients.
    private final IBinder mBinder = new ScannerBinder();
    
    /**
     * 
     */
    public ScannerService()
    {
	super();
	if (Common.isEmdkDevice())
	{
		
		//scannerServiceImplementation = new EmdkScannerServiceRebranded(this);
		scannerServiceImplementation = new StubScannerService(this);
		
	}
	else
	    scannerServiceImplementation = new StubScannerService(this);
    }

    
    @Override
    public void onCreate()
    {
	scannerServiceImplementation.onCreate();
    }




    @Override
    public IBinder onBind(Intent intent)
    {
	return mBinder;
    }
    
    
    
    /* (non-Javadoc)
     * @see android.app.Service#onDestroy()
     */
    @Override
    public void onDestroy()
    {
	scannerServiceImplementation.onDestroy();
	super.onDestroy();
    }

    /* (non-Javadoc)
     * @see android.app.Service#onStartCommand(android.content.Intent, int, int)
     */
    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
	return scannerServiceImplementation.onStartCommand(intent, flags, startId);
    }

    /* (non-Javadoc)
     * @see android.app.Service#onUnbind(android.content.Intent)
     */
    @Override
    public boolean onUnbind(Intent intent)
    {
	return scannerServiceImplementation.onUnbind(intent);
    }

    /**
     * A convenience method to allow a connection to bind with this service 
     * @param connection the connection to use to bind with this service.
     */
    public static boolean bind(ServiceConnection connection)
    {
	Intent bindIntent = new Intent(Common.mainActivity.getApplicationContext(), com.rho.rhoelements.services.ScannerService.class);
	return Common.mainActivity.getApplicationContext().bindService(bindIntent, connection, Context.BIND_AUTO_CREATE);
    }
    
    public void registerClient(IScannerClient client)
    {
	scannerServiceImplementation.registerClient(client);
    }
    
    public void clearSettings(IScannerClient client)
    {
	scannerServiceImplementation.registerClient(client);
    }
    
    public JSONArray enumerateScanners()
    {
	return scannerServiceImplementation.enumerateScanners();
    }
    
    public void removeScannerUser(IScannerClient scannerClient, boolean isReset)
    {
	scannerServiceImplementation.removeScannerUser(scannerClient, isReset);
    }
    
    public void startScanner(IScannerClient client)
    {
	scannerServiceImplementation.startScanner(client);
    }
    
    public void stopScanner(IScannerClient client)
    {
	scannerServiceImplementation.stopScanner(client);
    }
    
    public void addDecodeSetting(IScannerClient client, String decoder, String setting)
    {
	scannerServiceImplementation.addDecodeSetting(client, decoder, setting);
    }
    
    public void setScanner(IScannerClient client, String scanner)
    {
	scannerServiceImplementation.setScanner(client, scanner);
    }
    
    public String getDefaultScannerId()
    {
	return scannerServiceImplementation.getDefaultScannerId();
    }
    
    public void enableScanner(IScannerClient client)
    {
	scannerServiceImplementation.enableScanner(client);
    }
    
    public void disableScanner(IScannerClient client)
    {
	scannerServiceImplementation.disableScanner(client);
    }
    
    public boolean pauseScanner()
    {
	return scannerServiceImplementation.pauseScanner();
    }
    
    public void resumeScanner()
    {
	scannerServiceImplementation.resumeScanner();
    }
    
    /**
     * Class for clients to access.  Because we know this service always
     * runs in the same process as its clients, we don't need to deal with
     * IPC.
     */
    public class ScannerBinder extends Binder
    {
	public ScannerService getService()
	{
	    return ScannerService.this;
	}
    }
    
    /**
     * An interface that provides an event that will be called when a barcode scan event occurs.
     * This can be registered with the ScannerService.
     * @author Ben Kennedy (NCVT73)
     */
	public interface IScannerClient
	{
		/**
		 * An event called when a scan is received by the scanner service.
		 * @param barcode The data decoded by the scanner
		 * @param source The source device and human readable decoder type of the decoded barcode or symbol //TODO check that this is returned
		 * @param type Hex value representing the decoder type
		 * @param time the time of the scan in the format: "hh:mm:ss"
		 * @param length The length of the decoded barcode or symbol
		 * @param event The type of event which has occured at the scanner. This field will be either 'Decode' for barcode scanning or a message from a Bluetooth scanner.
		 */
		public abstract void onScanEvent(String barcode, String source, String type, String time, String length, String event);
	}
	
}
