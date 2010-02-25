package com.rhomobile.rhodes.mapview;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.Map;
import java.util.Vector;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.MapActivity;
import com.google.android.maps.MapController;
import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

public class MapView extends MapActivity {

	private static final String TAG = "MapView";
	
	private static final String SETTINGS_PREFIX = Rhodes.INTENT_EXTRA_PREFIX + "settings.";
	private static final String ANNOTATIONS_PREFIX = Rhodes.INTENT_EXTRA_PREFIX + "annotations.";
	
	private com.google.android.maps.MapView view;
	private AnnotationsOverlay annOverlay;
	
	private String apiKey;
	
	private Vector<Annotation> annotations;
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	@SuppressWarnings("unchecked")
	public static void create(String gapiKey, Map<String, Object> params) {
		try {
			Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(),
					MapView.class);
			intent.putExtra(SETTINGS_PREFIX + "api_key", gapiKey);
			
			Object settings = params.get("settings");
			if (settings != null && (settings instanceof Map<?,?>)) {
				Map<Object, Object> hash = (Map<Object, Object>)settings;
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
			
			Object annotations = params.get("annotations");
			if (annotations != null && (annotations instanceof Vector<?>)) {
				Vector<Object> arr = (Vector<Object>)annotations;
				
				intent.putExtra(ANNOTATIONS_PREFIX + "size", arr.size());
				
				for (int i = 0, lim = arr.size(); i < lim; ++i) {
					Object annObj = arr.elementAt(i);
					if (annObj == null || !(annObj instanceof Map<?, ?>))
						continue;
					
					Map<Object, Object> ann = (Map<Object, Object>)annObj;
					
					String prefix = ANNOTATIONS_PREFIX + Integer.toString(i) + ".";
					
					Object latitude = ann.get("latitude");
					if (latitude != null && (latitude instanceof String))
						intent.putExtra(prefix + "latitude", (String)latitude);
					
					Object longitude = ann.get("longitude");
					if (longitude != null && (longitude instanceof String))
						intent.putExtra(prefix + "longitude", (String)longitude);
					
					Object address = ann.get("street_address");
					if (address != null && (address instanceof String))
						intent.putExtra(prefix + "address", (String)address);
					
					Object title = ann.get("title");
					if (title != null && (title instanceof String))
						intent.putExtra(prefix + "title", (String)title);
					
					Object subtitle = ann.get("subtitle");
					if (subtitle != null && (subtitle instanceof String))
						intent.putExtra(prefix + "subtitle", (String)subtitle);
					
					Object url = ann.get("url");
					if (url != null && (url instanceof String))
						intent.putExtra(prefix + "url", (String)url);
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
		
		// Extrace parameters
		Bundle extras = getIntent().getExtras();
		apiKey = extras.getString(SETTINGS_PREFIX + "api_key");
		
		// Extract settings
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
		
		// Extract annotations
		int size = extras.getInt(ANNOTATIONS_PREFIX + "size");
		annotations = new Vector<Annotation>(size);
		for (int i = 0; i < size; ++i) {
			Annotation ann = new Annotation();
			String prefix = ANNOTATIONS_PREFIX + Integer.toString(i) + ".";
			
			ann.latitude = 10000;
			ann.longitude = 10000;
			
			String lat = extras.getString(prefix + "latitude");
			if (lat != null) {
				try {
					ann.latitude = Double.parseDouble(lat);
				}
				catch (NumberFormatException e) {}
			}
			
			String lon = extras.getString(prefix + "longitude");
			if (lon != null) {
				try {
					ann.longitude = Double.parseDouble(lon);
				}
				catch (NumberFormatException e) {}
			}
			
			ann.address = extras.getString(prefix + "address");
			ann.title = extras.getString(prefix + "title");
			ann.subtitle = extras.getString(prefix + "subtitle");
			ann.url = RhodesInstance.getInstance().normalizeUrl(extras.getString(prefix + "url"));
			annotations.addElement(ann);
		}
		
		// Create view
		view = new com.google.android.maps.MapView(this, apiKey);
		view.setClickable(true);
		layout.addView(view);
		
		Bitmap bitmap = BitmapFactory.decodeResource(getResources(), AndroidR.drawable.marker);
		Drawable marker = new BitmapDrawable(bitmap);
		marker.setBounds(0, 0, marker.getIntrinsicWidth(), marker.getIntrinsicHeight());
		annOverlay = new AnnotationsOverlay(this, marker);
		view.getOverlays().add(annOverlay);
		
		// Apply extracted parameters
		view.setBuiltInZoomControls(zoom_enabled);
		view.setSatellite(map_type.equals("hybrid") || map_type.equals("satellite"));
		view.setTraffic(false);
		view.setStreetView(false);
		
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
		
		Thread geocoding = new Thread(new Runnable() {
			public void run() {
				doGeocoding();
			}
		});
		geocoding.start();
	}
	
	private void doGeocoding() {
		for (int i = 0, lim = annotations.size(); i < lim; ++i) {
			Annotation ann = annotations.elementAt(i);
			if (ann.latitude == 10000 || ann.longitude == 10000)
				continue;
			annOverlay.addAnnotation(ann);
		}
		
		for (int i = 0, lim = annotations.size(); i < lim; ++i) {
			Annotation ann = annotations.elementAt(i);
			if (ann.latitude != 10000 && ann.longitude != 10000)
				continue;
			if (ann.address == null)
				continue;
			
			try {
				StringBuffer b = new StringBuffer();
				b.append("http://maps.google.com/maps/geo?q=");
				b.append(Uri.encode(ann.address));
				b.append("&output=csv&mobile=false&sensor=false");
				
				URL url = new URL(b.toString());
				
				b = new StringBuffer();
				
				InputStream is = url.openStream();
				byte[] buf = new byte[128];
				for (;;) {
					int n = is.read(buf);
					if (n == -1)
						break;
					
					String s = new String(buf, 0, n);
					b.append(s);
				}
				
				String response = b.toString();
				Vector<String> res = split(response, ",");
				if (res.size() != 4) {
					Logger.E(TAG, "Geocoding response parse error. Response: " + response);
					continue;
				}
				
				//int statusCode = Integer.parseInt(res.elementAt(0));
				//int accuracy = Integer.parseInt(res.elementAt(1));
				double latitude = Double.parseDouble(res.elementAt(2));
				double longitude = Double.parseDouble(res.elementAt(3));
				
				ann.latitude = latitude;
				ann.longitude = longitude;
				annOverlay.addAnnotation(ann);
			}
			catch (Exception e) {
				Logger.E(TAG, "GeoCoding request failed");
			}
			
			Rhodes.performOnUiThread(new Runnable() {
				public void run() {
					view.invalidate();
				}
			});
		}
	}

	@Override
	protected boolean isRouteDisplayed() {
		return false;
	}
	
	private static Vector<String> split(String s, String delimiter) {
		Vector<String> res = new Vector<String>();
		for (int start = 0, end = start;;) {
			end = s.indexOf(delimiter, start);
			if (end == -1) {
				res.addElement(s.substring(start));
				break;
			}
			else {
				res.addElement(s.substring(start, end));
				start = end + delimiter.length();
			}
		}

		return res;
	}
	
}
