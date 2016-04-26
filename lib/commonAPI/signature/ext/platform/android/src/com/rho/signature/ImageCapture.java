/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Symbol Technologies Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rho.signature;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.security.InvalidParameterException;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.os.Bundle;
import android.os.Environment;
import android.os.ResultReceiver;
import android.util.Base64;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.ImageButton;
import android.content.pm.ActivityInfo;


import com.rho.signature.Signature.IntentMessageType;
import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.Capabilities;

public class ImageCapture extends BaseActivity implements OnClickListener
{
	private static final String TAG = "SignatureActivity";
	
	private String imageFormat;
	private String filePath;
	private int penColor;
	private int penWidth;
	private int bgColor;

	private SignatureView surfaceView;
	private SurfaceHolder surfaceHolder;
	private ImageButton cancelButton;
	private ImageButton clearButton;
	private ImageButton okButton;

	private int singletonId;
	
	private static boolean isRunning = false;
	
	private ResultReceiver resultReceiver;

	private boolean isOnCreateCall = false;
	
	@Override
	public void onCreate(Bundle icicle) {
		Logger.D(TAG, "onCreate+");
		super.onCreate(icicle);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(RhoExtManager.getResourceId("layout", "signature"));
		
		Bundle extras = getIntent().getExtras();
		imageFormat = extras.getString(com.rho.signature.Signature.INTENT_EXTRA_PREFIX + "imageFormat");
		filePath = extras.getString(com.rho.signature.Signature.INTENT_EXTRA_PREFIX + "filePath");		
		penColor = extras.getInt(com.rho.signature.Signature.INTENT_EXTRA_PREFIX + "penColor");
		penWidth = extras.getInt(com.rho.signature.Signature.INTENT_EXTRA_PREFIX + "penWidth");
		bgColor = extras.getInt(com.rho.signature.Signature.INTENT_EXTRA_PREFIX + "bgColor");
		singletonId = extras.getInt(com.rho.signature.Signature.INTENT_EXTRA_PREFIX + "singletonId");
		
		surfaceView = (SignatureView) findViewById(RhoExtManager.getResourceId("id", "signature_view"));
		cancelButton = (ImageButton) findViewById(RhoExtManager.getResourceId("id", "sig_cancelButton"));
		clearButton = (ImageButton) findViewById(RhoExtManager.getResourceId("id", "sig_clearButton"));
		okButton = (ImageButton) findViewById(RhoExtManager.getResourceId("id", "sig_okButton"));
		
		surfaceHolder = surfaceView.getHolder();
		surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);
		surfaceView.setSingletonId(singletonId);
		
		cancelButton.setOnClickListener(this);
		clearButton.setOnClickListener(this);
		okButton.setOnClickListener(this);

		surfaceView.setupView(penColor, penWidth, bgColor, false);
		surfaceView.invalidate();
		
		resultReceiver = (ResultReceiver) getIntent().getParcelableExtra(com.rho.signature.Signature.INTENT_EXTRA_PREFIX + "resultReceiver");
		isOnCreateCall = true;
		if(BaseActivity.getScreenAutoRotateMode())     
        	{  
        		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
		}
		Logger.D(TAG, "onCreate-");
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		Logger.D(TAG, "onRestoreInstanceState");
		super.onRestoreInstanceState(savedInstanceState);
		surfaceView.onRestoreInstanceState(savedInstanceState);
	}

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		Logger.D(TAG, "onKeyDown");
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
		      {
		      	sendCancel();
			finish();
			return super.onKeyDown(keyCode, event);
		      }
		default:
			return false;
		}
	}

	protected void onResume() {
		Logger.D(TAG, "onResume+");
		super.onResume();
		
		if(isOnCreateCall)
		{
			isRunning = true;
			Logger.D(TAG, "isRunning = " + isRunning);
			resultReceiver.send(0, null);//Info isnt important, the call is important
			isOnCreateCall = false;
		}
		
		Logger.D(TAG, "onResume-");
	}

	protected void onSaveInstanceState(Bundle outState) {
		Logger.D(TAG, "onSaveInstanceState");
		surfaceView.onSaveInstanceState(outState);
		super.onSaveInstanceState(outState);
	}

	protected void onStop() {
		Logger.D(TAG, "onStop");
		super.onStop();
	}

	public void onClick(View v) {
		Logger.D(TAG, "onClick+");
		if (v.getId() == RhoExtManager.getResourceId("id", "sig_cancelButton"))
		{		
			sendCancel();
			finish();
		}
		if (v.getId() == RhoExtManager.getResourceId("id", "sig_clearButton"))
		{
			surfaceView.doClear();
		}
		if (v.getId() == RhoExtManager.getResourceId("id", "sig_okButton"))
		{
			if(SignatureFullScreen.takeResult != null) takeSignature(SignatureFullScreen.takeResult, penColor, bgColor);
			finish();
		}
		Logger.D(TAG, "onClick-");
	}
	
	private void sendCancel()
	{
		if(SignatureFullScreen.takeResult != null)
		{
			Map<String, Object> results = new HashMap<String, Object>(3);
			results.put("status", "cancel");
			results.put("imageUri", "");
			results.put("signature_uri", "");
			SignatureFullScreen.takeResult.set(results);
			SignatureFullScreen.takeResult.release();
		}
		else
		{
			Logger.E(TAG, "Error: lost callback in fullscreen cancel call");
		}
	}
	

	/**
	 * Captures the signature to the file name specified.
	 * @param result The MethodResult to send success/error messages to 
	 * @param imgFormat the format of the image to save (jpg, png, bmp or datauri)
	 * @param bitmap the bitmpa image to capture
	 * @param filePath the filePath (excluding extension) to save the data to.
	 * @param penColor the color of the pen
	 * @param bgColor the color of the background
	 * @author Ben Kennedy (NCVT73)
	 */
	public static void takeSignature(IMethodResult result, String imgFormat, Bitmap bitmap, String filePath, int penColor, int bgColor)
	{
		Logger.D(TAG, "takeSignature+");
		try
		{
			String outputFilePath;
			
			if(imgFormat.equals("jpg"))
			{
				outputFilePath = saveJpg(bitmap, filePath);
			}
			else if(imgFormat.equals("png"))
			{
				outputFilePath = savePng(bitmap, filePath);
			}
			else if(imgFormat.equals("bmp"))
			{
				outputFilePath = saveBmp(bitmap, filePath, penColor, bgColor);
			}
			else if(imgFormat.equals("datauri"))
			{
				outputFilePath = generateDataUri(bitmap);
			}
			else
			{
				Logger.E(TAG, "Unknown image format: " + imgFormat);
				return;
			}

			//Do Callback
			Map<String, Object> results = new HashMap<String, Object>(3);
			if(outputFilePath.contains("sdcard") && (Capabilities.READ_SDCARD_ENABLED==true))
			{
				results.put("status", "error");
				results.put("imageUri", "only read sdcard permission is there in build.yml");
				if(imgFormat.equals("datauri")) results.put("signature_uri", "");
				else						
				results.put("signature_uri", "");
			}
			else
			{
				results.put("status", "ok");
				results.put("imageUri", outputFilePath);
				if(imgFormat.equals("datauri")) results.put("signature_uri", "");
				else						
				results.put("signature_uri", outputFilePath);	
			}
			
			result.set(results);
		} 
	    catch (FileNotFoundException e)
	    {
	    	result.setError("FileNotFoundException: " + e);
	    	Logger.E(TAG, "FNFE:" + e.getMessage());
	    } 
		catch(URISyntaxException e)
		{
			result.setError("Illegal characters in filePath: " + filePath);
			Logger.E(TAG, "Illegal characters in filePath: " + filePath);
		}
		catch (IOException ex)
		{
			result.setError("IOException: " + ex);
			Logger.E(TAG, "IOEX " + ex.getMessage());
			ex.printStackTrace();
		}
		catch(Exception e)
		{
			result.setError("Could not capture due to exception");
			Logger.E(TAG, e.getMessage());
		}
		Logger.D(TAG, "takeSignature-");
	}
	
	/**
	 * Saves the given bitmap as a JPG file with the File URI as the default picture location + filePath + .jpg
	 * @param bitmap The bitmap to save as an image
	 * @param filePath the file name (without extension) to give the created file
	 * @return The URL of the created file
	 * @throws IOException If the file could not be written or a file exception occurs
	 * @throws URISyntaxException If the filePath contains illegal characters
	 * @author Ben Kennedy (NCVT73) 
	 */
	private static String saveJpg(Bitmap bitmap, String filePath) throws IOException, URISyntaxException
	{
		Logger.D(TAG, "saveJpg");
		return compressImgAndroid(bitmap, filePath + ".jpg", CompressFormat.JPEG);
	}
	
	/**
	 * Saves the given bitmap as a PNG file with the File URI as the default picture location + filePath + .png
	 * @param bitmap The bitmap to save as an image
	 * @param filePath the file name (without extension) to give the created file
	 * @return The URL of the created file
	 * @throws IOException If the file could not be written or a file exception occurs
	 * @throws URISyntaxException If the filePath contains illegal characters
	 * @author Ben Kennedy (NCVT73) 
	 */
	private static String savePng(Bitmap bitmap, String filePath) throws IOException, URISyntaxException
	{
		Logger.D(TAG, "savePng");
		return compressImgAndroid(bitmap, filePath + ".png", CompressFormat.PNG);
	}

	/**
	 * Saves the given bitmap as a JPG file with the File URI as the default picture location + filePath + .bmp 
	 * @param bitmap The bitmap to save as an image
	 * @param filePath the file name (without extension) to give the created file
	 * @param penColor The color of the pen of the signature (ARGB)
	 * @param bgColor The color of the signature background (ARGB)
	 * @return The URL of the created file
	 * @throws IOException If the file could not be written or a file exception occurs
	 * @throws URISyntaxException If the filePath contains illegal characters
	 * @author Ben Kennedy (NCVT73)
	 */
	private static String saveBmp(Bitmap bitmap, String filePath, int penColor, int bgColor) throws IOException, URISyntaxException
	{
		Logger.D(TAG, "saveBmp");
		return FormatBitmap.writeBitmapImage(bitmap, filePath + ".bmp", penColor, bgColor);
	}
	
	/**
	 * Converts a bitmap image into a DataUri
	 * @param bitmap the bitmap to convert
	 * @return the DataURI string
	 * @throws IOException If the temporary file could not be written or a file exception occurs
	 * @author Ben Kennedy (NCVT73)
	 */
	private static String generateDataUri(Bitmap bitmap) throws IOException
	{
		Logger.D(TAG, "generateDataUri+");
		String res;
		File tempFile = File.createTempFile("signature", "png", ContextFactory.getContext().getCacheDir());
		
		OutputStream output = null;
		Exception throwing = null;
		
		try
		{
			output = new FileOutputStream(tempFile);
			bitmap.compress(CompressFormat.PNG, 100, output);	
			output.flush();
		}
		catch(IOException e)
		{
			Logger.W(TAG, "Error in compressing signature image");
			throwing = e;
		}
		finally
		{
			try{if (output != null) output.close();}catch(IOException e){/*Nothing we can do*/};
			if(throwing != null) throw new IOException(throwing); 
		}
	
		InputStream is = null;
		try
		{
			is = new BufferedInputStream(new FileInputStream(tempFile));
			ByteArrayOutputStream bos = new ByteArrayOutputStream();
			while (is.available() > 0)
			{
			    bos.write(is.read());
			}
			res = Base64.encodeToString(bos.toByteArray(), Base64.DEFAULT);
			Logger.D(TAG, "generateDataUri-");
		}
		finally
		{
			if(is != null) is.close();
		}
		is.close();
		tempFile.delete();
	    return "data:image/png;base64," + res;
	}
	
	/**
	 * Compresses a bitmap to a specified compression format, native to android (png or jpg)
	 * @param bitmap the bitmap to convert
	 * @param filePath the file name (including extension) to save the file to. The file will be saved in the default picture directory.
	 * @param compressFormat the compression format (png or jpg)
	 * @return the URL of the saved file
	 * @throws IOException If an exception arises when trying to write the file.
	 * @throws MalformedURLException If the file name has illegal characters
	 * @author Ben Kennedy (NCVT73)
	 */
	private static String compressImgAndroid(Bitmap bitmap, String filePath, CompressFormat compressFormat) throws IOException
	{
		Logger.D(TAG, "compressImgAndroid+");
		Exception throwing = null;
		OutputStream output = null;
		
		try
		{
			Logger.D(TAG, "saving image to: " + filePath);
			output = getOutputStream(filePath);
			bitmap.compress(compressFormat, 100, output);		
			output.flush();
		}
		catch(InvalidParameterException e)
		{
			Logger.W(TAG, e.getMessage());
		}
		catch(FileNotFoundException e)
		{
			e.printStackTrace();
			Logger.W(TAG, "Cannot create file");
		}
		catch(IOException e)
		{
			e.printStackTrace();
			Logger.W(TAG, "Error in compressing signature image");
			throwing = e;	
		}
		catch(URISyntaxException e)
		{
			e.printStackTrace();
			Logger.W(TAG, "Error in creating signature image file");
			throwing = e;	
		}
		finally
		{
			try{if (output != null) output.close();}catch(IOException e){/*Nothing we can do*/};
			if(throwing != null) throw new IOException(throwing); 
		}
		
		return filePath;
	}
	
	/**
	 * Returns an outputStream for the input String formatted URL. Works with private data directory URLs (/data/data/com...) as long
	 * as the described file is in the root of the data directory. Deletes and creates a new file before opening
	 * @param filePath The file to open the stream of
	 * @return the output stream for the described file
	 * @throws InvalidParameterException If the path is a file that is in a subdirectory of the private data directory.
	 * @author Ben Kennedy (NCVT73)
	 * @throws URISyntaxException if the filePath cannot be converted to a URI
	 * @throws FileNotFoundException if the file cannot be created 
	 */
	public static FileOutputStream getOutputStream(String filePath) throws InvalidParameterException, FileNotFoundException, URISyntaxException
	{
		Logger.D(TAG, "getOutputStream+ " + filePath);
//		String dataDir = RhodesActivity.getContext().getApplicationInfo().dataDir;
//		Pattern dataDirPattern = Pattern.compile("^(?i:(file:[/\\\\]*))?(" + dataDir.replace(".", "[.]") + "/)(.*)$");
//		Matcher dataDirMatcher = dataDirPattern.matcher(filePath);
//		
//		if(dataDirMatcher.matches())
//		{
//			String dataDirPath = dataDirMatcher.group(3);
//			if(dataDirPath.contains("/"))
//			{
//				throw new InvalidParameterException("Cannot write to subdirectories of the android data directory. Can only write to root.");
//			}
//			RhodesActivity.getContext().deleteFile(dataDirPath);
//			return RhodesActivity.getContext().openFileOutput(dataDirPath, Context.MODE_WORLD_READABLE);
//		}
//		else
//		{
		File resultFile = new File(new URI(filePath));
		if(resultFile.exists()) resultFile.delete();
		File parentFile = resultFile.getParentFile();
		if(parentFile != null) parentFile.mkdirs();
		return new FileOutputStream(resultFile);
		//return stream;
//		}
	}

	/**
	 * Captures the signature to the file name specified when created.
	 * @param result The MethodResult to send success/error messages to 
	 * @param penColor the color of the pen
	 * @param bgColor the color of the background
	 * @author Ben Kennedy (NCVT73)
	 */
	private void takeSignature(MethodResult result, int penColor, int bgColor) {
		takeSignature(result, imageFormat, surfaceView.makeBitmap(), filePath, penColor, bgColor);
	}
	
	@Override
	protected void onNewIntent(Intent intent)
	{
		Logger.D(TAG, "onNewIntent+");
		Bundle bundle = intent.getExtras();
		int messageType = bundle.getInt(Signature.INTENT_MESSAGE_TYPE);
		String data = bundle.getString(Signature.INTENT_MESSAGE_DATA);
		IntentMessageType messageTypeEnum = Signature.IntentMessageType.values()[messageType];
		Logger.D(TAG, "onNewIntent " + messageTypeEnum);
		
		switch(messageTypeEnum)
		{
			case CAPTURE: 
				{
					if(SignatureFullScreen.takeResult != null)
					{
						if(surfaceView.getWidth() <= 0 || surfaceView.getHeight() <= 0)
						{
							SignatureFullScreen.takeResult.setError("Signature area not yet ready");
							break;
						}
						takeSignature(SignatureFullScreen.takeResult, penColor, bgColor); break;
					}
				}
			case CLEAR: surfaceView.doClear(); break;
			case DESTROY: sendCancel(); finish(); break;
			case VECTOR_CALLBACK: /*TODO*/; break;
			case BG: bgColor = Integer.parseInt(data); surfaceView.setupView(penColor, penWidth, bgColor, false); break;
			case PEN_COLOR: penColor = Integer.parseInt(data); surfaceView.setupView(penColor, penWidth, bgColor, false); break;
			case PEN_WIDTH: penWidth = Integer.parseInt(data); surfaceView.setupView(penColor, penWidth, bgColor, false); break;
			case IMAGE_FORMAT: imageFormat = data; break;
			case FILE_PATH: filePath = data; break;
			default: break;
		}
		Logger.D(TAG, "onNewIntent-");
	}

//	/**
//	 * Creates and returns a file from the default Picture folder, with the filePath specified. 
//	 * Deletes the file if it already exists 
//	 * @param filePath the filePath to create
//	 * @return The created File 
//	 * @throws IOException if a issue arises when creating/deleting the file.
//	 * @author Ben Kennedy (NCVT73)
//	 */
//	public static File getGalleryFile(String filePath) throws IOException
//	{
//		Logger.D(TAG, "getTempFile+");
//		File gallery = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
//		gallery.mkdirs();
//		File newFile = new File(gallery, filePath);
//		if(newFile.exists())
//		{
//			if(!newFile.delete())
//			{
//				Logger.E(TAG, "Cannot delete temporary file: " + newFile.toString());
//				throw new IOException("Cannot delete temporary file");
//			}
//		}
//		newFile.createNewFile();
//		Logger.D(TAG, "getTempFile-");
//		return newFile;
//	}
	
	@Override
	public void finish()
	{
		isRunning = false;
		Logger.D(TAG, "finish isRunning = " + isRunning);
		super.finish();
	}
	
	@Override
	protected void onDestroy()
	{
		isRunning = false;
		Logger.D(TAG, "onDestroy isRunning = " + isRunning);
		super.onDestroy();
	}

	@Override
	protected void finalize() throws Throwable
	{
		super.finalize();
	}

	public static boolean isRunning()
	{
		return isRunning;
	}
}
