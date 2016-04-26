package com.rho.keycapture;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.ReadableByteChannel;

import android.content.Intent;
import android.os.Environment;
import android.view.KeyEvent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * Enables the Key Capture Extension to listen to keys by being its RhoListener. RhoListeners can receive key events,
 * but are created by the Rhodes core. This is why this class and KeyCaptureSingleton are separate, as otherwise two
 * separate objects will be created.
 * @author Ben Kennedy
 */
public class KeyCaptureRhoListener extends AbstractRhoListener 
{
	private static final String TAG = "KeyCaptureKeyListener";
	private static KeyCaptureFactory factory;
	private KeyCaptureSingleton singleton;
	
	@Override
	public void onCreateApplication(IRhoExtManager extManager)
	{
		Logger.D(TAG, "KeyCaptureSingleton: onCreateApplication");
		extManager.startKeyEventUpdates(this); 
		extManager.addRhoListener(this);
	}
	
	/**
	 * Registers a factory with this listener, so that the listener can send key events
	 * @param inputFactory the factory object to register with this listener
	 * @author Ben Kennedy (NCVT73)
	 */
	public static void registerListener(KeyCaptureFactory inputFactory)
	{
		factory = inputFactory;
	}
	
	@Override
	public void onCreate(RhodesActivity activity, Intent intent) {
	
		copyMappingXML();
		super.onCreate(activity, intent);
	}
	
	@Override
	public boolean onKey(int keyCode, KeyEvent event)
	{
		if(factory != null)
		{
			if(singleton == null)
			{
				singleton = (KeyCaptureSingleton) factory.getApiSingleton();
			}
			return singleton.onKey(keyCode, event);
		}
		return false;
	}
	private void copyMappingXML()
	{
		boolean writeableStorage=true;
		String rawResourceName=Environment.getExternalStorageDirectory()+"/Android/data/"+RhodesActivity.safeGetInstance().getPackageName()+ "/keycodemapping.xml";
		int resourceId= RhoExtManager.getResourceId("raw","keycodemapping");
		File resourceFile = new File(rawResourceName);
		boolean fileExists = resourceFile.exists();
		
		if(!fileExists && writeableStorage)
		{
			
			try
			{
				
				File f=new File(resourceFile.getParentFile().getAbsolutePath()+"/");
				boolean suc=f.mkdirs();
				
				resourceFile.createNewFile();
				FileChannel writer = new FileOutputStream(resourceFile, false).getChannel();
				ReadableByteChannel channel = Channels.newChannel(RhodesActivity.safeGetInstance().getResources().openRawResource(resourceId));
				ByteBuffer buffer = ByteBuffer.allocate(1024);
				while(channel.read(buffer) > 0)
				{
					buffer.flip();
					writer.write(buffer);
					buffer.clear();
				}

				writer.close();
			}
			catch (IOException e)
			{
				
				resourceFile.delete();
			}
		}
		else
		{
		
		}
		
	}
}
