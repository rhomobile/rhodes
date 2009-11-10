package com.rhomobile.rhodes.camera;

public class CameraRunnable implements Runnable {
	private String loadUrl;
	private String loadImgUrl;
	private String errorStatus;
	private String errorMessage;

	public CameraRunnable(String loadUrl, String loadImgUrl,
			String errorStatus, String errorMessage) {
		this.loadUrl = loadUrl;
		this.loadImgUrl = loadImgUrl;
		this.errorStatus = errorStatus;
		this.errorMessage = errorMessage;
	}

	public void run() {
		/*
		IHttpConnection connection = null;

		HttpHeader headers = new HttpHeader();
		headers.setHeader("Content-Type", "application/x-www-form-urlencoded");
		
		try {
			if (loadImgUrl != null) {
				connection = AndroidHttpConnection.makeConnection(loadUrl, headers, ("status=ok&image_uri="
						+ loadImgUrl).getBytes());
			} else {
				connection = AndroidHttpConnection.makeConnection(loadUrl, headers, ("status="
						+ this.errorStatus + "&message=" + this.errorMessage).getBytes());
			}
			int code = connection.getResponseCode();
			if (code != IHttpConnection.HTTP_OK) {
				System.out.println("Error posting data: " + code);
			}
		} catch (Exception e) {
			System.out.println("Error posting data: " + e.getMessage());
		} finally {
			if (connection != null)
				try {
					connection.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
		}
		*/
	}

};
