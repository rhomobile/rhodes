package rhomobile;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.HttpConnection;

public class RhoConnection implements HttpConnection {
	/** Request URI **/
	URI uri;
	/** Method - GET, POST, HEAD **/
	String method;
    /** Numeric code returned from HTTP response header. */
    protected int responseCode = 200;
    /** Message string from HTTP response header. */
    protected String responseMsg = "OK";
    /** Content-Length from response header, or -1 if missing. */
    private int contentLength = -1;
    /** Collection of request headers as name/value pairs. */
    protected Properties reqHeaders = new Properties();
    /** Collection of response headers as name/value pairs. */
    protected Properties resHeaders = new Properties();
    /** Request state **/
    protected boolean requestProcessed = false;
    /** Input/Output streams **/
	private ByteArrayInputStream responseData = null;
	private ByteArrayOutputStream postData = new ByteArrayOutputStream();
	
	/** Construct connection using URI **/
    
    public RhoConnection(URI uri) {
    	this.uri = uri;
    }
    
	public long getDate() throws IOException {
		log("getDate");
		return getHeaderFieldDate("date", 0);
	}

	public long getExpiration() throws IOException {
		log("getExpiration");
		return getHeaderFieldDate("expires", 0);
	}

	public String getFile() {
		log("getFile");
		if (uri!=null) {
			String path = uri.getPath();
			if (path!=null) {
				int s0 = path.lastIndexOf('/');
				int s1 = path.lastIndexOf('\\');
				if (s1>s0) s0=s1;
				if (s0<0) s0 = 0;
				return path.substring(s0);
			}
		}
		return null;
	}

	public String getHeaderField(String name) throws IOException {
		log("getHeaderField: " + name);
		processRequest();
		return resHeaders.getPropertyIgnoreCase(name);
	}

	public String getHeaderField(int index) throws IOException {
		log("getHeaderField: " + index);
		processRequest();
        if (index >= resHeaders.size()) {
            return null;
        }
        return resHeaders.getValueAt(index);
	}

	public long getHeaderFieldDate(String name, long def) throws IOException {
		log("getHeaderFieldDate: " + name);
		processRequest();
        try {
            return DateParser.parse(getHeaderField(name));
        } catch (NumberFormatException nfe) {
            // fall through
        } catch (IllegalArgumentException iae) {
            // fall through
        } catch (NullPointerException npe) {
            // fall through
        }        
        return def;
	}

	public int getHeaderFieldInt(String name, int def) throws IOException {
		log("getHeaderFieldInt: " + name);
		processRequest();
        try {
            return Integer.parseInt(getHeaderField(name));
        } catch (IllegalArgumentException iae) {
            // fall through
        } catch (NullPointerException npe) {
            // fall through
        }
        return def;
	}

	public String getHeaderFieldKey(int index) throws IOException {
		log("getHeaderFieldKey: " + index);
		processRequest();
        if (index >= resHeaders.size())
            return null;
        return ((String)(resHeaders.getKeyAt(index)));
	}

	public String getHost() {
		log("getHost: " + uri.getHost());
		return uri.getHost();
	}

	public long getLastModified() throws IOException {
		log("getLastModified");
		return getHeaderFieldDate("last-modified", 0);
	}

	public int getPort() {
		log("getPort: " + uri.getPort());
		return uri.getPort();
	}

	public String getProtocol() {
		log("getProtocol: " + uri.getScheme());
		return uri.getScheme();
	}

	public String getQuery() {
		log("getQuery: " + uri.getQueryString());
		return uri.getQueryString();
	}

	public String getRef() {
		log("getRef: " + uri.getFragment());
		return uri.getFragment();
	}

	public String getRequestMethod() {
		log("getRequestMethod: " + method);
		return method;
	}

	public String getRequestProperty(String key) {
		log("getRequestProperty: " + key);
        return reqHeaders.getPropertyIgnoreCase(key);
	}

	public int getResponseCode() throws IOException {
		log("getResponseCode" + responseCode);
		processRequest();
		return responseCode;
	}

	public String getResponseMessage() throws IOException {
		log("getResponseMessage: " + responseMsg);
		processRequest();
		return responseMsg;
	}

	public String getURL() {
		log("getURL: " + uri.toString());
		return uri.toString();
	}

	public void setRequestMethod(String method) throws IOException {
		log("setRequestMethod: " + method);
        /*
		 * The request method can not be changed once the output stream has been
		 * opened.
		 */
// if (streamConnection != null) {
// throw new IOException("connection already open");
// }

        if (!method.equals(HEAD) && 
            !method.equals(GET) && 
            !method.equals(POST)) {
            throw new IOException("unsupported method: " + method);
        }

        this.method = method;
	}

	public void setRequestProperty(String key, String value) throws IOException {
		log("setRequestProperty: key = " + key + "; value = " + value);
		int index = 0;

		/*
		 * The request headers can not be changed once the output stream has
		 * been opened.
		 */
// if (streamConnection != null) {
// throw new IOException("connection already open");
// }

		// Look to see if a programmer embedded any extra fields.
		for (;;) {
			index = value.indexOf("\r\n", index);

			if (index == -1) {
				break;
			}

			// Allow legal header value continuations. CRLF + (SP|HT)
			index += 2;

			if (index >= value.length() || (value.charAt(index) != ' ' &&
					value.charAt(index) != '\t')) {
				// illegal values passed for properties - raise an exception
				throw new IllegalArgumentException("illegal value found");
			}
		}

		setRequestField(key, value);
	}

    /**
	 * Add the named field to the list of request fields. This method is where a
	 * subclass should override properties.
	 * 
	 * @param key
	 *            key for the request header field.
	 * @param value
	 *            the value for the request header field.
	 */
    protected void setRequestField(String key, String value) {
		log("setRequestField: key = " + key + "; value = " + value);

        /*
		 * If application setRequestProperties("Connection", "close") then we
		 * need to know this & take appropriate default close action
		 */
// if ((key.equalsIgnoreCase("connection")) &&
// (value.equalsIgnoreCase("close"))) {
// ConnectionCloseFlag = true;
// }

        /*
		 * Ref . Section 3.6 of RFC2616 : All transfer-coding values are
		 * case-insensitive.
		 */
// if ((key.equalsIgnoreCase("transfer-encoding")) &&
// (value.equalsIgnoreCase("chunked"))) {
// chunkedOut = true;
// }

        reqHeaders.setPropertyIgnoreCase(key, value);
    }
    
	public String getEncoding() {
		log("getEncloding");
        try {
            return getHeaderField("content-encoding");
        } catch (IOException x) {
            return null;
        }
	}

	public long getLength() {
		log("getLength: " + contentLength);
		try {
			processRequest();
		} catch (IOException ioe) {
			// Fall through to return -1 for length
		}
		return contentLength;
	}

	public String getType() {
		log("getType");
        try {
            return getHeaderField("content-type");
        } catch (IOException x) {
            return null;
        }
	}

	public DataInputStream openDataInputStream() throws IOException {
		return new DataInputStream(openInputStream());
	}

	public InputStream openInputStream() throws IOException {
		processRequest();
		return responseData;
	}

	public void close() throws IOException {
	}

	public DataOutputStream openDataOutputStream() throws IOException {
		return new DataOutputStream(postData);
	}

	public OutputStream openOutputStream() throws IOException {
		return postData;
	}
	
	protected void processRequest() throws IOException {
		if (!requestProcessed) {
			log("Processing request");
			String messageText = "<html><head></head><body>"+
			"<a href=\"http://google.com\">Google</a><br/>"+
			"This is the message</body>";
			responseData = new ByteArrayInputStream(messageText.getBytes());
			contentLength = messageText.length();
	    	resHeaders.addProperty("content-type", "text/html");
			requestProcessed = true;
		}
	}
	
	private void log(String txt) {
		System.out.println(txt);
	}
}

