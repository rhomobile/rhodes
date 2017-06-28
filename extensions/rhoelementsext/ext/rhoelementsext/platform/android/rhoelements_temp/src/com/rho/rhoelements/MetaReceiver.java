package com.rho.rhoelements;

import android.webkit.JavascriptInterface;


/**
 * @author Geoff Day (XFH386, Initial Creation), Ben Kennedy (NCVT73, Adapting for EMML Profiles)
 *
 */
public class MetaReceiver
{
    private static final String TAG = MetaReceiver.class.getSimpleName();
	public static enum PageEvent { PAGE_STARTED, PAGE_FINISHED };
	// Rho Focus events: Foreground and background
	public static enum focusEvent { ON_FOREGROUND, ON_BACKGROUND };

	public MetaHandler handler;
	protected final static Object syncObj = new Object();
	protected Boolean mInProgress = false;
	
	public MetaReceiver ()
	{
		// Construct Handler in this thread context
		handler = new MetaHandler();
	}
	
	/** Called by JavaScript once per meta tag on page after page is loaded.
	 * Called from a different thread, so uses MetaHandler to resync to main thread
	 * @param equiv http-equiv string from the meta tag
	 * @param content content string from the meta tag
	 */
	@JavascriptInterface
	public void setMeta (String equiv, String content)
	{
		com.rhomobile.rhodes.Logger.I(TAG, equiv + "', '" + content + "'");

		
		if ((equiv != null) && (equiv.equals("status")) && 
			(content != null) && (content.equals("eom")))
		{
			// meta harvesting is complete
			mInProgress = false;
			synchronized(syncObj)
			{
				// notify the sync object in case an onPageStarted event is waiting
				syncObj.notify();
			};
		}
		else
		{
			MetaTag tag = new MetaTag(equiv, content);
			handler.sendMessage(handler.obtainMessage(0, tag));
		}
	}
	
	/** Called by ElementsActivity when a new page is loaded.
	 * Called from the main thread.  onPageStarted must be passed on to the PluginManager
	 * once any MetaTags from the previous page have been received.  onPageFinished must
	 * be passed on to the PluginManager immediately.
	 * @param url URL of the new page being loaded
	 */
	public synchronized void pageEvent(PageEvent event, String url)
	{
		if (event == PageEvent.PAGE_STARTED)
		{
			
			if (mInProgress)
			{
				try
				{
					// Meta Havesting is in progress.. wait for it to finish
					syncObj.wait();
				}
				catch (InterruptedException e)
				{
				    com.rhomobile.rhodes.Logger.W(TAG, e);
				}
			}
			MetaTag tag = new MetaTag("pageevent", "started:" + url);
			handler.sendMessage(handler.obtainMessage(0, tag));
		}
		else if (event == PageEvent.PAGE_FINISHED)
		{
			
			mInProgress = true;
			MetaTag tag = new MetaTag("pageevent", "finished:" + url);
			handler.sendMessage(handler.obtainMessage(0, tag));
		}
	}

	/** Called by ElementsActivity when the app goes background/foreground.
	 * Called from the main thread. 	
	 */
	public synchronized void focusEvent(focusEvent event, String url)
	{
		if (event == focusEvent.ON_FOREGROUND)
		{			
			MetaTag tag = new MetaTag("focusevent", "foreground:" + url);
			handler.sendMessage(handler.obtainMessage(0, tag));
		}
		else if (event == focusEvent.ON_BACKGROUND)
		{			
			MetaTag tag = new MetaTag("focusevent", "background:" + url);
			handler.sendMessage(handler.obtainMessage(0, tag));
		}
	}
}
