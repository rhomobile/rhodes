package com.rho.signature;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.security.InvalidParameterException;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;

import android.graphics.Color;
import android.os.Environment;
import android.util.SparseArray;

import com.rho.signature.ISignatureSingleton;
import com.rho.signature.SignatureSingletonBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

/**
 * Signature Capture Singleton class
 * @author Ben Kennedy (NCVT73)
 */
public class SignatureSingleton extends SignatureSingletonBase implements ISignatureSingleton
{
	private static SparseArray<SignatureSingleton> singletonMap = new SparseArray<SignatureSingleton>();
	private static final String LOGTAG = "SignatureSingleton";
	private static final String HK_VECTOR_ARRAY="vectorArray";
	/**
	 * A class to hold the properties of a signature API object 
	 * @author Ben Kennedy (NCVT73)
	 */
	static class SignatureProperties
	{
		public int bgColor;
		public boolean border;
		public String compressionFormat;
		public String fileName;
		public String filePath;
		public int height;
		public int left;
		public String outputFormat;
		public int penColor;
		public int penWidth;
		public int top;
		public int width;
		public boolean isArgbBg;
		public boolean isArgbPen;
		
		public SignatureProperties(int bgColor, boolean border, String compressionFormat, String fileName, int height, int left, String outputFormat,
				int penColor, int penWidth, int top, int width, boolean isArgbBg, boolean isArgbPen)
		{
			this.bgColor = bgColor;
			this.border = border;
			this.compressionFormat = compressionFormat;
			this.fileName = fileName;
			this.height = height;
			this.left = left;
			this.outputFormat = outputFormat;
			this.penColor = penColor;
			this.penWidth = penWidth;
			this.top = top;
			this.width = width;
			this.isArgbBg = isArgbBg;
			this.isArgbPen = isArgbPen;
		}
	}
	/**
	 * The lowercase names of the properties of this API Object 
	 * @author Ben Kennedy (NCVT73)
	 */
	static class SignaturePropertyNames
	{
		public static final String BG_COLOR = "bgcolor";
		public static final String BORDER = "border";
		public static final String COMPRESSION_FORMAT = "compressionformat";
		public static final String FILE_NAME = "filename";
		public static final String FILE_PATH = "filepath";
		public static final String HEIGHT = "height";
		public static final String LEFT = "left";
		public static final String OUTPUT_FORMAT = "outputformat";
		public static final String PEN_COLOR = "pencolor";
		public static final String PEN_WIDTH = "penwidth";
		public static final String TOP = "top";
		public static final String WIDTH = "width";
	}
	
	private static final SignatureProperties defaultProperties = new SignatureProperties(0xFFFFFFFF, true, ISignatureSingleton.COMPRESSION_FORMAT_PNG, "signature", 150, 15, ISignatureSingleton.OUTPUT_FORMAT_IMAGE, 0xFF000000, 3, 60, 200, true, true);
	
	private Signature currentSignature;
	private SignatureProperties currentProperties;
	private MethodResult vectorCallback = null;
	private boolean isFullScreen = false;
	
	public SignatureSingleton()
	{
		super();
		Logger.D(LOGTAG, "Constructor");
		currentProperties = defaultProperties;
		try
		{
			currentProperties.filePath = parseFileName(currentProperties.fileName);
		}
		catch(URISyntaxException e)
		{
			//shouldnt happen
		}
		singletonMap.put(this.hashCode(), this);
	}
	
	@Override
	public void show(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.D(LOGTAG, "show+");
		setProperties(propertyMap, result);
		if(currentSignature != null)
		{
			if(isFullScreen)
			{
				currentSignature.destroy();
				currentSignature = new SignatureInline(currentProperties, result, this);
			}
		}
		else
		{
			currentSignature = new SignatureInline(currentProperties, result, this);
		}
		currentSignature.show();
		isFullScreen = false;
		Logger.D(LOGTAG, "show-");
	}
	
	@Override
	public void takeFullScreen(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.D(LOGTAG, "takeFullscreen+");
		setProperties(propertyMap, result);
		if(currentSignature != null)
		{
			if(!isFullScreen)
			{
				currentSignature.destroy();
				currentSignature = new SignatureFullScreen(currentProperties, result, this);
			}
			else
			{
				//Bug Fix ...takeFullScreen was not Taking the recent callback..so added this block instead of commenting the isFullScreen
				System.out.println("previous signature is  Full Screen, result= "+result);
				currentSignature.destroy();
				currentSignature=new SignatureFullScreen(currentProperties, result, this);
				
			}
		}
		else
		{
			currentSignature = new SignatureFullScreen(currentProperties, result, this);
		}
		currentSignature.show();
		isFullScreen = true;
		Logger.D(LOGTAG, "takeFullscreen-");
	}
	
	@Override
	public void capture(IMethodResult result)
	{
		Logger.D(LOGTAG, "capture+");
		if(currentSignature != null) currentSignature.capture(result);
		Logger.D(LOGTAG, "capture-");
	}

	@Override
	public void clear(IMethodResult result)
	{
		Logger.D(LOGTAG, "clear+");
		if(currentSignature != null) currentSignature.clear(result);
		Logger.D(LOGTAG, "clear-");
	}
	
	@Override
	public void hide(IMethodResult result)
	{
		Logger.D(LOGTAG, "hide+");
		if(currentSignature != null) currentSignature.hide(result);
		Logger.D(LOGTAG, "hide-");
	}
	
	@Override
	public void setVectorCallback(IMethodResult result)
	{
		Logger.D(LOGTAG, "setVectorCallback+");
		if(vectorCallback != null) vectorCallback.release();
		if (result == null || !result.hasCallback()) // No callback is set
		{
			Logger.D(LOGTAG, "VectorCallback cancelled");
			vectorCallback = null;
		}
		else
		{
			vectorCallback = ((MethodResult) result);
			vectorCallback.keepAlive();
		}
		Logger.D(LOGTAG, "setVectorCallback-");
	}
	
	/**
	 * Sends vectors to 
	 * @param vectors The vector data to send to the client app
	 * @param singletonHash the hashID of the singleton associated with the view that generated the vectors
	 * @author Ben Kennedy (NCVT73)
	 */
	public static void sendVectors(List<Object> vectors, Integer singletonHash)
	{
		SignatureSingleton currentSingleton = singletonMap.get(singletonHash);
		if(currentSingleton == null)
		{
			Logger.E(LOGTAG, "Cannot send vectors. Singleton has been lost!");
			return;
		}
		if(currentSingleton.isVectorCallbackSet())
		{
			currentSingleton.sendVectors(vectors);
		}
	}
	
	/**
	 * Sends vectors to the client app
	 * @param vectors The vector data to send to the client app
	 * @author Ben Kennedy (NCVT73)
	 */
	private void sendVectors(List<Object> vectors)
	{
		Logger.D(LOGTAG, "Sending Vectors...");
		Map<String, Object> resultMap = new HashMap<String, Object>();
		
		if(vectorCallback != null)
		{
			resultMap.put(HK_VECTOR_ARRAY,vectors);
		//	vectorCallback.set(vectors);
			vectorCallback.set(resultMap);
		}
	}
	
	/**
	 * Returns whether the vector callback has been set or not
	 * @return whether the vector callback has been set or not
	 * @author Ben Kennedy (NCVT73)
	 */
	public boolean isVectorCallbackSet()
	{
		if (vectorCallback != null) return true;
		return false;
	}
	
	/**
	 * Sets a bunch of properties
	 * @param propertyMap the map of properties to set
	 * @param result the IMethodResult created by the calling client function. Use this to send error messages.
	 * @author Ben Kennedy (NCVT73) 
	 */
	public void setProperties(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.D(LOGTAG, "setProperties+");
		if(propertyMap == null) return;
		for(Entry<String, String> entry: propertyMap.entrySet())
		{
			try
			{
				String property = entry.getKey().toLowerCase(Locale.ENGLISH);
				if(property.equals(SignaturePropertyNames.COMPRESSION_FORMAT))
				{
					setCompressionFormat(entry.getValue(), result);
				}
				else if(property.equals(SignaturePropertyNames.OUTPUT_FORMAT))
				{
					setOutputFormat(entry.getValue(), result);
				}
				else if(property.equals(SignaturePropertyNames.FILE_NAME))
				{
					setFileName(entry.getValue(), result);
				}
				else if(property.equals(SignaturePropertyNames.BORDER))
				{
					if(entry.getValue().toLowerCase(Locale.ENGLISH).equals("true"))
					{
						setBorder(true, result);
					}
					else if(entry.getValue().toLowerCase(Locale.ENGLISH).equals("false"))
					{
						setBorder(false, result);
					}
					else
					{
						result.setError("Invalid value for border: " + entry.getValue());
					}
				}
				else if(property.equals(SignaturePropertyNames.PEN_COLOR))
				{
					setPenColor(entry.getValue(), result);
				}
				else if(property.equals(SignaturePropertyNames.PEN_WIDTH))
				{
					setPenWidth(Integer.parseInt(entry.getValue()), result);
				}
				else if(property.equals(SignaturePropertyNames.BG_COLOR))
				{
					setBgColor(entry.getValue(), result);
				}
				else if(property.equals(SignaturePropertyNames.LEFT))
				{
					setLeft(Integer.parseInt(entry.getValue()), result);
				}
				else if(property.equals(SignaturePropertyNames.TOP))
				{
					setTop(Integer.parseInt(entry.getValue()), result);
				}
				else if(property.equals(SignaturePropertyNames.WIDTH))
				{
					setWidth(Integer.parseInt(entry.getValue()), result);
				}
				else if(property.equals(SignaturePropertyNames.HEIGHT))
				{
					setHeight(Integer.parseInt(entry.getValue()), result);
				}
				else
				{
					result.setError("Unknown Property: " + entry.getKey());
				}
			}
			catch(NumberFormatException e)
			{
				result.setError("Invalid value: " + entry.getValue() + " for property: " + entry.getKey());
			}
		}
		Logger.D(LOGTAG, "setProperties-");
	}
	
	//MUTATORS

	/**
	 * Sets and validates the the background color
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setBgColor(String bgColor, IMethodResult result)
	{
		Logger.D(LOGTAG, "setBgColor+ " + bgColor);
		try
		{
			if(bgColor == null) throw new IllegalArgumentException();
			if(!bgColor.startsWith("#")) bgColor = "#" + bgColor;
			int color = Color.parseColor(bgColor);
			Logger.D(LOGTAG, "setBgColor value " + color);	
			if(bgColor.length() == 9) currentProperties.isArgbBg = true;
			else currentProperties.isArgbBg = false;
			
			currentProperties.bgColor = color;
			if(currentSignature != null) currentSignature.setBgColor(currentProperties.bgColor);
		}
		catch(IllegalArgumentException e)
		{
			result.setError("Invalid bgColor value");
			Logger.W(LOGTAG, "Invalid Color Code: " + bgColor);
		}
		Logger.D(LOGTAG, "setBgColor-");
	}
	
	/**
	 * Sets and validates the the background color
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setBgColor(int bgColor, IMethodResult result)
	{
		Logger.D(LOGTAG, "setBgColor+");
		currentProperties.bgColor = bgColor;
		if(currentSignature != null) currentSignature.setBgColor(currentProperties.bgColor);
		Logger.D(LOGTAG, "setBgColor-");
	}

	/**
	 * Sets the border
	 * @param border whether the border should be visible (true) or not (false)
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setBorder(boolean border, IMethodResult result)
	{
		Logger.D(LOGTAG, "setBorder+");
		currentProperties.border = border;
		if(currentSignature != null) currentSignature.setBorder(border);
		Logger.D(LOGTAG, "setBorder-");
	}

	/**
	 * Sets and validates the compression format
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setCompressionFormat(String compressionFormat, IMethodResult result)
	{
		Logger.D(LOGTAG, "setCompressionFormat+");
		if (compressionFormat.toLowerCase(Locale.ENGLISH).equals(COMPRESSION_FORMAT_JPG)
				|| compressionFormat.toLowerCase(Locale.ENGLISH).equals(COMPRESSION_FORMAT_PNG)
				|| compressionFormat.toLowerCase(Locale.ENGLISH).equals(COMPRESSION_FORMAT_BMP))
		{
			currentProperties.compressionFormat = compressionFormat.toLowerCase(Locale.ENGLISH);
			if(currentSignature != null) currentSignature.setCompressionFormat(currentProperties.compressionFormat);
		}
		else result.setError("Invalid compression format");
		Logger.D(LOGTAG, "setCompressionFormat-");
	}

	/**
	 * Sets the fileName. Errors if an invalid file name is given. This includes path characters ('/', '\')
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setFileName(String fileName, IMethodResult result)
	{
		Logger.D(LOGTAG, "setFileName+");
		if (fileName == null || fileName.length() == 0)
		{
			fileName = defaultProperties.fileName;
		}
		currentProperties.fileName = fileName;
			
		try
		{
			currentProperties.filePath = parseFileName(fileName);
		}
		catch (InvalidParameterException e)
		{
			Logger.W(LOGTAG, "FileName not set. Invalid Parameter");
			result.setError(e.getMessage());
			currentProperties.filePath = generateDefaultFileURIString();
		}
		catch (URISyntaxException e)
		{
			Logger.W(LOGTAG, "FileName not set. Bad characters");
			result.setError("Illegal characters in fileName");
			currentProperties.filePath  = generateDefaultFileURIString();
		}
		if(currentSignature != null) currentSignature.setFilePath(currentProperties.filePath);

		Logger.D(LOGTAG, "setFileName-");
	}

	/**
	 * Generates the full URI string of the default file name
	 * @return the full URI string of the default file name
	 * @author Ben Kennedy (NCVT73)
	 */
	private String generateDefaultFileURIString()
	{
		try
		{
			return parseFileName(defaultProperties.fileName);
		}
		catch (InvalidParameterException e)
		{
			//Will never happen
		}
		catch (URISyntaxException e)
		{
			//Will never happen
		}
		return null; //Will never happen
	}

	/**
	 * Converts a file URI fragment into a file:/// URI string, resolving against the default Picture directory if
	 * needed. Absolute paths (i.e. starting with / or a protocol) will not be resolved, but will be formed into a
	 * syntatically correct URI string.
	 * @param fileName the filename to convert into an absolute URI string
	 * @return the absolute URI string
	 * @throws URISyntaxException If fileName contains characters that are illegal in URIs
	 * @throws InvalidParameterException  If fileName has a protocol that isnt file:// or isnt empty.
	 * @author Ben Kennedy (NCVT73)
	 */
	private static String parseFileName(String fileName) throws URISyntaxException, InvalidParameterException
	{
		String normalisedFileName = fileName.replace('\\', '/');
		URI fileUri = new URI(normalisedFileName);
		if(fileUri.getScheme() != null && !fileUri.getScheme().equalsIgnoreCase("file"))
		{
			throw new InvalidParameterException("Only file protocols are supported");
		}
		URI absoluteURI = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).toURI().resolve(fileUri);
		return absoluteURI.getScheme() + "://" + absoluteURI.getPath();
	}

	/**
	 * Sets the height. Errors if it is lower than 1
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setHeight(int height, IMethodResult result)
	{
		Logger.D(LOGTAG, "setHeight+");
		if (height > 0)
		{
			//TODO check max height
			currentProperties.height = height;
			if(currentSignature != null) currentSignature.setHeight(height);
		}
		else	result.setError("Invalid height value");
		Logger.D(LOGTAG, "setHeight-");
	}

	/**
	 * Sets the left. Errors if it is lower than 0
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setLeft(int left, IMethodResult result)
	{
		Logger.D(LOGTAG, "setLeft+");
		if (left >= 0)
		{
			//TODO check max left
			currentProperties.left = left;
			if(currentSignature != null) currentSignature.setLeft(left);
		}
		else	result.setError("Invalid left value");
		Logger.D(LOGTAG, "setLeft-");
	}

	/**
	 * Sets the outputFormat. Errors if an invalid outputFormat is given.
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setOutputFormat(String outputFormat, IMethodResult result)
	{
		Logger.D(LOGTAG, "setOutputFormat+");
		if (outputFormat.equalsIgnoreCase(OUTPUT_FORMAT_IMAGE)
				|| outputFormat.equalsIgnoreCase(OUTPUT_FORMAT_DATAURI))
		{
			currentProperties.outputFormat = outputFormat;
			if(currentSignature != null) currentSignature.setOutputFormat(currentProperties.outputFormat);
		}
		else
		{
			result.setError("Invalid outputFormat");
		}
		Logger.D(LOGTAG, "setOutputFormat-");
	}


	/**
	 * Sets and validates the the penColor
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setPenColor(String penColor, IMethodResult result)
	{
		Logger.D(LOGTAG, "setPenColor+");
		try
		{
			if(penColor == null) throw new IllegalArgumentException();
			if(!penColor.startsWith("#")) penColor = "#" + penColor;
			int color = Color.parseColor(penColor);
			
			if(penColor.length() == 9) currentProperties.isArgbPen = true;
			else currentProperties.isArgbPen = false;
			
			currentProperties.penColor = color;
			if(currentSignature != null) currentSignature.setPenColor(currentProperties.penColor);
		}
		catch(IllegalArgumentException e)
		{
			result.setError("Invalid penColor value");
		}
		Logger.D(LOGTAG, "setPenColor-");
	}
	
	/**
	 * Sets and validates the the penColor
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setPenColor(int penColor, IMethodResult result)
	{
		Logger.D(LOGTAG, "setPenColor+");
		currentProperties.penColor = penColor;
		if(currentSignature != null) currentSignature.setPenColor(currentProperties.penColor);
		Logger.D(LOGTAG, "setPenColor-");
	}

	/**
	 * Sets the penWidth. Errors if it is lower than 1
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setPenWidth(int penWidth, IMethodResult result)
	{
		Logger.D(LOGTAG, "setPenWidth+");
		if (penWidth > 0)
		{
			currentProperties.penWidth = penWidth;
			if(currentSignature != null) currentSignature.setPenWidth(penWidth);
		}
		else	result.setError("Invalid penWidth value");
		Logger.D(LOGTAG, "setPenWidth-");
	}

	/**
	 * Sets the top. Errors if it is lower than 0
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setTop(int top, IMethodResult result)
	{
		Logger.D(LOGTAG, "setTop+");
		if (top >= 0)
		{
			//TODO check max top
			currentProperties.top = top;
			if(currentSignature != null) currentSignature.setTop(top);
		}
		else	result.setError("Invalid top value");
		Logger.D(LOGTAG, "setTop-");
	}

	/**
	 * Sets the left. Errors if it is lower than 0
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setWidth(int width, IMethodResult result)
	{
		Logger.D(LOGTAG, "setWidth+");
		if (width > 0)
		{
			//TODO check max width
			currentProperties.width = width;
			if(currentSignature != null) currentSignature.setWidth(width);
		}
		else	result.setError("Invalid width value");
		Logger.D(LOGTAG, "setWidth-");
	}
	
	//ACCESSORS

	public void getBgColor(IMethodResult result)
	{
		result.set(toColorString(currentProperties.bgColor, currentProperties.isArgbBg));
//		toColorString(currentProperties.bgColor, currentProperties.isArgbBg);
//		result.set(currentProperties.bgColor);
	}

	public void getBorder(IMethodResult result)
	{
		result.set(currentProperties.border);
	}

	public void getCompressionFormat(IMethodResult result)
	{
		result.set(currentProperties.compressionFormat);
	}

	public void getFileName(IMethodResult result)
	{
		result.set(currentProperties.fileName);
	}

	public void getHeight(IMethodResult result)
	{
		result.set(currentProperties.height);
	}

	public void getLeft(IMethodResult result)
	{
		result.set(currentProperties.left);
	}

	public void getOutputFormat(IMethodResult result)
	{
		result.set(currentProperties.outputFormat);
	}

	public void getPenColor(IMethodResult result)
	{
		result.set(toColorString(currentProperties.penColor, currentProperties.isArgbPen));
//		toColorString(currentProperties.penColor, currentProperties.isArgbPen);
//		result.set(currentProperties.penColor);
	}

	public void getPenWidth(IMethodResult result)
	{
		result.set(currentProperties.penWidth);
	}

	public void getTop(IMethodResult result)
	{
		result.set(currentProperties.top);
	}

	public void getWidth(IMethodResult result)
	{
		result.set(currentProperties.width);
	}
	
	/**
	 * Converts a stored int version color into a human readable Web style color: #AARRGGBB
	 * @param colorInt the int formatted color value
	 * @param isArgb whether the color has Alpha
	 * @return an #AARRGGBB formatted string if isArgb true, else #RRGGBB
	 * @author Ben Kennedy (NCVT73)
	 */
	private static String toColorString(int colorInt, boolean isArgb)
	{
		Logger.D(LOGTAG, "toColorString+");
		String color = Integer.toHexString(colorInt).toUpperCase(Locale.ENGLISH);
		Logger.D(LOGTAG, "toColorString color: " + color);
		int requiredPadding;
		
		if(isArgb) requiredPadding = 8 - color.length();
		else requiredPadding = 6 - color.length();
		
		if(requiredPadding < 0)
		{
			color = color.substring(0 - requiredPadding, color.length());
		}
		else
		{
			for(;requiredPadding > 0; requiredPadding--)
			{
				color = "0" + color;
			}
		}
		Logger.D(LOGTAG, "toColorString- #" + color);
		return "#" + color;
	}
	
	@Override
	protected void finalize() throws Throwable
	{
		//Removes the reference from the HashMap
		singletonMap.remove(this.hashCode());
		super.finalize();
	}
}
