package com.rho.net;

import com.google.gwt.core.client.JavaScriptObject;

//It is an javascript overlay type
public class NetResponse extends JavaScriptObject {

	// Overlay types always have protected, zero-arg ctors
	// http://code.google.com/webtoolkit/doc/latest/DevGuideCodingBasicsOverlay.html
	protected NetResponse() { }

	/*
	 * NetResponse json object structure:
	 * 
	 * var netResponse = {
	 * 		body: "",
	 * 		statusCode: 200,
	 * 		statusText: "200 OK"
	 * };
	 * 
	 */

	public final native String getCharData() /*-{
		return this.body;
	}-*/;

	public final native String getCookies() /*-{
		return $doc.cookie;
	}-*/;

	public final native void setCharData(String data) /*-{
		this.body = data;
	}-*/;

	public final native int getRespCode() /*-{
	return this.statusCode;
}-*/;

	public final native String getRespCodeText() /*-{
	return this.statusText;
}-*/;

	public final native boolean isOK()  /*-{
		return (200 <= this.statusCode && this.statusCode < 300);
	}-*/;


	public final native void setCookies(String strCookies) /*-{
		$doc.cookie = strCookies;
	}-*/;
}
