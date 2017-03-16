// License information is available from LICENSE file

package io.jxcore.node;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import org.json.JSONArray;
import org.json.JSONException;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Looper;
import android.util.Log;

import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.RhodesActivity;
import android.content.Intent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.file.RhoFileApi;


public class jxcore extends AbstractRhoListener implements IRhoListener {

  // Methods below are inherited from jxcore-cordova JNI
  // Make sure reach these methods under the same thread
  public native void setNativeContext(final Context context, final AssetManager assetManager);
  public native int loopOnce();
  public native void startEngine();
  public native void prepareEngine(String home, String fileTree);
  public native void stopEngine();
  public native void defineMainFile(String content);
  public native long evalEngine(String script);
  public native int getType(long id);
  public native String getString(long id);
  public native String convertToString(long id);
  public native long callCBString(String event_name, String param, int is_json);
  public native long callCBArray(String event_name, Object[] arr, int size);

  public static String TAG = "jxcore";
  public static Activity activity = null;
  public static jxcore addon;

  //Map<String, CallbackContext> callbacks;
  static Map<String, JXcoreCallback> java_callbacks;
  public static CoreThread coreThread = null;
  public static boolean app_paused = false;
  public static boolean jxcoreInitialized = false;

  public static void CreateResult(Object value, String callback_id,
      boolean async, boolean is_error) {
    //PluginResult result;

    if (value == null) {
      //result = new PluginResult(is_error ? Status.ERROR : Status.OK, 0);
    } else if (is_error) {
      //result = new PluginResult(Status.ERROR, (String) value);
    } else if (value.getClass().equals(Integer.class)) {
      //result = new PluginResult(Status.OK, (Integer) value);
    } else if (value.getClass().equals(Boolean.class)) {
      //result = new PluginResult(Status.OK, (Boolean) value);
    } else if (value.getClass().equals(Double.class)) {
      //result = new PluginResult(Status.OK, (Float) value);
    } else if (value.getClass().equals(String.class)) {
      //result = new PluginResult(Status.OK, (String) value);
    } else if (value.getClass().equals(byte[].class)) {
      //result = new PluginResult(Status.OK, ((byte[]) value));
    } else if (value.getClass().equals(String[].class)) {
      /*
      String[] arr = (String[]) value;
      try {
        //result = new PluginResult(Status.OK, new JSONArray(arr[0]));
      } catch (JSONException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
        return;
      }
      */
    } else {
      //result = new PluginResult(Status.OK, value.toString());
    }

/*
    if (async) {
      result.setKeepCallback(true);      
      if (addon.callbacks.containsKey(callback_id)) {
        activity.runOnUiThread(new CoreRunnable(callback_id, result) {
          @Override
          public void run() {
            CallbackContext ctx = addon.callbacks.get(callback_id_);
            ctx.sendPluginResult(result_);
          }
        });
      }
    } else {
      CallbackContext ctx = addon.callbacks.remove(callback_id);
      ctx.sendPluginResult(result);
    }
*/

  }

  public interface JXcoreCallback {
    public void Receiver(ArrayList<Object> params, String callbackId);
  }

  public static void jx_callback(Object value, Object error, String callbackId) {
    CreateResult(error == null ? value : error, callbackId, true, error != null);
  }

  public static void javaCall(ArrayList<Object> params) {
    if (params.size() < 2 || params.get(0).getClass() != String.class
        || params.get(params.size() - 1).getClass() != String.class) {
      Log.e(TAG, "JavaCall recevied an unknown call");
      return;
    }

    String receiver = params.remove(0).toString();
    String callId = params.remove(params.size() - 1).toString();

    if (!java_callbacks.containsKey(receiver)) {
      Log.e(TAG, "JavaCall recevied a call for unknown method " + receiver);
      return;
    }

    java_callbacks.get(receiver).Receiver(params, callId);
  }

  public static void RegisterMethod(String name, JXcoreCallback callback) {
    java_callbacks.put(name, callback);
  }

  private static void callJSMethod(String id, Object[] args) {
    long ret = addon.callCBArray(id, args, args.length);
    int tp = addon.getType(ret);

    // STRING 4, OBJECT 5, ERROR 9 - See jx_types
    if (tp == 4 || tp == 5 || tp == 9) {
      Log.e(TAG, "jxcore.CallJSMethod :" + addon.getString(ret));
    }
  }

  private static void callJSMethod(String id, String args) {
    long ret = addon.callCBString(id, args, 1);
    int tp = addon.getType(ret);

    // STRING 4, OBJECT 5, ERROR 9 - See jx_types
    if (tp == 4 || tp == 5 || tp == 9) {
      Log.e(TAG, "jxcore.CallJSMethod :" + addon.getString(ret));
    }
  }

  public static boolean CallJSMethod(String id, Object[] args) {
    if (jxcore.coreThread == null) {
      Log.e(TAG, "JXcore wasn't initialized yet");
      return false;
    }

    if (Looper.myLooper() != coreThread.handler.getLooper()) {
      coreThread.handler.post(new CoreRunnable(id, args) {
        @Override
        public void run() {
          callJSMethod(callback_id_, params_);
        }
      });
    } else {
      callJSMethod(id, args);
    }

    return true;
  }

  public static boolean CallJSMethod(String id, String json) {
    if (jxcore.coreThread == null) {
      Log.e(TAG, "JXcore wasn't initialized yet");
      return false;
    }

    if (Looper.myLooper() != coreThread.handler.getLooper()) {
      coreThread.handler.post(new CoreRunnable(id, json) {
        @Override
        public void run() {
          callJSMethod(callback_id_, str_param_);
        }
      });
    } else {
      callJSMethod(id, json);
    }

    return true;
  }

/*
  @Override
  public boolean execute(final String action, final JSONArray data,
      final CallbackContext callbackContext) {

    PluginResult result = null;
    try {
      if (action.equals("isReady")) {
        result = new PluginResult(Status.OK, jxcoreInitialized);
      } else if (action.equals("Evaluate")) {
        final String json = data.get(0).toString() + ", '"
            + callbackContext.getCallbackId() + "')";
        callbacks.put(callbackContext.getCallbackId(), callbackContext);

        result = new PluginResult(Status.NO_RESULT);
        result.setKeepCallback(true);

        coreThread.handler.post(new CoreRunnable(callbackContext
            .getCallbackId(), json) {
          @Override
          public void run() {
            long res = evalEngine(str_param_);
            if (res >= 0) {
              String str_err = getString(res);

              CreateResult(str_err, callback_id_, true, true);
            }
          }
        });

      } else {
        result = new PluginResult(Status.OK);
      }
    } catch (Exception ex) {
      result = new PluginResult(Status.ERROR, ex.toString());
    }

    if (result != null)
      callbackContext.sendPluginResult(result);

    return true;
  }
*/


  @Override
  public void onPause(RhodesActivity activity) {
    super.onPause(activity);
    jxcore.CallJSMethod("JXcore_Device_OnPause", "{}");
    app_paused = true;
  }

  @Override
  public void onResume(RhodesActivity activity) {
    super.onResume(activity);
    jxcore.CallJSMethod("JXcore_Device_OnResume", "{}");
    app_paused = false;
  }

  private void buildJsonList( String home, String root, StringBuilder builder ) {
    String[] children = RhoFileApi.getChildren( root );

    for (String file:children) {
        String fullname = root + "/" + file;
        String name = fullname;//.substring(home.length() + 1);

        if ( RhoFileApi.getChildren( fullname ).length > 0 ) {
          buildJsonList( home, fullname, builder);
        } else {
          if ( builder.toString().length() > 1 ) {
            builder.append(",");
          }
          builder.append("\"" + name + "\":0");
        }
    }
  }

  // Make sure calling this method under CoreThread!!
  public void Initialize(String root) {
    Logger.I(TAG, ">>>>>>>>>>> jxcore.Initialize: " + root);

    String jxroot = root + "/jxcore";
    StringBuilder builder = new StringBuilder();
    builder.append("{");
    buildJsonList(jxroot, jxroot, builder);
    builder.append("}");

      String fullroot = RhoFileApi.getRootPath() + jxroot;

    prepareEngine( fullroot, builder.toString());

    String mainFile = FileManager.readFile(RhoFileApi.getRootPath() + root + "/main.js");
    Logger.I( TAG, ">>>>>>>>>>> jxcore.mainFile: " + mainFile );


    String data = "process.setPaths = function(){ process.cwd = function() { return '"
        + RhoFileApi.getRootPath()
        + "';};\n"
        + "process.userPath ='"
        + RhoFileApi.getRootPath()
        + "';\n"
        + "};" + mainFile;

      Logger.I( TAG, ">>>>>>>>>>> jxcore.data: " + data );


    defineMainFile(data);

    startEngine();

    jxcoreInitialized = true;
  }


  @Override
  public void onCreateApplication(IRhoExtManager extManager) {
    Logger.I( TAG, ">>>>>>>>>>>>> jxcore.onCreateApplication" );
    extManager.addRhoListener(this);
  }


  /* 
    Rhodes entry point for initialization of JXCore.
  */
  @Override
  public void onCreate(RhodesActivity a, Intent intent) {

    Logger.I( TAG, ">>>>>>>>>>>>> jxcore.onCreate" );

    final boolean new_instance = activity == null;
    activity = a;
    if (!new_instance) {
      Log.d(TAG, "jxcore rhomobile android initializing");
    }
    addon = this;

    //callbacks = new HashMap<String, CallbackContext>();
    java_callbacks = new HashMap<String, JXcoreCallback>();

    RegisterMethod("  _callback_  ", new JXcoreCallback() {
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        if (params.size() < 3 || !params.get(2).getClass().equals(String.class)) {
          Log.e(TAG, "Unkown _callback_ received");
          return;
        }
        jxcore.jx_callback(params.get(0), params.get(1), params.get(2)
            .toString());
      } 
    });

    io.jxcore.node.JXcoreExtension.LoadExtensions();
    io.jxcore.node.JXMobile.Initialize();

    if (new_instance) {
      Runnable runnable = new Runnable() {
        @Override
        public void run() {

    Logger.I(TAG, ">>>>>>>>>>>>> setNativeContext");


          setNativeContext(activity.getBaseContext(), activity.getAssets());
          

    Logger.I(TAG, ">>>>>>>>>>>>> Initialize");


          addon.Initialize("apps/app");

    Logger.I(TAG, ">>>>>>>>>>>>> StartApplication");


          addon.callCBArray("StartApplication", new Object[]{"apps/app/jxcore/app.js"}, 1);


          Runnable runnable2 = new Runnable() {
            @Override
            public void run() {
              int active = addon.loopOnce();
              final int wait_long = app_paused ? 10 : 3;
              if (active == 0)
                coreThread.handler.postDelayed(this, wait_long);
              else
                coreThread.handler.postDelayed(this, 1);
            }
          };

          coreThread.handler.postDelayed(runnable2, 1);
        }
      };

      if (coreThread != null) {
        coreThread.handler.getLooper().quit();
      }

      coreThread = new CoreThread(runnable);
      coreThread.start();

    } else {
      Runnable runnable = new Runnable() {
        @Override
        public void run() {
          setNativeContext(activity.getBaseContext(), activity.getAssets());
        }
      };
      coreThread.handler.post(runnable);
    }
  }
}