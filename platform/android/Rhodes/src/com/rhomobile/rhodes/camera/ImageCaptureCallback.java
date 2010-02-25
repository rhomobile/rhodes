/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
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
