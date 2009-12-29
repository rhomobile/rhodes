package rhomobile.mapview;

import rhomobile.mapview.MapProvider;
import rhomobile.mapview.RhoMapField;

public class BBMapProvider implements MapProvider {

	public boolean accept(String name) {
		return name.equalsIgnoreCase("blackberry");
	}
	
	public RhoMapField createMap() {
		return new BBMapField();
	}

}
