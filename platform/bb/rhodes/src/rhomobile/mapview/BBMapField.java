package rhomobile.mapview;

import rhomobile.mapview.RhoMapField;
import net.rim.device.api.lbs.MapField;
import net.rim.device.api.ui.Field;

public class BBMapField extends MapField implements RhoMapField {

	public Field getBBField() {
		return this;
	}

	public void moveTo(double latitude, double longitude) {
		super.moveTo((int)(latitude*100000), (int)(longitude*100000));
	}

	public int calculateZoom(double latDelta, double lonDelta) {
		// TODO: implement
		return 0;
	}
	
	public void setMapType(String type) {
		// Ignore
	}
	
	public void redraw() {
		invalidate();
	}
	
	public void addAnnotation(Annotation ann) {
		// Not implemented
	}

}
