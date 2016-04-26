package com.rho.devicememory;

import java.io.File;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;

import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;

public class DeviceMemoryListener extends AbstractRhoListener{
	public static long toalPhysicalMemory = 0;
	public static long availablePhysicalMemory = 0;
	public static long internalMemory = 0;
	public static long externalMemory =0;
	public static int memoryLoad =0;

	private int timeInterval  = 5000; 
	//private Handler handler =  new Handler();
	DeviceMemorySingleton deviceMemorySingleton = null;
	private final String TAG="LogMemory";

	
	@Override
	public void onCreateApplication(IRhoExtManager extManager) {
		extManager.addRhoListener(this);
		deviceMemorySingleton = new DeviceMemorySingleton();
		MyLogmemoryExtension myLogmemoryExtension = new MyLogmemoryExtension();
		extManager.registerExtension("MyLogMemoryExt",myLogmemoryExtension);
		  

	}


	class MyLogmemoryExtension extends AbstractRhoExtension{
		
		@Override
		 public boolean onNewConfig(IRhoExtManager extManager, IRhoConfig config, String name, boolean res) {
			
					if(config.isExist("logmemperiod") && name.equalsIgnoreCase("rhoelementsext")){
				String logMemoryPeriodValue = config.getString("logmemperiod");
				if(logMemoryPeriodValue != null){
					timeInterval = Integer.parseInt(logMemoryPeriodValue);
	        	}
				
			}
			
			if(config.isExist("logmemory")  && name.equalsIgnoreCase("rhoelementsext")){
				String logMemoryValue = config.getString("logmemory");
				
				if(logMemoryValue != null){
					if(logMemoryValue.contains("1")){
						createHandler();
					}
	        	}
				
			}
			
	        return res;
	    } 
		
		private void createHandler() {
			
	       	   
	        Thread thread = new Thread() {
	          public void run() {
	               Looper.prepare();

	               final Handler handler = new Handler();
	               
	           
		               handler.postDelayed(new Runnable() {
		                    @Override
		                    public void run() {
		                    	while(true){
		                       // Do Work
		                    		try {
										Thread.currentThread().sleep(timeInterval);
										
										deviceMemorySingleton.getAvailableMemory(null);
										deviceMemorySingleton.getExternalStorage(null);
										deviceMemorySingleton.getInternalStorage(null);
										memoryLoad = (int) (((toalPhysicalMemory-availablePhysicalMemory)*100)/toalPhysicalMemory) ; 
										
										Logger.I(TAG, " MEMORY| Stats: Load ="+ memoryLoad +  "%  " + "TotalPhy = " + toalPhysicalMemory +" KB" +"  FreePhy= " +
												availablePhysicalMemory +" KB" + "  Total Internal Memory = "+ internalMemory +" KB" + " Total External Memory = "+ externalMemory +" KB");
										
									} catch (InterruptedException e) {
										// TODO Auto-generated catch block
										e.printStackTrace();
									}
		                    	
		                    	
		                        handler.removeCallbacks(this);
		                        Looper.myLooper().quit();
		                    	}
		                   }
		                }, 0);
	              
	                Looper.loop();
	            }
	        };
	        thread.start();
	    }
		
	}
	
	

}
