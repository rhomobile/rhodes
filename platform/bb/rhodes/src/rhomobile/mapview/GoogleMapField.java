package rhomobile.mapview;

import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.net.IHttpConnection;
import com.rho.net.URI;
import com.xruby.runtime.builtin.ObjectFactory;

import rhomobile.WebView;
import rhomobile.mapview.RhoMapField;
import net.rim.device.api.math.Fixed32;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Graphics;
import net.rim.device.api.util.Comparator;
//import net.rim.device.api.util.MathUtilities;
import net.rim.device.api.util.SimpleSortingVector;
import j2me.lang.MathEx;

public class GoogleMapField extends Field implements RhoMapField {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GoogleMapField");
	
	// Configurable parameters
	private static final int ADD_X = 100;
	private static final int ADD_Y = 100;
	
	// Sensivity of annotations area (in pixels)
	private static final int SENSIVITY_DELTA = 8;
	
	private static final int FETCH_THREAD_POOL_SIZE = 1;
	private static final long MAX_FETCH_TIME = 20000; // milliseconds
	
	private static final int MAP_REDRAW_INTERVAL = 1000; // milliseconds
	
	private static final int WAITING_FETCH_COMMAND_MAX = 10;
	
	private static final int REDRAW_DELAY = 1000;
	
	// Maximum size of image cache (number of images stored locally)
	private static final int MAX_CACHE_SIZE = 10;
	
	// Static google parameters
	private static final int MIN_ZOOM = 0;
	private static final int MAX_ZOOM = 19;
	
	private static final int TILE_SIZE = 256;
	
	private static final int DECODE_MODE = EncodedImage.DECODE_NATIVE |
		EncodedImage.DECODE_NO_DITHER | EncodedImage.DECODE_READONLY;
	
	private static final long MAX_LATITUDE = degreesToPixels(180, MAX_ZOOM);
	private static final long MAX_LONGITUDE = degreesToPixels(360, MAX_ZOOM);
	
	private static final boolean VERBOSE_TRACING = false;
	
	private void VERBOSE_TRACE(String message) {
		if (VERBOSE_TRACING)
			LOG.TRACE(message);
	}
	
	// Coordinates of center in pixels of maximum zoom level
	private long latitude = 0;
	private long longitude = 0;
	private int zoom = 0;
	
	private int width;
	private int height;
	private String maptype;
	private Vector annotations;
	
	private boolean needToCloseMap = false;
	
	public boolean needToClose() {
		return needToCloseMap;
	}
	
	private static class CachedImage {
		public EncodedImage image;
		public long latitude;
		public long longitude;
		public int zoom;
		public String key;
		public long lastUsed;
		
		public CachedImage(EncodedImage img, long lat, long lon, int z) {
			image = img;
			latitude = lat;
			longitude = lon;
			zoom = z;
			key = makeCacheKey(latitude, longitude, zoom);
			lastUsed = System.currentTimeMillis();
		}
	};
	
	private static class CachedBitmap extends CachedImage {
		public Bitmap bitmap;
		
		public CachedBitmap(CachedImage img) {
			super(img.image, img.latitude, img.longitude, img.zoom);
			bitmap = img.image.getBitmap();
		}
		
		public CachedBitmap(EncodedImage img, long lat, long lon, int z) {
			super(img, lat, lon, z);
			bitmap = img.getBitmap();
		}
	};

	private Hashtable cache = new Hashtable();
	private CachedBitmap image = null;
	
	private long lastFetchCommandSent = 0;
	private Timer timer = new Timer();
	
	private static abstract class MapCommand {
		public abstract String type();
		public abstract String description();
	}
	
	private static class MapFetchCommand extends MapCommand {
		
		public long latitude;
		public long longitude;
		public int zoom;
		public int width;
		public int height;
		public String maptype;
		public Vector annotations;
		public GoogleMapField mapField;
		
		public MapFetchCommand(long lat, long lon, int z, int w, int h,
				String m, Vector a, GoogleMapField mf) {
			latitude = lat;
			longitude = lon;
			zoom = z;
			width = w;
			height = h;
			maptype = m;
			annotations = a;
			mapField = mf;
		}
		
		public boolean equals(Object c) {
			if (!(c instanceof MapFetchCommand))
				return false;
			MapFetchCommand cmd = (MapFetchCommand)c;
			return latitude == cmd.latitude && longitude == cmd.longitude &&
				zoom == cmd.zoom && width == cmd.width && maptype.equals(cmd.maptype) &&
				annotations == cmd.annotations && mapField == cmd.mapField;
		}
		
		public String type() {
			return "fetch";
		}
		
		public String description() {
			return makeCoordDescription(latitude, longitude, zoom);
		}
	}
	
	private static class MapGeocodingCommand extends MapCommand {

		public Annotation annotation;
		public GoogleMapField mapField;
		
		public MapGeocodingCommand(Annotation a, GoogleMapField mf) {
			annotation = a;
			mapField = mf;
		}
		
		public boolean equals(Object c) {
			if (!(c instanceof MapGeocodingCommand))
				return false;
			MapGeocodingCommand cmd = (MapGeocodingCommand)c;
			return annotation.equals(cmd.annotation);
		}

		public String type() {
			return "geocoding";
		}
		
		public String description() {
			if (annotation.street_address != null)
				return "location:\"" + annotation.street_address + "\"";
			else if (annotation.coordinates != null)
				return "location:" + annotation.coordinates.latitude + "," +
					annotation.coordinates.longitude;
			else
				return "location:null";
		}
	}
		
	private static class MapThread extends Thread {

		private static final String mapkey = "ABQIAAAA-X8Mm7F-7Nmz820lFEBHYxT2yXp_ZAY8_ufC3CFXhHIE1NvwkxSfNPZbryNEPHF-5PQKi9c7Fbdf-A";
		
		private static final int BLOCK_SIZE = 1024;
		
		private MapCommand processing = null;
		private long taskStartTime;
		private boolean active = true;
		
		// Return time of start current task. Return undefined value if no current task exists.
		public long startTime() {
			return taskStartTime;
		}
		
		public boolean isBusy() {
			return processing != null;
		}
		
		public boolean isProcessing(MapCommand cmd) {
			return processing == null ? false : processing.equals(cmd);
		}
		
		public void process(MapCommand cmd) {
			synchronized (this) {
				processing = cmd;
				notify();
			}
		}
		
		public void stop() {
			active = false;
			interrupt();
		}
		
		private byte[] fetchData(String url) throws IOException {
			IHttpConnection conn = RhoClassFactory.getNetworkAccess().connect(url,false);
			
			conn.setRequestMethod("GET");
			
			//conn.setRequestProperty("User-Agent", "Blackberry");
			//conn.setRequestProperty("Accept", "*/*");
			
			InputStream is = conn.openInputStream();
			
			int code = conn.getResponseCode();
			if (code/100 != 2)
				throw new IOException("Google map respond with " + code + " " + conn.getResponseMessage());
			
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
			
			// Process received command
			double latitude = 90.0 - pixelsToDegrees(cmd.latitude, MAX_ZOOM);
			double longitude = pixelsToDegrees(cmd.longitude, MAX_ZOOM) - 180.0;
			StringBuffer url = new StringBuffer();
			url.append("http://maps.google.com/maps/api/staticmap?");
			url.append("center=" + Double.toString(latitude) + "," + Double.toString(longitude));
			url.append("&zoom=" + cmd.zoom);
			url.append("&size=" + (cmd.width + ADD_X*2) + "x" + (cmd.height + ADD_Y*2));
			url.append("&maptype=" + cmd.maptype);
			url.append("&format=png&mobile=true&sensor=false");
			url.append("&key=" + mapkey);
			if (!cmd.annotations.isEmpty()) {
				url.append("&markers=color:blue");
				for (int i = 0, lim = cmd.annotations.size(); i < lim; ++i) {
					Annotation ann = (Annotation)cmd.annotations.elementAt(i);
					if (ann.coordinates != null) {
						url.append('|');
						url.append(ann.coordinates.latitude);
						url.append(',');
						url.append(ann.coordinates.longitude);
					}
					else if (ann.street_address != null) {
						url.append('|');
						url.append(URI.urlEncode(ann.street_address));
					}
				}
			}
			String finalUrl = url.toString();
		
			byte[] data = fetchData(finalUrl);
			
			EncodedImage img = EncodedImage.createEncodedImage(data, 0, data.length);
			img.setDecodeMode(DECODE_MODE);
			LOG.TRACE("Map request done, draw just received image");
			cmd.mapField.draw(cmd.latitude, cmd.longitude, cmd.zoom, img);
		}
		
		private void processCommand(MapGeocodingCommand cmd) throws IOException {
			LOG.TRACE("Processing map geocoding command (thread #" + hashCode() + "): " + cmd.description());
			
			if (cmd.annotation.street_address == null) {
				LOG.ERROR("Can not make geocoding request: street address is null");
				return;
			}
			
			StringBuffer url = new StringBuffer();
			url.append("http://maps.google.com/maps/geo?");
			url.append("q=" + URI.urlEncode(cmd.annotation.street_address));
			url.append("&output=csv&mobile=true&sensor=false");
			url.append("&key=" + mapkey);
			
			String finalUrl = url.toString();
			
			byte[] data = fetchData(finalUrl);
			String response = new String(data);
			
			// Parse response
			Vector res = split(response, ",");
			if (res.size() != 4) {
				LOG.ERROR("Geocoding response parse error. Response: " + response);
				return;
			}
			
			int statusCode = Integer.parseInt((String)res.elementAt(0));
			int accuracy = Integer.parseInt((String)res.elementAt(1));
			double latitude = Double.parseDouble((String)res.elementAt(2));
			double longitude = Double.parseDouble((String)res.elementAt(3));
			
			LOG.TRACE("Geocoding request done, pass results back");
			cmd.mapField.geocodingDone(cmd.annotation, statusCode, accuracy, latitude, longitude);
		}
		
		public void run() {
			try {
				while (active) {
					MapCommand cmd;
					synchronized (this) {
						processing = null;
						try {
							wait();
						} catch (InterruptedException e) {
							LOG.ERROR(e);
							continue;
						}
						if (processing == null)
							continue;
						
						cmd = processing;
						taskStartTime = System.currentTimeMillis();
					}
					
					try {
						if (cmd instanceof MapFetchCommand)
							processCommand((MapFetchCommand)cmd);
						else if (cmd instanceof MapGeocodingCommand)
							processCommand((MapGeocodingCommand)cmd);
						else
							LOG.INFO("Received unknown command: " + cmd.type() + ", ignore it");
					}
					catch (Exception e) {
						LOG.ERROR("Processing of map command failed", e);
					}
				}
			}
			finally {
				LOG.INFO("Map thread exit");
			}
		}
		
	};
	
	private static class MapThreadPool extends Thread {

		private Vector queue = new Vector();
		private Vector threads = new Vector();
		
		public MapThreadPool() {
			setPriority(Thread.MIN_PRIORITY);
			for (int i = 0; i != FETCH_THREAD_POOL_SIZE; ++i) {
				Thread th = new MapThread();
				th.setPriority(Thread.MIN_PRIORITY);
				th.start();
				threads.addElement(th);
			}
		}
		
		public void send(MapCommand cmd) {
			if (cmd == null)
				return;
			LOG.TRACE("Send command: " + cmd.description());
			synchronized (queue) {
				queue.addElement(cmd);
				queue.notify();
			}
		}

		public void run() {
			Vector cmds;
			Vector waiting = new Vector();
			for(;;) {
				cmds = waiting;
				waiting = new Vector();
				
				synchronized (queue) {
					try {
						if (cmds.isEmpty())
							queue.wait();
						else
							queue.wait(MAP_REDRAW_INTERVAL);
					}
					catch (InterruptedException e) {
						LOG.ERROR(e);
						continue;
					}
					
					Enumeration e = queue.elements();
					while (e.hasMoreElements())
						cmds.addElement(e.nextElement());
					queue.removeAllElements();
				}
				
				if (cmds.isEmpty())
					continue;
				
				// Handle them in reverse order to get more recent commands processed quickly
				for (int ix = cmds.size() - 1; ix >= 0; --ix) {
					MapCommand cmd = (MapCommand)cmds.elementAt(ix);
					LOG.TRACE("Look for thread to process command: " + cmd.description());
					
					boolean done = false;
					for (int i = 0, lim = threads.size(); !done && i < lim; ++i) {
						MapThread th = (MapThread)threads.elementAt(i);
						//LOG.TRACE("Receive fetch command #" + cmd.hashCode() + ", check if thread #" + th.hashCode() + " could process it...");
						synchronized (th) {
							if (th.isBusy()) {
								long maxTime = th.startTime() + MAX_FETCH_TIME;
								long curTime = System.currentTimeMillis();
								if (curTime > maxTime) {
									LOG.TRACE("Thread #" + th.hashCode() + " takes too much time to process command so interrupt it");
									th.stop();
									th = new MapThread();
									th.setPriority(Thread.MIN_PRIORITY);
									th.start();
									threads.setElementAt(th, i);
									th.process(cmd);
									done = true;
								}
								else if (th.isProcessing(cmd)) {
									//LOG.TRACE("Thread #" + th.hashCode() + " already processing given fetch command");
									done = true;
								}
								/*
								else
									LOG.TRACE("Thread #" + th.hashCode() + " is busy");
								*/
							}
							else {
								//LOG.TRACE("Thread #" + th.hashCode() + " is ready to process fetch command");
								th.process(cmd);
								done = true;
							}
						}
					}
					
					if (!done) {
						LOG.TRACE("No threads ready to fetch data, waiting...");
						waiting.addElement(cmd);
					}
					else {
						LOG.TRACE("Thread found, command transferred there");
					}
				}
				
				if (!waiting.isEmpty()) {
					// Reduce waiting queue size to WAITING_FETCH_COMMAND_MAX,
					// removing most outdated commands (from begin of queue)
					for (int i = 0, lim = waiting.size() - WAITING_FETCH_COMMAND_MAX; i < lim; ++i)
						waiting.removeElementAt(0);
					LOG.TRACE("Waiting commands queue: " + waiting.size() + " elements");
				}
			}
		}
		
	};
	
	private static MapThreadPool fetchThreadPool = null;
	
	public GoogleMapField() {
		if (fetchThreadPool == null) {
			fetchThreadPool = new MapThreadPool();
			fetchThreadPool.start();
		}
		
		maptype = "roadmap";
		annotations = new Vector();
	}
	
	public Field getBBField() {
		return this;
	}
	
	protected void layout(int w, int h) {
		width = Math.min(width, w);
		height = Math.min(height, h);
		setExtent(width, height);
	}
	
	private static String makeCacheKey(long lat, long lon, int z) {
		long x = lon/ADD_X;
		long y = lat/ADD_Y;
		StringBuffer buf = new StringBuffer();
		buf.append(z);
		buf.append(';');
		buf.append(x);
		buf.append(';');
		buf.append(y);
		String key = buf.toString();
		return key;
	}
	
	private static String makeCoordDescription(long lat, long lon, int z) {
		return Long.toString(lat) + ',' + Long.toString(lon) + ";zoom=" + Integer.toString(z);
	}
	
	protected void paint(Graphics graphics) {
		//graphics.clear();
		
		CachedBitmap img = null;
		synchronized (this) {
			img = image;
		}
		if (img == null) {
			VERBOSE_TRACE("PAINT: img: null");
			return;
		}
		
		// Draw map bitmap
		long left = 0;
		long top = 0;
		if (img.zoom == zoom) {
			//left = -(int)degreesToPixels(longitude - img.longitude, zoom);
			left = -toCurrentZoom(longitude - img.longitude, zoom);
			//top = (int)degreesToPixels(latitude - img.latitude, zoom);
			top = -toCurrentZoom(latitude - img.latitude, zoom);
		}
		VERBOSE_TRACE("PAINT: left: " + left);
		VERBOSE_TRACE("PAINT: top: " + top);
		
		int imgWidth = img.bitmap.getWidth();
		int imgHeight = img.bitmap.getHeight();
		left += (width - imgWidth)/2;
		top += (height - imgHeight)/2;
		
		int w = width - (int)left;
		int h = height - (int)top;
		
		VERBOSE_TRACE("PAINT: img: left: " + left + ", top: " + top + ", " + w + 'x' + h);
		graphics.drawBitmap((int)left, (int)top, w, h, img.bitmap, 0, 0);
		
		// Draw pointer at center
		graphics.setColor(0x00000000); // Black
		// Draw black cross
		int xCenter = width/2;
		int yCenter = height/2;
		int delta = 10;
		int yTop = yCenter - delta;
		int yBottom = yCenter + delta;
		int xLeft = xCenter - delta;
		int xRight = xCenter + delta;
		graphics.drawLine(xCenter, yTop, xCenter, yBottom);
		graphics.drawLine(xLeft, yCenter, xRight, yCenter);
		
		Annotation a = getCurrentAnnotation();
		if (a != null && a.url != null) {
			// Annotation found, so draw pointer
			//graphics.setColor(0x00FFFFFF); // Black
			for (delta = 3; delta <= 7; delta += 2)
				graphics.drawEllipse(xCenter, yCenter, xCenter + delta, yCenter, xCenter, yCenter + delta, 0, 360);
		}
	}
	
	public void redraw() {
		String key = makeCacheKey(latitude, longitude, zoom);
		VERBOSE_TRACE("REDRAW: key: " + key);
		MapFetchCommand cmd = null;
		synchronized (this) {
			CachedImage img = (CachedImage)cache.get(key);
			if (img == null) {
				long curTime = System.currentTimeMillis();
				if (curTime - lastFetchCommandSent > REDRAW_DELAY) {
					cmd = new MapFetchCommand(latitude, longitude, zoom,
							width, height, maptype, annotations, this);
					lastFetchCommandSent = curTime;
				}
			}
			else {
				img.lastUsed = System.currentTimeMillis();
				image = new CachedBitmap(img);
			}
		}
		VERBOSE_TRACE("REDRAW: image: " + (image == null ? "null" : "not null"));
		fetchThreadPool.send(cmd);
		VERBOSE_TRACE("REDRAW: invalidate");
		invalidate();
	}
	
	public void draw(long lat, long lon, int z, EncodedImage img) {
		VERBOSE_TRACE("DRAW: " + makeCoordDescription(lat, lon, z));
		CachedImage newImage = new CachedImage(img, lat, lon, z);
		String newKey = newImage.key;
		String curKey = image == null ? "" : image.key;
		VERBOSE_TRACE("DRAW: newkey: " + newKey);
		VERBOSE_TRACE("DRAW: curkey: " + curKey);
		synchronized (this) {
			cache.put(newKey, newImage);
			checkCache();
			if (!newKey.equals(curKey)) {
				image = new CachedBitmap(newImage);
			}
		}
		
		VERBOSE_TRACE("DRAW: invalidate");
		invalidate();
	}
	
	private static class CachedImageComparator implements Comparator {
		
		public int compare (Object o1, Object o2) {
			long l1 = ((CachedImage)o1).lastUsed;
			long l2 = ((CachedImage)o2).lastUsed;
			return l1 < l2 ? 1 : l1 > l2 ? -1 : 0;
		}
		
	};
	
	private void checkCache() {
		// TODO: implement more smart policy
		
		if (cache.size() <= MAX_CACHE_SIZE)
			return;
		
		SimpleSortingVector vec = new SimpleSortingVector();
		vec.setSort(true);
		vec.setSortComparator(new CachedImageComparator());
		
		Enumeration e = cache.elements();
		while (e.hasMoreElements())
			vec.addElement(e.nextElement());
		
		Hashtable newCache = new Hashtable();
		for (int i = 0; i < MAX_CACHE_SIZE; ++i) {
			CachedImage img = (CachedImage)vec.elementAt(i);
			newCache.put(img.key, img);
		}
		
		cache = newCache;
	}
	
	public int getPreferredWidth() {
		return width;
	}
	
	public int getPreferredHeight() {
		return height;
	}

	public void setPreferredSize(int w, int h) {
		width = w;
		height = h;
		setExtent(w, h);
	}
	
	private void validateCoordinates() {
		/*
		if (latitude < -90)
			latitude = -90;
		if (latitude > 90)
			latitude = 90;
		if (longitude < -180)
			longitude = -180;
		if (longitude > 180)
			longitude = 180;
		*/
		if (latitude < 0) latitude = 0;
		if (latitude > MAX_LATITUDE) latitude = MAX_LATITUDE;
		if (longitude < 0) longitude = 0;
		if (longitude > MAX_LONGITUDE) longitude = MAX_LONGITUDE;
	}
	
	private void validateZoom() {
		if (zoom < MIN_ZOOM)
			zoom = MIN_ZOOM;
		if (zoom > MAX_ZOOM)
			zoom = MAX_ZOOM;
	}
	
	private void scheduleFetch() {
		lastFetchCommandSent = System.currentTimeMillis();
		if (timer != null)
			timer.cancel();
		timer = new Timer();
		timer.schedule(new TimerTask() {
			public void run() {
				long curTime = System.currentTimeMillis();
				if (curTime - lastFetchCommandSent < REDRAW_DELAY)
					return;
				redraw();
			}}, REDRAW_DELAY);
	}

	public void moveTo(double lat, double lon) {
		//latitude = lat;
		latitude = normalize(90 - lat);
		//longitude = lon;
		longitude = normalize(lon + 180);
		validateCoordinates();
		
		scheduleFetch();
	}

	public void move(int dx, int dy) {
		//latitude -= pixelsToDegrees(dy, zoom);
		latitude += toMaxZoom(dy, zoom);
		//longitude += pixelsToDegrees(dx, zoom);
		longitude += toMaxZoom(dx, zoom);
		validateCoordinates();
		
		scheduleFetch();
	}

	public int getMaxZoom() {
		return MAX_ZOOM;
	}

	public int getMinZoom() {
		return MIN_ZOOM;
	}

	public int getZoom() {
		return zoom;
	}

	public void setZoom(int z) {
		int prevZoom = zoom;
		zoom = z;
		validateZoom();
		lastFetchCommandSent = 0;
		if (image != null && image.image != null) {
			double x = MathEx.pow(2, prevZoom - zoom);
			int factor = Fixed32.tenThouToFP((int)(x*10000));
			//factor = Fixed32.mul(factor, image.image.getScaleX32());
			image.image = image.image.scaleImage32(factor, factor);
			image.bitmap = image.image.getBitmap();
		}
	}
	
	public int calculateZoom(double latDelta, double lonDelta) {
		int zoom1 = calcZoom(latDelta, width);
		int zoom2 = calcZoom(lonDelta, height);
		return zoom1 < zoom2 ? zoom1 : zoom2;
	}
	
	public void setMapType(String type) {
		maptype = type;
		lastFetchCommandSent = 0;
	}
	
	public void addAnnotation(Annotation ann) {
		if (ann.coordinates != null) {
			long nlat = normalize(90 - ann.coordinates.latitude);
			long nlon = normalize(180 + ann.coordinates.longitude);
			ann.normalized_coordinates = new Annotation.NormalizedCoordinates(nlat, nlon);
		}
		annotations.addElement(ann);
		if (ann.coordinates == null)
			fetchThreadPool.send(new MapGeocodingCommand(ann, this));
	}
	
	private void geocodingDone(Annotation ann, int statusCode,
			int accuracy, double lat, double lon) {
		
		LOG.TRACE("Apply geocoding result: " + statusCode + "," + accuracy +
				"," + lat + "," + lon);
		if (statusCode/100 != 2) {
			LOG.ERROR("Error received from geocoding service: " + statusCode);
			return;
		}
		ann.coordinates = new Annotation.Coordinates(lat, lon);
		long nlat = normalize(90.0 - lat);
		long nlon = normalize(180.0 + lon);
		ann.normalized_coordinates = new Annotation.NormalizedCoordinates(nlat, nlon);
	}
	
	private static int calcZoom(double degrees, int pixels) {
		double angleRatio = degrees*TILE_SIZE/pixels;
		
		double twoInZoomExp = 360/angleRatio;
		int zoom = (int)MathEx.log2(twoInZoomExp);
		return zoom;
	}
	
	private static long normalize(double coord) {
		return degreesToPixels(coord, MAX_ZOOM);
	}
	
	private static long toMaxZoom(long n, int zoom) {
		return n == 0 ? 0 : n*(long)MathEx.pow(2, MAX_ZOOM - zoom);
	}
	
	private static long toCurrentZoom(long coord, int zoom) {
		if (coord == 0) return 0;
		long pow = (long)MathEx.pow(2, MAX_ZOOM - zoom);
		return coord/pow;
	}

	private static long degreesToPixels(double n, int z) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/MathEx.pow(2, z);
		double val = n*TILE_SIZE/angleRatio;
		return (long)val;
	}
	
	private static double pixelsToDegrees(long n, int z) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/MathEx.pow(2, z);
		double val = n*angleRatio/TILE_SIZE;
		return val;
	}
	
	private static Vector split(String s, String delimiter) {
		Vector res = new Vector();
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
	
	private Annotation getCurrentAnnotation() {
		// return current annotation (point we are under now)
		Enumeration e = annotations.elements();
		while (e.hasMoreElements()) {
			Annotation a = (Annotation)e.nextElement();
			Annotation.NormalizedCoordinates coords = a.normalized_coordinates;
			if (coords == null)
				continue;
			
			//long deltaY = degreesToPixels(latitude - a.coordinates.latitude, zoom) - SENSIVITY_DELTA/2;
			long deltaY = toCurrentZoom(latitude - coords.latitude, zoom) + SENSIVITY_DELTA/2;
			//long deltaX = degreesToPixels(longitude - a.coordinates.longitude, zoom);
			long deltaX = toCurrentZoom(longitude - coords.longitude, zoom);
			
			double distance = MathEx.pow(MathEx.pow(deltaX, 2) + MathEx.pow(deltaY, 2), 0.5);
			if ((int)distance > SENSIVITY_DELTA)
				continue;
			
			return a;
		}
		return null;
	}
	
	public boolean handleClick() {
		Annotation a = getCurrentAnnotation();
		if (a == null || a.url == null)
			return false;
		
		needToCloseMap = true;
		WebView.navigate(ObjectFactory.createString(a.url));
		return true;
	}
}
