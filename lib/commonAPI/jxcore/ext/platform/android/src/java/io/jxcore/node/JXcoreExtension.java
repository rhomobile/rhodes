// License information is available from LICENSE file

/*
 * This file is only a sample
 * You are free to update it with the methods you need
 * from the Native Mobile API
 */

package io.jxcore.node;

import io.jxcore.node.jxcore.JXcoreCallback;
import java.util.ArrayList;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Point;
import android.provider.Settings.SettingNotFoundException;
import android.view.Display;
import android.view.WindowManager;

public class JXcoreExtension {
  public static void LoadExtensions() {
    jxcore.RegisterMethod("ScreenInfo", new JXcoreCallback() {
      @SuppressLint("NewApi")
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        Context context = jxcore.activity.getBaseContext();
        WindowManager wm = (WindowManager) context
            .getSystemService(Context.WINDOW_SERVICE);
        Display display = wm.getDefaultDisplay();

        Point outSize = new Point();
        display.getSize(outSize);

        // we can deliver the size in 2 ways (array of arguments OR JSON
        // string) lets send it as arguments
        ArrayList<Object> args = new ArrayList<Object>();
        args.add(outSize.x);
        args.add(outSize.y);

        jxcore.CallJSMethod(callbackId, args.toArray());
      }
    });

    jxcore.RegisterMethod("ScreenBrightness", new JXcoreCallback() {
      @SuppressLint("NewApi")
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        Context context = jxcore.activity.getBaseContext();

        int br;
        try {
          br = android.provider.Settings.System.getInt(
              context.getContentResolver(),
              android.provider.Settings.System.SCREEN_BRIGHTNESS);
        } catch (SettingNotFoundException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
          br = 0;
        }

        ArrayList<Object> args = new ArrayList<Object>();
        args.add(br);

        jxcore.CallJSMethod(callbackId, args.toArray());
      }
    });

    jxcore.RegisterMethod("TestParams", new JXcoreCallback() {
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        ArrayList<Object> args = new ArrayList<Object>();
        args.add(100); // int
        args.add(-1); // int
        args.add(4.5); // double
        args.add(true); // boolean
        args.add("Hello World"); // string
        args.add("Test Buffer".getBytes()); // buffer
        args.add("Another String with UTF8 中國"); // utf8 string

        jxcore.CallJSMethod(callbackId, args.toArray());
      }
    });
  }
}