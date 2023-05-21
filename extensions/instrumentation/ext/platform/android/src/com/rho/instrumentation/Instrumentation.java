package com.rho.instrumentation;

import java.io.File;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Map;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.graphics.Rect;
import android.os.Debug;
import android.os.Environment;
import android.os.SystemClock;
import android.view.MotionEvent;
import android.view.View;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class Instrumentation extends InstrumentationBase implements IInstrumentation, IRhoExtension {
	private static final String TAG = "Rho::Instrumentation";

    public Instrumentation(String id) {
        super(id);
        RhoExtManager.getInstance().registerExtension("instrumentation", this);
    }

	@Override
	public void simulate_key_event_code(int keycode, IMethodResult result) {
		Logger.D(TAG, "simulateKeyEvent -- START");
		Logger.D(TAG, "simulateKeyEvent -- keyCode: " + keycode);
		final int code = keycode;
		new Thread(new Runnable() {
			public void run() {
				android.app.Instrumentation ins = new android.app.Instrumentation();
				ins.sendKeyDownUpSync(code);
			}
		}).start();
	}

	@Override
	public void simulate_key_event_string(String str, IMethodResult result) {
		Logger.D(TAG, "simulateKeyEvent -- START");
		Logger.D(TAG, "simulateKeyEvent -- stringToType: " + str);
		final String androidString = str;
		new Thread(new Runnable() {
			public void run() {
				android.app.Instrumentation ins = new android.app.Instrumentation();
				ins.sendStringSync(androidString);
			}
		}).start();
	}

	@Override
	public void simulate_touch_event(final int event_type, final int x, final int y, IMethodResult result)
	{
		new Thread(new Runnable()
		{
			public void run()
			{
				MotionEvent motionEvent = MotionEvent.obtain(
						SystemClock.uptimeMillis(), SystemClock.uptimeMillis(),
						event_type, x, y, 0);

				android.app.Instrumentation ins = new android.app.Instrumentation();
				ins.sendPointerSync(motionEvent);
				motionEvent.recycle();
			}
		}).start();
	}

	@Override
	public void screen_capture(String pszFilename, IMethodResult result) {
		Bitmap bitmap = takeScreenshot();

		// If saveBitmap returns false, then set the result to -1
		if (saveBitmap(bitmap, pszFilename)) {
			result.set(1);
		} else {
			result.set(-1);
		}
	}

	private boolean saveBitmap(Bitmap bitmap, String pszFilename) {
		File sdCard = Environment.getExternalStorageDirectory();
		File directory = new File(sdCard.getAbsolutePath() + "/ScreenCapture");
		directory.mkdirs();

		File imagePath = new File(directory, pszFilename);
		FileOutputStream fos;

		try {
			fos = new FileOutputStream(imagePath);
			bitmap.compress(CompressFormat.PNG, 100, fos);
			fos.flush();

			fos.close();
			Logger.D(TAG, "saveBitmap -- File " + imagePath.toString() + " successful");

			return true;
		} catch (FileNotFoundException e) {
			Logger.D(TAG, "saveBitmap -- FileNotFoundException " + e.getMessage());
			return false;
		} catch (IOException e) {
			Logger.D(TAG, "saveBitmap -- IOException " + e.getMessage());
			return false;
		}
	}

	private Bitmap takeScreenshot() {
		//View rootView = findViewById(android.R.id.content).getRootView();
		View rootView = RhoExtManager.getImplementationInstance().getWebView().getView();
		rootView.setDrawingCacheEnabled(true);
		return rootView.getDrawingCache();
	}

	@Override
	public void get_allocated_memory(IMethodResult result) {
		Debug.MemoryInfo memoryInfo = new Debug.MemoryInfo();
		Debug.getMemoryInfo(memoryInfo);

		result.set(memoryInfo.getTotalPss());
	}

	@Override
	public void delete_file(String fileName, IMethodResult result) {
		int res = 0;
		final String basePath = Environment.getExternalStorageDirectory().getAbsolutePath();

		if ( (fileName == null) || (fileName.equals("")) ) {
			result.set(-1);
		    return;
		}

		fileName = fileName.replaceAll("\\\\", "/");
		try
		{
		    File file = new File(basePath, fileName);
		    if (file.delete())
		    	res = 1;
		    else
		    	res = 0;
		}
		catch (NullPointerException e)
		{
		    res = -1;
		}

		result.set(res);

	}

	@Override
	public void file_exists(String fileName, IMethodResult result) {
		int res = 0;
		final String basePath = Environment.getExternalStorageDirectory().getAbsolutePath();

		if ( (fileName == null) || (fileName.equals("")) ) {
			result.set(-1);
		    return;
		}

		fileName = fileName.replaceAll("\\\\", "/");
		try
		{
		    File file = new File(basePath, fileName);
		    if (file.exists())
		    	res = 1;
		    else
		    	res = 0;
		}
		catch (NullPointerException e)
		{
		    res = -1;
		}

		result.set(res);

	}

	@Override
	public void re_simulate_navigation(IMethodResult result) {
		//Get the instance of rhoelementsext
	    IRhoExtension rhoElementsExt = RhoExtManager.getInstance().getExtByName("rhoelements");
	    if (rhoElementsExt == null)
	    {
		Logger.D(TAG, "rhoelementsext is not registered and therefore can't be used");
		return;
	    }
	    Logger.D(TAG, "rhoelementsext is available");

		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				try {
					Class commonClass = Class.forName(
							"com.rho.rhoelements.Common", false,
							RhodesActivity.safeGetInstance().getClassLoader());
					Field pluginManagerField = commonClass
							.getField("pluginManager");
					Object pluginManagerObj = pluginManagerField.get(null);
					// Get the references to the pageStarted() and
					// pageFinished() methods from PluginManager
					Method pageEventMethod = pluginManagerObj.getClass()
							.getMethod("pageStarted",
									new Class[] { java.lang.String.class });
					pageEventMethod.invoke(pluginManagerObj,
							new Object[] { "" });
					pageEventMethod = pluginManagerObj.getClass().getMethod(
							"pageFinished",
							new Class[] { java.lang.String.class });
					pageEventMethod.invoke(pluginManagerObj,
							new Object[] { "" });
				} catch (java.lang.Exception e) {
					e.printStackTrace();
					Logger.D(TAG, e.getMessage());
				}
			}
		});

	}

	@Override
	public IRhoWebView onCreateWebView(IRhoExtManager extManager, int tabIndex) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean onWebViewCreated(IRhoExtManager extManager, IRhoWebView ext,
			boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onSetPropertiesData(IRhoExtManager extManager,
			String propId, String data, int pos, int total, IRhoWebView ext,
			boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onSetPropertiesDataEnd(IRhoExtManager extManager,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onSetProperty(IRhoExtManager extManager, String name,
			String value, IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onBeforeNavigate(IRhoExtManager extManager, String url,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onNavigateStarted(IRhoExtManager extManager, String url,
		IRhoWebView ext, boolean res) {
	    // TODO Auto-generated method stub
	    return false;
	}

	@Override
	public boolean onNavigateProgress(IRhoExtManager extManager, String url,
			int pos, int total, IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onNavigateComplete(IRhoExtManager extManager, String url,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onDocumentComplete(IRhoExtManager extManager, String url,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
    public boolean onGoBack(IRhoExtManager extManager, String current_url, String back_url, IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean onGoForward(IRhoExtManager extManager, String current_url, String forward_url, IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
        return false;
    }



	@Override
	public boolean onSelect(IRhoExtManager extManager, String[] items,
			int selected, IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onStatus(IRhoExtManager extManager, String status,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onTitle(IRhoExtManager extManager, String title,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onConsole(IRhoExtManager extManager, String message,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onInputMethod(IRhoExtManager extManager, boolean enabled,
			String type, Rect area, IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onNavigateError(IRhoExtManager extManager, String url,
			LoadErrorReason reason, IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onAuthRequest(IRhoExtManager extManager, IRhoExtension.IAuthRequest request, IRhoWebView ext, boolean res) {
		return false;
	}

	@Override
	public void onAppActivate(IRhoExtManager extManager, boolean bActivate) {
		// TODO Auto-generated method stub

	}

	@Override
	public boolean startLocationUpdates(IRhoExtManager extManager,
			boolean highAccuracy, IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean stopLocationUpdates(IRhoExtManager extManager,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onNewConfig(IRhoExtManager extManager, IRhoConfig config,
			String name, boolean res) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public String onGetProperty(IRhoExtManager extManager, String name) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean onAlert(IRhoExtManager extManager, String message,
		IRhoWebView ext, IAlertResult alertResult, boolean res) {
	    // TODO Auto-generated method stub
	    return false;
	}

	@Override
	public boolean onConfirm(IRhoExtManager extManager, String message,
		IRhoWebView ext, IAlertResult confirmResult, boolean res) {
	    // TODO Auto-generated method stub
	    return false;
	}

	@Override
	public boolean onPrompt(IRhoExtManager extManager, String message,
		String defaultResponse, IRhoWebView ext,
		IPromptResult promptResult, boolean res) {
	    // TODO Auto-generated method stub
	    return false;
	}

	@Override
	public boolean onLoadResource(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
		return false;
	}
}
