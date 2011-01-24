package rhomobile.mapview;


public class ESRIMapProvider implements MapProvider {

	public boolean accept(String name) {
		return name.equalsIgnoreCase("esri");
	}

	public RhoMapField createMap() {
		return new ESRIMapField();
	}

}
