package com.rhomobile.rhodes.gmaps;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import androidx.fragment.app.FragmentActivity;
import android.view.Window;
import android.view.WindowManager;

import com.google.android.gms.maps.CameraUpdate;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.GoogleMapOptions;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.UiSettings;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.CameraPosition;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.LatLngBounds;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.WebView;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.mapview.ExtrasHolder;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.location.Address;
import android.location.Geocoder;
import android.os.Bundle;
import android.os.IBinder;

public class GMapActivity extends FragmentActivity implements OnMapReadyCallback {
    
    static class MarkerAdds {
        public String url;
        public String address;
        public MarkerOptions options;
        MarkerAdds(String url, String address) {
            this.url = url;
            this.address = address;
        }
    }
    
    class OnCameraChangeListener implements GoogleMap.OnCameraChangeListener {
        private CameraUpdate mInitialCameraUpdate;
        OnCameraChangeListener(CameraUpdate initialCameraUpdate) {
            mInitialCameraUpdate = initialCameraUpdate;
        }
        @Override
        public void onCameraChange(CameraPosition arg0) {
            if (mInitialCameraUpdate != null) {
                Logger.T(TAG, "Move camera to initial region");
                mMap.animateCamera(mInitialCameraUpdate);
                mInitialCameraUpdate = null;
            }
        }
    }
    
    class OnInfoWindowClickListener implements GoogleMap.OnInfoWindowClickListener {

        @Override
        public void onInfoWindowClick(Marker marker) {
            Logger.T(TAG, "Marker callout has clicked");
            MarkerAdds adds = mAnnotations.get(marker);
            if (adds != null && adds.url != null) {
                Logger.T(TAG, "Marker URL: " + adds.url);
                WebView.navigate(adds.url, WebView.activeTab());
                finish();
            }
        }
    }
    
    private static final String TAG = GMapActivity.class.getSimpleName();

    private static final String SETTINGS_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + ".settings.";
    private static final String ANNOTATIONS_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + ".annotations.";

    private static ExtrasHolder mHolder;
    private static GMapActivity mInstance;
    private ServiceConnection mServiceConnection;
    private GoogleMap mMap;
    private CameraUpdate mInitialCameraUpdate;
    private HashMap<Marker, MarkerAdds> mAnnotations = new HashMap<Marker, MarkerAdds>();
    private ArrayList<MarkerAdds> mGeoCodingQueue = new ArrayList<MarkerAdds>();

    @Override
    protected void onCreate(Bundle state) {
        super.onCreate(state);
        
        Intent intent = new Intent(this, RhodesService.class);
        mServiceConnection = new ServiceConnection() {
            @Override
            public void onServiceDisconnected(ComponentName name) {}
            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {}
        };
        bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE);

        setFullScreen(BaseActivity.getFullScreenMode());

        setContentView(R.layout.gmaplayout);
        
        SupportMapFragment mapFragment = (SupportMapFragment)getSupportFragmentManager().findFragmentById(R.id.map);

        mapFragment.getMapAsync(this);
    }


    @Override
    public void onMapReady(GoogleMap googleMap){
        mMap = googleMap;
        
        if ( null == mMap ) {
            Logger.E(TAG,"GoogleMap object is null from fragment. Finishing activity.");
            finish();
            return;
        }
        
        Logger.T(TAG, "Map options: " + mHolder.toString());
        
        String map_type = mHolder.getString(SETTINGS_PREFIX + "map_type");
        if (map_type == null || map_type.equals(""))
            Logger.T(TAG, "Show normal map");
        else if (map_type.equalsIgnoreCase("satellite")) {
            Logger.T(TAG, "Show satellite map");
            mMap.setMapType(GoogleMap.MAP_TYPE_SATELLITE);
        } else if (map_type.equalsIgnoreCase("hybrid")) {
            Logger.T(TAG, "Show hybrid map");
            mMap.setMapType(GoogleMap.MAP_TYPE_HYBRID);
        } else if (map_type.equalsIgnoreCase("terrain")) {
            Logger.T(TAG, "Show terrain map");
            mMap.setMapType(GoogleMap.MAP_TYPE_TERRAIN);
        } else {
            Logger.T(TAG, "Show normal map");
        }
        
        UiSettings mapSettings = mMap.getUiSettings();

        boolean zoomEnabled = mHolder.getBoolean(SETTINGS_PREFIX + "zoom_enabled");
        mapSettings.setZoomControlsEnabled(zoomEnabled);
        mapSettings.setZoomGesturesEnabled(zoomEnabled);

        boolean scrollEnabled = mHolder.getBoolean(SETTINGS_PREFIX + "scroll_enabled");
        mapSettings.setRotateGesturesEnabled(scrollEnabled);
        mapSettings.setScrollGesturesEnabled(scrollEnabled);
        mapSettings.setTiltGesturesEnabled(scrollEnabled);

        boolean userLocation = mHolder.getBoolean(SETTINGS_PREFIX + "shows_user_location");
        Logger.T(TAG, "Show user location: " + userLocation);
        mapSettings.setMyLocationButtonEnabled(userLocation);
        mMap.setMyLocationEnabled(userLocation);

        String type = mHolder.getString(SETTINGS_PREFIX + "region");
        try {
            LatLngBounds.Builder latLngBoundsBuilder = LatLngBounds.builder();
            if ("square".equals(type)) {
                double centerLat = 0;
                double centerLng = 0;
                String latitude = mHolder.getString(SETTINGS_PREFIX + "region.latitude");
                String longitude = mHolder.getString(SETTINGS_PREFIX + "region.longitude");
                if (latitude != null && longitude != null) {
                    centerLat = Double.parseDouble(latitude);
                    centerLng = Double.parseDouble(longitude);
                    latLngBoundsBuilder.include(new LatLng(centerLat, centerLng));
                }
                
                String latSpan = mHolder.getString(SETTINGS_PREFIX + "region.latSpan");
                String lngSpan = mHolder.getString(SETTINGS_PREFIX + "region.lonSpan");
                if (latSpan != null && lngSpan != null) {
                    double lat = Double.parseDouble(latSpan);
                    double lng = Double.parseDouble(lngSpan);
                    latLngBoundsBuilder.include(new LatLng(centerLat - lat, centerLng - lng));
                    latLngBoundsBuilder.include(new LatLng(centerLat + lat, centerLng + lng));

                }
                mInitialCameraUpdate = CameraUpdateFactory.newLatLngBounds(latLngBoundsBuilder.build(), 0);
                
            }
            else if ("circle".equals(type)) {
                String center = mHolder.getString(SETTINGS_PREFIX + "region.center");
                String radius = mHolder.getString(SETTINGS_PREFIX + "region.radius");
//                if (center != null && radius != null) {
//                    double span = Double.parseDouble(radius);
//                    spanLat = spanLon = span;
//                    Annotation ann = new Annotation();
//                    ann.type = "center";
//                    ann.latitude = ann.longitude = 10000;
//                    ann.address = center;
//                    annotations.insertElementAt(ann, 0);
//                }
            }
        }
        catch (NumberFormatException e) {
            Logger.E(TAG, "Wrong region span");
            Logger.E(TAG, e);
        }
        
        mMap.setOnCameraChangeListener(new OnCameraChangeListener(mInitialCameraUpdate));

        // Extract annotations
        int size = mHolder.getInt(ANNOTATIONS_PREFIX + "size");
        List<MarkerOptions> annotations = new ArrayList<MarkerOptions>(size);
        for (int i = 0; i < size; ++i) {
            MarkerOptions ann = new MarkerOptions();
            String prefix = ANNOTATIONS_PREFIX + Integer.toString(i) + ".";
            
            String title = mHolder.getString(prefix + "title");
            Logger.T(TAG, "Marker title: " + title);
            ann.title(title);
            
            String subtitle = mHolder.getString(prefix + "subtitle");
            Logger.T(TAG, "Marker snippet: " + subtitle);

            String address = mHolder.getString(prefix + "address");
            Logger.T(TAG, "Marker address: " + address);

            String url = mHolder.getString(prefix + "url");

            if (subtitle != null || address != null) {
                String snippet = null;
                if (subtitle != null) {
                    snippet = subtitle;
                }
                if (address != null) {
                    if (snippet == null) {
                        snippet = address;
                    }
                }
                if (snippet != null) {
                    ann.snippet(snippet);
                }
            }
            if (url != null) {
                url = RhodesService.getInstance().normalizeUrl(url);
            }
            
            try {
                double lat = mHolder.getDouble(prefix + "latitude");
                double lng = mHolder.getDouble(prefix + "longitude");
                if (lat != 0.0 || lng != 0.0) {
                    Logger.T(TAG, "Marker position: {" + lat + ", " + lng + "}");
                    ann.position(new LatLng(lat, lng));
                }
            } catch (NumberFormatException e) {}
            
            
            String image = mHolder.getString(prefix+"image");
            if (image != null) {

                image = RhoFileApi.normalizePath("apps/" + image);
                Logger.T(TAG, "Final image path: " + image);

                Bitmap icon = BitmapFactory.decodeStream(RhoFileApi.open(image));
                Logger.T(TAG, "Marker custom icon size: " + icon.getWidth() + "X" + icon.getHeight());
                
                ann.icon(BitmapDescriptorFactory.fromBitmap(icon));
                
                if (mHolder.getString(prefix + "image_x_offset") != null &&
                        mHolder.getString(prefix + "image_y_offset") != null) {
                    
                    int xOffset = mHolder.getInt(prefix + "image_x_offset");
                    int yOffset = mHolder.getInt(prefix + "image_y_offset");
                    Logger.T(TAG, "Marker image: " + image + ", offset: " + xOffset + ", " + yOffset);
                
                    ann.anchor((float)xOffset / icon.getWidth(), (float)yOffset/icon.getHeight());
                }
            }
            
            if (ann.getPosition() != null) {
                addMarker(ann, (address != null || url != null) ? new MarkerAdds(url, address) : null );
            } else {
                Logger.T(TAG, "Marker has no position yet, need wait for geocoding result.");
                
                MarkerAdds markerAdds = new MarkerAdds(url, address);
                markerAdds.options = ann;
                mGeoCodingQueue.add(markerAdds);
            }
            
        }
        mMap.setOnInfoWindowClickListener(new OnInfoWindowClickListener());
        mInstance = this;

        Thread geocoding = new Thread(new Runnable() {
            public void run() {
                doGeocoding();
            }
        });
        geocoding.start();
    }


    @Override
    protected void onStart() {
        super.onStart();
        BaseActivity.onActivityStarted(this);
    }

    @Override
    protected void onStop() {
        BaseActivity.onActivityStopped(this);
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        clearGeoCodingQueue();
        clearMarkerAdds();
        if (mServiceConnection != null) {
            unbindService(mServiceConnection);
            mServiceConnection = null;
        }
        mInstance = null;
        super.onDestroy();
    }
    
    private void addMarker(MarkerOptions markerOptions, MarkerAdds markerAdds) {
        Logger.T(TAG, "Adding marker to map...");
        synchronized(mAnnotations) {
            Marker marker = mMap.addMarker(markerOptions);
            if (markerAdds != null) {
                Logger.T(TAG, "Add marker adds, url: " + markerAdds.url);
                mAnnotations.put(marker, markerAdds);
            }
        }
    }
    private void clearMarkerAdds() {
        Logger.T(TAG, "Clearing markers");
        synchronized(mAnnotations) {
            mAnnotations.clear();
        }
    }

    private void doGeocoding() {

        MarkerAdds adds = null;
        
        while (!mGeoCodingQueue.isEmpty()) {
            synchronized (mGeoCodingQueue) {
                if (adds != null) {
                    mGeoCodingQueue.remove(adds);
                }
                if (mGeoCodingQueue.isEmpty()) {
                    break;
                }
                adds = mGeoCodingQueue.get(0);
            }
            
            Geocoder gc = new Geocoder(this);
            try {
                List<Address> geoAddressList = gc.getFromLocationName(adds.address, 1);
                if (geoAddressList.size() == 0) {
                    Logger.W(TAG, "Geo coding is failed for address: " + adds.address);
                    continue;
                }
                
                Address geoAddress = geoAddressList.get(0);
                
                adds.options.position(new LatLng(geoAddress.getLatitude(), geoAddress.getLongitude()));
                final MarkerAdds finalAdds = adds;
                PerformOnUiThread.exec(new Runnable() {
                    @Override public void run() {
                        addMarker(finalAdds.options, finalAdds);
                    }});
//                if (ann.type.equals("center")) {
//                    MapController controller = view.getController();
//                    center.latitude = ann.latitude;
//                    center.longitude = ann.longitude;
//                    controller.setCenter(new GeoPoint((int)(ann.latitude*1000000), (int)(ann.longitude*1000000)));
//                    controller.zoomToSpan((int)(spanLat*1000000), (int)(spanLon*1000000));
//                    PerformOnUiThread.exec(new Runnable() {
//                        public void run() {
//                            view.invalidate();
//                        }
//                    });
//                }
            } catch (IOException e) {
                Logger.E(TAG, "GeoCoding request failed.");
                Logger.E(TAG, e);
            }
            
        }
    }
    
    private void clearGeoCodingQueue() {
        synchronized (mGeoCodingQueue) {
            mGeoCodingQueue.clear();
        }
    }
    
    public static void create(Map<String, Object> params) {
        mHolder = new ExtrasHolder();

        Object settings = params.get("settings");
        if (settings != null && (settings instanceof Map<?,?>)) {
            Map<Object, Object> hash = (Map<Object, Object>)settings;
            Object map_type = hash.get("map_type");
            if (map_type != null && (map_type instanceof String)) {
                mHolder.putExtra(SETTINGS_PREFIX + "map_type", (String)map_type);
                Logger.T(TAG, "Map type: " + (String)map_type);
            }
            
            Object zoom_enabled = hash.get("zoom_enabled");
            if (zoom_enabled != null && (zoom_enabled instanceof String)) {
                mHolder.putExtra(SETTINGS_PREFIX + "zoom_enabled", ((String)zoom_enabled).equalsIgnoreCase("true"));
                Logger.T(TAG, "Zoom: " + (String)zoom_enabled);
            }
            
            Object scroll_enabled = hash.get("scroll_enabled");
            if (scroll_enabled != null && (scroll_enabled instanceof String)) {
                mHolder.putExtra(SETTINGS_PREFIX + "scroll_enabled", ((String)scroll_enabled).equalsIgnoreCase("true"));
                Logger.T(TAG, "Scroll: " + (String)scroll_enabled);
            }
            
            Object shows_user_location = hash.get("shows_user_location");
            if (shows_user_location != null && (shows_user_location instanceof String)) {
                mHolder.putExtra(SETTINGS_PREFIX + "shows_user_location", ((String)shows_user_location).equalsIgnoreCase("true"));
                Logger.T(TAG, "User location: " + (String)shows_user_location);
            }
            
            Object region = hash.get("region");
            if (region != null) {
                if (region instanceof Vector<?>) {
                    Vector<String> reg = (Vector<String>)region;
                    if (reg.size() == 4) {
                        mHolder.putExtra(SETTINGS_PREFIX + "region", "square");
                        mHolder.putExtra(SETTINGS_PREFIX + "region.latitude", reg.elementAt(0));
                        mHolder.putExtra(SETTINGS_PREFIX + "region.longitude", reg.elementAt(1));
                        mHolder.putExtra(SETTINGS_PREFIX + "region.latSpan", reg.elementAt(2));
                        mHolder.putExtra(SETTINGS_PREFIX + "region.lonSpan", reg.elementAt(3));
                    }
                }
                else if (region instanceof Map<?,?>) {
                    Map<Object, Object> reg = (Map<Object,Object>)region;
                    String center = null;
                    String radius = null;
                    
                    Object centerObj = reg.get("center");
                    if (centerObj != null && (centerObj instanceof String))
                        center = (String)centerObj;
                    
                    Object radiusObj = reg.get("radius");
                    if (radiusObj != null && (radiusObj instanceof String))
                        radius = (String)radiusObj;
                    
                    if (center != null && radius != null) {
                        mHolder.putExtra(SETTINGS_PREFIX + "region", "circle");
                        mHolder.putExtra(SETTINGS_PREFIX + "region.center", center);
                        mHolder.putExtra(SETTINGS_PREFIX + "region.radius", radius);
                    }
                }
            }
        }

        Object annotations = params.get("annotations");
        if (annotations != null && (annotations instanceof Vector<?>)) {
            Vector<Object> arr = (Vector<Object>)annotations;
            
            mHolder.putExtra(ANNOTATIONS_PREFIX + "size", arr.size());
            
            for (int i = 0, lim = arr.size(); i < lim; ++i) {
                Object annObj = arr.elementAt(i);
                if (annObj == null || !(annObj instanceof Map<?, ?>))
                    continue;
                
                Map<Object, Object> ann = (Map<Object, Object>)annObj;
                
                String prefix = ANNOTATIONS_PREFIX + Integer.toString(i) + ".";
                
                Object latitude = ann.get("latitude");
                if (latitude != null && (latitude instanceof String))
                    mHolder.putExtra(prefix + "latitude", (String)latitude);
                
                Object longitude = ann.get("longitude");
                if (longitude != null && (longitude instanceof String))
                    mHolder.putExtra(prefix + "longitude", (String)longitude);
                
                Object address = ann.get("street_address");
                if (address != null && (address instanceof String))
                    mHolder.putExtra(prefix + "address", (String)address);
                
                Object title = ann.get("title");
                if (title != null && (title instanceof String))
                    mHolder.putExtra(prefix + "title", (String)title);
                
                Object subtitle = ann.get("subtitle");
                if (subtitle != null && (subtitle instanceof String))
                    mHolder.putExtra(prefix + "subtitle", (String)subtitle);
                
                Object url = ann.get("url");
                if (url != null && (url instanceof String))
                    mHolder.putExtra(prefix + "url", (String)url);

                Object image = ann.get("image");
                if (image != null && (image instanceof String))
                    mHolder.putExtra(prefix + "image", (String)image);

                Object image_x_offset = ann.get("image_x_offset");
                if (image_x_offset != null && (image_x_offset instanceof String))
                    mHolder.putExtra(prefix + "image_x_offset", (String)image_x_offset);
                
                Object image_y_offset = ann.get("image_y_offset");
                if (image_y_offset != null && (image_y_offset instanceof String))
                    mHolder.putExtra(prefix + "image_y_offset", (String)image_y_offset);
            }
        }
        
        Intent intent = new Intent(RhodesActivity.getContext(), GMapActivity.class);
        RhodesService.getInstance().startActivity(intent);
    }

    public static void close() {
        if (mInstance != null) {
            mInstance.finish();
        }
    }

    public static boolean isStarted() {
        return false;
    }

    public static double getCenterLatitude() {
        return 0.0;
    }

    public static double getCenterLongitude() {
        return 0.0;
    }

    public void setFullScreen(boolean enable) {
        Window window = getWindow();
        if (enable) {
            window.clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
            window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
        else {
            window.clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
            window.setFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN, WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
        }
    }
    
}
