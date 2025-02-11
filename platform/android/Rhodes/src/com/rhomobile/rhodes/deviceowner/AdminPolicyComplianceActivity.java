package com.rhomobile.rhodes.deviceowner;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.rhomobile.rhodes.R;


import androidx.annotation.Nullable;

public class AdminPolicyComplianceActivity extends Activity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_policy_compliance);

        Intent intent = getIntent();

        setResult(RESULT_OK, intent);
        finish();
    }
}