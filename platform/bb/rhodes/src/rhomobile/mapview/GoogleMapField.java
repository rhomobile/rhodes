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
	
	private static final int FETCH_THREAD_POOL_SIZE = 5;
	private static final long MAX_FETCH_TIME = 10000; // milliseconds
	
	private static final int MAP_REDRAW_INTERVAL = 500; // milliseconds
	
	// Static google parameters
	private static final int MIN_ZOOM = 0;
	private static final int MAX_ZOOM = 19;
	
	private static final int TILE_SIZE = 256;
	
	private static final String[] MAP_TYPES = {"roadmap", "satellite", "terrain", "hybrid"};
	public static final int MAP_TYPE_ROADMAP = 0;
	public static final int MAP_TYPE_SATELLITE = 1;
	public static final int MAP_TYPE_TERRAIN = 2;
	public static final int MAP_TYPE_HYBRID = 3;
	
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
		
		public void process(MapFetchCommand cmd) {
			synchronized (this) {
				processing = cmd;
				notify();
			}
		}
		
		public void run() {
			try {
				for(;;) {
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
						
						taskStartTime = System.currentTimeMillis();
					}
					
					MapFetchCommand cmd = processing;
					LOG.TRACE("Processing map fetch command: " + cmd.latitude + "," + cmd.longitude + ";zoom=" + cmd.zoom);
					
					// Process received command
					StringBuffer url = new StringBuffer();
					url.append("http://maps.google.com/maps/api/staticmap?");
					url.append("center=" + cmd.latitude + "," + cmd.longitude + "&");
					url.append("zoom=" + cmd.zoom + "&");
					url.append("size=" + (cmd.width + DELTA_X*2) + "x" + (cmd.height + DELTA_Y*2) + "&");
					url.append("maptype=" + cmd.maptype + "&");
					url.append("format=png&");
					url.append("key=" + mapkey + "&");
					url.append("mobile=true&sensor=true");
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
			synchronized (queue) {
				queue.addElement(cmd);
				queue.notify();
			}
		}

		public void run() {
			Vector cmds = new Vector();
			Vector waiting = new Vector();
			for(;;) {
				cmds.removeAllElements();
				for (Enumeration e = waiting.elements(); e.hasMoreElements();)
					cmds.addElement(e.nextElement());
				waiting.removeAllElements();
				
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
					if (mf != null)
						mf.redraw();
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
							synchronized (th) {
								if (th.isBusy()) {
									if (th.startTime() > System.currentTimeMillis() + MAX_FETCH_TIME)
										th.interrupt();
									continue;
								}
							}
							
							th.process(cmd);
							done = true;
						}
					}
					
					if (done)
						continue;
					
					waiting.addElement(cmd);
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
		
		maptype = MAP_TYPES[MAP_TYPE_ROADMAP];
	}
	
	public Field getBBField() {
		return this;
	}
	
	protected void layout(int w, int h) {
		width = Math.min(width, w);
		height = Math.min(height, h);
		setExtent(width, height);
	}
	
	private String makeCacheKey(double lat, double lon, int z) {
		long x = (degreesToPixels(lon + 180, z)/DELTA_X)*DELTA_X + DELTA_X;
		long y = (degreesToPixels(90 - lat, z)/DELTA_Y)*DELTA_Y + DELTA_Y;
		return zoom + ";" + x + ";" + y;
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
		left -= DELTA_X;
		top -= DELTA_Y;
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
		String key = makeCacheKey(lat, lon, z);
		String curKey = makeCacheKey(latitude, longitude, zoom);
		CachedImage newImage = new CachedImage(img, lat, lon, z);
		synchronized (this) {
			cache.put(key, newImage);
			if (key == curKey) {
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
		zoom = z;
		validateZoom();
		redraw();
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

	private static long degreesToPixels(double n, int zoom) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/pow(2, zoom);
		
		return (long)(n*TILE_SIZE/angleRatio);
	}
	
	private static double pixelsToDegrees(long n, int zoom) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/pow(2, zoom);
		
		return n*angleRatio/TILE_SIZE;
	}
}
