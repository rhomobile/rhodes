package rhomobile.mapview;

import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.net.IHttpConnection;

import rhomobile.mapview.RhoMapField;
import net.rim.device.api.math.Fixed32;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Graphics;

public class GoogleMapField extends Field implements RhoMapField {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GoogleMapField");
	
	// Configurable parameters
	private static final int DELTA_X = 100;
	private static final int DELTA_Y = 100;
	
	private static final int FETCH_THREAD_POOL_SIZE = 4;
	private static final long MAX_FETCH_TIME = 10000; // milliseconds
	
	private static final int MAP_REDRAW_INTERVAL = 1000; // milliseconds
	
	// Static google parameters
	private static final int MIN_ZOOM = 0;
	private static final int MAX_ZOOM = 19;
	
	private static final int TILE_SIZE = 256;
	
	// Coordinates of center
	private double latitude;
	private double longitude;
	private int zoom;
	
	private int width;
	private int height;
	private String maptype;
	
	private static class CachedImage {
		public EncodedImage image;
		public double latitude;
		public double longitude;
		public int zoom;
		
		public CachedImage(EncodedImage img, double lat, double lon, int z) {
			image = img;
			latitude = lat;
			longitude = lon;
			zoom = z;
		}
	};
	
	private static class CachedBitmap extends CachedImage {
		public Bitmap bitmap;
		
		public CachedBitmap(CachedImage img) {
			super(img.image, img.latitude, img.longitude, img.zoom);
			bitmap = img.image.getBitmap();
		}
		
		public CachedBitmap(EncodedImage img, double lat, double lon, int z) {
			super(img, lat, lon, z);
			bitmap = img.getBitmap();
		}
	};

	private Hashtable cache = new Hashtable();
	private CachedBitmap image = null;
	
	private static class MapFetchCommand {
		
		public double latitude;
		public double longitude;
		public int zoom;
		public int width;
		public int height;
		public String maptype;
		public GoogleMapField mapField;
		
		public MapFetchCommand(double lat, double lon, int z, int w, int h,
				String m, GoogleMapField mf) {
			latitude = lat;
			longitude = lon;
			zoom = z;
			width = w;
			height = h;
			maptype = m;
			mapField = mf;
		}
		
		public boolean equals(MapFetchCommand cmd) {
			return latitude == cmd.latitude && longitude == cmd.longitude &&
				zoom == cmd.zoom && width == cmd.width && maptype.equals(cmd.maptype) &&
				mapField == cmd.mapField;
		}
		
	}
	
	private static class MapFetchThread extends Thread {

		private static final String mapkey = "ABQIAAAA-X8Mm7F-7Nmz820lFEBHYxT2yXp_ZAY8_ufC3CFXhHIE1NvwkxSfNPZbryNEPHF-5PQKi9c7Fbdf-A";
		
		private MapFetchCommand processing = null;
		private long taskStartTime;
		
		// Return time of start current task. Return undefined value if no current task exists.
		public long startTime() {
			return taskStartTime;
		}
		
		public boolean isBusy() {
			return processing != null;
		}
		
		public boolean isProcessing(MapFetchCommand cmd) {
			return processing == null ? false : processing.equals(cmd);
		}
		
		public void process(MapFetchCommand cmd) {
			processing = cmd;
			notify();
		}
		
		public void run() {
			try {
				for(;;) {
					MapFetchCommand cmd;
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
					
					LOG.TRACE("Processing map fetch command (thread #" + hashCode() + "): " + cmd.latitude + "," + cmd.longitude + ";zoom=" + cmd.zoom);
					
					// Process received command
					StringBuffer url = new StringBuffer();
					url.append("http://maps.google.com/maps/api/staticmap?");
					url.append("center=" + cmd.latitude + "," + cmd.longitude + "&");
					url.append("zoom=" + cmd.zoom + "&");
					url.append("size=" + (cmd.width + DELTA_X*2) + "x" + (cmd.height + DELTA_Y*2) + "&");
					url.append("maptype=" + cmd.maptype + "&");
					url.append("format=png&");
					url.append("key=" + mapkey + "&");
					url.append("mobile=false&sensor=true");
					String finalUrl = url.toString();
					
					int size = 0;
					byte[] data = null;
					try {
						IHttpConnection conn = RhoClassFactory.getNetworkAccess().connect(finalUrl);
						
						conn.setRequestMethod("GET");
						
						InputStream is = conn.openInputStream();
						
						int code = conn.getResponseCode();
						if (code/100 != 2)
							throw new IOException("Google map respond with " + code + " " + conn.getResponseMessage());
						
						size = conn.getHeaderFieldInt("Content-Length", 0);
						data = new byte[size];
						for (int offset = 0; offset < size;) {
							int n = is.read(data, offset, size - offset);
							if (n < 0)
								break;
							offset += n;
						}
					}
					catch (IOException e) {
						LOG.ERROR("Cannot fetch map", e);
						continue;
					}
					
					EncodedImage img = null;
					try {
						img = EncodedImage.createEncodedImage(data, 0, size);
					}
					catch (Exception e) {
						LOG.ERROR("Cannot create EncodedImage from fetched data", e);
						continue;
					}
				
					LOG.TRACE("Map request done, draw just received image");
					cmd.mapField.draw(cmd.latitude, cmd.longitude, cmd.zoom, img);
				}
			}
			finally {
				LOG.INFO("Map fetch thread exit");
			}
		}
		
	};
	
	private static class MapFetchThreadPool extends Thread {

		private Vector queue = new Vector();
		private Vector threads = new Vector();
		private Hashtable mapfields = new Hashtable();
		
		public MapFetchThreadPool() {
			for (int i = 0; i != FETCH_THREAD_POOL_SIZE; ++i) {
				Thread th = new MapFetchThread();
				th.start();
				threads.addElement(th);
			}
		}
		
		public void send(MapFetchCommand cmd) {
			if (cmd == null)
				return;
			//LOG.TRACE("Send fetch command #" + cmd.hashCode());
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
				
				for (Enumeration e = mapfields.elements(); e.hasMoreElements();) {
					WeakReference ref = (WeakReference)e.nextElement();
					GoogleMapField mf = (GoogleMapField)ref.get();
					if (mf != null) {
						//LOG.TRACE("Redraw mapfield #" + mf.hashCode());
						mf.redraw();
					}
				}
				
				if (cmds.isEmpty())
					continue;
				
				for (Enumeration e = cmds.elements(); e.hasMoreElements(); ) {
					MapFetchCommand cmd = (MapFetchCommand)e.nextElement();
					mapfields.put(new Integer(cmd.mapField.hashCode()), new WeakReference(cmd.mapField));
					
					boolean done = false;
					synchronized (threads) {
						for (Enumeration et = threads.elements(); !done && et.hasMoreElements(); ) {
							MapFetchThread th = (MapFetchThread)et.nextElement();
							//LOG.TRACE("Receive fetch command #" + cmd.hashCode() + ", check if thread #" + th.hashCode() + " could process it...");
							synchronized (th) {
								if (th.isBusy()) {
									if (th.startTime() > System.currentTimeMillis() + MAX_FETCH_TIME) {
										LOG.TRACE("Thread #" + th.hashCode() + " takes too much time to process command so interrupt it");
										th.interrupt();
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
					}
					
					if (!done) {
						LOG.TRACE("No threads ready to fetch data (waiting queue contains " + waiting.size() + " elements)");
						waiting.addElement(cmd);
					}
				}
			}
		}
		
	};
	
	private static MapFetchThreadPool fetchThreadPool = null;
	
	public GoogleMapField() {
		if (fetchThreadPool == null) {
			fetchThreadPool = new MapFetchThreadPool();
			fetchThreadPool.start();
		}
		
		maptype = "roadmap";
	}
	
	public Field getBBField() {
		return this;
	}
	
	protected void layout(int w, int h) {
		width = Math.min(width, w);
		height = Math.min(height, h);
		setExtent(width, height);
	}
	
	private static String makeCacheKey(double lat, double lon, int z) {
		long x = degreesToPixels(lon + 180, z)/DELTA_X;
		long y = degreesToPixels(90 - lat, z)/DELTA_Y;
		StringBuffer buf = new StringBuffer();
		buf.append(z);
		buf.append(';');
		buf.append(x);
		buf.append(';');
		buf.append(y);
		String key = buf.toString();
		return key;
	}
	
	protected void paint(Graphics graphics) {
		graphics.clear();
		
		CachedBitmap img = null;
		synchronized (this) {
			img = image;
		}
		if (img == null)
			return;
		
		int left = 0;
		int top = 0;
		if (img.zoom == zoom) {
			left = -(int)degreesToPixels(longitude - img.longitude, zoom);
			top = (int)degreesToPixels(latitude - img.latitude, zoom);
		}
		
		left += (width - img.image.getScaledWidth())/2;
		top += (height - img.image.getScaledHeight())/2;
		graphics.drawBitmap(left, top, width - left, height - top, img.bitmap, 0, 0);
	}
	
	public void redraw() {
		String key = makeCacheKey(latitude, longitude, zoom);
		MapFetchCommand cmd = null;
		synchronized (this) {
			CachedImage img = (CachedImage)cache.get(key);
			if (img == null) {
				cmd = new MapFetchCommand(latitude, longitude, zoom,
						width, height, maptype, this);
			}
			else
				image = new CachedBitmap(img);
		}
		fetchThreadPool.send(cmd);
		invalidate();
	}
	
	public void draw(double lat, double lon, int z, EncodedImage img) {
		String newKey = makeCacheKey(lat, lon, z);
		String curKey = makeCacheKey(latitude, longitude, zoom);
		LOG.TRACE("DRAW: lat: " + lat + ", lon: " + lon + ", z: " + z + ", NEWKEY: " + newKey);
		LOG.TRACE("DRAW: lat: " + latitude + ", lon: " + longitude + ", z: " + zoom + ", CURKEY: " + curKey);
		CachedImage newImage = new CachedImage(img, lat, lon, z);
		synchronized (this) {
			cache.put(newKey, newImage);
			if (newKey.equals(curKey)) {
				image = new CachedBitmap(newImage);
			}
		}
		invalidate();
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
		if (latitude < -90)
			latitude = -90;
		if (latitude > 90)
			latitude = 90;
		if (longitude < -180)
			longitude = -180;
		if (longitude > 180)
			longitude = 180;
	}
	
	private void validateZoom() {
		if (zoom < MIN_ZOOM)
			zoom = MIN_ZOOM;
		if (zoom > MAX_ZOOM)
			zoom = MAX_ZOOM;
	}

	public void moveTo(double lat, double lon) {
		latitude = lat;
		longitude = lon;
		validateCoordinates();
		redraw();
	}

	public void move(int dx, int dy) {
		latitude -= pixelsToDegrees(dy, zoom);
		longitude += pixelsToDegrees(dx, zoom);
		validateCoordinates();
		redraw();
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
		if (image != null && image.image != null) {
			double x = pow(2, prevZoom - zoom);
			int factor = Fixed32.tenThouToFP((int)(x*10000));
			image.image = image.image.scaleImage32(factor, factor);
			image.bitmap = image.image.getBitmap();
		}
		redraw();
	}
	
	public int calculateZoom(double latDelta, double lonDelta) {
		int zoom1 = calcZoom(latDelta, width);
		int zoom2 = calcZoom(lonDelta, height);
		return zoom1 < zoom2 ? zoom1 : zoom2;
	}
	
	public void setMapType(String type) {
		maptype = type;
	}
	
	private static double pow(double val, int pow) {
		double result = 1.0;
		if (pow < 0) {
			for (int i = 0; i != pow; --i) {
				result /= val;
			}
		}
		else {
			for (int i = 0; i != pow; ++i) {
				result *= val;
			}
		}
		return result;
	}
	
	private static int calcZoom(double degrees, int pixels) {
		double angleRatio = degrees*TILE_SIZE/pixels;
		
		double twoInZoomExp = 360/angleRatio;
		// Calculate logarithm
		int zoom = 0;
		while (twoInZoomExp > 1) {
			twoInZoomExp /= 2;
			++zoom;
		}
		return zoom;
	}

	private static long degreesToPixels(double n, int z) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/pow(2, z);
		double val = n*TILE_SIZE/angleRatio;
		return (long)val;
	}
	
	private static double pixelsToDegrees(long n, int z) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/pow(2, z);
		double val = n*angleRatio/TILE_SIZE;
		return val;
	}
}
