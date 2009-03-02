package com.rhomobile.rhodes.http;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.net.Socket;

import android.util.Log;

public class HttpListener extends ThreadedServer {

	private static final Integer DEFAULT_IDLE_TIMEOUT = 30000;

	private InetAddrPort address = null;
	private HttpServer server = null;

	public InetAddrPort getAddress() {
		return address;
	}

	/**
	 * Construct a HttpListener
	 */
	public HttpListener(InetAddrPort address, HttpServer server)
			throws IOException {
		this(address, server, 0, 0, DEFAULT_IDLE_TIMEOUT);
	}

	/**
	 * Constructor.
	 */
	public HttpListener(InetAddrPort address, HttpServer server,
			int minThreads, int maxThreads, int maxIdleTimeMs)
			throws IOException {
		super(address, minThreads, maxThreads, maxIdleTimeMs);
		if (address.getPort() == 0) {
			address.setPort(HttpServer.DEFAULT_PORT);
			super.setAddress(address.getInetAddress(), address.getPort());
		}

		this.address = address;
		this.server = server;
	}

	public void start() throws IOException {
		super.start();
		Log.i(this.getClass().getSimpleName(), "Started on " + getAddress());
	}

	/**
	 * Handle a connection to the server by trying to read a HttpRequest and
	 * finding the right type of handler for that request, which provides the
	 * HttpResponse.
	 */
	public void handleConnection(Socket connection) {
		try {
			Log.d(this.getClass().getSimpleName(), "CONNECT: " + connection);

			while (true) {
				HttpRequest request = null;
				HttpResponse response = null;

				try {
					Log.d(this.getClass().getSimpleName(),
							"Waiting for request...");

					request = new HttpRequest(server, connection, address);

					Log.i(this.getClass().getSimpleName(),
							"Received HTTP request:" + request.getMethod()
									+ " " + request.getRequestURI());

					response = new HttpResponse(connection.getOutputStream(), request);

					server.handle(request, response);

					response.complete();

					Log.i(this.getClass().getSimpleName(), "RESPONSE: "
							+ response.getStatus());

					String connection_header = response
							.getHeader(HttpHeader.Connection);
					
					if (connection_header != null)
						connection_header = connection_header.toLowerCase();

					// Break request loop if 1.0 and not keep-alive
					if (HttpHeader.HTTP_1_0.equals(request.getProtocol())
							&& !"keep-alive".equals(connection_header))
						break;

					// Break request loop of close requested
					if (HttpHeader.Close.equals(connection_header)) {
						Log.w(this.getClass().getSimpleName(),
								"Closing persistent connection");
						break;
					}

					// Read any remaining input.
					if (request.getContentLength() > 0) {
						HttpInputStream in = request.getHttpInputStream();
						try {
							// Skip/read remaining input
							while (in.getContentLength() > 0
									&& (in.skip(4096) > 0 || in.read() >= 0))
								;

						} catch (IOException e) {
							Log.e(this.getClass().getSimpleName(),e.getMessage());
						}
						finally
						{
							if ( in != null )
								in.close();
						}
					}
				} 
				catch (InterruptedIOException e)
                {
                    //ignore
                }
                catch (Exception e) {
					// If no respones - must have a request error
					if (response == null) {
						// try to write BAD_REQUEST
						response = new HttpResponse(connection
								.getOutputStream(), null);

						response.setHeader(HttpHeader.Connection,
								HttpHeader.Close);
						response.sendError(HttpResponse.SC_BAD_REQUEST);
					}
				} finally {
					if (request != null)
						request.destroy();
					if (response != null)
						response.destroy();
				}
			}
		} catch (Exception e) {
			Log.e(this.getClass().getSimpleName(), "Request problem: "
					+ e.getMessage());
		}
	}
}
