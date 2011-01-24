package rhomobile.mapview;

import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.net.IHttpConnection;

import net.rim.device.api.math.Fixed32;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Graphics;
import net.rim.device.api.util.Comparator;
import net.rim.device.api.util.SimpleSortingVector;

public class ESRIMapField extends Field implements RhoMapField {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("ESRIMapField");
	
	private static final int TILE_SIZE = 256;
	private static final int MIN_ZOOM = 0;
	private static final int MAX_ZOOM = 19;
	
	private static final int CACHE_UPDATE_INTERVAL = 500;
	
	// Maximum size of image cache (number of images stored locally)
	private static final int MAX_IMAGE_CACHE_SIZE = 32;
	
	// Mode of decoding EncodedImage to bitmap
	private static final int DECODE_MODE = EncodedImage.DECODE_NATIVE |
		EncodedImage.DECODE_NO_DITHER | EncodedImage.DECODE_READONLY |
		EncodedImage.DECODE_ALPHA;
	
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
	
	private static class ByCoordinatesComparator implements Comparator {
		
		public int compare (Object o1, Object o2) {
			CachedImage img1 = (CachedImage)o1;
			CachedImage img2 = (CachedImage)o2;
			if (img1.latitude < img2.latitude) return -1;
			if (img1.latitude > img2.latitude) return 1;
			if (img1.longitude < img2.longitude) return 1;
			if (img1.longitude > img2.longitude) return -1;
			if (img1.zoom < img2.zoom) return -1;
			if (img1.zoom > img2.zoom) return 1;
			return 0;
		}
		
	}
	
	private static class ByAccessTimeComparator implements Comparator {
		
		public int compare (Object o1, Object o2) {
			long l1 = ((CachedImage)o1).lastUsed;
			long l2 = ((CachedImage)o2).lastUsed;
			return l1 < l2 ? 1 : l1 > l2 ? -1 : 0;
		}
		
	};
	
	private class CachedImage {
		public EncodedImage image;
		public Bitmap bitmap;
		public long latitude;
		public long longitude;
		public int zoom;
		public String key;
		public long lastUsed;
		
		public CachedImage(EncodedImage img, long lat, long lon, int z) {
			image = img;
			bitmap = null;
			latitude = lat;
			longitude = lon;
			zoom = z;
			key = makeCacheKey(latitude, longitude, zoom);
			lastUsed = System.currentTimeMillis();
		}
	};
	
	private class ImageCache {
		private Hashtable hash;
		private SimpleSortingVector cvec;
		private SimpleSortingVector tvec;
		
		public ImageCache() {
			reinit();
		}
		
		private void reinit() {
			hash = new Hashtable();
			cvec = new SimpleSortingVector();
			cvec.setSortComparator(new ByCoordinatesComparator());
			cvec.setSort(true);
			tvec = new SimpleSortingVector();
			tvec.setSortComparator(new ByAccessTimeComparator());
			tvec.setSort(true);
		}
		
		public ImageCache clone() {
			ImageCache cloned = new ImageCache();
			for (Enumeration e = hash.elements(); e.hasMoreElements();)
				cloned.put((CachedImage)e.nextElement());
			return cloned;
		}
		
		public Enumeration sortedByCoordinates() {
			return cvec.elements();
		}
		
		public CachedImage get(String key) {
			return (CachedImage)hash.get(key);
		}
		
		public void put(CachedImage img) {
			put(img, true);
		}
		
		private void put(CachedImage img, boolean doCheck) {
			hash.put(img.key, img);
			cvec.addElement(img);
			tvec.addElement(img);
			if (doCheck) check();
		}
		
		private void check() {
			if (hash.size() < MAX_IMAGE_CACHE_SIZE)
				return;
			
			SimpleSortingVector vec = tvec;
			reinit();
			
			Enumeration e = vec.elements();
			while (e.hasMoreElements()) {
				CachedImage img = (CachedImage)e.nextElement();
				put(img, false);
			}
		}
	};
	
	private ImageCache mImgCache = new ImageCache();
	
	private static abstract class MapCommand {
		public abstract String type();
		public abstract String description();
	}
	
	private static class MapFetchCommand extends MapCommand {
		
		public String baseUrl;
		public int zoom;
		public long latitude;
		public long longitude;
		
		public MapFetchCommand(String baseUrl, int zoom, long latitude, long longitude) {
			this.baseUrl = baseUrl;
			this.zoom = zoom;
			this.latitude = latitude;
			this.longitude = longitude;
		}
		
		private String makeDescription() {
			return "" + zoom + "/" + latitude + "/" + longitude;
		}
		
		public String type() {
			return "fetch";
		}
		
		public String description() {
			return "fetch:" + makeDescription();
		}
		
	};
	
	private class MapThread extends Thread {
		
		private static final int BLOCK_SIZE = 1024;
		
		private Vector commands = new Vector();
		private boolean active = true;
		
		public void process(MapCommand cmd) {
			synchronized (commands) {
				commands.addElement(cmd);
				commands.notify();
			}
		}
		
		public void stop() {
			active = false;
			interrupt();
		}
		
		public void run() {
			try {
				while (active) {
					MapCommand cmd = null;
					synchronized (commands) {
						if (commands.isEmpty()) {
							try {
								commands.wait();
							} catch (InterruptedException e) {
								// Nothing
							}
							continue;
						}
						cmd = (MapCommand)commands.elementAt(0);
						commands.removeElementAt(0);
						if (cmd == null)
							continue;
					}
					
					try {
						if (cmd instanceof MapFetchCommand)
							processCommand((MapFetchCommand)cmd);
						else
							LOG.INFO("Received unknown command: " + cmd.type() + ", ignore it");
					}
					catch (Exception e) {
						LOG.ERROR("Processing of map command failed", e);
					}
				}
			}
			catch (Exception e) {
				LOG.ERROR("Fatal error in map thread", e);
			}
			finally {
				LOG.INFO("Map thread exit");
			}
		}
		
		private byte[] fetchData(String url) throws IOException {
			IHttpConnection conn = RhoClassFactory.getNetworkAccess().connect(url,false);
			
			conn.setRequestMethod("GET");
			
			//conn.setRequestProperty("User-Agent", "Blackberry");
			//conn.setRequestProperty("Accept", "*/*");
			
			InputStream is = conn.openInputStream();
			
			int code = conn.getResponseCode();
			if (code/100 != 2)
				throw new IOException("ESRI map server respond with " + code + " " + conn.getResponseMessage());
			
			int size = conn.getHeaderFieldInt("Content-Length", 0);
			byte[] data = new byte[size];
			if (size == 0)
				size = 1073741824; // 1Gb :)
			
			byte[] buf = new byte[BLOCK_SIZE];
			for (int offset = 0; offset < size;) {
				int n = is.read(buf, 0, BLOCK_SIZE);
				if (n <= 0)
					break;
				if (offset + n > data.length) {
					byte[] newData = new byte[offset + n];
					System.arraycopy(data, 0, newData, 0, data.length);
					data = newData;
				}
				System.arraycopy(buf, 0, data, offset, n);
				offset += n;
			}
			
			return data;
		}
		
		private void processCommand(MapFetchCommand cmd) throws IOException {
			LOG.TRACE("Processing map fetch command (thread #" + hashCode() + "): " + cmd.description());
			
			long ts = toMaxZoom(TILE_SIZE, cmd.zoom);
			int row = (int)(cmd.latitude/ts);
			int column = (int)(cmd.longitude/ts);
			StringBuffer url = new StringBuffer();
			url.append(cmd.baseUrl);
			url.append("/MapServer/tile/");
			url.append(cmd.zoom);
			url.append('/');
			url.append(row);
			url.append('/');
			url.append(column);
			
			String finalUrl = url.toString();
			byte[] data = fetchData(finalUrl);
			
			EncodedImage img = EncodedImage.createEncodedImage(data, 0, data.length);
			img.setDecodeMode(DECODE_MODE);
			
			long lat = row*ts + ts/2;
			long lon = column*ts + ts/2;
			LOG.TRACE("Map request done, draw just received image: zoom=" + cmd.zoom + ", lat=" + lat + ", lon=" + lon);
			CachedImage cachedImage = new CachedImage(img, lat, lon, cmd.zoom);
			// Put image to the cache and trigger redraw
			synchronized (ESRIMapField.this) {
				mImgCache.put(cachedImage);
			}
			redraw();
		}
		
	};
	
	private MapThread mMapThread = new MapThread();
	
	private class CacheUpdate extends Thread {
		
		private boolean active = true;
		
		public void stop() throws InterruptedException {
			active = false;
			join();
		}
		
		public void run() {
			LOG.TRACE("Cache update thread started");
			while (active) {
				try {
					Thread.sleep(CACHE_UPDATE_INTERVAL);
				}
				catch (InterruptedException e) {
					// Ignore
				}
				
				//LOG.TRACE("Cache update: next loop; mLatitude=" + mLatitude +
				//		", mLongitude=" + mLongitude + ", mZoom=" + mZoom);
				
				long ts = toMaxZoom(TILE_SIZE, mZoom);
				//LOG.TRACE("Tile size on the maximum zoom level: " + ts);
				long h = toMaxZoom(mHeight, mZoom);
				//LOG.TRACE("Height of the screen on the maximum zoom level: " + h);
				long w = toMaxZoom(mWidth, mZoom);
				//LOG.TRACE("Width of the screen on the maximum zoom level: " + w);
				
				long totalTiles = MapTools.math_pow2(mZoom);
				//LOG.TRACE("Total tiles count on zoom level " + mZoom + ": " + totalTiles);
				
				long tlLat = mLatitude - h/2;
				if (tlLat < 0) tlLat = 0;
				long tlLon = mLongitude - w/2;
				if (tlLon < 0) tlLon = 0;
				//LOG.TRACE("tlLat=" + tlLat + ", tlLon=" + tlLon);
				for (long lat = (tlLat/ts)*ts, latLim = Math.min(tlLat + h + ts, ts*totalTiles); lat < latLim; lat += ts) {
					for (long lon = (tlLon/ts)*ts, lonLim = Math.min(tlLon + w + ts, ts*totalTiles); lon < lonLim; lon += ts) {
						String key = makeCacheKey(lat, lon, mZoom);
						MapFetchCommand cmd = null;
						synchronized (ESRIMapField.this) {
							CachedImage img = mImgCache.get(key);
							if (img == null) {
								//LOG.TRACE("lat=" + lat + ", lon=" + lon + "; key=" + key);
								String baseUrl = (String)mMapUrls.get(mMapType);
								cmd = new MapFetchCommand(baseUrl, mZoom, lat, lon);
								
								CachedImage dummy = new CachedImage(null, lat, lon, mZoom);
								mImgCache.put(dummy);
							}
						}
						if (cmd != null)
							mMapThread.process(cmd);
					}
				}
			}
			LOG.TRACE("Cache update thread stopped");
		}
		
	};
	
	private CacheUpdate mCacheUpdate = new CacheUpdate();
	
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
		
		LOG.TRACE("ESRIMapField ctor: mLatitude=" + mLatitude + ", mLongitude=" + mLongitude);
		
		mMapThread.start();
		mCacheUpdate.start();
	}
	
	public void close() {
		mMapThread.stop();
		try {
			mCacheUpdate.stop();
		}
		catch (InterruptedException e) {
			LOG.ERROR("Stopping of cache update thread was interrupted", e);
		}
	}
	
	public void redraw() {
		invalidate();
	}
	
	protected void paint(Graphics graphics) {
		// Draw background
		for (int i = 1, lim = 2*Math.max(mWidth, mHeight); i < lim; i += 5) {
			graphics.drawLine(0, i, i, 0);
		}
		
		ImageCache imgCache;
		synchronized (this) {
			imgCache = mImgCache.clone();
		}
		
		// Draw map tiles
		Enumeration e = imgCache.sortedByCoordinates();
		while (e.hasMoreElements()) {
			// Draw map
			CachedImage img = (CachedImage)e.nextElement();
			if (img.image == null)
				continue;
			
			paintImage(graphics, img);
		}
	}
	
	private void paintImage(Graphics graphics, CachedImage img) {
		// Skip images with zoom level which differ from the current zoom level
		if (img.zoom != mZoom)
			return;
		
		long left = -toCurrentZoom(mLongitude - img.longitude, mZoom);
		long top = -toCurrentZoom(mLatitude - img.latitude, mZoom);
		
		if (img.zoom != mZoom) {
			double x = MapTools.math_pow2d(img.zoom - mZoom);
			int factor = Fixed32.tenThouToFP((int)(x*10000));
			img.image = img.image.scaleImage32(factor, factor);
			img.bitmap = null;
		}
		
		int imgWidth = img.image.getScaledWidth();
		int imgHeight = img.image.getScaledHeight();
		left += (mWidth - imgWidth)/2;
		top += (mHeight - imgHeight)/2;
		
		int w = mWidth - (int)left;
		int h = mHeight - (int)top;
		
		int maxW = mWidth + TILE_SIZE;
		int maxH = mHeight + TILE_SIZE;
		if (w < 0 || h < 0 || w > maxW || h > maxH) {
			// Image will not be displayed, free its bitmap and skip it
			img.bitmap = null;
			return;
		}
		
		if (img.bitmap == null)
			img.bitmap = img.image.getBitmap();
		graphics.drawBitmap((int)left, (int)top, w, h, img.bitmap, 0, 0);
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
		
		//LOG.TRACE("moveTo(" + lat + ", " + lon + "): mLatitude=" + mLatitude + ", mLongitude=" + mLongitude);
	}
	
	public void move(int dx, int dy) {
		mLatitude += toMaxZoom(dy, mZoom);
		mLongitude += toMaxZoom(dx, mZoom);
		validateCoordinates();
		
		//LOG.TRACE("move(" + dx + ", " + dy + "): mLatitude=" + mLatitude + ", mLongitude=" + mLongitude);
	}

	public void setMapType(String type) {
		mMapType = type;
		//LOG.TRACE("setMapType: " + mMapType);
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
		LOG.TRACE("setZoom: " + mZoom);
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
		if (n == 0) return 0;
		long pow = MapTools.math_pow2(MAX_ZOOM - zoom);
		return n*pow;
	}
	
	private static long toCurrentZoom(long coord, int zoom) {
		if (coord == 0) return 0;
		long pow = MapTools.math_pow2(MAX_ZOOM - zoom);
		return coord/pow;
	}

	private static long degreesToPixelsX(double n, int z) {
		while (n < -180.0) n += 360.0;
		while (n > 180.0) n -= 360.0;
		double angleRatio = 360d/MapTools.math_pow2(z);
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
		double val = TILE_SIZE * MapTools.math_pow2(z) * (1 - ath/PI)/2;
		return (long)val;
	}
	
	private static double pixelsToDegreesX(long n, int z) {
		while (n < 0) n += MAX_LONGITUDE;
		while (n > MAX_LONGITUDE) n -= MAX_LONGITUDE;
		double angleRatio = 360d/MapTools.math_pow2(z);
		double val = n*angleRatio/TILE_SIZE - 180.0;
		return val;
	}
	
	private static double pixelsToDegreesY(long n, int z) {
		// Revert calculation of Merkator projection
		double ath = PI - 2*PI*n/(TILE_SIZE*MapTools.math_pow2(z));
		double th = MapTools.math_tanh(ath);
		double val = 180*MapTools.math_asin(th)/PI;
		return val;
	}
	
	private String makeCacheKey(long lat, long lon, int z) {
		while (lon < 0) lon += MAX_LONGITUDE;
		while (lon > MAX_LONGITUDE) lon -= MAX_LONGITUDE;
		long ts = toMaxZoom(TILE_SIZE, z);
		long x = lon/ts;
		long y = lat/ts;
		StringBuffer buf = new StringBuffer();
		buf.append(z);
		buf.append(';');
		buf.append(x);
		buf.append(';');
		buf.append(y);
		String key = buf.toString();
		return key;
	}

	public long toScreenCoordinateX(double n) {
		long v = degreesToPixelsX(n, mZoom);
		long center = toCurrentZoom(mLongitude, mZoom);
		long begin = center - mWidth/2;
		return v - begin;
	}
	
	public long toScreenCoordinateY(double n) {
		long v = degreesToPixelsY(n, mZoom);
		long center = toCurrentZoom(mLatitude, mZoom);
		long begin = center - mHeight/2;
		return v - begin;
	}
	
}
