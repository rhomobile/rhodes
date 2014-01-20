package com.rho.intent;

import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;

public class IntentSingleton extends AbstractRhoListener implements IIntentSingleton, IIntentFactory, IRhoListener {

    @Override
    public void send(Map<String, Object> params, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void startListening(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void stopListening(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public IIntentSingleton getApiSingleton() {
        return this;
    }

    @Override
    public IIntent getApiObject(String id) {
        return null;
    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        IntentFactorySingleton.setInstance(this);
        extManager.addRhoListener(this);
    }

}
