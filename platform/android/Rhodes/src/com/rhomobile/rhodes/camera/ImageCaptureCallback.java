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

package com.rhomobile.rhodes.camera;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;

import com.rhomobile.rhodes.Logger;

import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;

public class ImageCaptureCallback implements PictureCallback {
	
	private static final String TAG = "ImageCapture";

	private ImageCapture mOwner;
	private String callbackUrl;
	private OutputStream osCommon;
	private String filePath;
	
	private int mImgWidth;
	private int mImgHeight;
	private String mImgFormat;
	private int dev_rotation=0;

	public ImageCaptureCallback(ImageCapture owner, String u, OutputStream o, String f, int w, int h, String format,int _dev_rotation) {
		mOwner = owner;
		callbackUrl = u;
		osCommon = o;
		filePath = f;
		//filePath="/mnt/sdcard/mypic.jpg";
		mImgWidth = w;
		mImgHeight = h;
		mImgFormat = format;
		dev_rotation =_dev_rotation;
	}

	public void onPictureTaken(byte[] data, Camera camera) {
		
		try {
			Logger.D(TAG, "PICTURE CALLBACK JPEG: " + data.length + " bytes");

			
		
			if (osCommon != null) {
				osCommon.write(data);
				osCommon.flush();
				osCommon.close();
				System.out.println("osCommon.write()");
			}
			
			
			
			OutputStream osOwn = new FileOutputStream(filePath);
			osOwn.write(data);
			osOwn.flush();
			osOwn.close();
			
			System.out.println("picture saved");
			
			
			/*
	        File pictureFile = new File(filePath);
			try 
			{
	          FileOutputStream fos = new FileOutputStream(pictureFile);
	          fos.write(data);
	          fos.flush();
	          fos.close();
	        } 
			catch (Exception ex) 
			{
	        	 Logger.E(TAG, ex.getMessage());
	        }
			*/  
		  Bitmap rotatedBitmap=null;
		  Bitmap bm=null;
		  
		  try
			{
				
				
		        BitmapFactory.Options bounds = new BitmapFactory.Options();
		        bounds.inSampleSize = 4;
		        bounds.inDither = false;
		        bounds.inPurgeable = true;
		        bounds.inInputShareable = true; 
		        bounds.inTempStorage = new byte[32 * 1024];
		        bounds.inPreferredConfig = Bitmap.Config.RGB_565;
	
		        bm= BitmapFactory.decodeFile(filePath, bounds);
				int rotationAngle = 0;
				if((dev_rotation>45)&&(dev_rotation<135))
				{
					rotationAngle =180;
					
				}
				else if((dev_rotation>134)&&(dev_rotation<225))
				{
					rotationAngle =270;
					
				}
				else if((dev_rotation>224)&&(dev_rotation<315))
				{
					rotationAngle =0;				
				}
				else
				{
					rotationAngle=90;
				}
				System.out.println("jdp rotationAngle"+rotationAngle);
			
				Matrix matrix = new Matrix();
				matrix.postRotate(rotationAngle);
				rotatedBitmap = Bitmap.createBitmap(bm, 0, 0, bm.getWidth(), bm.getHeight(), matrix, true);
			
			
			}
			catch(Exception e)
			{
				Logger.E(TAG,e.getMessage());
				System.out.println("exception"+e.getMessage());
			}
			
	
		  
		  if(rotatedBitmap!=null)
			{
			try {
			       

				
				
				FileOutputStream out = new FileOutputStream(filePath);
			       rotatedBitmap.compress(Bitmap.CompressFormat.JPEG, 90, out);
			       out.flush();
			       out.close();
			    
			       
			      
			       String temp= filePath;
			       
			       int lastIndex=0;
			      
			       lastIndex = temp.lastIndexOf("/");
			 
			       
			       String mystr = temp.substring(lastIndex);
			      
			       String sdcardFilepath= "/mnt/sdcard/"+mystr;
			  	   
			    		  
			       
			       FileOutputStream out1 = new FileOutputStream(sdcardFilepath);
			       rotatedBitmap.compress(Bitmap.CompressFormat.JPEG, 90, out1);
			       out1.flush();
			       out1.close();
		
			       
			}
			catch (Exception e) 
			{
				Logger.E(TAG,e.getMessage());
			}
			}
	  
		  	mImgWidth = rotatedBitmap.getWidth();
		  	mImgHeight = rotatedBitmap.getHeight();
		  	

		  	
		  	if(rotatedBitmap!=null)
		  	{
		  	 rotatedBitmap.recycle();	
			 rotatedBitmap=null;
		  	}
		  	
		  	if(bm!=null)
		  	{
			bm.recycle();
			bm=null;
		  	}
		  	
		  	/*
		  	
			if (osCommon != null) 
			{
				osCommon.write(data);
				osCommon.flush();
				osCommon.close();
			}
			System.out.println("jdp rotataion at last osCommon.write()");
			
			*/
			
			com.rhomobile.rhodes.camera.Camera.doCallback(filePath, mImgWidth, mImgHeight, mImgFormat);
			mOwner.finish();

		} catch (Exception e) {
			Logger.E(TAG, e);
		}
	}
	
	

	

	
	
}
