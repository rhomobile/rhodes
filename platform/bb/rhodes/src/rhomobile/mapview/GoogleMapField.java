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
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Graphics;

public class GoogleMapField extends Field implements RhoMapField {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GoogleMapField");
	
	private static final int MIN_ZOOM = 0;
	private static final int MAX_ZOOM = 19;
	
	private static final int TILE_SIZE = 256;
	
	private static final int DELTA_X = 70;
	private static final int DELTA_Y = 70;
	
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

	private Hashtable cache = new Hashtable();
	private CachedImage image = null;
	
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
	
	private static class MapFetchThread implements Runnable {

		private static final String mapkey = "ABQIAAAA-X8Mm7F-7Nmz820lFEBHYxQCTQSfxzjC-OXUcQUqlrjciHyFgxRH16HemsZX8ld50tKtL7bXExL13g";
		private Vector queue = new Vector();
		private Hashtable mapfields = new Hashtable();
		
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
			for(;;) {
				cmds.removeAllElements();
				synchronized (queue) {
					try {
						queue.wait(500);
					} catch (InterruptedException e) {
						LOG.ERROR(e);
						continue;
					}
					if (queue.size() == 0) {
						// Redraw all known mapfields
						Enumeration e = mapfields.elements();
						while (e.hasMoreElements()) {
							WeakReference ref = (WeakReference)e.nextElement();
							GoogleMapField mf = (GoogleMapField)ref.get();
							if (mf == null)
								continue;
							
							mf.redraw();
						}
						continue;
					}
					
					while (!queue.isEmpty()) {
						cmds.addElement(queue.elementAt(0));
						queue.removeElementAt(0);
					}
				}
				
				for (int i = 0, lim = cmds.size(); i != lim; ++i) {
					MapFetchCommand cmd = (MapFetchCommand)cmds.elementAt(i);
					if (cmd == null)
						continue;
					
					mapfields.put(new Integer(cmd.mapField.hashCode()), new WeakReference(cmd.mapField));
					
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
						String msg = conn.getResponseMessage();
						LOG.TRACE("Google map respond with " + code + " " + msg);
						
						size = conn.getHeaderFieldInt("Content-Length", 0);
						data = new byte[size];
						for (int offset = 0; offset < size;) {
							int n = is.read(data, offset, size - offset);
							if (n <= 0)
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
		}
		
	};
	private static MapFetchThread fetchThreadObj = null;
	
	public GoogleMapField() {
		if (fetchThreadObj == null) {
			fetchThreadObj = new MapFetchThread();
			Thread fetchThread = new Thread(fetchThreadObj);
			fetchThread.start();
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
		long x = (degreesToPixels(lon + 180, z)/DELTA_X) * DELTA_X;
		long y = (degreesToPixels(90 - lat, z)/DELTA_Y) * DELTA_Y;
		return zoom + ";" + x + ";" + y;
	}
	
	protected void paint(Graphics graphics) {
		graphics.clear();
		
		CachedImage img = null;
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
		graphics.drawImage(left, top, width - left, height - top, img.image, 0, 0, 0);
	}
	
	public void redraw() {
		String key = makeCacheKey(latitude, longitude, zoom);
		MapFetchCommand cmd = null;
		synchronized (this) {
			CachedImage img = (CachedImage)cache.get(key);
			if (img == null)
				cmd = new MapFetchCommand(latitude, longitude, zoom,
						width, height, maptype, this);
			else
				image = img;
		}
		fetchThreadObj.send(cmd);
		invalidate();
	}
	
	public void draw(double lat, double lon, int z, EncodedImage img) {
		String key = makeCacheKey(lat, lon, z);
		String curKey = makeCacheKey(latitude, longitude, zoom);
		CachedImage newImage = new CachedImage(img, lat, lon, z);
		synchronized (this) {
			cache.put(key, newImage);
			if (key == curKey)
				image = newImage;
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
		double pixelRatio = angleRatio/TILE_SIZE;
		
		return (int)(n/pixelRatio);
	}
	
	private static double pixelsToDegrees(long n, int zoom) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/pow(2, zoom);
		double pixelRatio = angleRatio/TILE_SIZE;
		
		return n*pixelRatio;
	}
}
