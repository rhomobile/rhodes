package rhomobile.mapview;

import java.util.Hashtable;

import com.rho.RhoConf;

import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Graphics;

public class ESRIMapField extends Field implements RhoMapField {
	
	private static final int TILE_SIZE = 256;
	private static final int MIN_ZOOM = 0;
	private static final int MAX_ZOOM = 19;
	
	// Constants required to coordinates calculations
	private static final long MIN_LATITUDE = degreesToPixelsY(90, MAX_ZOOM);
	private static final long MAX_LATITUDE = degreesToPixelsY(-90, MAX_ZOOM);
	private static final long MAX_LONGITUDE = degreesToPixelsX(180, MAX_ZOOM);
	
	// DON'T CHANGE THIS CONSTANT!!!
	// This is maximum absolute value of sine ( == sin(85*PI/180) ) allowed by Merkator projection
	private static final double MAX_SIN = 0.99627207622;
	
	private static final double PI = Math.PI;
	
	private Hashtable mMapUrls = new Hashtable();
	
	private String mMapType;
	
	//===============================================================================
	// Coordinates of center in pixels of maximum zoom level
	private long mLatitude = degreesToPixelsY(0, MAX_ZOOM);
	private long mLongitude = degreesToPixelsX(0, MAX_ZOOM);
	private int mZoom = 0;
	
	private int mWidth;
	private int mHeight;
	
	private static abstract class MapCommand {
		public abstract String type();
		public abstract String description();
	}
	
	private static class MapFetchCommand extends MapCommand {
		
		private String makeDescription() {
			// TODO:
			return "unknown/unknown";
		}
		
		public String type() {
			return "fetch";
		}
		
		public String description() {
			return "fetch:" + makeDescription();
		}
		
	};
	
	public ESRIMapField() {
		String url = RhoConf.getInstance().getString("esri_map_url_roadmap");
		if (url == null || url.length() == 0)
			url = "http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/";
		mMapUrls.put("roadmap", url);
		
		url = RhoConf.getInstance().getString("esri_map_url_satellite");
		if (url == null || url.length() == 0)
			url = "http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/";
		mMapUrls.put("satellite", url);
		
		mMapType = "roadmap";
	}
	
	public void close() {
		// TODO Auto-generated method stub

	}
	
	public boolean needToClose() {
		// TODO Auto-generated method stub
		return false;
	}

	public void redraw() {
		invalidate();
	}
	
	protected void paint(Graphics graphics) {
		// Draw background
		for (int i = 1, lim = 2*Math.max(mWidth, mHeight); i < lim; i += 5) {
			graphics.drawLine(0, i, i, 0);
		}
	}
	
	public boolean handleClick() {
		// TODO: handle annotation click here
		return false;
	}

	public void addAnnotation(Annotation ann) {
		// TODO: add annotation and start geocoding request if needed
	}

	protected void layout(int w, int h) {
		mWidth = Math.min(mWidth, w);
		mHeight = Math.min(mHeight, h);
		setExtent(mWidth, mHeight);
	}
	
	public int calculateZoom(double latDelta, double lonDelta) {
		int zoom1 = calcZoom(latDelta, mWidth);
		int zoom2 = calcZoom(lonDelta, mHeight);
		return zoom1 < zoom2 ? zoom1 : zoom2;
	}

	public Field getBBField() {
		return this;
	}

	public double getCenterLatitude() {
		return pixelsToDegreesY(mLatitude, MAX_ZOOM);
	}

	public double getCenterLongitude() {
		return pixelsToDegreesX(mLongitude, MAX_ZOOM);
	}

	private void validateCoordinates() {
		if (mLatitude < MIN_LATITUDE) mLatitude = MIN_LATITUDE;
		if (mLatitude > MAX_LATITUDE) mLatitude = MAX_LATITUDE;
	}
	
	public void moveTo(double lat, double lon) {
		mLatitude = degreesToPixelsY(lat, MAX_ZOOM);
		mLongitude = degreesToPixelsX(lon, MAX_ZOOM);
		validateCoordinates();
	}
	
	public void move(int dx, int dy) {
		mLatitude += toMaxZoom(dy, mZoom);
		mLongitude += toMaxZoom(dx, mZoom);
		validateCoordinates();
	}

	public void setMapType(String type) {
		mMapType = type;
	}

	public void setPreferredSize(int width, int height) {
		mWidth = width;
		mHeight = height;
	}
	
	public int getPreferredWidth() {
		return mWidth;
	}
	
	public int getPreferredHeight() {
		return mHeight;
	}

	public void setZoom(int zoom) {
		mZoom = zoom;
		if (mZoom < MIN_ZOOM) mZoom = MIN_ZOOM;
		if (mZoom > MAX_ZOOM) mZoom = MAX_ZOOM;
	}

	public int getMaxZoom() {
		return MAX_ZOOM;
	}

	public int getMinZoom() {
		return MIN_ZOOM;
	}

	public int getZoom() {
		return mZoom;
	}
	
	private static int calcZoom(double degrees, int pixels) {
		double angleRatio = degrees*TILE_SIZE/pixels;
		
		double twoInZoomExp = 360/angleRatio;
		int zoom = (int)MapTools.math_log2(twoInZoomExp);
		return zoom;
	}
	
	private static long toMaxZoom(long n, int zoom) {
		return n == 0 ? 0 : n*(long)MapTools.math_pow(2, MAX_ZOOM - zoom);
	}
	
	private static long toCurrentZoom(long coord, int zoom) {
		if (coord == 0) return 0;
		long pow = (long)MapTools.math_pow(2, MAX_ZOOM - zoom);
		return coord/pow;
	}

	private static long degreesToPixelsX(double n, int z) {
		while (n < -180.0) n += 360.0;
		while (n > 180.0) n -= 360.0;
		double angleRatio = 360/MapTools.math_pow(2, z);
		double val = (n + 180)*TILE_SIZE/angleRatio;
		return (long)val;
	}
	
	private static long degreesToPixelsY(double n, int z) {
		// Merkator projection
		double sin_phi = MapTools.math_sin(n*PI/180);
		// MAX_SIN - maximum value of sine allowed by Merkator projection
		// (~85.0 degrees of north latitude)
		if (sin_phi < -MAX_SIN) sin_phi = -MAX_SIN;
		if (sin_phi > MAX_SIN) sin_phi = MAX_SIN;
		
		double ath = MapTools.math_atanh(sin_phi);
		double val = TILE_SIZE * MapTools.math_pow(2, z) * (1 - ath/PI)/2;
		return (long)val;
	}
	
	private static double pixelsToDegreesX(long n, int z) {
		while (n < 0) n += MAX_LONGITUDE;
		while (n > MAX_LONGITUDE) n -= MAX_LONGITUDE;
		double angleRatio = 360/MapTools.math_pow(2, z);
		double val = n*angleRatio/TILE_SIZE - 180.0;
		return val;
	}
	
	private static double pixelsToDegreesY(long n, int z) {
		// Revert calculation of Merkator projection
		double ath = PI - 2*PI*n/(TILE_SIZE*MapTools.math_pow(2, z));
		double th = MapTools.math_tanh(ath);
		double val = 180*MapTools.math_asin(th)/PI;
		return val;
	}
	
}
