package com.rhomobile.rhodes.camera;

class CameraOldService implements CameraService {

	
	public android.hardware.Camera getMainCamera() {
		return android.hardware.Camera.open();
	}

	public android.hardware.Camera getFrontCamera() {
		return null;
	}
	
	public Size getClosestPictureSize(android.hardware.Camera camera, int w, int h) {
		return null;
	}
	
	public Size getClosestPreviewSize(android.hardware.Camera camera, int w, int h) {
		return null;
	}
	
	
}