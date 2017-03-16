// License information is available from LICENSE file

package io.jxcore.node;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import android.annotation.SuppressLint;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.FileReader;

import com.rhomobile.rhodes.file.RhoFileApi;

@SuppressLint("DefaultLocale")
public class FileManager {

  public static String readFile(String location) {
    return readFile(location, "UTF-8");
  }

  public static String readFile(String location, String encoding) {
    StringBuilder sb = new StringBuilder();
    try {
      BufferedReader br = new BufferedReader( new InputStreamReader( RhoFileApi.open(location) ) );

      String str = br.readLine();
      while (str != null) {
        sb.append(str + "\n");
        str = br.readLine();
      }

      br.close();
    } catch (IOException e) {
      Log.w("jxcore-FileManager", "readfile failed: " + location);
      e.printStackTrace();
      return null;
    }

    return sb.toString();
  }
}
