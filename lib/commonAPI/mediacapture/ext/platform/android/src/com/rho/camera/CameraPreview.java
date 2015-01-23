package com.rho.camera;

import android.content.Context;
import android.graphics.Rect;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.FrameLayout;

import com.rho.camera.ICameraObject.ISize;
import com.rhomobile.rhodes.Logger;

public class CameraPreview implements SurfaceHolder.Callback {
    private static final String TAG = CameraPreview.class.getSimpleName();

    private SurfaceView mSurfaceView;
    private SurfaceHolder mHolder;
    private ICameraObject mCamera;

    public CameraPreview(SurfaceView surfaceView) {
        Logger.T(TAG, "CameraPreview");
        
        mSurfaceView = surfaceView;
    }

    public void startPreview(ICameraObject camera, Context context) {
        Logger.T(TAG, "startPreview");

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

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Rect surfaceRect = holder.getSurfaceFrame();
        Logger.T(TAG, "surfaceCreated: " + surfaceRect.right + "x" + surfaceRect.bottom);
        
        mCamera.startPreview(holder);
        ISize size = mCamera.setPreviewSize(surfaceRect.right, surfaceRect.bottom);
        
        double previewRatio = (double) size.getWidth() / size.getHeight();
        double dR = previewRatio - (double) surfaceRect.right / surfaceRect.bottom;
        int marginX = 0;
        int marginY = 0;
        if (dR > 0) {
            marginY = (int)(((double)surfaceRect.bottom - surfaceRect.right / previewRatio) / 2);
        } else if (dR < 1.0) {
            marginX = (int)(((double)surfaceRect.right - surfaceRect.bottom * previewRatio) / 2);
        }
        

        FrameLayout.LayoutParams layoutParams = (FrameLayout.LayoutParams)mSurfaceView.getLayoutParams();
        layoutParams.leftMargin = marginX;
        layoutParams.rightMargin = marginX;
        layoutParams.topMargin = marginY;
        layoutParams.bottomMargin = marginY;
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
