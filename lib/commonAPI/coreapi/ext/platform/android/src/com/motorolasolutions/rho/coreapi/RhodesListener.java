package com.motorolasolutions.rho.coreapi;

import com.rho.intent.IntentSingleton;
import com.rho.notification.NotificationFactory;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;

public class RhodesListener extends AbstractRhoListener implements IRhoListener {


    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        new NotificationFactory().onCreateApplication(extManager);
        new IntentSingleton().onCreateApplication(extManager);

    }

}
