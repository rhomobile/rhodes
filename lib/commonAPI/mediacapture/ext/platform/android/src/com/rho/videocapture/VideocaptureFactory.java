package com.rho.videocapture;

import android.app.Dialog;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.KeyEvent;

import com.rho.videocapture.IVideocaptureFactory;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.RhoApiFactory;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;

import java.util.Map;
import java.util.HashMap;

public class VideocaptureFactory
        extends RhoApiFactory<Videocapture, VideocaptureSingleton>
        implements IVideocaptureFactory, IRhoListener {

    private String defaultId = "_DEFAULT_CAMERA";
    private final static Map<String, Videocapture> videocaptureKeeper = new HashMap<String, Videocapture>();

    @Override
    protected VideocaptureSingleton createSingleton() {
        return new VideocaptureSingleton(this);
    }

    @Override
    protected Videocapture createApiObject(String id) {
        defaultId = id;
        if (!videocaptureKeeper.containsKey(id)) { videocaptureKeeper.put(id, new Videocapture(id)); }
        return videocaptureKeeper.get(id);
    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        extManager.addRhoListener(this);
    }

    @Override
    public void onActivityResult(RhodesActivity activity, int requestCode, int resultCode, Intent intent) {
        getApiObject(defaultId).onActivityResultVideoCapture(activity, requestCode, resultCode, intent);
    }

    @Override
    public void onCreate(RhodesActivity activity, Intent intent) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onStart(RhodesActivity activity) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onResume(RhodesActivity activity) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onPause(RhodesActivity activity) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onStop(RhodesActivity activity) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onDestroy(RhodesActivity activity) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onNewIntent(RhodesActivity activity, Intent intent) {
        // TODO Auto-generated method stub
    }

    @Override
    public Dialog onCreateDialog(RhodesActivity activity, int id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void onConfigurationChanged(RhodesActivity activity, Configuration newConfig) {
        // TODO Auto-generated method stub
    }

    @Override
    public boolean onKey(int keyCode, KeyEvent event) {
        return false;
    }

    @Override
    public void onNewIntent(RhodesService service, Intent intent) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        // TODO Auto-generated method stub
    }

	@Override
	public void onEBLicenseVisible() { }

	@Override
	public void onEBLicenseHidden() { }

	@Override
	public void onEBLicenseDestroyed() { }
}
