package rhomobile.mapview;

public class Annotation {

	public static class Coordinates {
		public double latitude;
		public double longitude;
		
		public Coordinates(double lat, double lon) {
			latitude = lat;
			longitude = lon;
		}
	};
	
	public String title = null;
	public String subtitle = null;
	public String street_address = null;
	public String resolved_address = null;
	public String url = null;
	public Coordinates coordinates = null;
	
}
