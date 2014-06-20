package com.rhomobile.rhodes;


import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.UnknownHostException;

import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;


public class ConnectionCheckingService extends Service{

	static final String TAG="ConnectionCheckingService";
	ConThread conThread=null;
	ConnectionPageNavEvent ext=null;
	Context ctx;
	boolean isPageNavigationError=false;
	int isPageNavigationErrorCounter=0;
	int count=0;
	String badlinkUri="";
	String criticalUri="";
	
	@Override
	public IBinder onBind(Intent arg0) {
		
		return null;
	}
@Override
public void onCreate() {
	
	//super.onCreate();
	RhoAlertDialogue.createBuilder(RhodesActivity.safeGetInstance());
	//System.out.println("BadLink URI="+RhoConf.getString("BadLinkURI"));
	badlinkUri=RhoConf.getString("BadLinkURI");
	ext=new ConnectionPageNavEvent();
	RhoExtManager.getInstance().registerExtension("ConnectionCheckingService", ext);
	ctx=getApplicationContext();
	
	conThread=new ConThread();
	conThread.start();
	
}
private void showDialogue()
{
	PerformOnUiThread.exec(new Runnable(){

		@Override
		public void run() {
			RhoAlertDialogue.show();
		}});
	
	System.out.println("showDialogue()");
			
}
private void killDialogue()
{
	PerformOnUiThread.exec(new Runnable()
	{

		@Override
		public void run() {
			
			RhoAlertDialogue.hide();
			
		}
		
	});

	System.out.println("killDialogue()");
	
}
private void navigatetoBadLink()
{
	WebView.navigate(badlinkUri, 0);
}
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {

		return super.onStartCommand(intent, flags, startId);
	}
	class ConThread extends Thread{
		
		boolean runtheThread=true;
		
		String host="192.168.7.129";//DUmmy
		String message="Establishing Connection";
		int timeout=30000;
		int pollinterval=5000;
		int port=80;
		
		
		boolean isDialoguePresent=false;
		
		int pingTimeout=1000;//hardcoded to 1 secs
		int counterTimeout=0;
		int to=0;
		public ConThread()
		{
			try{
			
				host=RhoConf.getString("HostURL");
				//System.out.println("In the constructor of thread...host="+host);
				if(host.startsWith("http://"))
					host=host.substring(7);
				else if(host.startsWith("https://"))
					host=host.substring(8);
				if(host.contains(":"))
				{
					//System.out.println("host contains :");
					try{
						
						port=Integer.parseInt(host.split(":")[1]);
						host=host.split(":")[0];
						//System.out.println("port after splitting is ="+port);
					}
					catch(NumberFormatException ex)
					{
						//System.out.println("PORT NumberFormatException="+ex.getMessage());
						Logger.E(TAG, "NumberFormatException in host split"+ex.getMessage());
						port=80;
					}
					
				}
				
				message=RhoConf.getString("Message");
				try{
				timeout=Integer.parseInt(RhoConf.getString("Timeout"));
				}
				catch(NumberFormatException ex)
				{
					//System.out.println("NumberFormatException ex="+ex.getMessage());
					Logger.E(TAG, "NumberFormatException in Timeout"+ex.getMessage());
					timeout=30000;
				}
				try{
					pollinterval=Integer.parseInt(RhoConf.getString("PollInterval"));
				}
				catch(NumberFormatException ex)
				{
					//System.out.println("NumberFormatException ex="+ex.getMessage());
					Logger.E(TAG, "NumberFormatException in PollInterval"+ex.getMessage());
					pollinterval=5000;
				}
			
			 
			 
			 }
			 catch(Exception ex)
			 {
				//System.out.println("Exception in constructor,"+ex.getMessage());
				 Logger.E(TAG, "Exception in COnstructor"+ex.getMessage());
				
			 }
		}
		
		
		private boolean isReachable(int timeOut)
		{
			boolean reachable=false;
			InetAddress iNetAddress = null;
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
						//System.out.println("Socket Connected");
						reachable=true;
						socket.close();
					}
					else
						{
							//System.out.println("Socket NOT Connected");
							reachable=false;
							socket.close();
						}
				}
				catch (Exception e)
				{
					e.printStackTrace();
					System.out.println("Exception in finally..."+e.getMessage());
					Logger.E(TAG,"Exception in finally..."+e.getMessage());
				}
			}
			System.out.println("hostUrl="+host+"="+reachable);
			
			return reachable;
		}
		
		
		
		
		
		public void run()
		{
			
			
			while(runtheThread)
			{
				//System.out.println("***INSIDE WHILE******....host="+host+" port="+port);
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
		
					if(counterTimeout>timeout)
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
					
					sleep(pollinterval);
					if(counterTimeout!=0)
						counterTimeout=counterTimeout+5000;
				} catch (InterruptedException e) {
					
					e.printStackTrace();
				}
				catch(Exception e)
				{
					//System.out.println("Exception during sleep,exception="+e.getMessage());
					Logger.E(TAG, "excetion in Thread run()"+e.getMessage());
				}
			}
		
		System.out.println("Thread is about to exit");
		}
		
	}
	class ConnectionPageNavEvent extends AbstractRhoExtension
	{
		@Override
		public boolean onNavigateComplete(IRhoExtManager extManager,
				String url, IRhoWebView ext, boolean res) {
			
			System.out.println("ConnectionPageNavEvent...onNavigateComplete,url="+url);
			Logger.I(TAG, "ConnectionPageNavEvent...onNavigateComplete,url="+url);
			
			if(isPageNavigationError==true)
				criticalUri=url;
			else
				criticalUri="";
			if(conThread!=null && !conThread.isAlive())
			{
				
				if(shouldThreadStart(url) )
						{
							conThread=new ConThread();
							conThread.start();
						}
				
			}
			return super.onNavigateComplete(extManager, url, ext, res);
		}
		private boolean shouldThreadStart(String url)
		{
			boolean start=true;
			if(url.contains(badlinkUri) || url.equalsIgnoreCase("badlinkUri"))
				{
					start=false;
					criticalUri="";
					isPageNavigationError=false;
				}
			else if(isPageNavigationError==true )
				start=false;
			return start;
		}
		@Override
		public boolean onNavigateError(IRhoExtManager extManager, String url,
				LoadErrorReason reason, IRhoWebView ext, boolean res) {
			//System.out.println("onNavigateError");
			isPageNavigationError=true;
			isPageNavigationErrorCounter++;
			System.out.println("onNavigateError...isPageNavigationErrorCounter="+isPageNavigationErrorCounter+" url="+url);
			Logger.I(TAG, "onNavigateError...isPageNavigationErrorCounter="+isPageNavigationErrorCounter+" url="+url);
			return super.onNavigateError(extManager, url, reason, ext, res);
		}
	}
}
