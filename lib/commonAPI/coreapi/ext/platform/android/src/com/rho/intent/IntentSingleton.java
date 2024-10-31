package com.rho.intent;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.net.Uri;
import android.net.Uri.Builder;
import android.os.Build;
import android.os.Bundle;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.RhodesApplication;

public class IntentSingleton extends AbstractRhoListener implements IIntentSingleton, IIntentFactory, IRhoListener {
    private static final String TAG = IntentSingleton.class.getSimpleName();
    
    private IMethodResult methodResult;
    private IntentReceiver mReceiver = new IntentReceiver();
   
    private static final String HK_CREATE_CHOOSER = "createChooser";
    private static final String HK_CHOOSER_TITLE = "chooserTitle";
    
    private List<Map.Entry<Integer, IMethodResult>> localMethodResults = new ArrayList<Map.Entry<Integer, IMethodResult>>();

    private String constant(String name) {
        try {
            Class<?> rClass = Class.forName("android.content.Intent");
            Field field = rClass.getDeclaredField(name);
            return (String)field.get(null);
        } catch (Throwable e) {
            return name;
        }
    }
    
    private Intent makeIntent(Map<String, Object> params) {
        Intent intent = new Intent();
        
        Object actionObj = params.get(HK_ACTION);
        Object categoriesObj = params.get(HK_CATEGORIES);
        Object appNameObj = params.get(HK_APP_NAME);
        Object targetClassObj = params.get(HK_TARGET_CLASS);
        Object uriObj = params.get(HK_URI);
        Object mimeObj = params.get(HK_MIME_TYPE);
        Object extrasObj = params.get(HK_DATA);
        Object createChooserObj = params.get(HK_CREATE_CHOOSER);
        Object chooserTitleObj = params.get(HK_CHOOSER_TITLE);

        String action = null;
        List<String> categories = null;
        String appName = null;
        String targetClass = null;
        String uri = null;
        String mime = null;
        Map<String, Object> extras = null;
        Boolean createChooser = null;
        String chooserTitle = null;

        //--- Check param types ---

        if (actionObj != null) {
            if (!String.class.isInstance(actionObj)) {
                throw new RuntimeException("Wrong intent action: " + actionObj.toString());
            }
            action = constant((String)actionObj);
        }

        if (categoriesObj != null) {
            if (!List.class.isInstance(categoriesObj)) {
                throw new RuntimeException("Wrong intent categories: " + categoriesObj.toString());
            }
            categories = new ArrayList<String>();
            for (String cat:(List<String>)categoriesObj) {
                categories.add(constant(cat));
            }
        }

        if (appNameObj != null) {
            if (!String.class.isInstance(appNameObj)) {
                throw new RuntimeException("Wrong intent appName: " + appNameObj.toString());
            }
            appName = (String)appNameObj;
        }

        if (targetClassObj != null) {
            if (!String.class.isInstance(targetClassObj)) {
                throw new RuntimeException("Wrong intent targetClass: " + targetClassObj.toString());
            }
            targetClass = (String)targetClassObj;
        }

        if (uriObj != null) {
            if (!String.class.isInstance(uriObj)) {
                throw new RuntimeException("Wrong intent uri: " + uriObj.toString());
            }
            uri = (String)uriObj;
        }

        if (mimeObj != null) {
            if (!String.class.isInstance(mimeObj)) {
                throw new RuntimeException("Wrong intent mimeType: " + mimeObj.toString());
            }
            mime = (String)mimeObj;
        }

        if (extrasObj != null) {
            if (!Map.class.isInstance(extrasObj)) {
                throw new RuntimeException("Wrong intent data: " + extrasObj.toString());
            }
            extras = (Map<String, Object>)extrasObj;
        }

        if (createChooserObj != null) {
            if (!Boolean.class.isInstance(createChooserObj)) {
                throw new RuntimeException("Wrong intent createChooser: " + createChooserObj.toString());
            }
            createChooser = (Boolean)createChooserObj;
        }
        
        if (chooserTitleObj != null) {
            if (!String.class.isInstance(chooserTitleObj)) {
                throw new RuntimeException("Wrong intent chooserTitle: " + chooserTitleObj.toString());
            }
            chooserTitle = (String)chooserTitleObj;
        }

        //--- Fill intent fields ---
        
        if (action != null) { 
            intent.setAction(action);
        }

        if (categories != null) {
            for(String category : categories) {
                intent.addCategory(category);
                if(intent.toString().contains(" xxx ") && isTablet(ContextFactory.getContext())) {
                	intent.removeCategory(category);
                }
            }
        }
        
        if (targetClass != null) {
            if (appName == null) {
                throw new RuntimeException("Wrong intent appName: cannot be nil if targetClass is set");
            }
            intent.setClassName(appName, targetClass);
        }
        else if (appName != null){
            intent.setPackage(appName);
        }

        if (uri != null) {
            Uri data = Uri.parse(uri);
            if (mime == null) {
                intent.setData(data);
            }
            else {
                if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
                {
                    intent.setDataAndTypeAndNormalize(data, mime);
                }
                else {
                    intent.setDataAndType(data, mime);
                }
            }
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_ACTIVITY_FORWARD_RESULT);
        }
        else if (mime != null) {
            if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            {
                intent.setTypeAndNormalize(mime);
            }
            else {
                intent.setType(mime);
            }
        }

        if (extras != null) {
            for (Map.Entry<String, Object> entry: extras.entrySet()) {

                if (String.class.isInstance(entry.getValue())) {
                    if (entry.getKey().equals("output") && intent.toString().contains("VIDEO_CAPTURE")){
                        Logger.T(TAG, "Action is VIDEO_CAPTURE");
                        try{
                            intent.putExtra(constant(entry.getKey()), Uri.parse((String)entry.getValue()));
                        }catch(Exception e){Logger.T(TAG, "Error on building intent: " + e.getMessage());}
                    }else{
                        if (entry.getKey().equals("android.intent.extra.STREAM")){
                            Logger.T(TAG, "key is android.intent.extra.STREAM - value hould be convert to Uri");
                            try{
                                intent.putExtra(constant(entry.getKey()), Uri.parse(Uri.decode((String)entry.getValue())));
                            }catch(Exception e){Logger.T(TAG, "Error on building intent: " + e.getMessage());}
                        }else{
                            intent.putExtra(constant(entry.getKey()), (String)entry.getValue());
                            Logger.T(TAG, "Putting extra to intent: key - " + entry.getKey() + ", type - String");
                        }
                    }

                }
                else if (Boolean.class.isInstance(entry.getValue())) {
                    intent.putExtra(constant(entry.getKey()), ((Boolean)entry.getValue()).booleanValue());
                    Logger.T(TAG, "Putting extra to intent: key - " + entry.getKey() + ", type - Boolean");
                }
                else if (Integer.class.isInstance(entry.getValue())) {
                    intent.putExtra(constant(entry.getKey()), ((Integer)entry.getValue()).intValue());
                    Logger.T(TAG, "Putting extra to intent: key - " + entry.getKey() + ", type - Int");
                }
                else if (Double.class.isInstance(entry.getValue())) {
                    intent.putExtra(constant(entry.getKey()), ((Double)entry.getValue()).doubleValue());
                    Logger.T(TAG, "Putting extra to intent: key - " + entry.getKey() + ", type - Double");
                }
                else if (ArrayList.class.isInstance(entry.getValue())) {
                    Logger.T(TAG, "Putting extra to intent: key - " + entry.getKey() + ", type - ArrayList");
                    ArrayList list = (ArrayList)entry.getValue();
                    if ( list.size() > 0 ) {
                        if (String.class.isInstance(list.get(0))) {
                            intent.putExtra(constant(entry.getKey()),list.toArray(new String[list.size()]));
                        } else if (Integer.class.isInstance(list.get(0))) {
                            intent.putExtra(constant(entry.getKey()),list.toArray(new Integer[list.size()]));
                        } else {
                            throw new RuntimeException("Wrong intent data: array of " + list.get(0).getClass().getName() + " is not supported as value");
                        }
                    } else {
                        intent.putExtra(constant(entry.getKey()),new String[0]);
                    }
                }
                else {
                    throw new RuntimeException("Wrong intent data: " + entry.getValue().getClass().getName() + " is not supported as value");
                }
            }
        }
        
        if (createChooser != null && createChooser){
            intent = Intent.createChooser(intent, chooserTitle);
        }
        
        return intent;
    }
    
    private boolean isTablet(Context context) {
		return (context.getResources().getConfiguration().screenLayout 
				& Configuration.SCREENLAYOUT_SIZE_MASK) 
				>= Configuration.SCREENLAYOUT_SIZE_LARGE;
	}
	
    private Map<String, Object> parseIntent(Intent intent) {
        Map<String, Object> params = new HashMap<String, Object>();
        
        if (intent != null) {
            String action = intent.getAction();
            if (action != null) {
                params.put(HK_ACTION, action);
            }
            
            Set<String> categories = intent.getCategories();
            if (categories != null) {
                params.put(HK_CATEGORIES, categories);
            }
            
            String appName = intent.getPackage();
            if (appName != null) {
                params.put(HK_APP_NAME, appName);
            }
            
            String uri = intent.getDataString();
            if (uri != null) {
                uri = Uri.decode(uri);
                params.put(HK_URI, uri);
            }
            
            String mime = intent.getType();
            if (mime != null) {
                params.put(HK_MIME_TYPE, mime);
            }
            
            Bundle extras = intent.getExtras();
            if (extras != null) {
                params.put(HK_DATA, extras);
            }
        }
        
        return params;
    }
    
   /* @Override
    public void onDestroy(RhodesActivity activity) {
    	super.onDestroy(activity);
    	if(IntentRhoExtension.RECEIVER_REGISTER_STATUS == 1){
    		try {
        		RhodesActivity.safeGetInstance().getApplicationContext().unregisterReceiver(mReceiver);
        		IntentRhoExtension.RECEIVER_REGISTER_STATUS =0;
    		} catch (IllegalArgumentException e) {
    		}
    	}
    }
    */
    
    @Override
    public void send(Map<String, Object> params, final IMethodResult result) {
        Intent intent = makeIntent(params);
        Object type = params.get(HK_INTENT_TYPE);
        if (BROADCAST.equals(type)) {
            Object permissionObj = params.get(HK_PERMISSION);
            String permission = null;
            if (permissionObj != null) {
                if (String.class.isInstance(permissionObj)) {
                    permission = (String)permissionObj;
                }
                else {
                    result.setArgError("Wrong intent permission: " + permissionObj);
                    return;
                }
            }
            Logger.T(TAG, "Send broadcast: " + intent);
            ContextFactory.getAppContext().sendBroadcast(intent, permission);
        }
        else if (START_ACTIVITY.equals(type)) {
            if (result.hasCallback()) {
                int request;
                synchronized (localMethodResults) {
                    request = RhoExtManager.getInstance().getActivityResultNextRequestCode(this);
                    final Integer finalKey = Integer.valueOf(request);
                    Map.Entry<Integer, IMethodResult> entry = new Map.Entry<Integer, IMethodResult>() {
                        Integer key = finalKey;
                        IMethodResult value = result;
                        @Override public Integer getKey() { return key; }
                        @Override public IMethodResult getValue() { return value; }
                        @Override public IMethodResult setValue(IMethodResult v) { return result; }
                    };
                    localMethodResults.add(entry);
                }

                try{
                    RhodesActivity.safeGetInstance().startActivityForResult(intent, request);
                    Logger.T(TAG, "Start activity for result: " + intent);
                }
                catch (Exception e) {
                    Logger.E(TAG, e);
                    return;
                }
            }
            else {
                Logger.T(TAG, "Start activity: " + intent);
                ContextFactory.getUiContext().startActivity(intent);
            }
        }
        else if (START_SERVICE.equals(type)) {
            Logger.T(TAG, "Start service: " + intent);
            ContextFactory.getContext().startService(intent);
        }
        else {
            result.setArgError("Wrong intent type: " + type);
        }
    }

    @Override
    synchronized public void startListening(IMethodResult result) {
        if (methodResult!= null) {
            methodResult.release();
        }
        methodResult = result;
        methodResult.keepAlive();
    }

    @Override
    synchronized public void stopListening(IMethodResult result) {
        if (methodResult != null) {
            methodResult.release();
            methodResult = null;
        }
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

    @Override
    public void onNewIntent(RhodesActivity activity, Intent intent) {
        onNewIntent(START_ACTIVITY, intent);
    }

    @Override
    public void onNewIntent(RhodesService activity, Intent intent) {
        onNewIntent(START_SERVICE, intent);
    }
    
    synchronized public void onNewIntent(String type, Intent intent) {
        if (methodResult != null) {
            Logger.T(TAG, "New Intent: " + type);
            Map<String, Object> params = parseIntent(intent);
            params.put(HK_INTENT_TYPE, type);
            methodResult.set(params);
        }
    }
    
    @Override
    public void onActivityResult(RhodesActivity activity, int requestCode, int resCode, Intent intent) {
        IMethodResult result = null;
        synchronized (localMethodResults) {
            Iterator<Map.Entry<Integer, IMethodResult> > iter = localMethodResults.iterator();
            while (iter.hasNext()) {
            //for (Map.Entry<Integer, IMethodResult> resultEntry : localMethodResults) {
                Map.Entry<Integer, IMethodResult> resultEntry = iter.next();
                if(resultEntry.getKey().intValue() == requestCode) {
                    Logger.T(TAG, "Activity result request: " + requestCode);
                    result = resultEntry.getValue();
                    iter.remove();
                }
            }
        }
        if (result != null) {
            Map<String, Object> params = parseIntent(intent);
            params.put(HK_INTENT_TYPE, START_ACTIVITY);
            params.put(HK_RESPONSE_CODE, Integer.valueOf(resCode));
            result.set(params);
        }
    }
    @Override
    public void cleanStartParameters(IMethodResult result){
        RhodesApplication.setStartParametersApp("");
    
    }


}
