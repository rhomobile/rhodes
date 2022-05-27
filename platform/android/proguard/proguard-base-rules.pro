-injars  TempJars/preguard/
-outjars TempJars/postguard/classes-processed.jar

-printmapping result/classes-processed.map

# Preverification is irrelevant for the dex compiler and the Dalvik VM.
#-optimizationpasses 5
-dontusemixedcaseclassnames
-dontskipnonpubliclibraryclasses
-dontskipnonpubliclibraryclassmembers
-dontshrink
-dontoptimize
-dontpreverify
-useuniqueclassmembernames

-verbose
-dump class_files.txt
-printseeds seeds.txt
-printusage unused.txt
-printmapping mapping.txt
# Switch off some optimizations that trip older versions of the Dalvik VM.
-optimizations !code/simplification/arithmetic,!field/*,!class/merging/*

-allowaccessmodification

# RemoteViews might need annotations.
# Understand the @Keep support annotation.
-keepattributes *Annotation*
-keep class android.support.annotation.Keep
-keep @android.support.annotation.Keep class * {*;}
-keepclasseswithmembers class * {
    @android.support.annotation.Keep <methods>;
}

-keepclasseswithmembers class * {
    @android.support.annotation.Keep <fields>;
}

-keepclasseswithmembers class * {
    @android.support.annotation.Keep <init>(...);
}

# Keep a fixed source file attribute and all line number tables to get line
# numbers in the stack traces.
# You can comment this out if you're not interested in stack traces.

#-renamesourcefileattribute SourceFile
#-keepattributes SourceFile,LineNumberTable
# Reduce the size of the output some more.
-repackageclasses ''

-dontwarn android.support.**
-dontwarn org.apache.harmony.**
-dontwarn com.tonicsystems.**
-dontwarn org.firebirdsql.**
-dontwarn org.antlr.**
-dontwarn org.apache.**
-dontwarn android.**
-dontwarn java.**
-dontwarn javax.**
-dontwarn org.**
-dontwarn com.rhomobile.**
-dontwarn com.rho.**
-dontwarn com.google.**

-dontnote **


# Preserve all fundamental application classes.
-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider
-keep public class * extends android.app.backup.BackupAgentHelper
-keep public class * extends android.preference.Preference

-keep public class * implements android.media.MediaScannerConnection.OnScanCompletedListener{*;}
-keep public interface android.media.MediaScannerConnection.OnScanCompletedListener{*;}
-keepclasseswithmembers class * {
    public void onScanCompleted(java.lang.String,android.net.Uri);
}

-keep public class com.android.**
-keep public class java.lang.**
-keep public class com.android.vending.licensing.ILicensingService
-keep public class com.google.vending.licensing.ILicensingService
-dontnote com.android.vending.licensing.ILicensingService

-keep public class android.support.v7.preference.Preference {
  public <init>(android.content.Context, android.util.AttributeSet);
}
-keep public class * extends android.support.v7.preference.Preference {
  public <init>(android.content.Context, android.util.AttributeSet);
}

-keepclassmembers class * implements android.os.Parcelable {
    public <fields>;
}
-keepclassmembers class * implements android.widget.AbsListView.OnScrollListener{
    public <fields>;
}
-keepclassmembers class * implements android.widget.AbsListView{
    public <fields>;
}
# Preserve all View implementations, their special context constructors, and
# their setters.

-keep public class * extends android.view.View {
    public <init>(android.content.Context);
    public <init>(android.content.Context, android.util.AttributeSet);
    public <init>(android.content.Context, android.util.AttributeSet, int);
    public void set*(...);
}

# Explicitly preserve all serialization members. The Serializable interface
# is only a marker interface, so it wouldn't save them.
-keepclassmembers class * implements java.io.Serializable {
static final long serialVersionUID;
private static final java.io.ObjectStreamField[] serialPersistentFields;
private void writeObject(java.io.ObjectOutputStream);
private void readObject(java.io.ObjectInputStream);
java.lang.Object writeReplace();
java.lang.Object readResolve();
}

# Preserve all native method names and the names of their classes.
-keepclasseswithmembernames class * {
    native <methods>;
}
-keepclassmembers public class * extends android.view.View {
   void set*(***);
   *** get*();
}

# Preserve all classes that have special context constructors, and the
# constructors themselves.
-keepclasseswithmembernames class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}

-keepclasseswithmembernames class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}
 
-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}

-keepclassmembers class **.R$* {
public static <fields>;
}
# Preserve all classes that have special context constructors, and the
# constructors themselves.

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

# Preserve the special static methods that are required in all enumeration classes.
-keepclassmembers enum * {
public static **[] values();
public static ** valueOf(java.lang.String);
}
# Preserve all possible onClick handlers.

-keepclassmembers class * extends android.content.Context {
   public void *(android.view.View);
   public void *(android.view.MenuItem);
}

-keep public class * {
public protected *;
}

-keep class * implements android.os.Parcelable {
public static final android.os.Parcelable$Creator *;
}

-keepclassmembers class **.R$* {
    public static <fields>;
}

# Preserve the special fields of all Parcelable implementations.

-keepclassmembers class * implements android.os.Parcelable {
    static android.os.Parcelable$Creator CREATOR;
}

# Preserve static fields of inner classes of R classes that might be accessed
# through introspection.


# Preserve annotated Javascript interface methods.

-keepclassmembers class * {
    @android.webkit.JavascriptInterface <methods>;
}

# Preserve the required interface from the License Verification Library
# (but don't nag the developer if the library is not used at all).

-keep public interface com.android.vending.licensing.ILicensingService

-dontnote com.android.vending.licensing.ILicensingService

# The Android Compatibility library references some classes that may not be
# present in all versions of the API, but we know that's ok.

-dontwarn android.support.**

# Preserve all native method names and the names of their classes.

-keepclasseswithmembernames,includedescriptorclasses class * {
    native <methods>;
}

# Preserve the special static methods that are required in all enumeration
# classes.

-keepclassmembers,allowoptimization enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

# Explicitly preserve all serialization members. The Serializable interface
# is only a marker interface, so it wouldn't save them.
# You can comment this out if your application doesn't use serialization.
# If your code contains serializable classes that have to be backward 
# compatible, please refer to the manual.

-keepclassmembers class * implements java.io.Serializable {
    static final long serialVersionUID;
    static final java.io.ObjectStreamField[] serialPersistentFields;
    private void writeObject(java.io.ObjectOutputStream);
    private void readObject(java.io.ObjectInputStream);
    java.lang.Object writeReplace();
    java.lang.Object readResolve();
}

# Your application may contain more items that need to be preserved; 
# typically classes that are dynamically created using Class.forName:

# -keep public class mypackage.MyClass
# -keep public interface mypackage.MyInterface
# -keep public class * implements mypackage.MyInterface

# If you wish, you can let the optimization step remove Android logging calls.

#-assumenosideeffects class android.util.Log {
#    public static boolean isLoggable(java.lang.String, int);
#    public static int v(...);
#    public static int i(...);
#    public static int w(...);
#    public static int d(...);
#    public static int e(...);
#}

-keep class com.rhomobile.rhodes.file.RhoFileApi {
    *;
}
-keep class com.rhomobile.rhodes.api.* {
    *;
}
-keep class com.rhomobile.rhodes.ui.* {
    *;
}

-keep class java.* {*;}
-keep class * implements java.*
-keep public interface java.* {*;}

-dontwarn java.**
-keep class java.**

-keep class javax.* {*;}
-dontwarn javax.**
-keep class javax.**
-keep class * implements javax.*
-keep public interface javax.* {*;}

-dontwarn android.**
-keep class android.**

-dontwarn com.sun.**
-keep class com.sun.**

-dontwarn java.awt.**
-dontwarn java.beans.Beans
-dontwarn javax.security.**
-keep class javamail.** {*;}
-keep class javax.mail.** {*;}
-keep class javax.activation.** {*;}
-keep class org.apache.** {*;}


-keep public class com.app.main {
    public static void main(java.lang.String[]);
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

-keepclassmembers class * extends android.content.Context {
   public void *(android.view.View);
   public void *(android.view.MenuItem);
}

-keepclassmembers class * implements android.os.Parcelable {
    static ** CREATOR;
}

-keep class * implements javax.net.ssl.HostnameVerifier {
    public *;
}

-keep class com.rhomobile.rhodes.NetRequest$RhoHostVerifier {
    *;
}


-keepclassmembers class * implements javax.net.ssl.X509TrustManager {
    public *;
}

-keep class com.rhomobile.rhodes.socket.SSLImpl$* {
    public *;
}

-keepclassmembers class **.R$* {
    public static <fields>;
}

-keepclassmembers class * {
    @android.webkit.JavascriptInterface <methods>;
}

-keep class java.lang.Runnable
{
    public void run();
}

-keepclasseswithmembernames class * {
    public void run();
}

#-keep public class * extends *
#-keep interface * { *; }
-keepattributes Signature
-adaptresourcefilecontents **.MF

-microedition
-dontskipnonpubliclibraryclassmembers
-keepdirectories
-forceprocessing
-dontoptimize
-allowaccessmodification
-overloadaggressively
-keeppackagenames org.**
-keeppackagenames java.**

-keep class org.sqlite.** {
    <fields>;
    <methods>;
}

-keep class com.sun.** {
    <fields>;
    <methods>;
}

-keep class com.google.** {
    <fields>;
    <methods>;
}

-keep class oshi.** {
    <fields>;
    <methods>;
}

-keep class com.rhomobile.rhodes.socket.RhoSockAddr {
    <fields>;
}

-keep class com.rhomobile.rhodes.socket.RhoSocketImpl {
    <fields>;
}

-keep class com.rhomobile.rhoelements.ans.ANSFacade {
    <fields>;
    <methods>;
}

-keep class com.rhoelementsext.RhoElementsExt {
    <fields>;
    <methods>;
}

-keep class com.rho.network.Network {
    <fields>;
    <methods>;
}

-keep class com.audiocapture.Audiocapture {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.rhodes.RhodesService {
    <fields>;
    <methods>;
}
-keep class com.rho.development.Development {
    <fields>;
    <methods>;
}

-keep class com.rhomobile.rhodes.gcm.GCMFacade {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.nfc.NfcActivity {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.rhodes.gmaps.GMapActivity {
    <fields>;
    <methods>;
}
-keep class com.rawsensors.Rawsensors {
    <fields>;
    <methods>;
}
-keep class com.videocapture.Videocapture {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.rhodes.mapview.Callout {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.rhodes.mapview.MapView {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.rhodes.api.MethodExecutor {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.rhodes.nativeview.RhoNativeViewManager {
    <fields>;
    <methods>;
}
-keep class com.barcode.Barcode {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.rhodes.nativeview.RhoNativeViewManager {
    <fields>;
    <methods>;
}
-keep class com.rhomobile.rhodes.nativeview.RhoNativeViewManager {
    <fields>;
    <methods>;
}
-keep class com.rho.audiocapture.AudioCaptureFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.barcode.BarcodeFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.clipboard.ClipboardFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.connectionchecking.ConnectionCheckingFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.notification.led.LedFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.nativetabbar.NativeTabbarFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.nativetoolbar.NativeToolbarFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.devicesecurity.DeviceSecurityFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.eventsource.EventsourceFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.keycapture.KeyCaptureFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.battery.BatteryFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.signalindicators.SignalIndicatorsFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.javascriptvm.JavascriptVMFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.videocapture.VideocaptureFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.mediaplayer.MediaplayerFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.printer.PrinterFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.printingepson.PrintingEpsonFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.printerzebra.PrinterZebraFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.screenorientation.ScreenOrientationFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.sensor.SensorFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.signature.SignatureFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.smartcradle.SmartCradleFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.device.DeviceFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.devicememory.DeviceMemoryFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.websocket.WebSocketFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.development.DevelopmentFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.rhoelements.barcode1.Barcode1Factory {
    <fields>;
    <methods>;
}
-keep class com.rho.megamodule.MegamoduleFactory {
    <fields>;
    <methods>;
}
-keep class com.rho.examples.simpleonlystaticmodule.SimpleonlystaticmoduleFactory {
    <fields>;
    <methods>;
}

-keepclasseswithmembernames class * {
    native <methods>;
}

-keep class com.firebase.** { *; }
-keep class org.apache.** { *; }
-keepnames class com.fasterxml.jackson.** { *; }
-keepnames class javax.servlet.** { *; }
-keepnames class org.ietf.jgss.** { *; }
-dontwarn org.w3c.dom.**
-dontwarn org.joda.time.**
-dontwarn org.shaded.apache.**
-dontwarn org.ietf.jgss.**
-keep class com.shaded.fasterxml.jackson.** { *; }
-keep interface org.conscrypt.** { *; }
-keep class org.conscrypt.** { *; }


-keep interface android.support.v4.** { *; }
-keep interface android.support.v7.** { *; }
-keep class android.support.** { *; }
-keep class com.google.firebase.** { *; }
-keep class com.rhomobile.rhodes.NetRequest.* {*; }
-keep class javax.net.ssl.* {*; }

-dontwarn android.support.design.internal.**
-dontwarn com.google.android.gms.**
-dontwarn android.support.v4.**

-keep public class androidx.**
-keep public class kotlin.**
-dontwarn androidx.**
-dontwarn kotlin.**
