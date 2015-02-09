package com.rho.development;

import java.util.Map;

import android.media.AudioManager;
import android.media.ToneGenerator;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.util.Utils;

public class Development extends DevelopmentBase implements IDevelopment {
    
    public Development(String id) {
        super(id);
    }


    @Override
    public void getPlatformName(IMethodResult result) {
          result.set("Android");	
    }


    @Override
    public void calcSumm(int a, int b, IMethodResult result) {
          result.set(a+b);
    }

    @Override
    public void joinStrings(String a, String b, IMethodResult result) {
          result.set(a+b);
    }

    
    private static class RhoGestureListener extends RhodesActivity.GestureListenerAdapter implements RhodesActivity.GestureListener {
		@Override
		public void onTripleTap() {
			Utils.platformLog("Development Extra", "$$$$$ TRIPLE TAP $$$$$");
			ToneGenerator tg = new ToneGenerator(AudioManager.STREAM_NOTIFICATION, 100);
		    tg.startTone(ToneGenerator.TONE_PROP_BEEP);
			onTripleTapNative();
		}
		
		@Override
		public void onQuadroTap() {
			Utils.platformLog("Development Extra", "$$$$$ QUADRO TAP $$$$$");
			onQuadroTapNative();
		}    	
    };
    
    public static void initExtension() {
    	Utils.platformLog("Development Extra", "$$$$$ InitExtension() $$$$$");
    	RhodesActivity.addGestureListener(new RhoGestureListener());
    }
    
    public static native void onTripleTapNative();
    public static native void onQuadroTapNative();

}