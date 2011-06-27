package com.rhomobile.rhodes.camera;

import java.util.Iterator;
import java.util.List;

import android.hardware.Camera;
import android.hardware.Camera.Size;


class CameraSemiService implements CameraService {

	public android.hardware.Camera getMainCamera() {
		return android.hardware.Camera.open();
	}

	public android.hardware.Camera getFrontCamera() {
		return null;
	}
	
	public Size getClosestPictureSize(android.hardware.Camera camera, int w, int h) {

		int neww = w;
		int newh = h;
		
		Camera.Parameters p = camera.getParameters();
		List<android.hardware.Camera.Size> sizes = p.getSupportedPictureSizes();
		Iterator<android.hardware.Camera.Size> iter = sizes.iterator();
		// find closest preview size
		float min_r = -1;
		int minW = 0;
		int minH = 0;
		while (iter.hasNext()) {
			android.hardware.Camera.Size s = iter.next();
			if (min_r < 0) {
				min_r = (float)s.width*(float)s.width+(float)s.height*(float)s.height;
				minW = s.width;
				minH = s.height;
			}
			else {
				float cur_r = ((float)neww-(float)s.width)*((float)neww-(float)s.width)+((float)newh-(float)s.height)*((float)newh-(float)s.height);
				if (cur_r < min_r) {
					min_r = cur_r;
					minW = s.width;
					minH = s.height;
				}
			}
		}
		if (min_r >= 0) {
			neww = minW;
			newh = minH;
		}
		return new Size(neww, newh);
	}
	
	public Size getClosestPreviewSize(android.hardware.Camera camera, int w, int h) {

		int neww = w;
		int newh = h;
		
		Camera.Parameters p = camera.getParameters();
		List<android.hardware.Camera.Size> sizes = p.getSupportedPreviewSizes();
		Iterator<android.hardware.Camera.Size> iter = sizes.iterator();
		// find closest preview size
		float min_r = -1;
		int minW = 0;
		int minH = 0;
		while (iter.hasNext()) {
			android.hardware.Camera.Size s = iter.next();
			if (min_r < 0) {
				min_r = (float)s.width*(float)s.width+(float)s.height*(float)s.height;
				minW = s.width;
				minH = s.height;
			}
			else {
				float cur_r = ((float)neww-(float)s.width)*((float)neww-(float)s.width)+((float)newh-(float)s.height)*((float)newh-(float)s.height);
				if (cur_r < min_r) {
					min_r = cur_r;
					minW = s.width;
					minH = s.height;
				}
			}
		}
		if (min_r >= 0) {
			neww = minW;
			newh = minH;
		}
		return new Size(neww, newh);
	}
	
}