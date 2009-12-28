package rhomobile.mapview;

import rhomobile.mapview.RhoMapField;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Graphics;

public class GoogleMapField extends Field implements RhoMapField {

	private int currentZoom;
	
	protected void layout(int width, int height) {
		setExtent(width, height);
	}

	protected void paint(Graphics graphics) {
		graphics.clear();
		graphics.setColor(0x00000000);
		graphics.drawText("Google map", getPreferredWidth()/2 - 30, getPreferredHeight()/2);
	}

	public void setPreferredSize(int width, int height) {
		setExtent(width, height);
	}

	public void moveTo(double latitude, double longitude) {
		// TODO Auto-generated method stub
		
	}

	public void move(int dx, int dy) {
		// TODO Auto-generated method stub
		
	}

	public Field getBBField() {
		return this;
	}

	public int getMaxZoom() {
		// TODO: retrieve data from Google
		return 15;
	}

	public int getMinZoom() {
		// TODO: retrieve data from Google
		return 0;
	}

	public int getZoom() {
		return currentZoom;
	}

	public void setZoom(int zoom) {
		currentZoom = zoom;
	}
	
}
