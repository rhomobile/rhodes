package rhomobile.mapview;

public interface MapProvider {
	
	public boolean accept(String name);
	public RhoMapField createMap();
	
}
