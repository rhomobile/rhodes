package com.motorolasolutions.rho.screenorientation;

import java.util.Map;

import android.app.Dialog;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Rect;

import com.rho.screenorientation.IScreenOrientation;
import com.rho.screenorientation.ScreenOrientationBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class ScreenOrientation extends ScreenOrientationBase 
	implements IScreenOrientation, IRhoExtension, IRhoListener
{
    private String TAG = ScreenOrientation.class.getSimpleName();
    
    private OrientationPlugin mOrientationPlugin = null;
    
    public ScreenOrientation(String id)
    {
	super(id);
        RhoExtManager.getInstance().registerExtension("screenorientation", this);
        Logger.D(TAG, "ScreenOrientation object constructed correctly");
    }
    
    @Override
    public void setAutoRotate(boolean autoRotate, IMethodResult result)
    {
	Logger.D(TAG, "setAutorotate -- START");
	super.setAutoRotate(autoRotate, result);
	Logger.D(TAG, "setAutorotate -- mOrientationPlugin: " + mOrientationPlugin);
	mOrientationPlugin.setAutoRotate(autoRotate);
	Logger.D(TAG, "setAutorotate -- END");
    }

    @Override
    public void normal(IMethodResult result)
    {
	mOrientationPlugin.onSetting("normal", null);
    }

    @Override
    public void rightHanded(IMethodResult result)
    {
	mOrientationPlugin.onSetting("rightHanded", null);
    }

    @Override
    public void leftHanded(IMethodResult result) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void upsideDown(IMethodResult result) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onCreate(RhodesActivity activity, Intent intent)
    {
	Logger.D(TAG, "onCreate -- START");
    }

    @Override
    public void onStart(RhodesActivity activity)
    {
	Logger.D(TAG, "onStart -- START");
	mOrientationPlugin = new OrientationPlugin(activity);
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
    public void onActivityResult(RhodesActivity activity, int requestCode,
	    int resultCode, Intent intent) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public Dialog onCreateDialog(RhodesActivity activity, int id) {
	// TODO Auto-generated method stub
	return null;
    }

    @Override
    public void onConfigurationChanged(RhodesActivity activity,
	    Configuration newConfig) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public IRhoWebView onCreateWebView(IRhoExtManager extManager, int tabIndex) {
	// TODO Auto-generated method stub
	return null;
    }

    @Override
    public boolean onWebViewCreated(IRhoExtManager extManager, IRhoWebView ext,
	    boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onSetPropertiesData(IRhoExtManager extManager,
	    String propId, String data, int pos, int total, IRhoWebView ext,
	    boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onSetPropertiesDataEnd(IRhoExtManager extManager,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onSetProperty(IRhoExtManager extManager, String name,
	    String value, IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onBeforeNavigate(IRhoExtManager extManager, String url,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onNavigateProgress(IRhoExtManager extManager, String url,
	    int pos, int total, IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onNavigateComplete(IRhoExtManager extManager, String url,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onDocumentComplete(IRhoExtManager extManager, String url,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onAlert(IRhoExtManager extManager, String message,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onConfirm(IRhoExtManager extManager, String message,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onPrompt(IRhoExtManager extManager, String message,
	    String defaultResponse, IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onSelect(IRhoExtManager extManager, String[] items,
	    int selected, IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onStatus(IRhoExtManager extManager, String status,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onTitle(IRhoExtManager extManager, String title,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onConsole(IRhoExtManager extManager, String message,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onInputMethod(IRhoExtManager extManager, boolean enabled,
	    String type, Rect area, IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onNavigateError(IRhoExtManager extManager, String url,
	    LoadErrorReason reason, IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onAuthRequired(IRhoExtManager extManager, String type,
	    String url, String realm, IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public void onAppActivate(IRhoExtManager extManager, boolean bActivate) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public boolean startLocationUpdates(IRhoExtManager extManager,
	    boolean highAccuracy, IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean stopLocationUpdates(IRhoExtManager extManager,
	    IRhoWebView ext, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public boolean onNewConfig(IRhoExtManager extManager, IRhoConfig config,
	    String name, boolean res) {
	// TODO Auto-generated method stub
	return false;
    }

    @Override
    public String onGetProperty(IRhoExtManager extManager, String name) {
	// TODO Auto-generated method stub
	return null;
    }

    
}