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
	private static final int MAX_MOVE_STEP = 32;
	
	private static final int MOVE_TIMEOUT_DOUBLING = 300;
	
	private static final MapProvider[] providers = {
		new BBMapProvider(),
		new GoogleMapProvider()
	};
	
	private MapProvider mapProvider;
	private RhoMapField mapField;
	
	private int mode;
	
	private long prevMoveTime = 0;
	private int prevDx = 0;
	private int prevDy = 0;
	
	/*
	private class CloseMenuItem extends MenuItem {
		
		private MapViewScreen screen;
		
		public CloseMenuItem(MapViewScreen scr, String text, int ordinal, int priority) {
			super(text, ordinal, priority);
			screen = scr;
		}

		public void run() {
			screen.close();
		}
	}
	*/
	
	MapViewScreen(String providerName, Hashtable settings, Vector annotations) {
		super(DEFAULT_MENU | DEFAULT_CLOSE);
		//MenuItem closeItem = new CloseMenuItem(this, "Close", 0, 100);
		//addMenuItem(closeItem);
		createMapProvider(providerName);
		createUI(settings, annotations);
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
		
		Hashtable region = (Hashtable)settings.get("region");
		double lat = ((Double)region.get("latitude")).doubleValue();
		double lon = ((Double)region.get("longitude")).doubleValue();
		
		double latDelta = ((Double)region.get("latDelta")).doubleValue();
		double lonDelta = ((Double)region.get("lonDelta")).doubleValue();
		
		String map_type = (String)settings.get("map_type");
		
		int zoom = mapField.calculateZoom(latDelta, lonDelta);
		
		mapField.setMapType(map_type);
		mapField.moveTo(lat, lon);
		mapField.setZoom(zoom);
		
		Enumeration e = annotations.elements();
		while (e.hasMoreElements()) {
			Annotation ann = (Annotation)e.nextElement();
			mapField.addAnnotation(ann);
		}
		
		mode = PAN_MODE;
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
		// Draw black cross at center
		int xCenter = mapField.getLeft() + mapField.getWidth()/2;
		int yCenter = mapField.getTop() + mapField.getHeight()/2;
		int delta = 10;
		int yTop = yCenter - delta;
		int yBottom = yCenter + delta;
		int xLeft = xCenter - delta;
		int xRight = xCenter + delta;
		graphics.drawLine(xCenter, yTop, xCenter, yBottom);
		graphics.drawLine(xLeft, yCenter, xRight, yCenter);
		
		// Draw current mode
		String strMode  = null;
		if (mode == PAN_MODE)
			strMode = "Pan mode";
		else if (mode == ZOOM_MODE)
			strMode = "Zoom mode";
		
		if (strMode != null) {
			int x = mapField.getLeft() + mapField.getWidth()/2 - 60;
			int y = mapField.getTop() + mapField.getHeight() - 40;
			graphics.drawText(strMode, x, y);
		}
	}
	
	protected boolean navigationMovement(int dx, int dy, int status, int time) {
		if ((status & KeypadListener.STATUS_TRACKWHEEL) == 0 &&
				(status & KeypadListener.STATUS_FOUR_WAY) == 0)
			return false;

		if (mode == PAN_MODE) {
			long curTime = System.currentTimeMillis();
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
			
			prevMoveTime = curTime;
			
			//int newDx = dx*10;
			//int newDy = dy*10;
			//LOG.TRACE("Scroll by " + newDx + "," + newDy);
			mapField.move(newDx, newDy);
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
		}
		return true;
	}
	
	protected boolean trackwheelClick(int status, int time) {
		if (mode == PAN_MODE)
			mode = ZOOM_MODE;
		else if (mode == ZOOM_MODE)
			mode = PAN_MODE;
		else
			return false;
		
		invalidate();
		return true;
	}
}
