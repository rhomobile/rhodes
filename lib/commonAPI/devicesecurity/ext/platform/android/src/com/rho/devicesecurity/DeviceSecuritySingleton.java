package com.rho.devicesecurity;

import java.util.Collection;
import java.util.LinkedList;
import java.util.List;
import java.util.ArrayList;

import java.io.File;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import java.security.MessageDigest;

import android.util.Base64;
import android.os.Build;
import android.os.Environment;
import android.bluetooth.BluetoothAdapter;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

import android.content.Context;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.Utils;

class DeviceSecuritySingleton extends DeviceSecuritySingletonBase implements IDeviceSecuritySingleton {

    protected static String TAG = DeviceSecuritySingleton.class.getSimpleName();

    public DeviceSecuritySingleton(DeviceSecurityFactory factory) {
        super();
    }


    @Override
    public void isDeviceSecured(IMethodResult result) {
        result.set((!checkDeviceRooted()) && (!checkEmulator()) && (!checkDebuggable()) && checkAllowedInstallers() && checkAllowedSignatures() );
    }

    @Override
    public void isDeviceJailbroken(IMethodResult result) {
        result.set(false);
    }


    public static boolean checkDeviceRooted() {
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
        result.set(checkDeviceRooted());
    }


    private static String getSystemProperty(String name) throws Exception {

        Class systemPropertyClazz = Class.forName("android.os.SystemProperties");
        return (String) systemPropertyClazz.getMethod("get", new Class[] { String.class }).invoke(systemPropertyClazz, new Object[] { name });
    }

    private static boolean checkEmulator() {
        return checkEmulatorMethod3();
    }

    private static boolean checkEmulatorMethod1() {

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

    private static boolean checkEmulatorMethod2() {
        return Build.FINGERPRINT.startsWith("generic")
            || Build.FINGERPRINT.startsWith("unknown")
            || Build.MODEL.contains("google_sdk")
            || Build.MODEL.contains("Emulator")
            || Build.MODEL.contains("Android SDK built for x86")
            || Build.MANUFACTURER.contains("Genymotion")
            || (Build.BRAND.startsWith("generic") && Build.DEVICE.startsWith("generic"))
            || "google_sdk".equals(Build.PRODUCT)
            || "goldfish".equals(Build.HARDWARE);
    }

    private static boolean checkEmulatorMethod3() {
        int rating = 0;

        if (Build.PRODUCT.contains("sdk") ||
                Build.PRODUCT.contains("Andy") ||
                Build.PRODUCT.contains("ttVM_Hdragon") ||
                Build.PRODUCT.contains("google_sdk") ||
                Build.PRODUCT.contains("Droid4X") ||
                Build.PRODUCT.contains("sdk_x86") ||
                Build.PRODUCT.contains("sdk_google") ||
                Build.PRODUCT.contains("vbox86p")) {
            rating++;
        }

        if (Build.MANUFACTURER.equals("unknown") ||
                Build.MANUFACTURER.equals("Genymotion") ||
                Build.MANUFACTURER.contains("Andy") ||
                Build.MANUFACTURER.contains("TiantianVM")){
            rating++;
        }

        if (Build.BRAND.equals("generic") ||
                Build.BRAND.equals("generic_x86") ||
                Build.BRAND.equals("TTVM") ||
                Build.BRAND.contains("Andy")) {
            rating++;
        }

        if (Build.DEVICE.contains("generic") ||
                Build.DEVICE.contains("generic_x86") ||
                Build.DEVICE.contains("Andy") ||
                Build.DEVICE.contains("ttVM_Hdragon") ||
                Build.DEVICE.contains("Droid4X") ||
                Build.DEVICE.contains("generic_x86_64") ||
                Build.DEVICE.contains("vbox86p")) {
            rating++;
        }

        if (Build.MODEL.equals("sdk") ||
                Build.MODEL.equals("google_sdk") ||
                Build.MODEL.contains("Droid4X") ||
                Build.MODEL.contains("TiantianVM") ||
                Build.MODEL.contains("Andy") ||
                Build.MODEL.equals("Android SDK built for x86_64") ||
                Build.MODEL.equals("Android SDK built for x86")) {
            rating++;
        }

        if (Build.HARDWARE.equals("goldfish") ||
                Build.HARDWARE.equals("vbox86") ||
                Build.HARDWARE.contains("ttVM_x86")) {
            rating++;
        }

        if (Build.FINGERPRINT.contains("generic/sdk/generic") ||
                Build.FINGERPRINT.contains("generic_x86/sdk_x86/generic_x86") ||
                Build.FINGERPRINT.contains("Andy") ||
                Build.FINGERPRINT.contains("ttVM_Hdragon") ||
                Build.FINGERPRINT.contains("generic_x86_64") ||
                Build.FINGERPRINT.contains("generic/google_sdk/generic") ||
                Build.FINGERPRINT.contains("vbox86p") ||
                Build.FINGERPRINT.contains("generic/vbox86p/vbox86p")) {
            rating++;
        }
/*
        try {
            String opengl = android.opengl.GLES20.glGetString(android.opengl.GLES20.GL_RENDERER);
            if (opengl != null && opengl.contains("Bluestacks")){
                rating += 10;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
*/
        try {
            File sharedFolder = new File(Environment
                    .getExternalStorageDirectory().toString()
                    + File.separatorChar
                    + "windows"
                    + File.separatorChar
                    + "BstSharedFolder");

            if (sharedFolder.exists()) {
                rating += 10;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            BluetoothAdapter m_BluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            String m_bluetoothAdd = m_BluetoothAdapter.getAddress();
            if (m_bluetoothAdd == null){
                rating += 3;
            }
        } catch (Exception e) {
            Logger.W( TAG, "Can't get bluetooth adapter ( " + e.toString() + " ). Skipping security checks for bluetooth." );            
        }    

        return rating > 3;
    }

    private static Context getContext() {
        Context context = null;
        try {
            context = RhodesService.getContext();
        }
        catch (IllegalStateException e) {
            context = null;
            Logger.E(TAG, "Check Debuggable ERROR : Rhodes main Service (com.rhomobile.rhodes.RhodesService) is not run !!!");
        }

        return context;
    }

    @Override
    public void isRunOnSimulator(IMethodResult result) {
        result.set(checkEmulator());
    }

    private static boolean checkDebuggable(){

        Context context = getContext();

        boolean res = false;
        if (context != null) {
            res = (context.getApplicationInfo().flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0;
        }
        return res;

    }


    @Override
    public void isDebugMode(IMethodResult result)  {
        result.set(checkDebuggable());

    }

    //should always be 1 signature really...
    private Signature[] getSignatures() {
        try {
            Context context = getContext();
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            return packageInfo.signatures;
        } catch ( Exception e ) {
            e.printStackTrace();
        }

        return null;
    }

    private String signatureToString( Signature signature ) {
        try {
            byte[] signatureBytes = signature.toByteArray();
            MessageDigest md = MessageDigest.getInstance("SHA");
            md.update(signature.toByteArray());
            return Base64.encodeToString(md.digest(), Base64.DEFAULT).trim();
        } catch ( Exception e ) {
            e.printStackTrace();
        }

        return null;
    }

    @Override
    public void getAppCertificateSignatures(IMethodResult result) {        

        Collection<Object> signatures = new ArrayList<Object>();

        try {
            Signature[] sigs = getSignatures();
            
            for (Signature signature : sigs ) {
                signatures.add( signatureToString( signature ) );
            }

            result.set(signatures);

        } catch (Exception e) {
            result.setError( Utils.getExceptionDetails(e) );
        }
    }

    @Override
    public void getAllowedCertificateSignatures(IMethodResult result) {
        Collection<Object> signatures = new ArrayList<Object>();
        for ( String s : InbuiltValues.ALLOWED_CERT_SIGNATURES ) {
            signatures.add( s );
        }

        result.set( signatures );
    }

    private String getInsallerPackage() {
        Context context = getContext();        
        return context.getPackageManager().getInstallerPackageName(context.getPackageName());
    }

    @Override
    public void getInstallerPackageName(IMethodResult result) {
        Context context = getContext();

        try {
            final String installer = getInsallerPackage();
            result.set( installer );
        } catch ( Exception e ) {
            result.setError( Utils.getExceptionDetails(e) );
        }
    }

    @Override
    public void getAllowedInstallerPackages(IMethodResult result) {
        Collection<Object> packages = new ArrayList<Object>();
        for ( String p : InbuiltValues.ALLOWED_INSTALLERS ) {
            packages.add( p );
        }

        result.set( packages );
    }

    private boolean checkAllowedInstallers() {
        //allow any installer if none specified
        boolean res = (InbuiltValues.ALLOWED_INSTALLERS.length == 0);
        final String myInstaller = getInsallerPackage();

        for ( String installer: InbuiltValues.ALLOWED_INSTALLERS ) {
            if ( installer.equals( myInstaller ) ) {
                res = true;
                break;
            }
        }

        return res;
    }

    @Override
    public void haveAnyInstallerIssues(IMethodResult result) {
        result.set( !checkAllowedInstallers() );
    }

    private boolean checkAllowedSignatures() {                
        Signature[] mySignatures = getSignatures();

        //allow any signature if none specified
        final int requiredMatches = (InbuiltValues.ALLOWED_CERT_SIGNATURES.length==0)?0:mySignatures.length;
        int foundMatches = 0;

        for ( Signature mySignature : mySignatures ) {
            for ( String allowedSignature : InbuiltValues.ALLOWED_CERT_SIGNATURES ) {

                final String sMySignature = signatureToString( mySignature );

                //Logger.I( TAG, "Comparing signatures, system: " 
                //    + sMySignature + " ;len:" + String.valueOf(sMySignature.length() ) +
                //    "; inbuilt: " + allowedSignature + " ;len: " + String.valueOf(allowedSignature.length() ) );

                if ( sMySignature.equals(allowedSignature) ) {
                    //Logger.I( TAG, "Found matching signature: " + sMySignature );
                    ++foundMatches;
                    break;
                }
            }
        }

        //Logger.I( TAG, "checkAllowedSignatures, requiredMatches: " 
        //                + String.valueOf(requiredMatches) + " ; foundMatches: " + String.valueOf( foundMatches ) );

        return (foundMatches>=requiredMatches);
    }

    @Override
    public void haveAnySignatureIssues(IMethodResult result) {
        result.set( !checkAllowedSignatures() );
    }

}
