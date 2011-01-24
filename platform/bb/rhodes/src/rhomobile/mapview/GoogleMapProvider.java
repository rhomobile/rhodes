package rhomobile.mapview;

import rhomobile.mapview.MapProvider;
import rhomobile.mapview.RhoMapField;

public class GoogleMapProvider implements MapProvider {

	public boolean accept(String name) {
		return name.equalsIgnoreCase("google");
	}
	
	public RhoMapField createMap() {
		return new GoogleMapField();
	}

}
