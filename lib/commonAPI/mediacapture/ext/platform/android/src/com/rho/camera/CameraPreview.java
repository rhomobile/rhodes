package com.rho.camera;

import android.content.Context;
import android.app.Activity;
import android.graphics.Rect;
import android.hardware.Camera;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.FrameLayout;
import android.view.Surface;

import com.rho.camera.ICameraObject.ISize;
import com.rhomobile.rhodes.Logger;

public class CameraPreview implements SurfaceHolder.Callback {
    private static final String TAG = CameraPreview.class.getSimpleName();

    private SurfaceView mSurfaceView;
    private SurfaceHolder mHolder;
    private ICameraObject mCamera;
    private Context mContext;
    
    public CameraPreview(SurfaceView surfaceView) {
        Logger.T(TAG, "CameraPreview");
        
        mSurfaceView = surfaceView;
    }

    public void startPreview(ICameraObject camera, Context context) {
        Logger.T(TAG, "startPreview");
        mContext = context;
        if (camera == null) {
            Logger.E(TAG, "Camera is null");
            return;
        }

        mCamera = camera;

        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed.
        mHolder = mSurfaceView.getHolder();
        mHolder.addCallback(this);
        mHolder.setSizeFromLayout();
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }
    
    public void stopPreview() {
        Logger.T(TAG, "stopPreview");
        if (mCamera != null) {
            mCamera.stopPreview();
            mCamera = null;
        } else {
            Logger.T(TAG, "No camera to stop preview");
        }
    }
    
	public static String getDeviceOrientation(int right,int bottom,int curentRotation )
	{

	Logger.I(TAG, "getDeviceOrientation right:- "+right);
	Logger.I(TAG, "getDeviceOrientation bottom:- "+bottom);
	Logger.I(TAG, "getDeviceOrientation curentRotation:- "+curentRotation);
	String DeviceOrientation="undefined";
	
	if(right>bottom)
	{	
		if((curentRotation == 0)||(curentRotation == 2))
		{	
			Logger.I(TAG, "JDP getDeviceOrientation-1");
			DeviceOrientation="LANDSCAPE";
		}
		else
		{	
			Logger.I(TAG, "JDP getDeviceOrientation-2");
			DeviceOrientation="PORTRAIT";
		}
	}
	else
	{	
		if((curentRotation == 0)||(curentRotation == 2))
		{	
			Logger.I(TAG, "JDP getDeviceOrientation-3");
			DeviceOrientation="PORTRAIT";
		}
		else
		{	
			Logger.I(TAG, "JDP getDeviceOrientation-4");
			DeviceOrientation="LANDSCAPE";
		}
	}
	return DeviceOrientation;
	
	}
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Rect surfaceRect = holder.getSurfaceFrame();
        Logger.T(TAG, "surfaceCreated: " + surfaceRect.right + "x" + surfaceRect.bottom);
        try{
        if (mCamera != null) {
           mCamera.startPreview(holder);
         }
        }
        catch(RuntimeException e){
        	
        }
        
        int rotation = ((Activity)mContext).getWindowManager().getDefaultDisplay().getRotation();
    	int degrees = 0;
    	String DeviceOriginalOrinetation=getDeviceOrientation(surfaceRect.right,surfaceRect.bottom,rotation);
    	if (DeviceOriginalOrinetation.equals("LANDSCAPE"))
		{
			switch (rotation) 
			{
				case Surface.ROTATION_0:
					degrees = 0;
					break;
				case Surface.ROTATION_90:
					degrees = 270;
					break;
				case Surface.ROTATION_180:
					degrees = 180;
					break;
				case Surface.ROTATION_270:
					degrees = 90;
					break;
			}
		}
    	else{
			switch (rotation) {
			case Surface.ROTATION_0:
				degrees = 90;
				break;
			case Surface.ROTATION_90:
				degrees = 0;
				break;
			case Surface.ROTATION_180:
				degrees = 270;
				break;
			case Surface.ROTATION_270:
				degrees = 180;
				break;
			}
    	}
       mCamera.setDisplayOrientation(degrees);
       
        ISize size = mCamera.setPreviewSize(surfaceRect.right, surfaceRect.bottom);
        
        double previewRatio = (double) size.getWidth() / size.getHeight();
        double dR = previewRatio - (double) surfaceRect.right / surfaceRect.bottom;
        int marginX = -4;
        int marginY = -4;
        if (dR > 0) {
            marginY = (int)(((double)surfaceRect.bottom - surfaceRect.right / previewRatio) / 4);
        } else if (dR < 1.0) {
            marginX = (int)(((double)surfaceRect.right - surfaceRect.bottom * previewRatio) / 4);
        }
        

        FrameLayout.LayoutParams layoutParams = (FrameLayout.LayoutParams)mSurfaceView.getLayoutParams();
        layoutParams.leftMargin = marginX;
        layoutParams.rightMargin = marginX;
        layoutParams.topMargin = marginY;
        layoutParams.bottomMargin = marginY; 
        /*layoutParams.leftMargin = layoutParams.FILL_PARENT;
        layoutParams.rightMargin = layoutParams.FILL_PARENT;
        layoutParams.topMargin = layoutParams.FILL_PARENT;
        layoutParams.bottomMargin = layoutParams.FILL_PARENT;*/
        mSurfaceView.setLayoutParams(layoutParams);
        mSurfaceView.requestLayout();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Logger.T(TAG, "surfaceDestroyed");
        if (mCamera != null) {
            mCamera.stopPreview();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    }
}
