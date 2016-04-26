package com.rho.device;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.LinkedList;
import java.util.List;
import android.util.Log;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
//import android.content.ServiceConnection;
//import android.os.IBinder;
import android.os.PowerManager;
import android.os.RemoteException;

import com.rhomobile.rhodes.RhodesActivity;

import com.rhomobile.rhodes.api.IMethodResult;

class DeviceSingleton  implements IDeviceSingleton {
	
//	 private static final String XML_CONFIG_FILE ="reboot.xml";
//	 private static IMxFrameworkService mMxFrameworkService;
    public DeviceSingleton(DeviceFactory factory) {
       
    }
    
    
   /* private ServiceConnection mAidlMxFrameworkServiceConnection = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {


			mMxFrameworkService = IMxFrameworkService.Stub.asInterface(service);
			
		
			  String inputXML=getXml(XML_CONFIG_FILE);
			 try {
			String retString = mMxFrameworkService.processXML(inputXML);
				Log.d("rhomx","retSTring="+retString);
			} catch (RemoteException e) {
				Log.d("rhomx","exception ex="+e.getMessage());
				e.printStackTrace();
			}
		 
		 
}

		@Override
		public void onServiceDisconnected(ComponentName className) {
			
			mMxFrameworkService = null;
			
		}
	};

*/

	@Override
	public void calibrate(IMethodResult result) {
		
		
	}

	@Override
	public void suspend(IMethodResult result) {
		
		
	}

	@Override
	public void powerOff(IMethodResult result) {
		
		
	}

	@Override
	public void idle(IMethodResult result) {
		
		
	}

	@Override
	public void wake(IMethodResult result) {
		PowerManager pm = (PowerManager) RhodesActivity.safeGetInstance().getSystemService(Context.POWER_SERVICE);
		 PowerManager.WakeLock wake = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP|PowerManager.FULL_WAKE_LOCK, "MyTag");
		 Log.d("symbol","about to wake Rho");
		 wake.acquire(5000);
	}

	@Override
	public void reboot(String bootType, IMethodResult result) {
	/*	Log.d("symbol"," reboot,xml="+getXml(XML_CONFIG_FILE));
		
		
		 Intent bindServiceIntent = new Intent();
			// Send explicit intent
			bindServiceIntent.setComponent(new ComponentName(MX_FRAMEWORK_PKG, MX_FRAMEWORK_SERVICE_CLS));
			boolean bBind = RhodesActivity.safeGetInstance().bindService(bindServiceIntent, mAidlMxFrameworkServiceConnection, Context.BIND_AUTO_CREATE);
		
	*/	 
		
	}
/*	 private String getXml(String fileName) {
		 
		// String temp="<wap-provisioningdoc><characteristic version=\"0.4\" type=\"PowerMgr\"><parm name=\"ResetAction\" value=\"4\" /></characteristic></wap-provisioningdoc>";
			BufferedReader br;

			// convert xml file to string
			try {
				
				 br=new BufferedReader(new InputStreamReader(RhodesActivity.safeGetInstance().getAssets().open(fileName)));

				String line;
				StringBuilder sb = new StringBuilder();

				while ((line = br.readLine()) != null) {
					sb.append(line.trim());
				}
				br.close();
				Log.d("rhomx", "getXml: >>>>>>>>>>>>>>>sb.toString() = " + sb.toString());
				return sb.toString();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return null;
			
			
		// return temp;
		}
		*/
	  	
}
