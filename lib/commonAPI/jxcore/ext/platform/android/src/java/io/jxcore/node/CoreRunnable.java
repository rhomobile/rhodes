// License information is available from LICENSE file

package io.jxcore.node;

//import org.apache.cordova.PluginResult;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

public class CoreRunnable implements Runnable {
  @Override
  public void run() {
    // TODO Auto-generated method stub
  }

  public String callback_id_;
  //public PluginResult result_;
  public Object[] params_;
  public String str_param_;

  public CoreRunnable(String callback_id /*, PluginResult result*/) {
    callback_id_ = callback_id;
    //result_ = result;
  }

  public CoreRunnable(String callback_id, String str_param) {
    callback_id_ = callback_id;
    str_param_ = str_param;
  }

  public CoreRunnable(String callback_id, Object[] params) {
    callback_id_ = callback_id;
    params_ = params;
  }
}

class CoreThread extends Thread {
  public Handler handler;
  public Runnable io_runnable;
  
  public CoreThread(Runnable io) {
    io_runnable = io;
    handler = null;
  }

  public void run() {
    Looper.prepare();

    handler = new Handler() {
      public void handleMessage(Message msg) {
      }
    };

    handler.post(io_runnable);
    Looper.loop();
  }
}