package com.rhomobile.rhodes.mapview;

public interface MapTouch {
	
	public static class Touch {
		public float x;
		public float y;
		
		public Touch(float x, float y) {
			this.x = x;
			this.y = y;
		}
	};

	public void touchDown(Touch first, Touch second);
	public void touchUp(Touch first, Touch second);
	public void touchMove(Touch first, Touch second);
	public void touchClick(Touch touch);
	
}
