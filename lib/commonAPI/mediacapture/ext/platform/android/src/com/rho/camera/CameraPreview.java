package com.rho.camera;

import java.io.IOException;
import java.util.List;

import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup;

public class CameraPreview extends ViewGroup implements SurfaceHolder.Callback {
    private static final String TAG = CameraPreview.class.getSimpleName();

    private SurfaceView mSurfaceView;
    private SurfaceHolder mHolder;
    private Camera mCamera;
    private List<Size> mSupportedPreviewSizes;

    public CameraPreview(Context context) {
        super(context);

        mSurfaceView = new SurfaceView(context);
        addView(mSurfaceView);

        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed.
        mHolder = mSurfaceView.getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void setCamera(Camera camera) {
        if (camera == null) {
            Logger.E(TAG, "Camera is null");
            return;
        }

        mCamera = camera;

        List<Size> localSizes = mCamera.getParameters().getSupportedPreviewSizes();
        mSupportedPreviewSizes = localSizes;
        requestLayout();
      
        try {
            mCamera.setPreviewDisplay(mHolder);
        } catch (IOException e) {
            Logger.E(TAG, e);
            return;
        }
      
        /*
          Important: Call startPreview() to start updating the preview surface. Preview must 
          be started before you can take a picture.
          */
        mCamera.startPreview();
        
    }
    
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

        // Now that the size is known, set up the camera parameters and begin the preview.
        Camera.Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(width, height);
        requestLayout();
        mCamera.setParameters(parameters);

        /*
          Important: Call startPreview() to start updating the preview surface. Preview must be
          started before you can take a picture.
        */
        mCamera.startPreview();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        // Surface will be destroyed when we return, so stop the preview.
        if (mCamera != null) {
            mCamera.stopPreview();
        }
        
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        // TODO Auto-generated method stub
        
    }


}
