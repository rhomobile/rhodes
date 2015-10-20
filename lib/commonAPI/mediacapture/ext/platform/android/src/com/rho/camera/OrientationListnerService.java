package com.rho.camera;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.view.OrientationEventListener;

public class OrientationListnerService extends Service{
	public static int mRotation = 0;
	private OrientationEventListener mOrientationListener;
	@Override
	public void onCreate() {
		super.onCreate();
		mOrientationListener = new OrientationEventListener(this) {
			@Override
			public void onOrientationChanged(int rotation) {
				if(rotation >=315 && rotation < 45){
					mRotation = 0;
				}else if(rotation >=45 && rotation < 135){
					mRotation = 90;
				}else if(rotation >=135 && rotation < 225){
					mRotation = 180;
				}else if(rotation >=225 && rotation< 315){
					mRotation = 270;
				}else{
					mRotation = 0;
				}
			}
		};
	}
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		if (mOrientationListener.canDetectOrientation()) {
            mOrientationListener.enable();
        }
		return super.onStartCommand(intent, flags, startId);
	}
	@Override
	public void onDestroy() {
		mOrientationListener.disable();
		super.onDestroy();
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
}
