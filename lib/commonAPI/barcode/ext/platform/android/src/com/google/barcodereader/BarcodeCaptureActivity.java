/*
 * Copyright (C) The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.google.barcodereader;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.os.Build;
import android.os.Bundle;
import androidx.annotation.NonNull;
//import com.google.android.material.snackbar.Snackbar;
import androidx.core.app.ActivityCompat;
import androidx.appcompat.app.AppCompatActivity;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.common.api.CommonStatusCodes;
import com.google.barcodereader.ui.camera.CameraSource;
import com.google.barcodereader.ui.camera.CameraSourcePreview;

import com.google.barcodereader.ui.camera.GraphicOverlay;
import com.google.android.gms.vision.MultiProcessor;
import com.google.android.gms.vision.barcode.Barcode;
import com.google.android.gms.vision.barcode.BarcodeDetector;

import java.io.IOException;
import com.rhomobile.rhodes.R;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.Button;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rho.barcode.BarcodeFactory;
import com.rho.barcode.BarcodeCommon;
import android.content.pm.ActivityInfo;
import androidx.appcompat.widget.AppCompatImageButton;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.content.res.AssetFileDescriptor;
import android.content.res.Configuration;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import android.hardware.SensorManager;
import android.hardware.SensorEventListener;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Iterator;
import java.util.Date;
import java.util.HashMap;
import com.google.barcodereader.BarcodeFormats;

/**
 * Activity for the multi-tracker app.  This app detects barcodes and displays the value with the
 * rear facing camera. During detection overlay graphics are drawn to indicate the position,
 * size, and ID of each barcode.
 */
public final class BarcodeCaptureActivity extends AppCompatActivity implements BarcodeGraphicTracker.BarcodeUpdateListener, View.OnClickListener {
    private static final String TAG = "Barcode-reader";

    // intent request code to handle updating play services if needed.
    private static final int RC_HANDLE_GMS = 9001;

    // permission request codes need to be < 256
    private static final int RC_HANDLE_CAMERA_PERM = 2;

    // constants used to pass extra data in the intent
    public static final String BarcodeObject = "Barcode";


    private CameraSource mCameraSource;
    private CameraSourcePreview mPreview;
    private GraphicOverlay<BarcodeGraphic> mGraphicOverlay;

    private GestureDetector gestureDetector;

    private AppCompatImageButton buttonOk;
    private AppCompatImageButton buttonFlash;
    private AppCompatImageButton buttonRetake;
    private AppCompatImageButton buttonCancel;

    private String rhoBarcodeId;
    private boolean isUsingFlash = false;
    private static final int RC_BARCODE_CAPTURE = 9001;
    
    public static final String CAMERA_INDEX_EXTRA = "camera_index"; 
    public static final String RHO_BARCODE_ID = "barcode_obj_id";
    public static final String PROPERTY_MAP = "property_map";
    public int barcodeFormats = 0;

    public String lastResult = "";
    int camera_index = 0;
    HashMap<String, String> propertyMap = null;

    void rotateView(final boolean vertical){
        mGraphicOverlay.setVertical(vertical);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                final int degrees = vertical ? 0 : 90;
                buttonOk.setRotation(degrees);
                buttonFlash.setRotation(degrees);
                buttonRetake.setRotation(degrees);
                buttonCancel.setRotation(degrees);
            }
        });
    }


    SensorEventListener sensorEventListener;
    int currentOrientation = Configuration.ORIENTATION_PORTRAIT;
    public void refreshRotation(int orientation){
        if (orientation != currentOrientation){
            currentOrientation = orientation;
            if(currentOrientation == Configuration.ORIENTATION_PORTRAIT) {
                Log.i(TAG, "Orientation : PORTRAIT");
                rotateView(true);
            }
            else {
                Log.i(TAG, "Orientation : LANDSCAPE");
                rotateView(false);
            }
        }
        
    }
    /**
     * Initializes the UI and creates the detector pipeline.
     */
    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);      
        setContentView(R.layout.barcode_capture);
        
        Intent intent = getIntent();
        if (intent != null) {
            camera_index = intent.getIntExtra(CAMERA_INDEX_EXTRA, 0);
            Logger.D(TAG, "Intent Camera index: " + camera_index);
            rhoBarcodeId = intent.getStringExtra(RHO_BARCODE_ID);
            propertyMap = (HashMap<String, String>)intent.getSerializableExtra(PROPERTY_MAP);

            int enabledBarcodes = 0;
            int disabledBarcodes = 0;

            for (HashMap.Entry<String, String> entry : propertyMap.entrySet()) {
                if (entry.getValue().equalsIgnoreCase("true")){
                    enabledBarcodes |= BarcodeFormats.getBarcodeFormat(entry.getKey());
                }else if(entry.getValue().equalsIgnoreCase("false")){
                    disabledBarcodes |= BarcodeFormats.getBarcodeFormat(entry.getKey());
                }
                //System.out.println("Key: " + entry.getKey() + " Value: " + entry.getValue());
            }
            barcodeFormats = enabledBarcodes;

            CameraSource.setTargetCameraIndex(BarcodeCommon.getCameraIdByName(rhoBarcodeId));
        }else{
            CameraSource.setTargetCameraIndex(-1);
        }

        buttonFlash = (AppCompatImageButton) findViewById(R.id.button_flash);
        buttonFlash.setOnClickListener(this);
        buttonOk = (AppCompatImageButton) findViewById(R.id.barcode_ok);
        buttonOk.setOnClickListener(this);
        buttonRetake = (AppCompatImageButton) findViewById(R.id.button_retake);
        buttonRetake.setOnClickListener(this);
        buttonCancel = (AppCompatImageButton) findViewById(R.id.button_back);
        buttonCancel.setOnClickListener(this);


        mPreview = (CameraSourcePreview) findViewById(R.id.preview);
        mGraphicOverlay = (GraphicOverlay<BarcodeGraphic>) findViewById(R.id.graphicOverlay);

        // read parameters from the intent used to launch the activity.
        // Check for the camera permission before accessing the camera.  If the
        // permission is not granted yet, request permission.
        int rc = ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA);
        if (rc == PackageManager.PERMISSION_GRANTED) {
            createCameraSource(isUsingFlash);
        } else {
            requestCameraPermission();
        }

        gestureDetector = new GestureDetector(this, new CaptureGestureListener());
        initBeeper();


        sensorEventListener = new SensorEventListener() {
            LinkedList<Integer> list = new LinkedList<Integer>();
            @Override
            public void onSensorChanged(SensorEvent event) {
                if (event.values[1]<6.5 && event.values[1]>-6.5) {
                    list.add(0);
                } else {
                    list.add(1);
                }

                if (list.size() >= 8){
                    while (list.size() > 8){
                        list.removeFirst();
                    }

                    float summ = 0;

                    Iterator<Integer> iterator = list.iterator();
                    while (iterator.hasNext()) {
                        summ += iterator.next();
                    }

                    if (summ == 0){
                        refreshRotation(Configuration.ORIENTATION_LANDSCAPE );
                    }else if (summ == list.size()){
                        refreshRotation(Configuration.ORIENTATION_PORTRAIT ); 
                    }
                }
            }
            @Override
            public void onAccuracyChanged(Sensor sensor, int accuracy) {}
        };

        SensorManager sm = (SensorManager)getSystemService(SENSOR_SERVICE);
        sm.registerListener(sensorEventListener, sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
        refreshRotation(Configuration.ORIENTATION_PORTRAIT);

    }


    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        Log.w(TAG, "Changing configuration");
    }

    private void requestCameraPermission() {
        Log.w(TAG, "Camera permission is not granted. Requesting permission");

        final String[] permissions = new String[]{Manifest.permission.CAMERA};

        if (!ActivityCompat.shouldShowRequestPermissionRationale(this, Manifest.permission.CAMERA)) {
            ActivityCompat.requestPermissions(this, permissions, RC_HANDLE_CAMERA_PERM);
            return;
        }

        /*final Activity thisActivity = this;

        View.OnClickListener listener = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ActivityCompat.requestPermissions(thisActivity, permissions,  RC_HANDLE_CAMERA_PERM);
            }
        };

        findViewById(R.id.topLayout).setOnClickListener(listener);
        Snackbar.make(mGraphicOverlay, R.string.permission_camera_rationale, Snackbar.LENGTH_INDEFINITE)
                .setAction(R.string.ok, listener)
                .show();*/
        
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        boolean c = gestureDetector.onTouchEvent(e);
        return  c || super.onTouchEvent(e);
    }

    @SuppressLint("InlinedApi")
    private void createCameraSource(boolean useFlash) {
        Context context = getApplicationContext();

        BarcodeDetector barcodeDetector = new BarcodeDetector.Builder(context).setBarcodeFormats(barcodeFormats).build();
        BarcodeTrackerFactory barcodeFactory = new BarcodeTrackerFactory(mGraphicOverlay, this);
        barcodeDetector.setProcessor(new MultiProcessor.Builder<Barcode>(barcodeFactory).build());

        if (!barcodeDetector.isOperational()) {
            // Note: The first time that an app using the barcode or face API is installed on a
            // device, GMS will download a native libraries to the device in order to do detection.
            // Usually this completes before the app is run for the first time.  But if that
            // download has not yet completed, then the above call will not detect any barcodes
            // and/or faces.
            //
            // isOperational() can be used to check if the required native libraries are currently
            // available.  The detectors will automatically become operational once the library
            // downloads complete on device.
            Log.w(TAG, "Detector dependencies are not yet available.");

            // Check for low storage.  If there is low storage, the native library will not be
            // downloaded, so detection will not become operational.
            IntentFilter lowstorageFilter = new IntentFilter(Intent.ACTION_DEVICE_STORAGE_LOW);
            boolean hasLowStorage = registerReceiver(null, lowstorageFilter) != null;

            if (hasLowStorage) {
                Toast.makeText(this, R.string.low_storage_error, Toast.LENGTH_LONG).show();
                Log.w(TAG, getString(R.string.low_storage_error));
            }
        }

        // Creates and starts the camera.  Note that this uses a higher resolution in comparison
        // to other detection examples to enable the barcode detector to detect small barcodes
        // at long distances.
        CameraSource.Builder builder = new CameraSource.Builder(getApplicationContext(), barcodeDetector)
                .setFacing(CameraSource.CAMERA_FACING_BACK)
                .setRequestedPreviewSize(1600, 1024)
                .setRequestedFps(60.0f);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            builder = builder.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
        }

        mCameraSource = builder
                .setFlashMode(useFlash ? Camera.Parameters.FLASH_MODE_TORCH : null)
                .build();
    }

    /**
     * Restarts the camera.
     */
    @Override
    protected void onResume() {
        super.onResume();
        startCameraSource();
        refresh();
    }

    /**
     * Stops the camera.
     */
    @Override
    protected void onPause() {
        super.onPause();
        if (mPreview != null) {
            mPreview.stop();
        }
    }



    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode != RC_HANDLE_CAMERA_PERM) {
            Log.d(TAG, "Got unexpected permission result: " + requestCode);
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
            return;
        }

        if (grantResults.length != 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            Log.d(TAG, "Camera permission granted - initialize the camera source");
            // we have permission, so create the camerasource
            createCameraSource( isUsingFlash);
            return;
        }

        Log.e(TAG, "Permission not granted: results len = " + grantResults.length +
                " Result code = " + (grantResults.length > 0 ? grantResults[0] : "(empty)"));

        DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                finish();
            }
        };

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Multitracker sample")
                .setMessage(R.string.no_camera_permission)
                .setPositiveButton(R.string.ok, listener)
                .show();
    }

    private void startCameraSource() throws SecurityException {
        int code = GoogleApiAvailability.getInstance().isGooglePlayServicesAvailable(
                getApplicationContext());
        if (code != ConnectionResult.SUCCESS) {
            Dialog dlg =
                    GoogleApiAvailability.getInstance().getErrorDialog(this, code, RC_HANDLE_GMS);
            dlg.show();
        }

        if (mCameraSource != null) {
            try {
                mPreview.start(mCameraSource, mGraphicOverlay);
            } catch (IOException e) {
                Log.e(TAG, "Unable to start camera source.", e);
                mCameraSource.release();
                mCameraSource = null;
            }
        }
    }


    private boolean onTap(float rawX, float rawY) {
        // Find tap point in preview frame coordinates.
        int[] location = new int[2];
        mGraphicOverlay.getLocationOnScreen(location);
        float x = (rawX - location[0]) / mGraphicOverlay.getWidthScaleFactor();
        float y = (rawY - location[1]) / mGraphicOverlay.getHeightScaleFactor();

        // Find the barcode whose center is closest to the tapped point.
        Barcode best = null;
        float bestDistance = Float.MAX_VALUE;
        for (BarcodeGraphic graphic : mGraphicOverlay.getGraphics()) {
            Barcode barcode = graphic.getBarcode();
            if (barcode.getBoundingBox().contains((int) x, (int) y)) {
                // Exact hit, no need to keep looking.
                best = barcode;
                break;
            }
            float dx = x - barcode.getBoundingBox().centerX();
            float dy = y - barcode.getBoundingBox().centerY();
            float distance = (dx * dx) + (dy * dy);  // actually squared distance
            if (distance < bestDistance) {
                best = barcode;
                bestDistance = distance;
            }
        }

        if (best != null) {
            lastResult = best.displayValue;
            refresh();
            beep();
            return true;
        }
        return false;
    }



    private class CaptureGestureListener extends GestureDetector.SimpleOnGestureListener {
        @Override
        public boolean onSingleTapConfirmed(MotionEvent e) {
            return onTap(e.getRawX(), e.getRawY()) || super.onSingleTapConfirmed(e);
        }
    }


    private MediaPlayer mediaPlayer = null;
    void initBeeper(){
        setVolumeControlStream(AudioManager.STREAM_MUSIC);

        mediaPlayer = new MediaPlayer();
        mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
        mediaPlayer.setOnCompletionListener(new OnCompletionListener() {
            public void onCompletion(MediaPlayer mediaPlayer) {
                mediaPlayer.seekTo(0);
            }
        });

        AssetFileDescriptor file = getResources().openRawResourceFd(RhoExtManager.getResourceId("raw", "beep"));
        try {
            mediaPlayer.setDataSource(file.getFileDescriptor(), file.getStartOffset(),
            file.getLength());
            file.close();
            final float BEEP_VOLUME = 0.10f;
            mediaPlayer.setVolume(BEEP_VOLUME, BEEP_VOLUME);
            mediaPlayer.prepare();
        } catch (IOException e) {
            mediaPlayer = null;
        }
    }

    void beep(){
        boolean playBeep = true;
        if (playBeep) {
            AudioManager audioService = (AudioManager) getSystemService(AUDIO_SERVICE);
            if (audioService.getRingerMode() != AudioManager.RINGER_MODE_NORMAL) {
                playBeep = false;
            }
        }
        if (playBeep && mediaPlayer == null) {
            initBeeper();
        }
        if (playBeep && mediaPlayer != null) {
            mediaPlayer.start();
        }
    }

    @Override
    public void onBarcodeDetected(Barcode barcode) {
        if (lastResult == ""){
            lastResult = barcode.displayValue;
            Log.d(TAG, "Barcode read: " + barcode.displayValue);
            refresh();
            beep();
        }
    }



    private class TimeAndBarcode {
        public TimeAndBarcode(Barcode b){
            barcode = b;
            Date date = new Date();
            time = date.getTime();
        }
        public Barcode barcode = null;
        public long time = 0;
    }

    LinkedList<TimeAndBarcode> barcodesInCenter = new LinkedList<TimeAndBarcode>();
    final int barcodesInCenterMaxSize = 20;
    final int barcodesInCenterMaxTime = 2000;


    @Override
    public void onBarcodeInCenter(Barcode barcode){
        Log.d(TAG, "Barcode detected in center");
        String tempResult = null;

        synchronized (barcodesInCenter) {
            barcodesInCenter.add(new TimeAndBarcode(barcode));

            if (barcodesInCenter.size() < 2) return;
            while(barcodesInCenter.size() > barcodesInCenterMaxSize || 
                (barcodesInCenter.getLast().time - barcodesInCenter.getFirst().time > barcodesInCenterMaxTime)){
                barcodesInCenter.removeFirst();
            }
            if (barcodesInCenter.size() < 3) return;

            if (barcodesInCenter.getLast().time - barcodesInCenter.getFirst().time < 500) return;

            Iterator<TimeAndBarcode> iterator = barcodesInCenter.iterator();
            while (iterator.hasNext()) {
                if (tempResult == null){
                    tempResult = iterator.next().barcode.displayValue;
                }else if (!iterator.next().barcode.displayValue.equals(tempResult)) {
                    Log.d(TAG, "Barcode detected in center: found dublicates");
                    return;
                }
            }
        }

        if (!lastResult.equals(tempResult)){
            lastResult = tempResult;
            refresh();
            beep();
            barcodesInCenter.clear();
        }
    }


    void refresh(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mGraphicOverlay.setResult(lastResult);
                if (lastResult == ""){
                    buttonOk.setVisibility(View.INVISIBLE);
                    buttonRetake.setVisibility(View.INVISIBLE);
                }else{
                    buttonOk.setVisibility(View.VISIBLE);
                    buttonRetake.setVisibility(View.VISIBLE);
                }

            }
        });

    }

    @Override
    public void onClick(View v) {
        switch(v.getId()){
            case R.id.button_flash:
                final boolean isOff = mCameraSource.switchFlashMode();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (isOff){
                            buttonFlash.setImageResource(R.drawable.barcode_flash_off);
                        }else{
                            buttonFlash.setImageResource(R.drawable.barcode_flash_on);
                        }

                    }
                });
                break;
            case R.id.button_retake:
                lastResult = "";
                refresh();
                break;
            case R.id.button_back:
                onCancel();
                break;
            case R.id.barcode_ok:
                onOK();
                break;
            default:
                break;
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mPreview != null) {
            mPreview.release();
        }
        if (currentState == State.OK){
            runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        BarcodeFactory.callOKCallback(lastResult, rhoBarcodeId);
                    }
                }
            );
        }else if (currentState == State.CANCEL){
            runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        BarcodeFactory.callCancelCallback(rhoBarcodeId);
                    }
                }
            );
        }
        currentState = State.EXECUTET;
    }

    enum State{OK, CANCEL, EXECUTET};
    State currentState = State.CANCEL;

    public void onCancel() {
        currentState = State.CANCEL;
        finish();
    }

    public void onOK() {
        if (lastResult != ""){
            currentState = State.OK;
        }
        finish();
    }



}
