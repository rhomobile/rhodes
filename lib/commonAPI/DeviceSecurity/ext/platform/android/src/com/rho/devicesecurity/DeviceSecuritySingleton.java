package com.rho.devicesecurity;

import java.util.LinkedList;
import java.util.List;
import android.content.Context;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.Logger;

class DeviceSecuritySingleton extends DeviceSecuritySingletonBase implements IDeviceSecuritySingleton {

    protected static String TAG = DeviceSecuritySingleton.class.getSimpleName();

    public DeviceSecuritySingleton(DeviceSecurityFactory factory) {
        super(factory);
    }


    @Override
    public void isDeviceSecured(IMethodResult result) {
        result.set(true);
    }

    @Override
    public void isDeviceJailbroken(IMethodResult result) {
        result.set(false);
    }


    public static boolean isDeviceRooted() {
            return checkRootMethod1() || checkRootMethod2() || checkRootMethod3();
        }

        private static boolean checkRootMethod1() {
            String buildTags = android.os.Build.TAGS;
            return buildTags != null && buildTags.contains("test-keys");
        }

        private static boolean checkRootMethod2() {
            String[] paths = { "/system/app/Superuser.apk", "/sbin/su", "/system/bin/su", "/system/xbin/su", "/data/local/xbin/su", "/data/local/bin/su", "/system/sd/xbin/su",
                    "/system/bin/failsafe/su", "/data/local/su", "/su/bin/su"};
            for (String path : paths) {
                if (new File(path).exists()) return true;
            }
            return false;
        }

        private static boolean checkRootMethod3() {
            Process process = null;
            try {
                process = Runtime.getRuntime().exec(new String[] { "/system/xbin/which", "su" });
                BufferedReader in = new BufferedReader(new InputStreamReader(process.getInputStream()));
                if (in.readLine() != null) return true;
                return false;
            } catch (Throwable t) {
                return false;
            } finally {
                if (process != null) process.destroy();
            }
        }



    @Override
    public void isDeviceRooted(IMethodResult result) {
        result.set(isDeviceRooted());
    }


    private static String getSystemProperty(String name) throws Exception {

        Class systemPropertyClazz = Class.forName("android.os.SystemProperties");
        return (String) systemPropertyClazz.getMethod("get", new Class[] { String.class }).invoke(systemPropertyClazz, new Object[] { name });
    }

    private static boolean checkEmulator() {

        try {

          boolean goldfish = getSystemProperty("ro.hardware").contains("goldfish");

          boolean emu = getSystemProperty("ro.kernel.qemu").length() > 0;

          boolean sdk = getSystemProperty("ro.product.model").equals("sdk");

          if (emu || goldfish || sdk) {

            return true;

          }

        } catch (Exception e) {

        }

        return false;

      }

    @Override
    public void isRunOnSimulator(IMethodResult result) {
        result.set(checkEmulator());
    }

    private static boolean checkDebuggable(Context context){

        Context context = null;
        try {
            context = RhodesService.getContext();
        }
        catch (IllegalStateException e) {
            context = null;
            Logger.E(TAG, "Check Debuggable ERROR : Rhodes main Service (com.rhomobile.rhodes.RhodesService) is not run !!!");
        }
        boolean res = false;
        if (context != null) {
            res = (context.getApplicationInfo().flags & ApplicationInfo.FLAG\_DEBUGGABLE) != 0;
        }
        return res;

    }


    @Override
    public void isDebugMode(IMethodResult result)  {
        result.set(checkDebuggable());

    }





}
