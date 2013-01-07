package com.rhomobile.rhodes.extmanager;

import android.app.Dialog;
import android.content.Intent;

import com.rhomobile.rhodes.RhodesActivity;

public abstract class AbstractRhoListener implements IRhoListener {

    @Override
    public void onCreate(RhodesActivity activity, Intent intent) { }

    @Override
    public void onStart(RhodesActivity activity) { }

    @Override
    public void onResume(RhodesActivity activity) { }

    @Override
    public void onPause(RhodesActivity activity) { }

    @Override
    public void onStop(RhodesActivity activity) { }

    @Override
    public void onDestroy(RhodesActivity activity) { }

    @Override
    public void onNewIntent(RhodesActivity activity, Intent intent) { }

    @Override
    public Dialog onCreateDialog(RhodesActivity activity, int id) { return null; }

}
