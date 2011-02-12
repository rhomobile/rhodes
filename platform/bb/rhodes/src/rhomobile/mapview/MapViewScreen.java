package rhomobile.mapview;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.rubyext.WebView;

import net.rim.device.api.system.Application;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.Display;
import net.rim.device.api.system.KeypadListener;
import net.rim.device.api.ui.Color;
import net.rim.device.api.ui.Graphics;
import net.rim.device.api.ui.MenuItem;
//import net.rim.device.api.ui.TouchEvent;
import net.rim.device.api.ui.UiApplication;
//import net.rim.device.api.ui.container.MainScreen;
import com.rho.RhoMainScreen;

public class MapViewScreen extends RhoMainScreen {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("MapViewScreen");
	
	private static final int PAN_MODE = 1;
	private static final int ZOOM_MODE = 2;
	
	private static final int MIN_MOVE_STEP = 1;
	private static final int MAX_MOVE_STEP = 8;
	
	private static final int MOVE_TIMEOUT_DOUBLING = 300;
	
	// Sensivity of annotations area (in pixels)
	private static final int ANNOTATION_SENSIVITY_AREA_RADIUS = 16;
	
	private static final MapProvider[] providers = {
		new GoogleMapProvider(),
		new ESRIMapProvider()
	};
	
	private MapViewParent parent;
	private MapProvider mapProvider;
	private RhoMapField mapField;
	private GeoCoding mapGeoCoding;
	private Vector annotations = new Vector();
	private Annotation mSelectedAnnotation;
	
	private int mode;
	
	private long prevMoveTime = 0;
	private int prevDx = 0;
	private int prevDy = 0;
	
	private Bitmap mapPinImage;
	
	private boolean mTouchDown = false;
	private int mTouchX;
	private int mTouchY;
	
	private class Rect
	{
		public int left, right, top, bottom;
		
		boolean isInside( int x, int y){ return x >= left && x <= right && y >= top && y<=bottom;}
	};
	Rect mCalloutRect;
	
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
		
		mapPinImage = Bitmap.getBitmapResource("mappin.png");
		
		mapParent = p;
		createMapProvider(providerName);

		mapGeoCoding = new GoogleGeoCoding();
		
		createUI(settings);

		this.annotations = annotations;
		handleAnnotations();
	}
	
	public void close() {
		mapField.close();
		mapGeoCoding.stop();
		mapParent.onChildClosed();
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
	
	private void createUI(Hashtable settings) {
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
		
		String center = (String)settings.get("center");
		if (center != null) {
			mapGeoCoding.resolve(center, new GeoCoding.OnGeocodingDone() {
				public void onSuccess(double latitude, double longitude) {
					mapField.moveTo(latitude, longitude);
				}
				public void onError(String description) {}
			});
		}
		
		mode = PAN_MODE;
		
		mapField.redraw();
	}
	
	private void handleAnnotations() {
		Enumeration e = annotations.elements();
		while (e.hasMoreElements()) {
			final Annotation ann = (Annotation)e.nextElement();
			if (ann == null)
				continue;
			if (ann.street_address == null && ann.coordinates == null)
				continue;
			if (ann.street_address != null && ann.street_address.length() > 0)
				mapGeoCoding.resolve(ann.street_address, new GeoCoding.OnGeocodingDone() {
					public void onSuccess(double latitude, double longitude) {
						ann.coordinates = new Annotation.Coordinates(latitude, longitude);
						UiApplication.getUiApplication().invokeLater(new Runnable() {
							public void run() {
								invalidate();
							}
						});
					}
					public void onError(String description) {
						ann.street_address = null;
					}
				});
		}
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
		
		// Draw annotations
		int pinWidth = mapPinImage.getWidth();
		int pinHeight = mapPinImage.getHeight();
		Enumeration e = annotations.elements();
		while (e.hasMoreElements()) {
			Annotation ann = (Annotation)e.nextElement();
			if (ann == null || ann.coordinates == null)
				continue;
			
			long x = mapField.toScreenCoordinateX(ann.coordinates.longitude);
			if (x + pinWidth/2 < 0 || x - pinWidth/2 > mapField.getWidth())
				continue;
			long y = mapField.toScreenCoordinateY(ann.coordinates.latitude);
			if (y + pinHeight/2 < 0 || y - pinHeight/2 > mapField.getHeight())
				continue;
			
			graphics.drawBitmap((int)(x - pinWidth/2), (int)(y - pinHeight/2), pinWidth, pinHeight, mapPinImage, 0, 0);
		}
		
		if (mSelectedAnnotation != null)
			drawTitle(graphics, mSelectedAnnotation);
		
		graphics.setColor(Color.BLACK);
		
		// Draw current mode
		String strMode  = null;
		if (mode == PAN_MODE)
			strMode = "Pan mode";
		else if (mode == ZOOM_MODE)
			strMode = "Zoom mode";
		
		if (strMode != null) {
			// Detect drawn text size
			int tw = graphics.getFont().getAdvance(strMode);
			int th = graphics.getFont().getHeight();
			// Actual drawing
			int x = mapField.getLeft() + mapField.getWidth()/2 - tw/2;
			int y = mapField.getTop() + mapField.getHeight() - th - 10; 
			tw = graphics.drawText(strMode, x, y);
		}
	}
	
	private void fillRectWithRoundedCorners(Graphics graphics, int left, int top, int width, int height, int roundRadius) {
		final int r = roundRadius;
		final int d = r*2;
		
		final int right = left + width;
		final int bottom = top + height;
		
		graphics.fillArc(left - r, top - r, d, d, 90, 90);
		graphics.fillArc(left - r, bottom - r, d, d, 180, 90);
		graphics.fillArc(right - r, bottom - r, d, d, 270, 90);
		graphics.fillArc(right - r, top - r, d, d, 0, 90);
		
		graphics.fillRect(left - r, top, r, bottom - top);
		graphics.fillRect(right, top, r, bottom - top);
		graphics.fillRect(left, top - r, right - left, r);
		graphics.fillRect(left, bottom, right - left, r);
		
		graphics.fillRect(left, top, right - left, bottom - top);
	}
	
	private void drawRectWithRoundedCorners(Graphics graphics, int left, int top, int width, int height, int roundRadius) {
		final int r = roundRadius;
		final int d = r*2;
		
		final int right = left + width;
		final int bottom = top + height;
		
		graphics.drawArc(left - r, top - r, d, d, 90, 90);
		graphics.drawArc(left - r, bottom - r, d, d, 180, 90);
		graphics.drawArc(right - r, bottom - r, d, d, 270, 90);
		graphics.drawArc(right - r, top - r, d, d, 0, 90);
		
		graphics.drawLine(left - r, top, left - r, bottom);
		graphics.drawLine(right + r, top, right + r, bottom);
		graphics.drawLine(left, top - r, right, top - r);
		graphics.drawLine(left, bottom + r, right, bottom + r);
	}
	
	private void drawTitle(Graphics graphics, Annotation ann) 
	{
		int nLines = 1;
		if ( ann.title.length()>0 && ann.subtitle.length()>0 )
			nLines++;
		
		int width = graphics.getFont().getAdvance(ann.title);
		int wSubtitle = graphics.getFont().getAdvance(ann.subtitle);
		if ( wSubtitle > width )
			width = wSubtitle;
		
		int height = graphics.getFont().getHeight()*nLines;
		
		int annX = (int)mapField.toScreenCoordinateX(ann.coordinates.longitude);
		int annY = (int)mapField.toScreenCoordinateY(ann.coordinates.latitude);
		
		int left = annX - width/2;
		int top = annY - height - 7*(mapPinImage.getHeight()/8);

		mCalloutRect = new Rect();
		mCalloutRect.left = left;
		mCalloutRect.top = top;
		mCalloutRect.right = left+width;
		mCalloutRect.bottom = top+height;
		
		final int roundRadius = 6;
		
		// Shadow
		graphics.setColor(Color.GRAY);
		fillRectWithRoundedCorners(graphics, left + 4, top + 4, width, height, roundRadius);
		// Actual shape
		graphics.setColor(Color.WHITE);
		fillRectWithRoundedCorners(graphics, left, top, width, height, roundRadius);
		graphics.setColor(Color.BLACK);
		drawRectWithRoundedCorners(graphics, left, top, width, height, roundRadius);
		
		graphics.setColor(Color.BLACK);
		
		if ( ann.title.length() > 0 )
		{
			graphics.drawText(ann.title, left, top);
			top += height/2;
		}
		
		if ( ann.subtitle.length() > 0 )
			graphics.drawText(ann.subtitle, left, top);
		
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
	
	private void handleMove(int dx, int dy) {
		if (mode == PAN_MODE) {
			//LOG.TRACE("Scroll by " + dx + "," + dy);
			mapField.move(dx, dy);
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
	}
	
	private void handleClick(int x, int y) 
	{
		Annotation old_selectedAnnotation = mSelectedAnnotation;
		if ( mSelectedAnnotation != null && mCalloutRect!=null && mCalloutRect.isInside(x, y) )
		{
			
		}else
			mSelectedAnnotation = getCurrentAnnotation(x, y);
		
		if (mSelectedAnnotation != null && old_selectedAnnotation != null && mSelectedAnnotation.equals(old_selectedAnnotation)) 
		{
			// We have clicked already selected annotation
			WebView.navigate(mSelectedAnnotation.url);
			mapParent.close();
			mSelectedAnnotation = null;
		}
		invalidate();
	}
	
	protected boolean navigationMovement(int dx, int dy, int status, int time) {
		if ((status & KeypadListener.STATUS_TRACKWHEEL) == 0 &&
				(status & KeypadListener.STATUS_FOUR_WAY) == 0)
			return false;

		if (mode == PAN_MODE) {
			long curTime = System.currentTimeMillis();
			dx = calcDx(dx, curTime);
			dy = calcDy(dy, curTime);
			prevMoveTime = curTime;
		}
		
		handleMove(dx, dy);
		
		return true;
	}
	
	protected boolean trackwheelClick(int status, int time) {
		int x = getWidth()/2;
		int y = getHeight()/2;
		handleClick(x, y);
		return true;
	}
	
	protected boolean onTouchClick(int x, int y)
	{ 
		handleClick(x, y);
		return true; 
	}
	
	protected boolean onTouchDown(int x, int y)
	{ 
		mTouchDown = true;
		mTouchX = x;
		mTouchY = y;
		return false; 
	}
	
	protected boolean onTouchUp(int x, int y)
	{ 
		mTouchDown = false;
		return false; 
	}
	
	protected boolean onTouchMove(int x, int y)
	{ 
		if (mTouchDown) 
		{
			int dx = x - mTouchX;
			int dy = y - mTouchY;
			if (mode == PAN_MODE) {
				dx = -dx;
				dy = -dy;
			}
			
			handleMove(dx, dy);
			
			mTouchX = x;
			mTouchY = y;
			return true;
		}
		
		return false; 
	}
	
/*	
	protected boolean touchEvent(TouchEvent message) {
		switch (message.getEvent()) {
		case TouchEvent.CLICK:
			handleClick(message.getX(1), message.getY(1));
			return true;
		case TouchEvent.DOWN:
			mTouchDown = true;
			mTouchX = message.getX(1);
			mTouchY = message.getY(1);
			break;
		case TouchEvent.UP:
			mTouchDown = false;
			break; 
		case TouchEvent.MOVE:
			if (mTouchDown) {
				int x = message.getX(1);
				int y = message.getY(1);
				
				int dx = x - mTouchX;
				int dy = y - mTouchY;
				if (mode == PAN_MODE) {
					dx = -dx;
					dy = -dy;
				}
				
				handleMove(dx, dy);
				
				mTouchX = x;
				mTouchY = y;
				return true;
			}
		}
		
		return super.touchEvent(message);
	}*/
	
	public double getCenterLatitude() {
		return mapField.getCenterLatitude();
	}
	
	public double getCenterLongitude() {
		return mapField.getCenterLongitude();
	}
	
	private Annotation getCurrentAnnotation(int x, int y) {
		// return current annotation (point we are under now)
		Enumeration e = annotations.elements();
		while (e.hasMoreElements()) {
			Annotation a = (Annotation)e.nextElement();
			Annotation.Coordinates coords = a.coordinates;
			if (coords == null)
				continue;
			
			long annX = mapField.toScreenCoordinateX(coords.longitude);
			long annY = mapField.toScreenCoordinateY(coords.latitude);
			annY -= mapPinImage.getHeight()/2;
			
			long deltaX = (long)x - annX;
			long deltaY = (long)y - annY;
			
			double distance = MapTools.math_sqrt(deltaX*deltaX + deltaY*deltaY);
			if ((int)distance > ANNOTATION_SENSIVITY_AREA_RADIUS)
				continue;
			
			return a;
		}
		return null;
	}
}
