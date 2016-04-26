package com.rho.signature;

import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Parcel;
import android.os.ResultReceiver;

import com.rho.signature.SignatureSingleton.SignatureProperties;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

/**
 * A control object for an fullscreen Signature
 * @author Ben Kennedy (NCVT73)
 */
public class SignatureFullScreen extends Signature
{
	private static final String LOGTAG = "SignatureFullScreen";
	//TODO sort out if minimized
	public static MethodResult takeResult = null;
	public static MethodResult vectorCallback = null;
	private RhoSignatureResultReceiver resultReceiver;
	private ArrayList<Intent> intentQueue;
	
	/**
	 * Creates a new FullScreen Signature Capture control object
	 * @param properties the Signature Capture area properties parsed from SignatureSingleton
	 * @param result The IMethodResult for takeFullscreen
	 * @param singleton the singleton class that created the object.
	 */
	public SignatureFullScreen(SignatureProperties properties, IMethodResult result, SignatureSingleton singleton)
	{
		super(properties, result, singleton);
		takeResult = (MethodResult) result;
		intentQueue = new ArrayList<Intent>();
	}

	@Override
	public void capture(IMethodResult result)
	{
		Logger.D(LOGTAG, "Capturing Fullscreen Signature");
		sendMessage(Signature.IntentMessageType.CAPTURE, "");
	}

	@Override
	public void clear(IMethodResult result)
	{
		Logger.D(LOGTAG, "Clearing Fullscreen Signature");
		sendMessage(Signature.IntentMessageType.CLEAR, "");
	}

	@Override
	public void hide(IMethodResult result)
	{
		Logger.D(LOGTAG, "Closing Fullscreen Application");
		sendMessage(Signature.IntentMessageType.DESTROY, "");
	}

	@Override
	public void destroy()
	{
		Logger.D(LOGTAG, "Closing Fullscreen Application");
		sendMessage(Signature.IntentMessageType.DESTROY, "");
	}

	@Override
	public void setBgColor(int bgColor)
	{
		int oldBgColor = properties.bgColor;
		super.setBgColor(bgColor);
		if(oldBgColor != bgColor)
		{
			sendMessage(Signature.IntentMessageType.BG, Integer.toString(bgColor));
		}
	}

	@Override
	public void setBorder(boolean border)
	{
		Logger.W(LOGTAG, "Border is not applicable to FullScreen signature area");
	}

	@Override
	public void setCompressionFormat(String compressionFormat)
	{
		String oldFormat = properties.compressionFormat;
		super.setCompressionFormat(compressionFormat);
		if(!oldFormat.equalsIgnoreCase(compressionFormat))
		{
			if(properties.outputFormat.equalsIgnoreCase(SignatureSingleton.OUTPUT_FORMAT_IMAGE))
			{
				sendMessage(Signature.IntentMessageType.IMAGE_FORMAT, compressionFormat);
			}
		}	
	}

	@Override
	public void setFilePath(String filePath)
	{
		String oldFilePath = properties.filePath;
		super.setFilePath(filePath);
		if(!oldFilePath.equalsIgnoreCase(filePath))
		{
			sendMessage(Signature.IntentMessageType.FILE_PATH, filePath);
		}
	}

	@Override
	public void setOutputFormat(String outputFormat)
	{
		String oldFormat = properties.outputFormat;
		super.setOutputFormat(outputFormat);
		if(!oldFormat.equalsIgnoreCase(outputFormat))
		{
			if(outputFormat.equalsIgnoreCase(SignatureSingleton.OUTPUT_FORMAT_DATAURI))
			{
				sendMessage(Signature.IntentMessageType.IMAGE_FORMAT, SignatureSingleton.OUTPUT_FORMAT_DATAURI);
			}
			else
			{
				sendMessage(Signature.IntentMessageType.IMAGE_FORMAT, properties.compressionFormat);
			}
		}
	}

	@Override
	public void setPenColor(int penColor)
	{
		int oldPenColor = properties.penColor;
		super.setPenColor(penColor);
		if(oldPenColor != penColor)
		{
			sendMessage(Signature.IntentMessageType.PEN_COLOR, Integer.toString(penColor));
		}
	}

	@Override
	public void setPenWidth(int penWidth)
	{
		int oldPenWidth = properties.penWidth;
		super.setPenWidth(penWidth);
		if(oldPenWidth != penWidth)
		{
			sendMessage(Signature.IntentMessageType.PEN_WIDTH, Integer.toString(penWidth));
		}
	}

	@Override
	public void show()
	{
		Logger.D(LOGTAG, "Showing Fullscreen Signature+");
		String imageFormat;
		if(properties.outputFormat.equalsIgnoreCase("datauri")) imageFormat = "datauri";
		else imageFormat = properties.compressionFormat;
		
		RhodesActivity ra = RhodesActivity.safeGetInstance();
		resultReceiver = new RhoSignatureResultReceiver(null);
		Intent intent = new Intent(ra, ImageCapture.class);
		intent.putExtra(INTENT_EXTRA_PREFIX + "filePath", properties.filePath);
		intent.putExtra(INTENT_EXTRA_PREFIX + "imageFormat", imageFormat);
		intent.putExtra(INTENT_EXTRA_PREFIX + "outputFormat", properties.outputFormat);
		intent.putExtra(INTENT_EXTRA_PREFIX + "penColor", properties.penColor);
		intent.putExtra(INTENT_EXTRA_PREFIX + "penWidth", properties.penWidth);
		intent.putExtra(INTENT_EXTRA_PREFIX + "bgColor", properties.bgColor);
		intent.putExtra(INTENT_EXTRA_PREFIX + "border", properties.border);
		intent.putExtra(INTENT_EXTRA_PREFIX + "singletonId", singleton.hashCode());
		intent.putExtra(INTENT_EXTRA_PREFIX + "resultReceiver", receiverForSending(resultReceiver));
		synchronized(intentQueue)
		{
			intentQueue.clear();
		}
		ra.startActivity(intent);
		Logger.D(LOGTAG, "Showing Fullscreen Signature-");
	}
	
	/**
	 * Sends a message to the Fullscreen Signature Capture Application. If the application is starting, then the
	 * messages are queued.
	 * @param messageType the message type
	 * @param data the data of the message
	 * @author Ben Kennedy (NCVT73)
	 */
	public void sendMessage(IntentMessageType messageType, String data)
	{
		Logger.D(LOGTAG, "sendMessage+");
		RhodesActivity ra = RhodesActivity.safeGetInstance();
		Intent intent = new Intent(ra, ImageCapture.class);
		intent.putExtra(INTENT_MESSAGE_TYPE, messageType.ordinal());
		intent.putExtra(INTENT_MESSAGE_DATA, data);
		
		Logger.D(LOGTAG, "sendMessage isFullscreenRunning: " +ImageCapture.isRunning());
		if(ImageCapture.isRunning())
		{
			Logger.D(LOGTAG, "SM Sending Message to FullScreen Signature: " +messageType.toString());
			ra.startActivity(intent);
		}
		else
		{
			synchronized(intentQueue)
			{
				intentQueue.add(intent);
			}
			if(ImageCapture.isRunning()) releaseIntentQueue(); //To clear up synch issues
		}
	}
	
	/**
	 * Fires off all of the queued messages to the FullScreen Signature app.
	 */
	private synchronized void releaseIntentQueue()
	{
		RhodesActivity ra = RhodesActivity.safeGetInstance();
		for(Intent intent: intentQueue)
		{
			Logger.D(LOGTAG, "IQ Sending Message to FullScreen Signature");
			ra.startActivity(intent);
		}
		intentQueue.clear();
	}
	
	private class RhoSignatureResultReceiver extends ResultReceiver
	{
		public RhoSignatureResultReceiver(Handler handler)
		{
			super(handler);
		}

		@Override
		protected void onReceiveResult(int resultCode, Bundle resultData)
		{
			Logger.D(LOGTAG, "Start result received");
			releaseIntentQueue();
			Logger.D(LOGTAG, "Intents Released");
		}
		
	}
	
	public static ResultReceiver receiverForSending(ResultReceiver actualReceiver) {
	    Parcel parcel = Parcel.obtain();
	    actualReceiver.writeToParcel(parcel,0);
	    parcel.setDataPosition(0);
	    ResultReceiver receiverForSending = ResultReceiver.CREATOR.createFromParcel(parcel);
	    parcel.recycle();
	    return receiverForSending;
	}
}