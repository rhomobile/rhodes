package io.jxcore.node;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import io.jxcore.node.JXMobile;

public class ConnectivityChangeListener extends BroadcastReceiver {

  private static String previousStatus = "";

  @Override
  public void onReceive(final Context context, final Intent intent) {
    ConnectivityManager cm = (ConnectivityManager) context
      .getSystemService(Context.CONNECTIVITY_SERVICE);

    String status = JXMobile.getStatusString(cm.getAllNetworkInfo(), false);
    // the onReceive method can be called multiple times,
    // so let's skip raising the event for same consecutive values values of
    // status
    if (previousStatus != status) {
      jxcore.CallJSMethod("JXcore_Device_OnConnectionStatusChanged",
        "{\"" + status + "\":1}");
      previousStatus = status;
    }
  }
}