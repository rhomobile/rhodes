package com.rhomobile.rhodes.camera;


interface CameraService {

	public class Size {
		public Size(int w, int h) {
			width = w;
			height = h;
		}
		public int width;
		public int height;
	}

	android.hardware.Camera getMainCamera();
	
	android.hardware.Camera getFrontCamera();
	
	Size getClosestPictureSize(android.hardware.Camera camera, int w, int h);
	
	Size getClosestPreviewSize(android.hardware.Camera camera, int w, int h);

}