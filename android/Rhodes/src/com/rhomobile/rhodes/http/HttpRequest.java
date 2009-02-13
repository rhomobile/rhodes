package com.rhomobile.rhodes.http;

import java.io.BufferedReader;
import java.io.EOFException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Enumeration;
import java.util.Vector;

import android.util.Log;

import com.rhomobile.rhodes.http.HttpInputStream.CharBuffer;
import com.rhomobile.rhodes.http.util.URI;
import com.rhomobile.rhodes.http.util.UrlEncoded;

@SuppressWarnings("unchecked")
public class HttpRequest extends HttpHeader {

	public static final String GET = "GET";
	public static final String POST = "POST";
	public static final String PUT = "PUT";
	public static final String HEAD = "HEAD";
	public static final String DELETE = "DELETE";
	public static final String MOVE = "MOVE";
	public static final String OPTIONS = "OPTIONS";
	public static final byte[] Continue = "HTTP/1.1 100 Continue\015\012\015\012"
			.getBytes();

	public static final String SESSIONID_NOT_CHECKED = "not checked";
	public static final String SESSIONID_URL = "url";
	public static final String SESSIONID_COOKIE = "cookie";
	public static final String SESSIONID_NONE = "none";

	private String method = null;
	private URI uri = null;
	private String version = null;

	private boolean localRequest = false;
	private HttpServer httpServer = null;
	private Socket connection;
	private HttpInputStream in;
	private InetAddrPort address;

	private UrlEncoded formParameters = null;

	private String scheme = "http";
	private String requestLine = null;
	private String protocolHostPort = null;
	private String serverName = null;
	private int serverPort = 0;
	private HttpResponse response = null;
	private BufferedReader reader = null;
	private int inputState = 0;

	private URI redirectParams = null;

	/**
	 * Construct received request.
	 */
	public HttpRequest(HttpServer httpServer, Socket connection,
			InetAddrPort address) throws IOException {
		this.httpServer = httpServer;
		this.connection = connection;
		this.in = new HttpInputStream(connection.getInputStream());
		this.address = address;

		// Get and Decode request header
		CharBuffer cb = null;
		do {
			cb = in.readCharBufferLine();
		} while (cb != null && cb.size == 0);

		if (cb == null)
			throw new IOException("EOF");
		decodeRequestLine(cb.chars, cb.size);

		// Handle version
		if (HttpHeader.HTTP_1_1.equals(version)) {
			// reset HTTP/1.1 version for faster matching
			version = HttpHeader.HTTP_1_1;
			// send continue
			OutputStream out = connection.getOutputStream();
			out.write(Continue);
			out.flush();
		}

		// Read headers
		super.read(in);

		if (Chunked.equals(getHeader(TransferEncoding))) {
			setHeader(ContentLength, null);
			this.in.chunking(true);
		} else {
			int content_length = getContentLength();
			if (content_length >= 0)
				in.setContentLength(content_length);
		}
	}

	/**
	 * Construct request to send
	 */
	public HttpRequest(HttpServer server, String method, String uri) {
		this.httpServer = server;
		this.method = method;
		this.uri = new URI(uri);
		version = HttpHeader.HTTP_1_0;
		protocolHostPort = "";
		localRequest = true;
	}

	/**
	 * Construct request to send
	 */
	public HttpRequest(HttpServer server, String method, URI uri) {
		this.httpServer = server;
		this.method = method;
		this.uri = uri;
		version = HttpHeader.HTTP_1_0;
		protocolHostPort = "";
		localRequest = true;
	}

	/**
	 * Get local request status
	 */
	public boolean isLocalRequest() {
		return localRequest;
	}

	/**
	 * Set associated response
	 */
	public void setHttpResponse(HttpResponse response) {
		this.response = response;
	}

	/**
	 * Get associated response
	 */
	public HttpResponse getHttpResponse() {
		return response;
	}

	/**
	 * Get the URI path minus any query strings with translations applied.
	 */
	public String getRequestPath() {
		if (uri == null)
			return null;
		return uri.getPath();
	}

	/**
	 * Set the URI path and redirect params
	 */
	public void setRequestPath(String path) {
		if (uri != null && path != null)
			uri.setPath(path);
	}

	/**
	 * Set the URI path and redirect params
	 */
	void setRequestPath(URI newuri) {
		if (uri != null)
			uri.setPath(newuri.getPath());
		redirectParams = newuri;
	}

	/**
	 * Return the HTTP request line as it was received
	 */
	public String getRequestLine() {
		if (requestLine == null)
			requestLine = method + " " + protocolHostPort + uri + " " + version;
		return requestLine;
	}

	/**
	 * Get the protocol version
	 */
	public String getVersion() {
		return version;
	}

	/**
	 * Set the request version
	 */
	public void setVersion(String version) {
		this.version = version;
	}

	/**
	 * decode Form Parameters After this call, form parameters may be fetch via
	 * the getParameter() method.
	 */
	public void decodeFormParameters() throws IOException {
		if (formParameters != null)
			return;

		String contentType = getContentType();
		if (contentType != null
				&& contentType.equals(HttpHeader.WwwFormUrlEncode)) {
			int contentLength = getContentLength();
			if (contentLength < 0)
				Log.w(this.getClass().getSimpleName(), "No contentLength for "
						+ HttpHeader.WwwFormUrlEncode);
			else {
				// Read all the post data
				InputStream in = getInputStream();
				byte[] postBytes = new byte[contentLength];
				int n = 0;
				while (n < contentLength) {
					int count = in.read(postBytes, n, contentLength - n);
					if (count < 0)
						throw new EOFException();
					n += count;
				}

				// Convert it to a hash table
				String content = new String(postBytes, "ISO8859_1");
				formParameters = new UrlEncoded(content);
			}
		}
	}

	/**
	 * Write the request header to an output stream
	 */
	public void write(OutputStream outstream) throws IOException {
		OutputStreamWriter out = new OutputStreamWriter(outstream, "ISO8859_1");

		out.write(method);
		out.write(' ');

		out.write(uri.toString());
		out.write(' ');
		out.write(version);
		out.write(CRLF);

		super.write(out, null);

		out.flush();
	}

	/**
	 * Put a parameter into the request.
	 */
	public void putParameter(String name, String value) {
		uri.put(name, value);
	}

	/**
	 * Put a multi-valued parameter into the request.
	 */
	public void putParameterValues(String name, String[] values) {
		uri.put(name, values);
	}

	/**
	 * Get the HttpServer of the request.
	 */
	public HttpServer getHttpServer() {
		return httpServer;
	}

	/**
	 * Get the HttpInputStream of the request.
	 */
	public HttpInputStream getHttpInputStream() {
		return in;
	}

	/**
	 * Get the length of any content in this request or -1 if not known.
	 */
	public int getContentLength() {
		return getIntHeader(ContentLength);
	}

	/**
	 * Return the MIME encoding type of any (form) content in this request or
	 * null if not known.
	 */
	public String getContentType() {
		return getHeaderNoParams(ContentType);
	}

	/**
	 * Get the actual protocol and version.
	 */
	public String getProtocol() {
		return version;
	}

	/**
	 * Get the host name of the server that received the request.
	 */
	public String getServerName() {
		if (serverName == null) {
			serverName = getHeader(Host);
			if (serverName != null && serverName.length() > 0) {
				int colon = serverName.indexOf(':');
				if (colon >= 0) {
					if (colon < serverName.length()) {
						try {
							serverPort = Integer.parseInt(serverName
									.substring(colon + 1));
						} catch (Exception e) {
							Log.w(this.getClass().getSimpleName(), e
									.getMessage());
							serverPort = 80;
						}
					}
					serverName = serverName.substring(0, colon);
				} else
					serverPort = 80;
			} else if (address != null && address.getInetAddress() != null)
				serverName = address.getInetAddress().getHostName();

			if (serverName == null) {
				try {
					serverName = InetAddress.getLocalHost().getHostName();
				} catch (java.net.UnknownHostException ignore) {
				}
			}

			int slash = serverName.indexOf("/");
			if (slash >= 0)
				serverName = serverName.substring(slash + 1);
		}

		return serverName;
	}

	/**
	 * Get the port number used in the request
	 */
	public int getServerPort() {
		if (serverName == null)
			getServerName();

		if (address != null && serverPort == 0)
			serverPort = address.getPort();

		return serverPort;
	}

	/**
	 * Get the remote IP address of the system that sent the request.
	 */
	public String getRemoteAddr() {
		if (connection != null)
			return connection.getInetAddress().getHostAddress();
		return "localhost";
	}

	/**
	 * Get the hostname of the system that sent the request.
	 */
	public String getRemoteHost() {
		String remoteHost = null;

		if (connection != null)
			remoteHost = connection.getInetAddress().getHostName();

		return remoteHost;
	}

	/**
	 * Get request input stream
	 */
	public synchronized InputStream getInputStream() {
		if (inputState != 0 && inputState != 1)
			throw new IllegalStateException();
		inputState = 1;
		return in;
	}

	/**
	 * Returns the multi-values of the specified parameter for the request.
	 */
	public String[] getParameterValues(String name) {
		Object values = null;

		if (redirectParams != null)
			values = redirectParams.getValues(name);
		if (values == null)
			values = uri.getValues(name);
		if (values == null && formParameters != null)
			values = formParameters.getValues(name);

		if (values != null && !(values instanceof String[])) {
			String[] a = new String[1];
			a[0] = values.toString();
			return a;
		}

		return (String[]) values;
	}

	/**
	 * Returns an enumeration of strings representing the parameter names for
	 * this request.
	 */
	public Enumeration getParameterNames() {
		if (formParameters == null && redirectParams == null)
			return uri.getParameterNames();

		Vector names = new Vector();
		Enumeration e = uri.getParameters().keys();
		while (e.hasMoreElements())
			names.addElement(e.nextElement());

		if (formParameters != null) {
			e = formParameters.keys();
			while (e.hasMoreElements())
				names.addElement(e.nextElement());
		}

		if (redirectParams != null) {
			e = redirectParams.getParameterNames();
			while (e.hasMoreElements())
				names.addElement(e.nextElement());
		}

		return names.elements();
	}

	/**
	 * Returns the scheme of the URL used in this request, for example "http",
	 * "https"
	 */
	public String getScheme() {
		return scheme;
	}

	/**
	 * Sets the scheme of the URL used in this request, for example "http",
	 * "https"
	 */
	public void setScheme(String scheme) {
		this.scheme = scheme;
	}

	/**
	 * Returns the character set encoding for the input of this request.
	 */
	public String getCharacterEncoding() {
		String s = getHeader(ContentType);
		try {
			int i1 = s.indexOf("charset=", s.indexOf(';')) + 8;
			int i2 = s.indexOf(' ', i1);
			String charset = (0 < i2) ? s.substring(i1, i2) : s.substring(i1);
			if (charset.equalsIgnoreCase("US-ASCII"))
				return "ISO8859_1";
			if (charset.equalsIgnoreCase("ISO-8859-1"))
				return "ISO8859_1";
			if (charset.equalsIgnoreCase("ISO-8859-2"))
				return "ISO8859_2";
			if (charset.equalsIgnoreCase("ISO-8859-3"))
				return "ISO8859_3";
			if (charset.equalsIgnoreCase("ISO-8859-4"))
				return "ISO8859_4";
			if (charset.equalsIgnoreCase("ISO-8859-5"))
				return "ISO8859_5";
			if (charset.equalsIgnoreCase("ISO-8859-6"))
				return "ISO8859_6";
			if (charset.equalsIgnoreCase("ISO-8859-7"))
				return "ISO8859_7";
			if (charset.equalsIgnoreCase("ISO-8859-8"))
				return "ISO8859_8";
			if (charset.equalsIgnoreCase("ISO-8859-9"))
				return "ISO8859_9";
			if (charset.equalsIgnoreCase("ISO-2022-JP"))
				return "ISO2022JP";
			if (charset.equalsIgnoreCase("ISO-2022-KR"))
				return "ISO2022KR";
			return charset;
		} catch (Exception e) {
			return "ISO8859_1";
		}
	}

	/**
	 * Get the HTTP method for this request.
	 */
	public String getMethod() {
		return method;
	}

	/**
	 * Get the full URI.
	 */
	public String getRequestURI() {
		String strUri = this.scheme + "://";
		strUri += this.getServerName();
		strUri += ":";
		strUri += this.getServerPort();
		strUri += uri.getPath();
		
		if ( uri.getQuery() != null && !"".equals(uri.getQuery()))
			strUri += "?" + uri.getQuery();
		
		return strUri;
	}

	public synchronized BufferedReader getReader() {
		if (inputState != 0 && inputState != 2)
			throw new IllegalStateException();
		if (reader == null) {
			try {
				reader = new BufferedReader(new InputStreamReader(
						getInputStream(), "ISO8859_1"));
			} catch (UnsupportedEncodingException e) {
				Log.e(this.getClass().getSimpleName(), e.getMessage());
				reader = new BufferedReader(new InputStreamReader(
						getInputStream()));
			}
			inputState = 2;
		}
		return reader;
	}

	void decodeRequestLine(char[] buf, int len) throws IOException {
		// Search for first space separated chunk
		int s1 = -1, s2 = -1, s3 = -1;
		int state = 0;
		startloop: for (int i = 0; i < len; i++) {
			char c = buf[i];
			switch (state) {
			case 0: // leading white
				if (c == ' ')
					continue;
				state = 1;
				s1 = i;

			case 1: // reading method
				if (c == ' ')
					state = 2;
				else
					s2 = i;
				continue;

			case 2: // skip whitespace after method
				s3 = i;
				if (c != ' ')
					break startloop;
			}
		}

		// Search for first space separated chunk
		int e1 = -1, e2 = -1, e3 = -1;
		state = 0;
		endloop: for (int i = len; i-- > 0;) {
			char c = buf[i];
			switch (state) {
			case 0: // leading white
				if (c == ' ')
					continue;
				state = 1;
				e1 = i;

			case 1: // reading method
				if (c == ' ')
					state = 2;
				else
					e2 = i;
				continue;

			case 2: // skip whitespace after method
				e3 = i;
				if (c != ' ')
					break endloop;
			}
		}

		// Check sufficient params
		if (s3 < 0 || e1 < 0 || e3 < s2)
			throw new IOException("Bad requestline");

		// get method
		method = new String(buf, s1, s2 - s1 + 1);

		// get version
		if (s2 != e3 || s3 != e2) {
			for (int i = e1; i <= e2; i++)
				if (buf[i] > 'a' && buf[i] < 'z')
					buf[i] = (char) (buf[i] - 'a' + 'A');
			version = new String(buf, e2, e1 - e2 + 1);
		} else {
			// missing version
			version = HttpHeader.HTTP_1_0;
			e3 = e1;
		}

		// rebuild requestline
		StringBuffer rl = new StringBuffer(len);
		rl.append(buf, s1, s2 - s1 + 2);
		rl.append(buf, s3, e3 - s3 + 1);
		rl.append(" ");
		rl.append(version);
		requestLine = rl.toString();

		// handle URI
		String uris = null;
		if (buf[s3] != '/') {
			// look for //
			for (int i = s3; i < e3; i++) {
				if (buf[i] == '/') {
					if (buf[i + 1] != '/')
						break;

					// look for next /
					for (int j = i + 2; j <= e3; j++) {
						if (buf[j] == '/') {
							protocolHostPort = new String(buf, s3, j - s3);
							uris = new String(buf, j, e3 - j + 1);
							break;
						}
					}
					break;
				}
			}
		}
		if (uris == null) {
			protocolHostPort = "";
			uris = new String(buf, s3, e3 - s3 + 1);
		}

		uri = new URI(uris);
	}

	/**
	 * Destroy the request. Help the garbage collector by null everything that
	 * we can.
	 */
	public void destroy() {
		method = null;
		uri = null;
		version = null;
		httpServer = null;
		connection = null;
		in = null;
		address = null;
		if (formParameters != null) {
			formParameters.clear();
			formParameters = null;
		}
		requestLine = null;
		protocolHostPort = null;
		serverName = null;
		response = null;
		reader = null;
		super.destroy();
	}

}