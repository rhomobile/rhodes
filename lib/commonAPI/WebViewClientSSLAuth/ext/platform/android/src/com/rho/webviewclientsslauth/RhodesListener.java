package com.rho.webviewclientsslauth;

import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;

import com.rhomobile.rhodes.Logger;

public class RhodesListener extends AbstractRhoListener implements IRhoListener {

    static final String TAG = "webviewclientsslauth:RhodesListener";

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
      Logger.I(TAG,"Registering extension");      
      extManager.registerExtension("webviewclientsslauthext", new WebViewClientSSLAuthExt());
    }

}
