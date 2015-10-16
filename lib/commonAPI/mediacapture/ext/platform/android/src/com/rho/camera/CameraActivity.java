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

package com.rho.camera;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhodesActivity;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.net.Uri;
import android.os.Bundle;
import android.view.OrientationEventListener;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.FrameLayout;

public class CameraActivity extends BaseActivity implements OnClickListener {
    private static final String TAG = CameraActivity.class.getSimpleName();
    private CameraPreview mPreview;
    private Camera mCamera = null;
    private ICameraObject camera = null;
    private Button button = null;
    private String id = null;
    public static boolean CURRENT_SCREEN_AUTO_ROTATE_MODE;
    public static boolean CURRENT_FULL_SCREEN_MODE;
    
    @Override
    protected void onCreate(Bundle extras) {
        super.onCreate(extras);
        Logger.T(TAG, "onCreate");
        CURRENT_SCREEN_AUTO_ROTATE_MODE = RhodesActivity.safeGetInstance().getScreenAutoRotateMode();
        CURRENT_FULL_SCREEN_MODE = RhodesActivity.safeGetInstance().getFullScreenMode();
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        RhodesActivity.safeGetInstance().setScreenAutoRotateMode(false);
        RhodesActivity.safeGetInstance().setFullScreenMode(true);
        if(getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT){
    	   setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    	   setContentView(R.layout.camera_port);
        }
        if(getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE){
    	   setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    	   setContentView(R.layout.camera_land);
        }        
        id = getIntent().getStringExtra(CameraExtension.INTENT_EXTRA_PREFIX + "CAMERA_ID");        
        camera = ((CameraFactory)CameraFactorySingleton.getInstance()).getCameraObject(id);
        button =(Button)findViewById(R.id.cameraButton);
        button.setOnClickListener(this);
        mPreview = new CameraPreview((SurfaceView)findViewById(R.id.previewSurface));

    }
    
    @Override
    protected void onResume() {
        super.onResume();
        Logger.T(TAG, "onResume");        
        startOrientationService();
        try{
          mPreview.startPreview(camera, this);
          // Setting focus if supported by camera of the device after camera previewing
          camera.setFocus(this);
        }
        catch(RuntimeException e){
        	
        }
    }
    
    @Override
    protected void onPause() {
    	super.onPause();
        Logger.T(TAG, "onPause");
        button.setEnabled(true);
        if(mCamera != null){
          mCamera.stopPreview();
          mCamera.release();
          mCamera = null;
        }
    }
    
    @Override
    protected void onStop() {
    	super.onStop();
	stopService(new Intent(CameraActivity.this,OrientationListnerService.class));
	button.setEnabled(true);
    }
    
    @Override
    protected void onDestroy() {
    	super.onDestroy();
    	if(camera !=null){
    		camera.stopPreview();
    		camera = null;
    	}
    }
	
    @Override
    public void onClick(View view) {
        Logger.T(TAG, "onClick");
        try{
        if (view.getId() == R.id.cameraButton) {
            Logger.T(TAG, "cameraButton");            
            camera.doTakePicture(this,0);
            button.setEnabled(false);
          }
        }
        catch(Exception e){
        	e.printStackTrace();
        }
    }
    
    private void startOrientationService()
    {
    	Intent i=new Intent(this,OrientationListnerService.class);
    	startService(i);
    }
}
