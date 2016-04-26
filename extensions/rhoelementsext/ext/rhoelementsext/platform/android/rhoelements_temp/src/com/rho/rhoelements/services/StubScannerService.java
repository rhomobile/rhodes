package com.rho.rhoelements.services;

import java.security.InvalidParameterException;

import org.json.JSONArray;

import com.rho.rhoelements.services.ScannerService.IScannerClient;

import android.content.Intent;
import android.os.IBinder;

public class StubScannerService implements IScannerImplementation
{
    
    public StubScannerService(ScannerService scannerService){} 
    
    @Override
    public IBinder onBind(Intent arg0)
    {
	return null;
    }

    @Override
    public boolean pauseScanner()
    {
	return true;
    }

    @Override
    public void resumeScanner()
    {
    }

    @Override
    public String getDefaultScannerId() {
	// TODO Auto-generated method stub
	return null;
    }

    @Override
    public JSONArray enumerateScanners() {
	// TODO Auto-generated method stub
	return null;
    }

    @Override
    public void registerClient(IScannerClient client) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void addDecodeSetting(IScannerClient client, String decoder,
	    String setting) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void setScanner(IScannerClient client, String scanner)
	    throws InvalidParameterException {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void enableScanner(IScannerClient client) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void disableScanner(IScannerClient client) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void removeScannerUser(IScannerClient scannerClient, boolean isReset) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void clearSettings(IScannerClient client) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void startScanner(IScannerClient client) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void stopScanner(IScannerClient client) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public boolean onUnbind(Intent intent) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public void onDestroy() {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onCreate() {
	// TODO Auto-generated method stub
	
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
	// TODO Auto-generated method stub
	return 0;
    }

}
