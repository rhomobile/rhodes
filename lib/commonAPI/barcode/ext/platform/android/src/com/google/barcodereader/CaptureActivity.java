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

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.Button;

import com.google.android.gms.common.api.CommonStatusCodes;
import com.google.android.gms.vision.barcode.Barcode;
import com.rhomobile.rhodes.R;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rho.barcode.BarcodeFactory;
/**
 * Main activity demonstrating how to pass extra parameters to an activity that
 * reads barcodes.
 */
public class CaptureActivity extends BaseActivity implements View.OnClickListener {

    // use a compound button so either checkbox or switch widgets work.
    private Button buttonOk;
    private CompoundButton useFlash;
    private TextView statusMessage;
    private TextView barcodeValue;
    private String rhoBarcodeId;

    private static final int RC_BARCODE_CAPTURE = 9001;
    private static final String TAG = "BarcodeMain";
    
    public static final String CAMERA_INDEX_EXTRA = "camera_index"; 
    public static final String RHO_BARCODE_ID = "barcode_obj_id";
    public String lastResult = "";
    int camera_index = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.capture_activity);

        Intent intent = getIntent();
        if (intent != null) {
            camera_index = intent.getIntExtra(CAMERA_INDEX_EXTRA, 0);
            Logger.D(TAG, "Intent Camera index: " + camera_index);
            this.camera_index = camera_index;
            rhoBarcodeId = intent.getStringExtra(RHO_BARCODE_ID);
        }

        statusMessage = (TextView)findViewById(R.id.status_message);
        barcodeValue = (TextView)findViewById(R.id.barcode_value);

        useFlash = (CompoundButton) findViewById(R.id.use_flash);
        buttonOk = (Button) findViewById(R.id.barcode_ok);

        findViewById(R.id.read_barcode).setOnClickListener(this);
        buttonOk.setOnClickListener(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        refresh();
        
    }

    void refresh(){
        if (lastResult == ""){
            buttonOk.setVisibility(View.INVISIBLE);
        }else{
            buttonOk.setVisibility(View.VISIBLE);
        }
        barcodeValue.setText(lastResult);
    }

    @Override
    public void onClick(View v) {
        switch(v.getId()){
            case R.id.read_barcode:
                lastResult = "";
                refresh();
                Intent intent = new Intent(this, BarcodeCaptureActivity.class);
                intent.putExtra(BarcodeCaptureActivity.UseFlash, useFlash.isChecked());
                startActivityForResult(intent, RC_BARCODE_CAPTURE);
                break;
            case R.id.barcode_ok:
                onOK();
                break;
            default:
                break;
        }

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == RC_BARCODE_CAPTURE) {
            if (resultCode == CommonStatusCodes.SUCCESS) {
                if (data != null) {
                    Barcode barcode = data.getParcelableExtra(BarcodeCaptureActivity.BarcodeObject);
                    statusMessage.setText(R.string.barcode_success);
                    lastResult = barcode.displayValue;
                    barcodeValue.setText(lastResult);
                    Log.d(TAG, "Barcode read: " + barcode.displayValue);
                } else {
                    statusMessage.setText(R.string.barcode_failure);
                    Log.d(TAG, "No barcode captured, intent data is null");
                }
            } else {
                statusMessage.setText(String.format(getString(R.string.barcode_error),
                        CommonStatusCodes.getStatusCodeString(resultCode)));
            }
        }
        else {
            super.onActivityResult(requestCode, resultCode, data);
        }
        refresh();

    }

    public void onCancel() {
        BarcodeFactory.callCancelCallback(rhoBarcodeId);
    }

    public void onOK() {
        if (lastResult != ""){
            buttonOkClicked = true;
            BarcodeFactory.callOKCallback(lastResult, rhoBarcodeId);
            finish();
        }
    }
    boolean buttonOkClicked = false;

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (!buttonOkClicked){
            onCancel();
        }
    }

}
