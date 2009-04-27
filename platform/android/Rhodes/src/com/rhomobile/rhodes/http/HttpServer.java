package com.rhomobile.rhodes.http;

import java.io.IOException;
import java.net.ServerSocket;

import android.util.Log;

/**
 * HTTP Server. This is the core class for the HTTP server.
 */
public class HttpServer {

	public static Integer DEFAULT_PORT = 8080;
	public static final String DEFAULT_HOST = "127.0.0.1";

	private IHttpHandler handler;
	private HttpListener listener;

	public HttpServer() {
	}

	public synchronized void configure(IHttpHandler handler) throws Exception {
		try {
			this.handler = handler;
			this.handler.setServer(this);
		} catch (Exception e) {
			Log.e(this.getClass().getSimpleName(),
					"HttpHander could not setServer: " + e.getMessage());
			throw e;
		}

		listener = new HttpListener(new InetAddrPort(DEFAULT_HOST + ":"
				+ DEFAULT_PORT.toString()), this);

	}

	public static int findFreePort() {
		try
		{
			ServerSocket server = new ServerSocket(0);
			int port = server.getLocalPort();
			server.close();
			return port;
		}
		catch(Exception e) {
			return 8080;
		}
	}

	/**
	 * Stop the HttpServer.
	 */
	public synchronized void stop() {
		listener.stop();
		listener = null;

		// Destroy handlers
		handler.destroy();
		handler = null;
	}

	/**
	 * Start the HttpServer.
	 * 
	 * @throws IOException
	 */
	public synchronized void start() throws IOException {
		listener.start();
	}

	/**
	 * join the HttpServer and listener
	 */
	public synchronized void join() throws InterruptedException {
		if (listener != null)
			listener.join();
	}

	/**
	 * Handle a HTTP request. Called by the HttpListener that received the
	 * request
	 */
	void handle(HttpRequest request, HttpResponse response) {
		try {

			if (request.getProtocol().equals(HttpHeader.HTTP_1_1)) {
				// Set response version.
				response.setVersion(HttpHeader.HTTP_1_1);

				// insist on Host header
				if (request.getHeader(HttpHeader.Host) == null) {
					response.sendError(HttpResponse.SC_BAD_REQUEST);
					return;
				}
			}

			// Give request to handlers
			boolean handled = false;

			// Select request handler stack by path
			Log.d(this.getClass().getSimpleName(), "Handler: " + handler);

			this.handler.handle(request, response);
			handled = response.requestHandled();
			if (handled)
				Log.d(this.getClass().getSimpleName(),
						"Request was handled by " + handler);
			else
				Log.d(this.getClass().getSimpleName(),
						"Request NOT handled by " + handler);

			if (handled)
				response.flush();
		} catch (IOException e) {
			Log.e(this.getClass().getSimpleName(), "request aborted: "
					+ request + " details: " + e.getMessage());
		}
	}

}
