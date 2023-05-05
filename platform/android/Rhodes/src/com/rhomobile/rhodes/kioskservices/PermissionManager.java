package com.rhomobile.rhodes.kioskservices;

import android.Manifest;
import android.accessibilityservice.AccessibilityServiceInfo;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.provider.Settings;
import android.view.accessibility.AccessibilityManager;
import android.widget.Toast;


import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import java.util.function.Predicate;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class PermissionManager {
    final static private Integer MY_PERMISSIONS_REQUEST = 343457842;

    static public Boolean checkPermissions(Context context, Activity activity){
        return checkAccessibilityServicePermission(context)
                && checkNotificationServicePermission(activity, context)
                && isMyLauncherDefault(context)
                && checkCallPhonePermission(context)
                && checkOverlayPermission(context);
    }

    static public void setAllPermissions(Context context, Activity activity){
        if (!checkAccessibilityServicePermission(context))
            setAccessibilityServicePermission(activity);
        else if(!checkNotificationServicePermission(activity, context))
            setNotificationServicePermission(activity);
        else if(!isMyLauncherDefault(context))
            setDefaultLauncher(activity);
        else if(!checkCallPhonePermission(context))
            setCallPhonePermission(activity);
        else if(!checkOverlayPermission(context))
            setOverlayPermission(activity);
    }

    //===================== ACCESSIBILITY SERVICE =====================

    static public Boolean checkAccessibilityServicePermission(Context context){
        AccessibilityManager am = (AccessibilityManager)context.getSystemService(Context.ACCESSIBILITY_SERVICE);
        List<AccessibilityServiceInfo> enabledService = am.getEnabledAccessibilityServiceList(AccessibilityServiceInfo.FEEDBACK_ALL_MASK);
        boolean status = false;
        for (AccessibilityServiceInfo service: enabledService){
            if(service.getId().equals(getAccessibilityServiceName(context.getPackageName(), MyAccessibilityService.class.getName()))){
                status = true;
                break;
            }
        }
        return status;
    }

    static public void setAccessibilityServicePermission(Activity activity){
        Intent intent = new Intent(Settings.ACTION_ACCESSIBILITY_SETTINGS);
        activity.startActivity(intent);
    }


    static public String getAccessibilityServiceName(String namePackage, String accessibilityService) {
        String droped = accessibilityService.replace(namePackage, "");
        System.out.println("WERTYUIOUYTREWERTGYHJKL NamePackage: "+ namePackage+"; accessibilityService"+accessibilityService);
        System.out.println("WERTYUIOUYTREWERTGYHJKL: "+ namePackage+"/"+droped);
        return namePackage+"/"+droped;
    }

    //===================== NOTIFICATION SERVICE =====================

    static public Boolean checkNotificationServicePermission(Activity activity, Context context){
        String theList = Settings.Secure.getString(activity.getContentResolver(), "enabled_notification_listeners");
        if (theList == null)
            theList = "";
        String[] theListList = theList.split(":", -1);

        List<String> theListListFiltered = new ArrayList<String>();
        for (String str : theListList){
            if (!str.isEmpty()) {
                theListListFiltered.add(str.toString());
            }
        }
        
        for (String str: theListListFiltered){
            if(str.equals("")){
                theListListFiltered.add("");
            }
        }

        String me = new ComponentName(context, MyNotificationListenerService.class).flattenToString();
        for (String next : theListListFiltered) {
            if (me.equals(next))
                return true;
        }
        return false;
    }

    static public void setNotificationServicePermission(Activity activity){
        activity.startActivity(new Intent("android.settings.ACTION_NOTIFICATION_LISTENER_SETTINGS"));
    }

    //===================== DEFAULT LAUNCHER =====================

    static public Boolean isMyLauncherDefault(Context context) {
        IntentFilter filter = new IntentFilter(Intent.ACTION_MAIN);
        filter.addCategory(Intent.CATEGORY_HOME);
        List<IntentFilter> filters = new ArrayList<IntentFilter>();
        filters.add(filter);
        String myPackageName = context.getPackageName();
        List<ComponentName> activities = new ArrayList<ComponentName>();
        PackageManager packageManager = context.getPackageManager();

        // You can use name of your package here as third argument
        packageManager.getPreferredActivities(filters, activities, null);
        for (ComponentName activity : activities) {
            if (myPackageName.equals(activity.getPackageName())) {
                return true;
            }
        }
        return false;
    }

    static public void setDefaultLauncher(Activity activity){
        Intent intent = new Intent(Settings.ACTION_HOME_SETTINGS);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        activity.startActivity(intent);
        Toast.makeText(activity.getApplicationContext(), "PLease select Tau browser with default home app", Toast.LENGTH_SHORT).show();
        activity.finishAffinity();
    }

    static public void setDefaultLauncherSystem(Context context, Class ClassForLauncher){
        PackageManager pm = context.getPackageManager();
        ComponentName cn = new ComponentName(context.getApplicationContext(), ClassForLauncher);

        pm.clearPackagePreferredActivities(context.getPackageName());

        pm.setComponentEnabledSetting(cn, PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);
        pm.setComponentEnabledSetting(cn, PackageManager.COMPONENT_ENABLED_STATE_ENABLED, PackageManager.DONT_KILL_APP);
    }

    //===================== CALL PHONE =====================

    static public Boolean checkCallPhonePermission(Context context){
        return ContextCompat.checkSelfPermission(context, Manifest.permission.CALL_PHONE) == PackageManager.PERMISSION_GRANTED;
    }

    static public void setCallPhonePermission(Activity activity){
        String[] permissions = {Manifest.permission.CALL_PHONE};
        ActivityCompat.requestPermissions(activity, permissions, MY_PERMISSIONS_REQUEST);
    }

    //===================== OVERLAY PERMISSION =====================

    static public Boolean checkOverlayPermission(Context context){
        return Settings.canDrawOverlays(context);
    }

    static public void setOverlayPermission(Activity activity){
        Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:"+ activity.getPackageName()));
        activity.startActivity(intent);
    }
}
