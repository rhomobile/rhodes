package com.rhomobile.rhodes.mapview;

import android.content.Intent;
import android.os.Bundle;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.MapActivity;
import com.google.android.maps.MapController;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

public class MapView extends MapActivity {

	private static final String TAG = "MapView";
	
	private com.google.android.maps.MapView view;
	
	private String apiKey;
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	public static void create(String gapiKey) {
		try {
			Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(),
					MapView.class);
			intent.putExtra("api_key", gapiKey);
			
			RhodesInstance.getInstance().startActivityForResult(intent, 5);
		}
		catch (Exception e) {
			reportFail("create", e);
		}
	}
	
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		
		getWindow().setFlags(Rhodes.WINDOW_FLAGS, Rhodes.WINDOW_MASK);
		
		RelativeLayout layout = new RelativeLayout(this);
		setContentView(layout, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		
		Bundle extras = getIntent().getExtras();
		apiKey = extras.getString("api_key");
		
		view = new com.google.android.maps.MapView(this, apiKey);
		view.setClickable(true);
		view.setBuiltInZoomControls(true);
		layout.addView(view);
		
		MapController controller = view.getController();
		controller.setZoom(0);
		controller.setCenter(new GeoPoint(0, 0));
	}

	@Override
	protected boolean isRouteDisplayed() {
		return false;
	}
	
}
