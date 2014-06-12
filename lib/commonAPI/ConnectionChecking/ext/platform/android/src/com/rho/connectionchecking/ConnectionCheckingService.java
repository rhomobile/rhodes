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

import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.app.Service;

import android.content.Intent;
import android.os.IBinder;



public class ConnectionCheckingService extends Service{

	static final String TAG="ConnectionCheckingService";
	ConThread conThread=null;
	
	static ConnectionCheckingService sInstance=null;
	
	
	
	
	@Override
	public IBinder onBind(Intent arg0) {
		
		return null;
	}
	
	public static ConnectionCheckingService getInstance()
	{
		return sInstance;
	}
	public ConThread getConnectionThread()
	{
		return conThread;
	}
	public void restartThread()
	{
		conThread=new ConThread();
		conThread.start();
	}
	
@Override
public void onCreate() {
	
	//super.onCreate();
	
	ConnectionCheckingDialogue.createBuilder(RhodesActivity.safeGetInstance());
	sInstance=this;
	conThread=new ConThread();
	conThread.start();
	
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

				// this method will block no more than timeout ms.
				socket.connect(socketAddress, 1000);
			}
			catch (Exception e)
			{
				System.out.println("Exception in connect..."+e.getMessage());
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
		
					if(counterTimeout>ConnectionCheckingSingleton.TIMEOUT)
					{
						
						navigatetoBadLink();
						killDialogue();
						counterTimeout=0;
						runtheThread=false;
						break;
					}
					else
					{
						
						if(!isDialoguePresent)
							showDialogue();
						
						isDialoguePresent=true;
						counterTimeout=counterTimeout+pingTimeout;
					}
				}
				
				try {
					
					sleep(ConnectionCheckingSingleton.POLLI_INTERVAL);
					if(counterTimeout!=0)
						counterTimeout=counterTimeout+5000;
				} catch (InterruptedException e) {
					
					e.printStackTrace();
				}
				catch(Exception e)
				{
					
					Logger.E(TAG, "excetion in Thread run()"+e.getMessage());
				}
			}
		
		System.out.println("Thread is about to exit");
		}
		
	}
	
	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		
		if(conThread.isAlive())
			conThread.destroy();
		
		conThread=null;
		super.onDestroy();
	}
	
}
class ConnectionPageNavEvent extends AbstractRhoExtension
{
	private final String TAG="ConnectionPageNavEvent";
	String criticalUri="";
	boolean isPageNavigationError=false;
@Override
	public boolean onNewConfig(IRhoExtManager extManager,
			IRhoConfig config, String name, boolean res) {
		System.out.println("ConnectionCheckingListener----onNewConfig,name="+name);
		if(name.equalsIgnoreCase("rhoelementsext"))
		{
			 String ip=config.getValue("hosturl");
		     String to=config.getValue("timeout");
		     String tc=config.getValue("trackconnection");
		     String msg=config.getValue("message");
		     String pi=config.getValue("pollinterval");
		     String badlinkUrl=config.getValue("badlinkuri");
				
		     System.out.println("onNewConfig...HostURL="+ip+" Timeout"+to+"TrackConnection "+tc+"Message"+msg+"PollInterval"+pi);
			 boolean shouldRestart=shouldRestartService(ip,to,tc,msg,pi);
				
				ConnectionCheckingSingleton.HOST=ip;
				ConnectionCheckingSingleton.MESSAGE=msg;
				ConnectionCheckingSingleton.BADLINK_URI=badlinkUrl;
				try{
				ConnectionCheckingSingleton.TIMEOUT=Integer.parseInt(to);
				ConnectionCheckingSingleton.POLLI_INTERVAL=Integer.parseInt(pi);
				ConnectionCheckingSingleton.TRACK_CONNECTION=Integer.parseInt(tc);
				}
				catch(NumberFormatException ex)
				{
					Logger.E(TAG, ex.getMessage());
				}
				
				try{ 
		 	   if(shouldRestart)
		 	   {									   
		     	Intent conIntent=new Intent(RhodesActivity.getContext(),ConnectionCheckingService.class);
		     	
		     	RhodesActivity.getContext().stopService(conIntent);
		     	if(ConnectionCheckingSingleton.TRACK_CONNECTION==1)
		     		RhodesActivity.getContext().startService(conIntent);
		     	
		 	   }
		    }
		    catch(NumberFormatException ex)
		    {
		 	   System.out.println("NumberFormatException...in TrackConnection.Not starting the Service,error="+ex.getMessage());
		    }
		    
		}
		
    
		
		
		
		return super.onNewConfig(extManager, config, name, res);
		
		
		
		
		
		
	}
	
	
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
