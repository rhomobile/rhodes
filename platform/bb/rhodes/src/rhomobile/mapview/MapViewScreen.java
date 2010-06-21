package rhomobile.mapview;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

import net.rim.device.api.system.Application;
import net.rim.device.api.system.Display;
import net.rim.device.api.system.KeypadListener;
import net.rim.device.api.ui.Graphics;
import net.rim.device.api.ui.MenuItem;
import net.rim.device.api.ui.container.MainScreen;

public class MapViewScreen extends MainScreen {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("MapViewScreen");
	
	private static final int PAN_MODE = 1;
	private static final int ZOOM_MODE = 2;
	
	private static final int MIN_MOVE_STEP = 1;
	private static final int MAX_MOVE_STEP = 8;
	
	private static final int MOVE_TIMEOUT_DOUBLING = 300;
	
	private static final MapProvider[] providers = {
		//new BBMapProvider(),
		new GoogleMapProvider()
	};
	
	private MapViewParent parent;
	private MapProvider mapProvider;
	private RhoMapField mapField;
	
	private int mode;
	
	private long prevMoveTime = 0;
	private int prevDx = 0;
	private int prevDy = 0;
	
	private class PanModeMenuItem extends MenuItem {
		
		private MapViewScreen screen;
		
		public PanModeMenuItem(MapViewScreen scr, int ordinal, int priority) {
			super("Pan mode", ordinal, priority);
			screen = scr;
		}
		
		public void run() {
			screen.setMode(MapViewScreen.PAN_MODE);
		}
	};
	
	private class ZoomModeMenuItem extends MenuItem {
		
		private MapViewScreen screen;
		
		public ZoomModeMenuItem(MapViewScreen scr, int ordinal, int priority) {
			super("Zoom mode", ordinal, priority);
			screen = scr;
		}
		
		public void run() {
			screen.setMode(MapViewScreen.ZOOM_MODE);
		}
	};
	
	MapViewScreen(MapViewParent p, String providerName, Hashtable settings, Vector annotations) {
		super(DEFAULT_MENU | DEFAULT_CLOSE);
		addMenuItem(new PanModeMenuItem(this, 0, 100));
		addMenuItem(new ZoomModeMenuItem(this, 1, 100));
		parent = p;
		createMapProvider(providerName);
		createUI(settings, annotations);
	}
	
	public void close() {
		mapField.close();
		super.close();
		parent.childClosed();
	}
	
	private void setMode(int m) {
		mode = m;
		mapField.redraw();
	}
	
	private void createMapProvider(String providerName) {
		mapProvider = null;
		for (int i = 0; i != providers.length; ++i) {
			if (providers[i].accept(providerName)) {
				mapProvider = providers[i];
				break;
			}
		}
		if (mapProvider == null)
			throw new IllegalArgumentException("Unknown map provider: " + providerName);
	}
	
	private void createUI(Hashtable settings, Vector annotations) {
		synchronized (Application.getEventLock()) {
			mapField = mapProvider.createMap();
			mapField.setPreferredSize(Display.getWidth(), Display.getHeight());
			add(mapField.getBBField());
		}
		
		// Set map type
		String map_type = (String)settings.get("map_type");
		if (map_type == null)
			map_type = "roadmap";
		mapField.setMapType(map_type);

		Hashtable region = (Hashtable)settings.get("region");
		if (region != null) {
			// Set coordinates
			Double lat = (Double)region.get("latitude");
			Double lon = (Double)region.get("longitude");
			if (lat != null && lon != null)
				mapField.moveTo(lat.doubleValue(), lon.doubleValue());
			
			// Set zoom
			Double latDelta = (Double)region.get("latDelta");
			Double lonDelta = (Double)region.get("lonDelta");
			if (latDelta != null && lonDelta != null) {
				int zoom = mapField.calculateZoom(latDelta.doubleValue(), lonDelta.doubleValue());
				mapField.setZoom(zoom);
			}
		}
		
		Double radius = (Double)settings.get("radius");
		if (radius != null) {
			int zoom = mapField.calculateZoom(radius.doubleValue(), radius.doubleValue());
			mapField.setZoom(zoom);
		}
		
		// Annotations
		Enumeration e = annotations.elements();
		while (e.hasMoreElements()) {
			Annotation ann = (Annotation)e.nextElement();
			if (ann != null)
				mapField.addAnnotation(ann);
		}
		
		mode = PAN_MODE;
		
		mapField.redraw();
	}
	
	/**
	 * Handle trackball click events.
	 * @see net.rim.device.api.ui.Screen#invokeAction(int)
	 */   
	protected boolean invokeAction(int action)
	{
		boolean handled = super.invokeAction(action);

		if(!handled)
		{
			switch(action)
			{
    			case ACTION_INVOKE: // Trackball click.
    			{
    				return true;
    			}
			}
		}
		return handled;
	}
	
	protected void paint(Graphics graphics) {
		super.paint(graphics);
		
		// TODO: draw overlays here
		graphics.setColor(0x00000000); // Black
		
		// Draw current mode
		String strMode  = null;
		if (mode == PAN_MODE)
			strMode = "Pan mode";
		else if (mode == ZOOM_MODE)
			strMode = "Zoom mode";
		
		if (strMode != null) {
			// Detect drawn text width
			int x = mapField.getLeft() + mapField.getWidth()/2;
			int y = mapField.getTop() + mapField.getHeight() + 20;
			int tw = graphics.drawText(strMode, x, y);
			// Actual drawing
			x -= tw/2;
			y -= 60;
			tw = graphics.drawText(strMode, x, y);
		}
	}
	
	private int calcDxSmooth(int dx, long curTime) {
		int newDx;
		if (curTime > prevMoveTime + MOVE_TIMEOUT_DOUBLING) {
			newDx = dx;
		}
		else {
			if (dx == 0)
				newDx = 0;
			else {
				newDx = dx < 0 ? (prevDx < 0 ? prevDx*2 : -MIN_MOVE_STEP) : (prevDx > 0 ? prevDx*2 : MIN_MOVE_STEP);
				if (newDx < -MAX_MOVE_STEP)
					newDx = -MAX_MOVE_STEP;
				else if (newDx > MAX_MOVE_STEP)
					newDx = MAX_MOVE_STEP;
			}
		}
		prevDx = newDx;
		return newDx;
	}
	
	private int calcDySmooth(int dy, long curTime) {
		int newDy;
		if (curTime > prevMoveTime + MOVE_TIMEOUT_DOUBLING) {
			newDy = dy;
		}
		else {
			if (dy == 0)
				newDy = 0;
			else {
				newDy = dy < 0 ? (prevDy < 0 ? prevDy*2 : -MIN_MOVE_STEP) : (prevDy > 0 ? prevDy*2 : MIN_MOVE_STEP);
				if (newDy < -MAX_MOVE_STEP)
					newDy = -MAX_MOVE_STEP;
				else if (newDy > MAX_MOVE_STEP)
					newDy = MAX_MOVE_STEP;
			}
		}
		prevDy = newDy;
		return newDy;
	}
	
	private int calcDx(int dx, long curTime) {
		//return dx*2;
		return calcDxSmooth(dx, curTime);
	}
	
	private int calcDy(int dy, long curTime) {
		//return dy*2;
		return calcDySmooth(dy, curTime);
	}
	
	protected boolean navigationMovement(int dx, int dy, int status, int time) {
		if ((status & KeypadListener.STATUS_TRACKWHEEL) == 0 &&
				(status & KeypadListener.STATUS_FOUR_WAY) == 0)
			return false;

		if (mode == PAN_MODE) {
			long curTime = System.currentTimeMillis();
			int newDx = calcDx(dx, curTime);
			int newDy = calcDy(dy, curTime);
			
			prevMoveTime = curTime;
			
			//int newDx = dx*10;
			//int newDy = dy*10;
			
			//LOG.TRACE("Scroll by " + newDx + "," + newDy);
			mapField.move(newDx, newDy);
			mapField.redraw();
		}
		else if (mode == ZOOM_MODE && dy != 0) {
			int currentZoom = mapField.getZoom();
			int minZoom = mapField.getMinZoom();
			int maxZoom = mapField.getMaxZoom();
			
			int newZoom;
			if (dy > 0) {
				newZoom = Math.max(currentZoom - 1, minZoom);
			}
			else {
				newZoom = Math.min(currentZoom + 1, maxZoom);
			}
			//LOG.TRACE("Set zoom to " + newZoom + " (was " + currentZoom + ")");
			mapField.setZoom(newZoom);
			mapField.redraw();
		}
		return true;
	}
	
	protected boolean trackwheelClick(int status, int time) {
		if (mapField.handleClick()) {
			if (mapField.needToClose())
				this.close();
		}
		
		invalidate();
		return true;
	}
	
	public double getCenterLatitude() {
		return mapField.getCenterLatitude();
	}
	
	public double getCenterLongitude() {
		return mapField.getCenterLongitude();
	}
}
