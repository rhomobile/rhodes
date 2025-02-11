package com.rhomobile.rhodes.deviceowner;

import static android.app.admin.DevicePolicyManager.EXTRA_PROVISIONING_ADMIN_EXTRAS_BUNDLE;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.PersistableBundle;

import com.rhomobile.rhodes.R;


import java.util.List;


public class ProvisioningModeActivity extends Activity {
    private String EXTRA_PROVISIONING_ALLOWED_PROVISIONING_MODES = "android.app.extra.PROVISIONING_ALLOWED_PROVISIONING_MODES";
    private int PROVISIONING_MODE_FULLY_MANAGED_DEVICE = 1;
    private int PROVISIONING_MODE_MANAGED_PROFILE = 2;
    private String EXTRA_PROVISIONING_MODE = "android.app.extra.PROVISIONING_MODE";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_get_provisioning_mode);

        Intent intent = getIntent();
        int provisioningMode = PROVISIONING_MODE_FULLY_MANAGED_DEVICE;
        List<Integer> allowedProvisioningModes = intent.getIntegerArrayListExtra(EXTRA_PROVISIONING_ALLOWED_PROVISIONING_MODES);

        if(allowedProvisioningModes != null){
            if(allowedProvisioningModes.contains(PROVISIONING_MODE_FULLY_MANAGED_DEVICE))
                provisioningMode = PROVISIONING_MODE_FULLY_MANAGED_DEVICE;
            else if (allowedProvisioningModes.contains(PROVISIONING_MODE_MANAGED_PROFILE))
                provisioningMode = PROVISIONING_MODE_MANAGED_PROFILE;
        }

        PersistableBundle extras = intent.getParcelableExtra(EXTRA_PROVISIONING_ADMIN_EXTRAS_BUNDLE);
        Intent resultIntent = getIntent();

        if(extras != null){
            if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
                resultIntent.putExtra(EXTRA_PROVISIONING_ADMIN_EXTRAS_BUNDLE, extras);
        }
        resultIntent.putExtra(EXTRA_PROVISIONING_MODE, provisioningMode);

        setResult(RESULT_OK, resultIntent);
        finish();
    }
}
