package com.rhomobile.rhoelements.ans;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.util.ContextFactory;

public class ANSFacade {
    private static final String TAG = ANSFacade.class.getSimpleName();

    private static final String ANS_APP_NAME = "Push.rhoconnect.pushAppName";
    private static final String ANS_SERVER   = "Push.rhoconnect.pushServer";
    private static final String ANS_USER_PREFERENCES_SUFFIX = ".user_preferences";
    private static final String ANS_USER_PREF_USER = "user";
    private static final String ANS_USER_PREF_PASS = "pass";
    private static final String ANS_USER_PREF_SESSION = "session";
    /*package*/ static final String ANS_PUSH_CLIENT = "rhoconnect";

	/*package*/ static String mAppName = null;
	/*package*/ static String mUserName = null;
	/*package*/ static String mUserPassword = null;
    /*package*/ static String mSession = null;
	
	/*package*/ static IRhoExtManager mExtManager = null;
	/*package*/ static Context mContext = null;
	/*package*/ static String mAppPackageName = null;

	public static Context getContext() {
		if (null == mContext) {
			mContext = ContextFactory.getContext();
		}
		return mContext;
	}
	
	public static void installService() {
	    //TODO: install ANS service
	}

    private static String getSharedPrefName(Context context) {
        return context.getApplicationInfo().packageName + ANS_USER_PREFERENCES_SUFFIX;
    }

    public static void register(String userName, String userPassword, String session) {
        Context context = ContextFactory.getContext();
        SharedPreferences userPrefs = context.getSharedPreferences(getSharedPrefName(context), 0);
        
        if (userName != null && userName.length() != 0 &&
            userPassword != null && userPassword.length() !=0) {
            
            Editor userEditor = userPrefs.edit();
            userEditor.putString(ANS_USER_PREF_USER, userName);
            userEditor.putString(ANS_USER_PREF_PASS, userPassword);
            userEditor.putString(ANS_USER_PREF_SESSION, session);
            userEditor.commit();
        }
        else {
            userName = userPrefs.getString(ANS_USER_PREF_USER, "");
            userPassword = userPrefs.getString(ANS_USER_PREF_PASS, "");
            if (session == null || session.length() == 0) {
                session = userPrefs.getString(ANS_USER_PREF_SESSION, "");
            }
        }

        if (userName == null || userName.length() ==0 ||
            userPassword == null || userPassword.length() == 0 ||
            session == null || session.length() == 0) {
            
            PushContract.handleError(context, "Incomplete registration credentials", ANS_PUSH_CLIENT);
        }

        ANSManager.register(context,
                RhoConf.getString(ANS_APP_NAME),
                userName, userPassword, session,
                RhoConf.getString(ANS_SERVER));
    }

    public static void unregister(String session) {
        Context context = ContextFactory.getContext();
        SharedPreferences userPrefs = context.getSharedPreferences(getSharedPrefName(context), 0);

        String userName = userPrefs.getString(ANS_USER_PREF_USER, "");
        String userPassword = userPrefs.getString(ANS_USER_PREF_PASS, "");
        if (session == null || session.length() == 0) {
            session = userPrefs.getString(ANS_USER_PREF_SESSION, "");
        }

        if (userName == null || userName.length() ==0 ||
            userPassword == null || userPassword.length() == 0 ||
            session == null || session.length() == 0) {
            
            PushContract.handleError(context, "Incomplete registration credentials", ANS_PUSH_CLIENT);
        }

        ANSManager.unregister(context, 
                RhoConf.getString(ANS_APP_NAME), 
                userName, userPassword, session, 
                RhoConf.getString(ANS_SERVER));
    }

    public static void checkRegistration(String session) {
        Context context = ContextFactory.getContext();
        SharedPreferences userPrefs = context.getSharedPreferences(getSharedPrefName(context), 0);

        String userName = userPrefs.getString(ANS_USER_PREF_USER, "");
        String userPassword = userPrefs.getString(ANS_USER_PREF_PASS, "");
        if (session == null || session.length() == 0) {
            session = userPrefs.getString(ANS_USER_PREF_SESSION, "");
        }

        if (userName == null || userName.length() ==0 ||
            userPassword == null || userPassword.length() == 0 ||
            session == null || session.length() == 0) {
            
            PushContract.handleError(context, "Incomplete registration credentials", ANS_PUSH_CLIENT);
        }

        ANSManager.checkRegister(getContext(), RhoConf.getString(ANS_APP_NAME), userName, userPassword, session);
    }


}

