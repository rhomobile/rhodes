package rhomobile.mapview;

import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;

import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.net.IHttpConnection;

import rhomobile.mapview.RhoMapField;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Graphics;
import net.rim.device.api.ui.UiApplication;

public class GoogleMapField extends Field implements RhoMapField {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GoogleMapField");
	
	private static final int MIN_ZOOM = 0;
	private static final int MAX_ZOOM = 19;
	
	private static final int TILE_SIZE = 256;
	
	private static final String[] MAP_TYPES = {"roadmap", "satellite", "terrain", "hybrid"};
	public static final int MAP_TYPE_ROADMAP = 0;
	public static final int MAP_TYPE_SATELLITE = 1;
	public static final int MAP_TYPE_TERRAIN = 2;
	public static final int MAP_TYPE_HYBRID = 3;
	
	private double prevLat;
	private double prevLon;
	private int prevZoom;
	
	private double latitude;
	private double longitude;
	private int currentZoom;
	
	private int width;
	private int height;
	private String maptype;
	
	private EncodedImage image = null;
	
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
		
		public void send(MapFetchCommand cmd) {
			synchronized (queue) {
				queue.addElement(cmd);
				queue.notify();
			}
		}
		
		public void run() {
			try {
				Vector cmds = new Vector();
				for(;;) {
					cmds.removeAllElements();
					synchronized (queue) {
						queue.wait();
						if (queue.size() == 0)
							break;
						
						while (!queue.isEmpty()) {
							cmds.addElement(queue.elementAt(0));
							queue.removeElementAt(0);
						}
					}
					
					for (int i = 0, lim = cmds.size(); i != lim; ++i) {
						MapFetchCommand cmd = (MapFetchCommand)cmds.elementAt(i);
						
						// Process received command
						StringBuffer url = new StringBuffer();
						url.append("http://maps.google.com/maps/api/staticmap?");
						url.append("center=" + cmd.latitude + "," + cmd.longitude + "&");
						url.append("zoom=" + cmd.zoom + "&");
						url.append("size=" + cmd.width + "x" + cmd.height + "&");
						url.append("maptype=" + cmd.maptype + "&");
						url.append("format=png&");
						url.append("key=" + mapkey + "&");
						url.append("mobile=true&sensor=true");
						String finalUrl = url.toString();
						
						IHttpConnection conn = RhoClassFactory.getNetworkAccess().connect(finalUrl);
						
						conn.setRequestMethod("GET");
						
						InputStream is = conn.openInputStream();
						
						int code = conn.getResponseCode();
						String msg = conn.getResponseMessage();
						LOG.TRACE("Google map respond with " + code + " " + msg);
						
						int size = conn.getHeaderFieldInt("Content-Length", 0);
						byte[] data = new byte[size];
						for (int offset = 0; offset < size;) {
							int n = is.read(data, offset, size - offset);
							if (n <= 0)
								break;
							offset += n;
						}
						
						EncodedImage img = EncodedImage.createEncodedImage(data, 0, size);
						
						cmd.mapField.draw(img);
					}
				}
			} catch (InterruptedException e) {
				LOG.ERROR(e);
			} catch (IOException e) {
				LOG.ERROR("Can not request map", e);
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
	
	protected void paint(Graphics graphics) {
		graphics.clear();
		
		EncodedImage img = image;
		if (img == null)
			return;
		
		int left = 0;
		int top = 0;
		if (prevZoom == currentZoom) {
			left = -degreesToPixels(longitude - prevLon);
			top = degreesToPixels(latitude - prevLat);
		}
		graphics.drawImage(left, top, width - left, height - top, img, 0, 0, 0);
	}
	
	public void redraw() {
		MapFetchCommand cmd = new MapFetchCommand(latitude, longitude, currentZoom,
				width, height, maptype, this);
		fetchThreadObj.send(cmd);
		invalidate();
	}
	
	public void draw(EncodedImage img) {
		synchronized (UiApplication.getEventLock()) {
			prevLat = latitude;
			prevLon = longitude;
			prevZoom = currentZoom;
			image = img;
			invalidate();
		}
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
		if (currentZoom < MIN_ZOOM)
			currentZoom = MIN_ZOOM;
		if (currentZoom > MAX_ZOOM)
			currentZoom = MAX_ZOOM;
	}

	public void moveTo(double lat, double lon) {
		prevLat = latitude;
		prevLon = longitude;
		latitude = lat;
		longitude = lon;
		validateCoordinates();
		redraw();
	}

	public void move(int dx, int dy) {
		prevLat = latitude;
		prevLon = longitude;
		latitude -= pixelsToDegrees(dy);
		longitude += pixelsToDegrees(dx);
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
		return currentZoom;
	}

	public void setZoom(int zoom) {
		prevZoom = currentZoom;
		currentZoom = zoom;
		validateZoom();
		redraw();
	}
	
	private double pow(double val, int pow) {
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

	private int degreesToPixels(double n) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/pow(2, currentZoom);
		double pixelRatio = angleRatio/TILE_SIZE;
		
		return (int)(n/pixelRatio);
	}
	
	private double pixelsToDegrees(int n) {
		if (n == 0)
			return 0;
		
		double angleRatio = 360/pow(2, currentZoom);
		double pixelRatio = angleRatio/TILE_SIZE;
		
		return n*pixelRatio;
	}
}
