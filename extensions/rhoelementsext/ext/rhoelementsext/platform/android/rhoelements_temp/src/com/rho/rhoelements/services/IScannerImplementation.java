/**
 * 
 */
package com.rho.rhoelements.services;

import java.security.InvalidParameterException;

import org.json.JSONArray;

import android.content.Intent;
import android.os.IBinder;

import com.rho.rhoelements.services.ScannerService.IScannerClient;

/**
 * @author FPC843
 *
 */
public interface IScannerImplementation
{
    /**
     * Resumes the paused scanner. Only call if a previous call to pauseScanner returned true.
     */
    public abstract void resumeScanner();
    
    /**
     * Pauses the scanner, removing the scanning window. Allows resuming by "resumeScanner()"
     * @return whether the scanner was active (window was shown)
     */
    public abstract boolean pauseScanner();
    
    /**
     * Registers a client (a module implementing IScannerClient) with the ScannerService.
     * This allows the scanning service to callback the client with scan decode events.
     * @param client the module to register with the ScannerService
     */
    public abstract void registerClient(IScannerClient client);
    
    /**
     * Adds a decode setting to the user that is identified by the identifier parameter
     * @param identifier identifier the toString() value of the calling object. Used to identify
     * who is calling the scanner service. 
     * @param decoder the name of the decoder upon which to set values.
     * @param setting the value of which to set the decoder.
     * @return whether the decode setting was successfully applied.
     */
    public abstract void addDecodeSetting(IScannerClient client, String decoder, String setting);
    
    /**
     * Sets which scanner is to be used by a specific user.
     * @param identifier the toString() value of the calling object. Used to identify
     * who is calling the scanner service.
     * @param scanner The ID of the scanner to be used. This presumes all valid scanner id's are of the
     * form ???n where n is a single or multi digit integer.
     * @throws InvalidParameterException if the scanner does not exist
     */
    public abstract void setScanner(IScannerClient client, String scanner) throws InvalidParameterException;
    
    /**
     * Enable the scanner for the client passed in input
     * @param client reference to the client using the scanner
     */
    public abstract void enableScanner(IScannerClient client);
    
    /**
     * Disable the scanner for the client passed in input
     * @param client reference to the client using the scanner
     */
    public abstract void disableScanner(IScannerClient client);
    
    /**
     * Returns the ID of the default scanner, specific to the device
     * @return the scannerID, with the format "SCNx" where x is a single or multi digit number
     */
    public abstract String getDefaultScannerId();
    
    /**
     * Removes the user and all their settings from the service.
     * @param scannerClient the calling object.
     * @param isReset whether the user is to be totally removed (false) (i.e. the licence page has completed)
     * or whether the call is a simple scanner:disabled call (true)
     */
    public abstract void removeScannerUser(IScannerClient scannerClient, boolean isReset);
    
    /**
     * Clears all of the decoder settings from a user, identified by the identifier parameter.
     * @param identifier the toString() value of the calling object. Used to identify
     * who is calling the scanner service. 
     */
    public abstract void clearSettings(IScannerClient client);
    
    /**
     * Enumerates all of the compatible scanners on the device together with the scanner description.
     * @return a JSONArray containing each scanner name together with a description
     * of each scanner.
     */
    public abstract JSONArray enumerateScanners();
    
    /**
     * Starts the scanner with the stored preferences of the user.
     * @param client the object to call "onScanEvent" when a scan is received.
     * @throws InvalidParameterException if the client is not registered with the ScannerService
     */
    public abstract void startScanner(IScannerClient client);
    
    /**
     * Stops the scanner from scanning if there is only one user currently using the scanner,
     * Otherwise removes the current user, leaves the preview window available
     * and changes the parameters and settings to those of the next user's. 
     */
    public abstract void stopScanner(IScannerClient client);
    
    /**
     * @See android.app.Service.onBind
     */
    public abstract IBinder onBind(Intent intent);
    
    /**
     * @See android.app.Service.onBind
     */
    public abstract boolean onUnbind(Intent intent);
    
    /**
     * @See android.app.Service.onBind
     */
    public abstract void onDestroy();
    
    /**
     * @See android.app.Service.onBind
     */
    public abstract void onCreate();
    
    /**
     * @See android.app.Service.onBind
     */
    public abstract int onStartCommand(Intent intent, int flags, int startId);
}
