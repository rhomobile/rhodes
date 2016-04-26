package com.rho.signature;

import com.rho.signature.SignatureSingleton.SignatureProperties;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;

/**
 * An abstract class for different Signature Capture types (for example, inline and fullscreen).
 * @author Ben Kennedy (NCVT73)
 */
public abstract class Signature
{
	protected static final String LOGTAG = "Signature";
	public static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + ".signature.";
	public static final String INTENT_MESSAGE_TYPE = RhodesService.INTENT_EXTRA_PREFIX + ".signature.messageType";
	public static final String INTENT_MESSAGE_DATA = RhodesService.INTENT_EXTRA_PREFIX + ".signature.messageData";
	
	/**
	 * Types of message to send to ImageCapture. Used in intents.
	 * @author Ben Kennedy (NCVT73)
	 */
	public enum IntentMessageType
	{
		CREATE, CAPTURE, CLEAR, DESTROY, VECTOR_CALLBACK, BG, PEN_COLOR, PEN_WIDTH, IMAGE_FORMAT, FILE_PATH
	}
	
	protected SignatureProperties properties;
	protected SignatureSingleton singleton;
	
	/**
	 * Constructs a signature Object
	 * @param properties the Signature Capture area properties parsed from SignatureSingleton
	 * @param result The IMethodResult from the creating call (could be from takeFullscreen, for example)
	 * @param singleton the singleton class that created the object.
	 */
	public Signature(SignatureProperties properties, IMethodResult result, SignatureSingleton singleton)
	{
		this.properties = properties;
		this.singleton = singleton;
	}
	
	public abstract void capture(IMethodResult result);
	public abstract void clear(IMethodResult result);
	public abstract void hide(IMethodResult result);
	public abstract void destroy();
	public abstract void show();
	
	public void setBgColor(int bgColor)
	{
		properties.bgColor = bgColor;
	}
	public void setBorder(boolean border)
	{
		properties.border = border;
	}
	public void setOutputFormat(String outputFormat)
	{
		properties.outputFormat = outputFormat;
	}
	public void setPenColor(int penColor)
	{
		properties.penColor = penColor;
	}
	public void setPenWidth(int penWidth)
	{
		properties.penWidth = penWidth;
	}
	public void setTop(int top)
	{
		properties.top = top;
	}
	public void setLeft(int left)
	{
		properties.left = left;
	}
	public void setHeight(int height)
	{
		properties.height = height;
	}
	public void setWidth(int width)
	{
		properties.width = width;
	}
	public void setCompressionFormat(String compressionFormat)
	{
		properties.compressionFormat = compressionFormat;
	}
	public void setFilePath(String filePath)
	{
		properties.filePath = filePath;
	}
}