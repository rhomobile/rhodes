package com.rhomobile.rhodes.camera;

import java.io.FileOutputStream;
import java.io.OutputStream;

import com.rhomobile.rhodes.Logger;

import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;

public class ImageCaptureCallback implements PictureCallback {

	private String filePath;
	private OutputStream filoutputStream;

	public ImageCaptureCallback(OutputStream filoutputStream, String filePath) {
		this.filoutputStream = filoutputStream;
		this.filePath = filePath;
	}

	public void onPictureTaken(byte[] data, Camera camera) {
		try {
			Logger.T("ImageCapture", "onPictureTaken=" + data
					+ " length = " + data.length);
			
			FileOutputStream buf = new FileOutputStream(filePath);
            buf.write(data);
            buf.flush();
            buf.close();
            
			//filoutputStream.write(data);
			filoutputStream.flush();
			filoutputStream.close();
			
			com.rhomobile.rhodes.camera.Camera.doCallback(filePath);
			
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
	
}
