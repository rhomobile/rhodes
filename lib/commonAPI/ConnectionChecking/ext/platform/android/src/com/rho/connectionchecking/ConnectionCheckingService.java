package com.rho.connectionchecking;



import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;


import com.rhomobile.rhodes.Logger;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.WebView;
import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.app.Service;

import android.content.Intent;
import android.os.IBinder;
import android.util.Patterns;
import android.webkit.URLUtil;



public class ConnectionCheckingService extends Service{

	static final String TAG="ConnectionCheckingService";
	private static final String[] selfLicensedDeviceList = {"mc18"};
	ConThread conThread=null;
	
	static ConnectionCheckingService sInstance=null;
	
	
	
	
	@Override
	public IBinder onBind(Intent arg0) {
		
		return null;
	}
	public static boolean isLicensePopupComing()
	{
		
		boolean res = false;
		try
		{
			Class.forName("com.symbol.enterprisebrowser.EBLicense", false, ContextFactory.getContext().getClassLoader());
			res = true; //EB will come
			//System.out.println(" isLicensePopupComing= "+res);
		}
		catch (ClassNotFoundException e)
		{
			//System.out.println("ClassNotFoundException isLicensePopupComing= "+res);
			Logger.E(TAG, "ClassNotFoundException,res="+res);
		}
		catch(Exception e)
		{
			Logger.E(TAG, "Exception,res="+res);
		}
		return res;
	}
	public static ConnectionCheckingService getInstance()
	{
		return sInstance;
	}
	public ConThread getConnectionThread()
	{
		return conThread;
	}
	/*public boolean shouldRestart()
	{
		
	}*/
	public void stopThread()
	{
		if(conThread!=null)
		conThread.runtheThread=false;
		stopSelf();
	}
	public void restartThread()
	{
		conThread=new ConThread();
		conThread.start();
	}
	public void handleLicenseCreate()
	{
		if(conThread.isDialoguePresent)
			
		 {
			killDialogue();
			conThread.isDialoguePresent=false;
			
		 }
		if(conThread.runtheThread)
			conThread.runtheThread=false;
		conThread.counterTimeout=0;	
		/*if(conThread.isAlive())
			conThread.stop();*/
		/*try{
		conThread.notify();
		}
		catch(Exception ex)
		{
			
		}*/
	}
@Override
public void onCreate() {
	
	//super.onCreate();
	
	try{
		//Log.d("Con","Service Oncreate");
		ConnectionCheckingDialogue.createBuilder(RhodesActivity.safeGetInstance());
		sInstance=this;
	
		if( (!isLicensePopupComing()) || isSelfLicensedDevice())// for self licensed device notification from license  screen won't come for starting thread...
		{
			conThread=new ConThread();
			conThread.start();
		}
		
	    }
	catch(Exception ex)
	{
		stopSelf();
	}
}
private boolean isSelfLicensedDevice()
{
	boolean selfLicense=false;
	for(String exemptDevice: selfLicensedDeviceList)
	{
		if(android.os.Build.MODEL.toLowerCase().contains(exemptDevice)) 
			selfLicense=true;
		 					
	}
	
	return selfLicense;
	}
private void showDialogue()
{
	PerformOnUiThread.exec(new Runnable(){

		@Override
		public void run() {
			ConnectionCheckingDialogue.show();
		}});
	
	//System.out.println("showDialogue()");
	Logger.I(TAG, "showDialogue");
			
}
private void killDialogue()
{
	PerformOnUiThread.exec(new Runnable()
	{

		@Override
		public void run() {
			
			ConnectionCheckingDialogue.hide();
			
		}
		
	});

	//System.out.println("killDialogue()");
	Logger.I(TAG, "killDialogue");
}
private void navigatetoBadLink()
{
	WebView.navigate(ConnectionCheckingSingleton.BADLINK_URI, 0);
}
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {

		return super.onStartCommand(intent, flags, startId);
	}
	class ConThread extends Thread{
		
		boolean runtheThread=true;
		
		
		int port=ConnectionCheckingSingleton.PORT;
		String host=ConnectionCheckingSingleton.HOST;
		
		boolean isDialoguePresent=false;
		
		int pingTimeout=1000;//hardcoded to 1 secs
		int counterTimeout=0;
		int to=0;
		long s=0;
		long f=0;
		
		public ConThread()
		{
			try{
				if(host.startsWith("http://"))
					host=host.substring(7);
				else if(host.startsWith("https://"))
					host=host.substring(8);
				if(host.contains(":"))
				{
					
					try{
						
						port=Integer.parseInt(host.split(":")[1]);
						host=host.split(":")[0];
						
					}
					catch(NumberFormatException ex)
					{
						
						Logger.E(TAG, "NumberFormatException in host split"+ex.getMessage());
						port=80;
					}
					
				}
				
			
			 
			 }
			 catch(Exception ex)
			 {
				
				 Logger.E(TAG, "Exception in COnstructor"+ex.getMessage());
				
			 }
		}
		
		
		private boolean isReachable(int timeOut)
		{
			boolean reachable=false;
			
			Socket socket = new Socket();
			
			
			try
			{
			
				System.out.println("before socketAddress...host="+host+" AND  port"+port);
				SocketAddress socketAddress = new InetSocketAddress(host,port);
				s=System.currentTimeMillis();
				// this method will block no more than timeout ms.
				socket.connect(socketAddress, 1000);
				f=System.currentTimeMillis();
				counterTimeout= counterTimeout+(int)(f-s);
			}
			catch (Exception e)
			{
				f=System.currentTimeMillis();
				System.out.println("Exception in connect..."+e.getMessage());
				counterTimeout= counterTimeout+(int)(f-s);
				Logger.E(TAG, "Exception in connect..."+e.getMessage());
			}
			finally
			{
				try
				{
					if (socket.isConnected())
					{
						
						reachable=true;
						socket.close();
					}
					else
						{
							
							reachable=false;
							socket.close();
						}
				}
				catch (Exception e)
				{
					e.printStackTrace();
					
					Logger.E(TAG,"Exception in finally..."+e.getMessage());
				}
			}
			
			
			return reachable;
		}
		
		
		
		
		
		public void run()
		{
			
			
			while(runtheThread)
			{
				System.out.println("Thread ID="+Thread.currentThread().getId()+" runtheThread="+runtheThread);
				if(isReachable(pingTimeout))
				{
					
					if(isDialoguePresent)
						{
							killDialogue();
							isDialoguePresent=false;
						}
					counterTimeout=0;
				}
				else
				{
		
					if(counterTimeout>=ConnectionCheckingSingleton.TIMEOUT)
					{
						
						navigatetoBadLink();
						killDialogue();
						counterTimeout=0;
						runtheThread=false;
						isDialoguePresent=false;
						break;
					}
					else
					{
						
						if(!isDialoguePresent)
							showDialogue();
						
						isDialoguePresent=true;
						//counterTimeout=counterTimeout+pingTimeout;
					}
				}
				
				try {
					Logger.I(TAG,"Thread is going to sleep");
					int sleepTime=(ConnectionCheckingSingleton.TIMEOUT-counterTimeout<ConnectionCheckingSingleton.POLLI_INTERVAL)?(ConnectionCheckingSingleton.TIMEOUT-counterTimeout-(int)(f-s)):ConnectionCheckingSingleton.POLLI_INTERVAL;
					sleep(sleepTime);
					Logger.I(TAG,"Thread is awaken");
					if(counterTimeout!=0)
						counterTimeout=counterTimeout+sleepTime;
				} catch (InterruptedException e) {
					
					e.printStackTrace();
					
				}
				catch(Exception e)
				{
					
					Logger.E(TAG, "excetion in Thread run()"+e.getMessage());
				}
			}
			
		if(isDialoguePresent)
		{
			killDialogue();
			isDialoguePresent=false;
			counterTimeout=0;
		}	
		
		System.out.println("Thread is about to exit");
		}
		
	}
	
	@Override
	public void onDestroy() {
		try{
		if(conThread.isAlive())
			{
				try{
					//conThread.destroy();
				conThread.runtheThread=false;
				
		
				}
				catch(Exception ex)
				{
					
				}
			}
		
		conThread=null;
		super.onDestroy();
		}
		catch(Exception ex)
		{
			super.onDestroy();
		}
	}
	
}
class ConnectionPageNavEvent extends AbstractRhoExtension
{
	private final String TAG="ConnectionPageNavEvent";
	String criticalUri="";
	boolean isPageNavigationError=false;
	static String curLink="";
	public static boolean isCurLinkBadLink()
	{
		boolean badlink=false;
		String chopBadLink=ConnectionCheckingSingleton.BADLINK_URI.substring(8);
		
		if(curLink.contains(chopBadLink))
			badlink=true;
		
		
		return badlink;
	}
	
	@Override
	public boolean onNewConfig(IRhoExtManager extManager,
			IRhoConfig config, String name, boolean res) {
		System.out.println("ConnectionCheckingListener----onNewConfig,name="+name);
		try{
		if(name.equalsIgnoreCase("rhoelementsext"))
		{
			 String ip=config.getString("hosturl");
		     String to=config.getString("timeout");
		     String tc=config.getString("trackconnection");
		     String msg=config.getString("message");
		     String pi=config.getString("pollinterval");
		     String badlinkUrl=config.getString("badlinkuri");
				
		     System.out.println("onNewConfig...HostURL="+ip+" Timeout"+to+"TrackConnection "+tc+"Message"+msg+"PollInterval"+pi);
			 boolean shouldRestart=shouldRestartService(ip,to,tc,msg,pi);
				
				ConnectionCheckingSingleton.HOST=ip;
				if(!(Patterns.WEB_URL.matcher(ConnectionCheckingSingleton.HOST).matches()))
					ConnectionCheckingSingleton.HOST="192.168.7.29";
				ConnectionCheckingSingleton.MESSAGE=msg;
				ConnectionCheckingSingleton.BADLINK_URI=badlinkUrl;
				try{
				ConnectionCheckingSingleton.TIMEOUT=Integer.parseInt(to);
				if(ConnectionCheckingSingleton.TIMEOUT<30000)
					ConnectionCheckingSingleton.TIMEOUT=30000;
				}
				catch(NumberFormatException ex)
				{
					Logger.E(TAG, ex.getMessage());
					ConnectionCheckingSingleton.TIMEOUT=30000;
					
				}
				try{
				ConnectionCheckingSingleton.POLLI_INTERVAL=Integer.parseInt(pi);
				if(ConnectionCheckingSingleton.POLLI_INTERVAL<5000)
					ConnectionCheckingSingleton.POLLI_INTERVAL=5000;
				}
				catch(NumberFormatException ex)
				{
					Logger.E(TAG, ex.getMessage());
					ConnectionCheckingSingleton.POLLI_INTERVAL=5000;
				}
				try{
				ConnectionCheckingSingleton.TRACK_CONNECTION=Integer.parseInt(tc);
				
				}
				catch(NumberFormatException ex)
				{
					Logger.E(TAG, ex.getMessage());
					ConnectionCheckingSingleton.TIMEOUT=0;
				}
				
				if((ConnectionCheckingSingleton.TIMEOUT/ConnectionCheckingSingleton.POLLI_INTERVAL)< 3)
				{
					ConnectionCheckingSingleton.TIMEOUT=30000;
					ConnectionCheckingSingleton.POLLI_INTERVAL=5000;
				}
				
				
				try{ 
		 	   if(shouldRestart)
		 	   {									   
		     	Intent conIntent=new Intent(RhodesActivity.getContext(),ConnectionCheckingService.class);
		     	
		     	RhodesActivity.getContext().stopService(conIntent);
		     	if(ConnectionCheckingSingleton.TRACK_CONNECTION!=0)
		     		RhodesActivity.getContext().startService(conIntent);
		     	
		 	   }
		    }
		    catch(NumberFormatException ex)
		    {
		 	   System.out.println("NumberFormatException...in TrackConnection.Not starting the Service,error="+ex.getMessage());
		    }
		    
		}
		
    
		}
		catch(Exception ex)
		{
			
		}
		
		
		return super.onNewConfig(extManager, config, name, res);
		
		
		
		
		
		
	}
	
	//////
	private boolean shouldRestartService(String ip,String to,String tc,String msg,String pi)
	{
		boolean restart=false;
		try{
		if(!(ip.equalsIgnoreCase(ConnectionCheckingSingleton.HOST) && Integer.parseInt("to")==ConnectionCheckingSingleton.TIMEOUT && Integer.parseInt(tc)==ConnectionCheckingSingleton.TRACK_CONNECTION && msg.equalsIgnoreCase(ConnectionCheckingSingleton.MESSAGE) && Integer.parseInt(pi)==ConnectionCheckingSingleton.POLLI_INTERVAL))
			restart=true;
		}
		catch(NumberFormatException ex)
		{
			Logger.E(TAG, ex.getMessage());
		}
		System.out.println("restartService="+restart);
		return restart;
	}
	
	@Override
	public boolean onNavigateComplete(IRhoExtManager extManager,
			String url, IRhoWebView ext, boolean res) {
		
		System.out.println("ConnectionPageNavEvent...onNavigateComplete,url="+url);
		Logger.I(TAG, "ConnectionPageNavEvent...onNavigateComplete,url="+url);
		curLink=url;
		if(isPageNavigationError==true)
			criticalUri=url;
		else
			criticalUri="";
		if(ConnectionCheckingService.getInstance()!=null && ConnectionCheckingService.getInstance().getConnectionThread()!=null && !ConnectionCheckingService.getInstance().getConnectionThread().isAlive())
		{
			
			if(shouldThreadStart(url) )
					{
						//conThread=new ConThread();
						//conThread.start();
						ConnectionCheckingService.getInstance().restartThread();
					}
			
		}
		return super.onNavigateComplete(extManager, url, ext, res);
	}
	private boolean shouldThreadStart(String url)
	{
		boolean start=true;
		System.out.println("BADLINK_URI="+ConnectionCheckingSingleton.BADLINK_URI+" url ="+url);
		String chopBadLink=ConnectionCheckingSingleton.BADLINK_URI.substring(8);
		
		if(url.contains(chopBadLink))
			{
				start=false;
				criticalUri="";
				isPageNavigationError=false;
			}
		else if(isPageNavigationError==true )
			start=false;
		
		System.out.println("shouldThreadstart="+start);
		return start;
	}
	@Override
	public boolean onNavigateError(IRhoExtManager extManager, String url,
			LoadErrorReason reason, IRhoWebView ext, boolean res) {
		System.out.println("New onNavigateError,url="+url);
		isPageNavigationError=true;
		return super.onNavigateError(extManager, url, reason, ext, res);
	}
}
