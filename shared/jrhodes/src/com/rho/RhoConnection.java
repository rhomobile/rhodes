package com.rho;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.rho.util.DateTimeTokenizer;
import com.rho.util.Properties;
import com.rho.util.URI;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RhoSupport;

public abstract class RhoConnection {
	
	//HTTP METHODS
	public final static String GET  = "GET";
	public final static String POST = "POST";
	public final static String HEAD = "HEAD";
	
	//HTTP RESPONSE CODES
	public static int HTTP_OK = 200;
	public static int HTTP_MOVED_TEMPORARILY = 302;
	public static int HTTP_MOVED_PERMANENTLY = 301;
	public static int HTTP_BAD_REQUEST = 400;	
	public static int HTTP_NOT_FOUND = 404;
	public static int HTTP_UNAUTHORIZED = 401;
	public static int HTTP_INTERNAL_ERROR = 500;
	
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
	private /*ByteArray*/InputStream responseData = null;
	private ByteArrayOutputStream postData = new ByteArrayOutputStream();
	
	/** Construct connection using URI **/
    
	public RhoConnection(String uri) {
    	this.uri = new URI(uri);
    }
	
    public RhoConnection(URI uri) {
    	this.uri = uri;
    }
    
    /**
     * These methods must be implemented in subclases
     */
    
    protected abstract void showGeoLocation();
    
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
            return DateTimeTokenizer.parse(getHeaderField(name));
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

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#getHost()
	 */
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

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#getRequestMethod()
	 */
	public String getRequestMethod() {
		log("getRequestMethod: " + method);
		return method;
	}

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#getRequestProperty(java.lang.String)
	 */
	public String getRequestProperty(String key) {
		log("getRequestProperty: " + key);
        return reqHeaders.getPropertyIgnoreCase(key);
	}

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#getResponseCode()
	 */
	public int getResponseCode() throws IOException {
		log("getResponseCode" + responseCode);
		processRequest();
		return responseCode;
	}

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#getResponseMessage()
	 */
	public String getResponseMessage() throws IOException {
		log("getResponseMessage: " + responseMsg);
		processRequest();
		return responseMsg;
	}

	public String getURL() {
		log("getURL: " + uri.toString());
		return uri.toString();
	}

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#setRequestMethod(java.lang.String)
	 */
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

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#getLength()
	 */
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

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#openDataInputStream()
	 */
	public DataInputStream openDataInputStream() throws IOException {
		return new DataInputStream(openInputStream());
	}

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#openInputStream()
	 */
	public InputStream openInputStream() throws IOException {
		processRequest();
		return responseData;
	}

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#close()
	 */
	public void close() throws IOException {
	}

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#openDataOutputStream()
	 */
	public DataOutputStream openDataOutputStream() throws IOException {
		return new DataOutputStream(postData);
	}

	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#openOutputStream()
	 */
	public OutputStream openOutputStream() throws IOException {
		return postData;
	}
	
	static private class UrlParser{
		String strPath;
		int nStart = 0;
		UrlParser( String url ){
			strPath = url;
			nStart = strPath.charAt(0) == '/' ? 1 : 0;
		}
		public boolean isEnd(){ return nStart >= strPath.length(); }
		public String next(){
			if (  isEnd() )
				return "";
			
			int nEnd = strPath.indexOf('/',nStart);
			if ( nEnd < 0 )
				nEnd = strPath.length();
			
			String res = strPath.substring(nStart, nEnd);
			nStart = nEnd+1;
			return res;
		}
	
	}
	
	void respondMoved( String location ){
		responseCode = HTTP_MOVED_PERMANENTLY;
		responseMsg = "Moved Permanently";
		resHeaders.addProperty("Location", location);
		contentLength = 0;
	}

	void respondOK(){
		responseCode = HTTP_OK;
		responseMsg = "Success";
		contentLength = 0;
	}
	
	void respondNotFound( String strError ){
		responseCode = HTTP_NOT_FOUND;
		responseMsg = "Not found";
		if ( strError != null && strError.length() != 0 )
			responseMsg += ".Error: " + strError;
		
		contentLength = 0;
	}
	
	static String getContentType( String path ){
		int nPoint = path.lastIndexOf('.');
		String strExt = "";
		if ( nPoint > 0 )
			strExt = path.substring(nPoint+1);
		
		if ( strExt.equals("png") )
			return "image/png";
		else if ( strExt.equals("js") )
			return "application/javascript";
		else if ( strExt.equals("css") )
			return "text/css";
		else if ( strExt.equals("gif") )
			return "image/gif";
		else if ( strExt.equals("html") || strExt.equals("htm") )
			return "text/html";
		
		return "";
	}
	
	static final String[] m_arIndexes = {"index_erb", "index.html", "index.htm"};
	
	public static int findIndex(String strUrl){
		String filename;
		int nLastSlash = strUrl.lastIndexOf('/');
		if ( nLastSlash >= 0 )
			filename = strUrl.substring(nLastSlash+1);
		else
			filename = strUrl;
		
		for( int i = 0; i < m_arIndexes.length; i++ ){
			if ( filename.equalsIgnoreCase(m_arIndexes[i]) )
				return i;
		}
		
		return -1;
	}

	protected boolean httpGetIndexFile(){
		
		String strPath = uri.getPath();
		if ( !strPath.startsWith("/apps") )
			strPath = "/apps" + strPath;
		
		String strIndex = null;
		String slash = "";
		if ( strPath!=null && strPath.length() > 0 )
			slash = strPath.charAt(strPath.length()-1) == '/' ? "" : "/";
		
		for( int i = 0; i < m_arIndexes.length; i++ ){
			String name = strPath + slash + m_arIndexes[i];
			if ( RhoSupport.findClass(name) != null || RhoRuby.loadFile(name) != null ){
				strIndex = name;
				break;
			}
		}
		
		if ( strIndex == null )
			return false;
		
		respondMoved(strIndex);
		return true;
	}

	protected boolean httpServeFile(String strContType)throws IOException{
		
		String strPath = uri.getPath();
		if ( !strPath.startsWith("/apps") )
			strPath = "/apps" + strPath; 
			
		if ( strContType.equals("application/javascript")){
			//responseData = RhoRuby.loadFile(strPath);
			//if ( responseData == null )
			String str = "";
			responseData = new ByteArrayInputStream(str.getBytes());
		}
		else	
			responseData = RhoRuby.loadFile(strPath);
	
		if (responseData== null)
			return false;
		
		if ( strContType.length() > 0 )
			resHeaders.addProperty("Content-Type", strContType );
		
		contentLength = responseData.available(); 
		resHeaders.addProperty("Content-Length", Integer.toString( contentLength ) );
		
		return true;
	}
	
	protected boolean httpGetFile()throws IOException{
		
		String strPath = uri.getPath();
		  if ( !strPath.startsWith("/apps") )
		   strPath = "/apps" + strPath;
		  
		String strContType = getContentType(strPath);
		if ( strContType.length() == 0 )
		{
			String strTemp = strPath;
			if ( strTemp.length() == 0 || strTemp.charAt(strTemp.length()-1)!='/')
				strTemp += '/';
	
			if( RhoSupport.findClass(strTemp + "controller") != null )
				return false;
			
			int nPos = findIndex(strPath);
			if ( nPos >= 0 ){
				RubyValue res = RhoRuby.processIndexRequest( 
						strPath + (nPos == 0 ? ".iseq" : "") ); //erb-compiled should load from class
				processResponse(res);
				return true;
			}
	
			if( httpGetIndexFile() )
				return true;
		}
		
		return httpServeFile(strContType);
	}
	
	protected boolean checkRhoExtensions(String application, String model ){
		if ( application.equalsIgnoreCase("AppManager") ){
			//TODO: AppManager
			respondOK();
			return true;
		}else if ( application.equalsIgnoreCase("system") ){
			if ( model.equalsIgnoreCase("geolocation") ){
				showGeoLocation();
				return true;
			}else if ( model.equalsIgnoreCase("syncdb") ){
				com.rho.sync.SyncEngine.wakeUp();
				respondOK();
				return true;
			}
		}else if ( application.equalsIgnoreCase("shared") )
			return false;
		
		return false;
	}
	
	protected boolean dispatch()throws IOException{
		
		String strPath = uri.getPath();
		  if ( !strPath.startsWith("/apps") )
		   strPath = "/apps" + strPath;
		
		UrlParser up = new UrlParser(strPath);
		String apps = up.next();
		String application;
		if ( apps.equalsIgnoreCase("apps") )
			application = up.next();
		else
			application = apps;
		
		String model = up.next();
		
		if ( model.length() == 0 )
			return false;
		
		if ( checkRhoExtensions(application, model ) )
			return true;
		
		Properties reqHash = new Properties();
		
		String actionid = up.next();
		String actionnext = up.next();
		if ( actionid.length() > 0 ){
			if ( actionid.length() > 2 && 
				 actionid.charAt(0)=='{' && actionid.charAt(actionid.length()-1)=='}' ){
				reqHash.setProperty( "id", actionid);
				reqHash.setProperty( "action", actionnext);
			}else{
				reqHash.setProperty( "id", actionnext);
				reqHash.setProperty( "action", actionid);
			}
		}
		reqHash.setProperty( "application",application);
		reqHash.setProperty( "model", model);

		reqHash.setProperty("request-method", this.method);
		reqHash.setProperty("request-uri", uri.toString());
		reqHash.setProperty("request-query", uri.getQueryString());
		
		if ( postData != null && postData.size() > 0 ){
			log(postData.toString());
			reqHash.setProperty("request-body", postData.toString());
		}
		
		RubyValue res = RhoRuby.processRequest( reqHash, reqHeaders, resHeaders);
		processResponse(res);
		
		return true;
	}
	
	/* (non-Javadoc)
	 * @see com.rho.IHttpConnection#processRequest()
	 */
	public void processRequest()  throws IOException{
		if (!requestProcessed) {
			String strErr = "";
			
			if ( "GET".equals(this.method) && httpGetFile() ){
				
			}else if ( dispatch() ){
			}else{
				respondNotFound(strErr);
			}
			
			requestProcessed = true;
		}
	}

	private void processResponse(RubyValue res){
		if ( res != null && res != RubyConstant.QNIL && res instanceof RubyHash ){
			RubyHash resHash = (RubyHash)res;
			RubyValue resBody = null;
			
			RubyArray arKeys = resHash.keys();
			RubyArray arValues = resHash.values();
			for( int i = 0; i < arKeys.size(); i++ ){
				String strKey = arKeys.get(i).toString();
				if ( strKey.equals("request-body") )
					resBody = arValues.get(i);
				else if (strKey.equals("status"))
					responseCode = arValues.get(i).toInt();
				else if (strKey.equals("message"))
					responseMsg = arValues.get(i).toString();
				else	
					resHeaders.addProperty( strKey, arValues.get(i).toString() );
					
			}
			if ( resBody != null && resBody != RubyConstant.QNIL ){
				String strBody = resBody.toRubyString().toString();
				log(strBody);
				
				responseData = new ByteArrayInputStream(strBody.getBytes()); 
				if ( responseData != null )
					contentLength = Integer.parseInt(resHeaders.getPropertyIgnoreCase("Content-Length"));
			}
		}
	}
	
	private void log(String txt) {
		System.out.println(txt);
	}
}


