package com.rhomobile.rhodes.mapview;

import java.util.Map;
import java.util.Vector;

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
	
	private static final String SETTINGS_PREFIX = Rhodes.INTENT_EXTRA_PREFIX + "settings.";
	private static final String ANNOTATIONS_PREFIX = Rhodes.INTENT_EXTRA_PREFIX + "annotations.";
	
	private com.google.android.maps.MapView view;
	
	private String apiKey;
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	/*
	private static class Region {
		public double latitude;
		public double longitude;
		public double latSpan;
		public double lonSpan;
	};
	
	private static class Settings {
		public String map_type;
		public boolean zoom_enabled;
		public boolean scroll_enabled;
		public boolean shows_user_location;
		public Region region;
	};
	*/
	
	@SuppressWarnings("unchecked")
	public static void create(String gapiKey, Map<String, Object> params) {
		try {
			Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(),
					MapView.class);
			intent.putExtra(SETTINGS_PREFIX + "api_key", gapiKey);
			
			Object settings = params.get("settings");
			if (settings != null && (settings instanceof Map<?,?>)) {
				Map<String, Object> hash = (Map<String, Object>)settings;
				Object map_type = hash.get("map_type");
				if (map_type != null && (map_type instanceof String))
					intent.putExtra(SETTINGS_PREFIX + "map_type", (String)map_type);
				
				Object zoom_enabled = hash.get("zoom_enabled");
				if (zoom_enabled != null && (zoom_enabled instanceof String))
					intent.putExtra(SETTINGS_PREFIX + "zoom_enabled", ((String)zoom_enabled).equalsIgnoreCase("true"));
				
				Object scroll_enabled = hash.get("scroll_enabled");
				if (scroll_enabled != null && (scroll_enabled instanceof String))
					intent.putExtra(SETTINGS_PREFIX + "scroll_enabled", ((String)scroll_enabled).equalsIgnoreCase("true"));
				
				Object shows_user_location = hash.get("shows_user_location");
				if (shows_user_location != null && (shows_user_location instanceof String))
					intent.putExtra(SETTINGS_PREFIX + "shows_user_location", ((String)shows_user_location).equalsIgnoreCase("true"));
				
				Object region = hash.get("region");
				if (region != null && (region instanceof Vector<?>)) {
					Vector<String> reg = (Vector<String>)region;
					if (reg.size() == 4) {
						intent.putExtra(SETTINGS_PREFIX + "region.latitude", reg.elementAt(0));
						intent.putExtra(SETTINGS_PREFIX + "region.longitude", reg.elementAt(1));
						intent.putExtra(SETTINGS_PREFIX + "region.latSpan", reg.elementAt(2));
						intent.putExtra(SETTINGS_PREFIX + "region.lonSpan", reg.elementAt(3));
					}
				}
			}
			
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
		apiKey = extras.getString(SETTINGS_PREFIX + "api_key");
		
		String map_type = extras.getString(SETTINGS_PREFIX + "map_type");
		if (map_type == null)
			map_type = "roadmap";
		
		boolean zoom_enabled = extras.getBoolean(SETTINGS_PREFIX + "zoom_enabled");
		//boolean scroll_enabled = extras.getBoolean(SETTINGS_PREFIX + "scroll_enabled");
		//boolean shows_user_location = extras.getBoolean(SETTINGS_PREFIX + "shows_user_location");
		
		String latitude = extras.getString(SETTINGS_PREFIX + "region.latitude");
		String longitude = extras.getString(SETTINGS_PREFIX + "region.longitude");
		String latSpan = extras.getString(SETTINGS_PREFIX + "region.latSpan");
		String lonSpan = extras.getString(SETTINGS_PREFIX + "region.lonSpan");
		
		view = new com.google.android.maps.MapView(this, apiKey);
		view.setClickable(true);
		view.setBuiltInZoomControls(zoom_enabled);
		view.setSatellite(map_type.equals("hybrid") || map_type.equals("satellite"));
		view.setTraffic(false);
		view.setStreetView(false);
		layout.addView(view);
		
		MapController controller = view.getController();
		if (latitude != null && longitude != null) {
			try {
				double lat = Double.parseDouble(latitude);
				double lon = Double.parseDouble(longitude);
				controller.setCenter(new GeoPoint((int)(lat*1000000), (int)(lon*1000000)));
			}
			catch (NumberFormatException e) {
				Logger.E(TAG, "Wrong region center: " + e.getMessage());
			}
		}
		
		if (latSpan != null && lonSpan != null) {
			try {
				double lat = Double.parseDouble(latSpan);
				double lon = Double.parseDouble(lonSpan);
				controller.zoomToSpan((int)(lat*1000000), (int)(lon*1000000));
			}
			catch (NumberFormatException e) {
				Logger.E(TAG, "Wrong region span: " + e.getMessage());
			}
		}
		
		view.preLoad();
	}

	@Override
	protected boolean isRouteDisplayed() {
		return false;
	}
	
}
