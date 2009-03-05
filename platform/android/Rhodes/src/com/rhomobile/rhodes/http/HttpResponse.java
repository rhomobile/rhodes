package com.rhomobile.rhodes.http;

import java.io.*;
import java.util.*;

import android.util.Log;

@SuppressWarnings("unchecked")
public class HttpResponse extends HttpHeader {

	public final static String MIME_Version = "MIME-Version";
	public final static String Server = "Server";
	public final static String Expires = "Expires";
	public final static String Location = "Location";
	public final static String Allow = "Allow";

	public static int SC_OK = 200;
	public static int SC_MOVED_TEMPORARILY = 302;
	public static int SC_BAD_REQUEST = 400;
	
	public final static Hashtable errorCodeMap = new Hashtable();
	static {
		errorCodeMap.put(new Integer(202), "Accepted");
		errorCodeMap.put(new Integer(502), "Bad Gateway");
		errorCodeMap.put(new Integer(400), "Bad Request");
		errorCodeMap.put(new Integer(409), "Conflict");
		errorCodeMap.put(new Integer(100), "Continue");
		errorCodeMap.put(new Integer(201), "Created");
		errorCodeMap.put(new Integer(417), "Expectation Failed");
		errorCodeMap.put(new Integer(424), "Failed Dependency");
		errorCodeMap.put(new Integer(403), "Forbidden");
		errorCodeMap.put(new Integer(504), "Gateway Timeout");
		errorCodeMap.put(new Integer(410), "Gone");
		errorCodeMap.put(new Integer(505), "HTTP Version Not Supported");
		errorCodeMap.put(new Integer(507), "Insufficient Storage");
		errorCodeMap.put(new Integer(500), "Server Error");
		errorCodeMap.put(new Integer(411), "Length Required");
		errorCodeMap.put(new Integer(423), "Locked");
		errorCodeMap.put(new Integer(405), "Method Not Allowed");
		errorCodeMap.put(new Integer(301), "Moved Permanently");
		errorCodeMap.put(new Integer(207), "Multi-Status");
		errorCodeMap.put(new Integer(300), "Mutliple Choices");
		errorCodeMap.put(new Integer(204), "No Content");
		errorCodeMap.put(new Integer(203), "Non Authoritative Information");
		errorCodeMap.put(new Integer(406), "Not Acceptable");
		errorCodeMap.put(new Integer(404), "Not Found");
		errorCodeMap.put(new Integer(501), "Not Implemented");
		errorCodeMap.put(new Integer(304), "Not Modified");
		errorCodeMap.put(new Integer(200), "Ok");
		errorCodeMap.put(new Integer(206), "Partial Content");
		errorCodeMap.put(new Integer(402), "Payment Required");
		errorCodeMap.put(new Integer(412), "Precondition Failed");
		errorCodeMap.put(new Integer(102), "Processing");
		errorCodeMap.put(new Integer(407), "Proxy Authentication Required");
		errorCodeMap.put(new Integer(408), "Request Timeout");
		errorCodeMap.put(new Integer(413), "Request Entity Too Large");
		errorCodeMap.put(new Integer(414), "Request-URI Too Long");
		errorCodeMap.put(new Integer(416), "Requested Range Not Satisfiable");
		errorCodeMap.put(new Integer(205), "Reset Content");
		errorCodeMap.put(new Integer(303), "See Other");
		errorCodeMap.put(new Integer(503), "Service Unavailable");
		errorCodeMap.put(new Integer(307), "Temporary Redirect");
		errorCodeMap.put(new Integer(401), "Unauthorized");
		errorCodeMap.put(new Integer(422), "Unprocessable Entity");
		errorCodeMap.put(new Integer(415), "Unsupported Media Type");
		errorCodeMap.put(new Integer(305), "Use Proxy");
	}

	private String version;
	private String status;
	private String reason;
	private HttpOutputStream httpOut;
	private OutputStream out;
	private PrintWriter writer;
	private boolean headersWritten = false;
	private Vector filters = null;
	private HttpRequest request = null;
	private Observable observable = null;
	private boolean chunkByDefault = false;
	private boolean doNotClose = false;
	private int outputState = 0;
	private boolean handled = false;

	private static final int IO_BUFFER_SIZE = 4 * 1024;

	public static void copy(InputStream in, OutputStream out)
			throws IOException {
		byte[] b = new byte[IO_BUFFER_SIZE];
		int read;
		while ((read = in.read(b)) != -1) {
			out.write(b, 0, read);
		}
	}

	/**
	 * Construct a response
	 */
	public HttpResponse(OutputStream out, HttpRequest request) {
		this.out = out;
		this.httpOut = new HttpOutputStream(out, this);
		this.request = request;
		if (request != null)
			request.setHttpResponse(this);
		version = HttpHeader.HTTP_1_1;
		status = Integer.toString(SC_OK);
		reason = "OK";
		setHeader(ContentType, "text/html");
		setHeader(MIME_Version, "1.0");

		setHeader(Server, "Rhodes/1.0.0");

		if (request != null
				&& HttpHeader.Close.equals(request
						.getHeader(HttpHeader.Connection)))
			setHeader(HttpHeader.Connection, HttpHeader.Close);
	}

	public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version;
	}

	public String getStatus() {
		return status;
	}

	public String getReason() {
		return reason;
	}

	/**
	 * Get the HttpRequest for this response
	 */
	public HttpRequest getRequest() {
		return request;
	}

	public String getResponseLine() {
		return version + " " + status + " " + reason;
	}

	public void setChunkByDefault(boolean chunk) {
		chunkByDefault = chunk;
	}

	/**
	 * Complete the response.
	 */
	public void complete() throws IOException {
		if (!doNotClose)
			httpOut.close();
		doNotClose = false;
	}

	/**
	 * Return true if the headers have already been written for this response
	 */
	public boolean headersWritten() {
		return headersWritten;
	}

	/**
	 * Return true if the headers have already been written for this response
	 */
	public boolean requestHandled() {
		return handled;
	}

	/**
	 * If the headers have not already been written, write them.
	 */
	public void writeHeaders() throws IOException {
		if (headersWritten)
			return;

		Log.d(this.getClass().getSimpleName(), "Write Headers");

		headersWritten = true;
		if (HttpHeader.HTTP_1_1.equals(version)) {
			String encoding = getHeader(HttpHeader.TransferEncoding);
			String connection = getHeader(Connection);
			String length = getHeader(HttpHeader.ContentLength);

			if (encoding != null && encoding.equals(HttpHeader.Chunked)) {
				httpOut.setChunking(true);
			} else if (length == null || "0".equals(length)) {
				if (!(HttpHeader.Close.equals(connection)) && chunkByDefault) {
					setHeader(HttpHeader.TransferEncoding, HttpHeader.Chunked);
					httpOut.setChunking(true);
				} else {
					setHeader(Connection, HttpHeader.Close);
				}
			} else {
				doNotClose = true;
			}
		} else if (HttpHeader.HTTP_1_0.equals(version)) {
			String connection = getHeader(Connection);
			if (connection == null) {
				// Assume we close unless otherwise
				setHeader(Connection, HttpHeader.Close);
			}
		} else {
			setHeader(Connection, HttpHeader.Close);
		}

		// Write the headers
		handled = true;
		OutputStreamWriter writer = new OutputStreamWriter(out, "UTF-8");
		synchronized (writer) {
			writer.write(version);
			writer.write(" ");
			writer.write(status);
			writer.write(" ");
			writer.write(reason);
			writer.write(CRLF);

			super.write(writer);
			writer.flush();
		}

		// Handle HEAD
		if (request != null && request.getMethod().equals("HEAD"))
			// Fake a break in the HttpOutputStream
			throw new IOException("break in the HttpOutputStream");
	}

	/**
	 * Copy all data from an input stream to the HttpResponse.
	 */
	public void writeInputStream(InputStream stream, long length)
			throws IOException {
		writeInputStream(stream, length, false);
	}

	/**
	 * Copy all data from an input stream to the HttpResponse.
	 */
	public void writeInputStream(InputStream stream, long length,
			boolean streamIncludesHeaders) throws IOException {
		Log.d(this.getClass().getSimpleName(), "writeInputStream: "
				+ new Long(length));
		if (streamIncludesHeaders) {
			headersWritten = true;
			handled = true;
			if (observable != null)
				observable.notifyObservers(this);
		}

		if (stream != null)
			copy(stream, getOutputStream());
	}

	/**
	 * Get the HttpOutputStream of the response.
	 */
	synchronized HttpOutputStream getHttpOutputStream() {
		return httpOut;
	}

	void flush() throws IOException {
		if (outputState == 2)
			writer.flush();
		else
			httpOut.flush();
	}

	/**
	 * Set the content length of the response
	 */
	public void setContentLength(long len) {
		setHeader(ContentLength, Long.toString(len));
		handled = true;
	}

	/**
	 * Set the content type of the response
	 */
	public void setContentType(String type) {
		setHeader(ContentType, type);
		handled = true;
	}

	/**
	 * Get the OutputStream of the response.
	 */
	public synchronized OutputStream getOutputStream() {
		handled = true;
		if (outputState != 0 && outputState != 1)
			throw new IllegalStateException();
		outputState = 1;
		return httpOut;
	}

	/**
	 * Sets the status code and a default message for this response.
	 */
	public void setStatus(int code) {
		handled = true;
		status = Integer.toString(code);
		String msg = (String) errorCodeMap.get(new Integer(code));
		reason = (msg != null) ? msg : status;
	}
	
	/**
	 * Sets the status code and a default message for this response.
	 */
	public void setStatus(int code, String message) {
		Log.i(this.getClass().getSimpleName(), "Response, status code:" + code + " msg:" + message);
		handled = true;
		status = Integer.toString(code);
		reason = message;
	}

	/**
	 * Sends an error response to the client using the specified status code and
	 * detail message.
	 */
	public void sendError(int code, String msg) throws IOException {
		setStatus(code);

		if (code != 204 && code != 304 && code >= 200) {
			setContentType("text/html");
			byte[] buf = ("<HTML><HEAD><TITLE>Error " + code
					+ "</TITLE><BODY><H2>HTTP ERROR: " + code + " " + msg + "</H2></BODY>\n</HTML>\n")
					.getBytes("UTF-8");

			if (writer != null)
				writer.flush();
			else
				setContentLength(buf.length);
			outputState = 0;
			OutputStream out = getOutputStream();
			out.write(buf);
			out.flush();
		} else {
			setHeader(ContentType, null);
			setHeader(ContentLength, null);

			writeHeaders();
		}
	}

	/**
     * Sends an error response to the client using the specified status
     * code and no default message.
     */
    public void sendError(int code)
        throws IOException
    {
        String msg = (String)errorCodeMap.get(new Integer(code));
        if (msg==null)
            sendError(code,"UNKNOWN ERROR CODE");
        else
            sendError(code,msg);
    }
	
	/**
	 * Sends a redirect response to the client using the specified redirect
	 * location URL.
	 */
	public void sendRedirect(String location) throws IOException {
		setHeader(Location, location);
		setStatus(SC_MOVED_TEMPORARILY);
		writeHeaders();
	}

	public boolean containsHeader(String headerKey) {
		return getHeader(headerKey) != null;
	}

	/**
	 * Returns the character set encoding for the input of this request.
	 */
	public String getCharacterEncoding() {
		String encoding = getHeader(ContentType);
		if (encoding == null || encoding.length() == 0)
			return "UTF-8";

		int i = encoding.indexOf(';');
		if (i < 0)
			return "UTF-8";

		i = encoding.indexOf("charset=", i);
		if (i < 0 || i + 8 >= encoding.length())
			return "UTF-8";

		encoding = encoding.substring(i + 8);
		i = encoding.indexOf(' ');
		if (i > 0)
			encoding = encoding.substring(0, i);

		return encoding;
	}

	public synchronized java.io.PrintWriter getWriter() {
		if (outputState != 0 && outputState != 2)
			throw new IllegalStateException();

		if (writer == null) {
			try {
				writer = new PrintWriter(new OutputStreamWriter(
						getOutputStream(), getCharacterEncoding()));
			} catch (UnsupportedEncodingException e) {
				Log.w(this.getClass().getSimpleName(), e.getMessage());
				writer = new PrintWriter(new OutputStreamWriter(
						getOutputStream()));
			}
		}
		outputState = 2;
		return writer;
	}

	/**
	 * Destroy the header. Help the garbage collector by null everything that we
	 * can.
	 */
	public void destroy() {
		version = null;
		status = null;
		reason = null;
		httpOut = null;
		out = null;
		writer = null;
		if (filters != null) {
			filters.removeAllElements();
			filters = null;
		}
		request = null;
		observable = null;
		super.destroy();
	}

	java.util.Stack handledStack = new java.util.Stack();

	void preDispatch() {
		handledStack.push(new Boolean(handled));
		handled = false;
		if (writer != null)
			writer.flush();
		outputState = 0;
	}

	void postDispatchInclude() {
		handled = ((Boolean) handledStack.pop()).booleanValue();
		if (writer != null)
			outputState = 2;
	}

	void postDispatchForward() {
		handledStack.pop();
	}
}
