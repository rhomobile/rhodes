package com.rhomobile.rhodes.signature;

import com.rhomobile.rhodes.Logger;

class SignatureProperties {
	
	public int penColor;
	public float penWidth;
	public int bgColor;
	
	public int left;
	public int top;
	public int width;
	public int height;
	
	String imageFormat;
	
	String callbackUrl;
	
	public static final String PEN_COLOR = "penColor";
	public static final String PEN_WIDTH = "penWidth";
	public static final String BG_COLOR = "bgColor";
	public static final String IMAGE_FORMAT = "imageFormat";
	public static final String LEFT = "left";
	public static final String TOP = "top";
	public static final String WIDTH = "width";
	public static final String HEIGHT = "height";
	public static final String CALLBACK = "callback";
	
	public static final String[] PROPERTIES_LIST = {	PEN_COLOR,
														PEN_WIDTH,
														BG_COLOR,
														IMAGE_FORMAT,
														LEFT,
														TOP,
														WIDTH,
														HEIGHT,
														CALLBACK};
	
	public SignatureProperties() {
		penColor = 0xFF66009A;
		penWidth = 2;
		bgColor = 0xFFFFFFFF;
		
		left = 0;
		top = 0;
		width = 200;
		height = 200;
		
		imageFormat = "png";
		
		callbackUrl = "";
	}
	
	void setPropertyByName(String name, String value) {
		if ((name == null) || (value == null)) {
			return;
		}
		try {
			if (PEN_COLOR.equalsIgnoreCase(name)) {
				penColor = (int)Long.parseLong(value);
			}
			else if (PEN_WIDTH.equalsIgnoreCase(name)) {
				penWidth = Float.parseFloat(value);
			}
			else if (BG_COLOR.equalsIgnoreCase(name)) {
				bgColor = (int)Long.parseLong(value);
			}
			else if (LEFT.equalsIgnoreCase(name)) {
				left = Integer.parseInt(value);
			}
			else if (TOP.equalsIgnoreCase(name)) {
				top = Integer.parseInt(value);
			}
			else if (WIDTH.equalsIgnoreCase(name)) {
				width = Integer.parseInt(value);
			}
			else if (HEIGHT.equalsIgnoreCase(name)) {
				height = Integer.parseInt(value);
			}
			else if (IMAGE_FORMAT.equalsIgnoreCase(name)) {
				imageFormat = new String(value);
			}
			else if (CALLBACK.equalsIgnoreCase(name)) {
				callbackUrl = new String(value);
			}
		}
		catch (Exception e) {
		    Logger.E(Signature.TAG, "SignatureProperties Exception during set param["+name+"] to value["+value+"]");
			Logger.E(Signature.TAG, e);
		}
	}	
	
}