package rhomobile.mapview;

public interface GeoCoding {
	
	public interface OnGeocodingDone {
		public void onError(String description);
		public void onSuccess(double latitude, double longitude);
	};

	public void stop();
	public void resolve(String address, OnGeocodingDone callback);
	
}
