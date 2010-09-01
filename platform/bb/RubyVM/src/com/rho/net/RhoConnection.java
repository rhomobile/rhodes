package com.rho.net;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Calendar;

import com.rho.net.IHttpConnection;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RhoSupport;
import com.rho.net.URI;
import com.rho.sync.SyncThread;
import com.rho.*;
import com.rho.file.SimpleFile;

public class RhoConnection implements IHttpConnection {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhoConnection");
	
	/** Request URI **/
	URI uri;
	String  url_external;
	
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
	private SimpleFile m_file = null;
	
	/** Construct connection using URI **/
    
    public RhoConnection(URI _uri) 
    {
    	url_external = _uri.toString();
    	uri = new URI(url_external);
    
    	if ( !uri.getPath().startsWith("/apps") )
    		uri.setPath("/apps" + uri.getPath());
    	else
    		uri.setPath(uri.getPath());
    }
    
    public void resetUrl(String url)
    {
    	url_external = url;
    	uri = new URI(url_external);
    
    	if ( !uri.getPath().startsWith("/apps") )
    		uri.setPath("/apps" + uri.getPath());
    	else
    		uri.setPath(uri.getPath());
    	
    	method = "";
        responseCode = 200;
        responseMsg = "OK";
        contentLength = -1;
        reqHeaders.clear();
        resHeaders.clear();
        requestProcessed = false;
        
        try{
        	clean();
        }catch(IOException exc)
        {
        	LOG.ERROR("clean failed.", exc);
        }
    }

	private void clean() throws IOException 
	{
		if ( m_file != null ){
			m_file.close();
			m_file = null;
		}else if (responseData != null)
			responseData.close();
		
		responseData = null;
		
		if ( postData != null )
			postData.close();
	}
	
	public void close() throws IOException 
	{
		clean();
		LOG.TRACE("Close browser connection.");
	}
    
	public Object getNativeConnection() {
		throw new RuntimeException("getNativeConnection - Not implemented");
	}
    
	public long getDate() throws IOException {
		LOG.TRACE("getDate");
		return getHeaderFieldDate("date", 0);
	}

	public long getExpiration() throws IOException {
		LOG.TRACE("getExpiration");
		return getHeaderFieldDate("expires", 0);
	}

	public String getFile() {
		LOG.TRACE("getFile");
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
		LOG.TRACE("getHeaderField: " + name);
		processRequest();
		return resHeaders.getPropertyIgnoreCase(name);
	}

	public String getHeaderField(int index) throws IOException {
		LOG.TRACE("getHeaderField: " + index);
		processRequest();
        if (index >= resHeaders.size()) {
            return null;
        }
        return resHeaders.getValueAt(index);
	}

	public long getHeaderFieldDate(String name, long def) throws IOException {
		LOG.TRACE("getHeaderFieldDate: " + name);
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
		LOG.TRACE("getHeaderFieldInt: " + name);
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
		LOG.TRACE("getHeaderFieldKey: " + index);
		processRequest();
        if (index >= resHeaders.size())
            return null;
        return ((String)(resHeaders.getKeyAt(index)));
	}

	public String getHost() {
		LOG.TRACE("getHost: " + uri.getHost());
		return uri.getHost();
	}

	public long getLastModified() throws IOException {
		LOG.TRACE("getLastModified");
		return getHeaderFieldDate("last-modified", 0);
	}

	public int getPort() {
		LOG.TRACE("getPort: " + uri.getPort());
		return uri.getPort();
	}

	public String getProtocol() {
		LOG.TRACE("getProtocol: " + uri.getScheme());
		return uri.getScheme();
	}

	public String getQuery() {
		LOG.TRACE("getQuery: " + uri.getQueryString());
		return uri.getQueryString();
	}

	public String getRef() {
		LOG.TRACE("getRef: " + uri.getFragment());
		return uri.getFragment();
	}

	public String getRequestMethod() {
		LOG.TRACE("getRequestMethod: " + method);
		return method;
	}

	public String getRequestProperty(String key) {
		LOG.TRACE("getRequestProperty: " + key);
        return reqHeaders.getPropertyIgnoreCase(key);
	}

	public int getResponseCode() throws IOException {
		processRequest();
		LOG.TRACE("getResponseCode" + responseCode);
		
		return responseCode;
	}

	public String getResponseMessage() throws IOException {
		LOG.TRACE("getResponseMessage: " + responseMsg);
		processRequest();
		return responseMsg;
	}

	public String getURL() {
		LOG.TRACE("getURL: " + url_external);
		return url_external;
	}

	public void setRequestMethod(String method) throws IOException {
		LOG.TRACE("setRequestMethod: " + method);
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
		LOG.TRACE("setRequestProperty: key = " + key + "; value = " + value);
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
    	LOG.TRACE("setRequestField: key = " + key + "; value = " + value);

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
		LOG.TRACE("getEncloding");
        try {
            return getHeaderField("content-encoding");
        } catch (IOException x) {
            return null;
        }
	}

	public long getLength() {
		LOG.TRACE("getLength: " + contentLength);
		try {
			processRequest();
		} catch (IOException ioe) {
			// Fall through to return -1 for length
		}
		return contentLength;
	}

	public String getType() {
		LOG.TRACE("getType");
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

	public DataOutputStream openDataOutputStream() throws IOException {
		return new DataOutputStream(postData);
	}

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
				return null;
			
			int nEnd = strPath.indexOf('/',nStart);
			if ( nEnd < 0 )
				nEnd = strPath.length();
			
			String res = strPath.substring(nStart, nEnd);
			nStart = nEnd+1;
			return res;
		}
	
	}
	
	void respondMoved( String location ){
		responseCode = HTTP_MOVED_PERM;
		responseMsg = "Moved Permanently";
		
		String strLoc = location;
		if ( strLoc.startsWith("/apps"))
			strLoc = strLoc.substring(5);
		
		String strQuery = uri.getQueryString();
		if ( strQuery != null && strQuery.length() > 0 )
			strLoc += "?" + strQuery;
		
		resHeaders.addProperty("Location", strLoc );
		contentLength = 0;
	}

	void respondOK(){
		responseCode = HTTP_OK;
		responseMsg = "Success";
		contentLength = 0;
	}

	void respondNotModified(){
		responseCode = HTTP_NOTMODIFIED;
		responseMsg = "Success";
		contentLength = 0;
	}
	
	void respondNotFound( String strError ){
		responseCode = HTTP_NOT_FOUND;
		responseMsg = "Not found";
		if ( strError != null && strError.length() != 0 )
			responseMsg += ".Error: " + strError;
		
		String strBody = "Page not found: " + uri.getPath(); 
		
		contentLength = strBody.length();
		responseData = new ByteArrayInputStream(strBody.getBytes());
		
		resHeaders.addProperty("Content-Type", "text/html" );
		resHeaders.addProperty("Content-Length", Integer.toString( contentLength ) );
	}
	
	String getContentType(){
		String contType = reqHeaders.getProperty("Content-Type");
		if ( contType == null || contType.length() == 0 )
			contType = reqHeaders.getProperty("content-type");
		
		if ( contType != null && contType.length() > 0 )
			return contType;
			
		String path = uri.getPath();
		int nPoint = path.lastIndexOf('.');
		String strExt = "";
		if ( nPoint > 0 )
			strExt = path.substring(nPoint+1);
		
		if ( strExt.equals("png") )
			return "image/png";
		else if ( strExt.equals("jpeg") )
			return "image/jpeg";
		else if ( strExt.equals("jpg") )
			return "image/jpg";
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
	
	static final String[] m_arIndexes = {"index_erb.iseq", "index.html", "index.htm"};
	
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
		String strIndex = null;
		String slash = "";
		if ( uri.getPath()!=null && uri.getPath().length() > 0 )
			slash = uri.getPath().charAt(uri.getPath().length()-1) == '/' ? "" : "/";
		
		for( int i = 0; i < m_arIndexes.length; i++ ){
			String name = uri.getPath() + slash + m_arIndexes[i];
			String nameClass = name;
			if ( nameClass.endsWith(".iseq"))
				nameClass = nameClass.substring(0, nameClass.length()-5);
			
			if ( RhoSupport.findClass(nameClass) != null || RhoRuby.resourceFileExists(name)  ){
				strIndex = name;
				break;
			}
		}
		
		if ( strIndex == null )
			return false;
		
		respondMoved(strIndex);
		return true;
	}

	private String getLocalHttpTimeString()
	{
		final String[] months= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		final String[] wdays= { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
		
		//Thu, 01 Dec 2010 16:00:00 GMT
		Calendar time = Calendar.getInstance();
		String strTime = "";
		strTime += wdays[time.get(Calendar.DAY_OF_WEEK)-1] + ", " +
			time.get(Calendar.DATE) + " " +
			months[time.get(Calendar.MONTH)] + " " +
			time.get(Calendar.YEAR) + " " + 
			time.get(Calendar.HOUR_OF_DAY) + ":" + 
			time.get(Calendar.MINUTE) +	":" + 
			time.get(Calendar.SECOND) + " " + 
			"GMT";
		
		return strTime;
	}
	
	protected boolean isDbFilesPath(String strPath)
	{
		return strPath.startsWith("/apps/app/db/db-files") || strPath.startsWith("/apps/db/db-files");
	}
	
	protected boolean httpServeFile(String strContType)throws IOException
	{
		String strPath = uri.getPath();
		//if ( !strPath.startsWith("/apps") )
		//	strPath = "/apps" + strPath; 

		LOG.TRACE("httpServeFile: " + strPath);
		
		if ( !isDbFilesPath(strPath) )
		{
			if ( strContType.equals("application/javascript")){
				responseData = RhoRuby.loadFile(strPath);
				if ( responseData == null ){
					String str = "";
					responseData = new ByteArrayInputStream(str.getBytes());
				}
			}
			else	
				responseData = RhoRuby.loadFile(strPath);
		}else
		{
			if ( strPath.startsWith("/apps/app/db/db-files") )
				strPath = strPath.substring(9);// remove /apps/app
			else
				strPath = strPath.substring(5);// remove /apps
		}
		
		if (responseData == null){
			  
			SimpleFile file = null;
			try{
				file = RhoClassFactory.createFile();
				String strFileName = strPath;
//				if ( strFileName.startsWith("/apps") )
//					strFileName = strPath.substring(5);
				
				file.open(strFileName, true, true);
				responseData = file.getInputStream();
				if (responseData != null) {
					contentLength = (int) file.length();
				}
				m_file = file;
			}catch(Exception exc){
				if ( file != null )
					file.close();
			}			
		} else {
			if (responseData != null) {
				contentLength = responseData.available(); 
			}
		}
		
		if (responseData== null)
			return false;
		
		if ( strContType.length() > 0 )
			resHeaders.addProperty("Content-Type", strContType );
		
		resHeaders.addProperty("Content-Length", Integer.toString( contentLength ) );
		
		//resHeaders.addProperty("Date",getLocalHttpTimeString());
		//resHeaders.addProperty("Cache-control", "public, max-age=3600" );
		//resHeaders.addProperty("Expires", "Thu, 01 Dec 2010 16:00:00 GMT" );
		
		return true;
	}
	
	private boolean isKnownExtension(String strPath)
	{
		int nDot = strPath.lastIndexOf('.');
		if ( nDot >= 0 )
		{
			String strExt = strPath.substring(nDot+1);
			return strExt.equalsIgnoreCase("png") || strExt.equalsIgnoreCase("jpg") ||
				strExt.equalsIgnoreCase("css") || strExt.equalsIgnoreCase("js");
		}
		
		return false;
	}
	
	protected boolean httpGetFile(String strContType)throws IOException
	{
		if ( !isDbFilesPath(uri.getPath()) && !isKnownExtension(uri.getPath()) && strContType.length() == 0 )
		{
			String strTemp = FilePath.join(uri.getPath(), "/");
	
			if( RhoSupport.findClass(strTemp + "controller") != null )
				return false;

			int nPos = findIndex(uri.getPath());
			if ( nPos >= 0 ){
				String url = uri.getPath();// + (nPos == 0 ? ".iseq" : "");
				RubyValue res = RhoRuby.processIndexRequest(url);//erb-compiled should load from class
				processResponse(res);
				
				RhodesApp.getInstance().keepLastVisitedUrl(url_external);
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
			}else if ( model.equalsIgnoreCase("loadserversources") ){
				RhoAppAdapter.loadServerSources(postData.toString());
				return true;
			}

		}else if ( application.equalsIgnoreCase("shared") )
			return false;
		
		return false;
	}
	
	void showGeoLocation(){
		String location = "";
		try {
			IRhoRubyHelper helper = RhoClassFactory.createRhoRubyHelper();
			location = helper.getGeoLocationText();
		}catch(Exception exc)
		{
			LOG.ERROR("getGeoLocationText failed", exc);
		}
		respondOK();
		
		contentLength = location.length();
		responseData = new ByteArrayInputStream(location.getBytes());
		resHeaders.addProperty("Content-Type", "text/html" );
		resHeaders.addProperty("Content-Length", Integer.toString( contentLength ) );
	}
	
	protected boolean dispatch()throws IOException
	{
		LOG.INFO("dispatch start : " + uri.getPath());
		
		UrlParser up = new UrlParser(uri.getPath());
		String apps = up.next();
		String application;
		if ( apps.equalsIgnoreCase("apps") )
			application = up.next();
		else
			application = apps;
		
		String model = up.next();
		
		if ( model == null || model.length() == 0 )
			return false;
		
		if ( checkRhoExtensions(application, model ) )
			return true;
		
		// Convert CamelCase to underscore_case
		StringBuffer cName = new StringBuffer();
		byte[] modelname = model.getBytes();
		char ch;
		for (int i = 0; i != model.length(); ++i) {
			if (modelname[i] >= (byte)'A' && modelname[i] <= (byte)'Z') {
				ch = (char)(modelname[i] + 0x20);
				if (i != 0)
					cName.append('_');
				
			}
			else ch = (char)modelname[i];
			cName.append(ch);
		}
		String controllerName = cName.toString();
		
		String strCtrl = "apps/" + application + '/' + model + '/' + controllerName + "_controller";
		if (RhoSupport.findClass(strCtrl) == null) {
			strCtrl = "apps/" + application + '/' + model + '/' + "controller";
			if( RhoSupport.findClass(strCtrl) == null )
				return false;
		}
		
		Properties reqHash = new Properties();
		
		String actionid = up.next();
		String actionnext = up.next();
		if ( actionid != null && actionid.length() > 0 ){
			if ( actionid.length() > 6 && actionid.startsWith("%7B") && 
			     actionid.endsWith("%7D") )
				actionid = "{" + actionid.substring(3, actionid.length()-3) + "}";
			
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
		reqHash.setProperty("request-uri", uri.getPath());
		reqHash.setProperty("request-query", uri.getQueryString());
		
		if ( postData != null && postData.size() > 0 ){
			LOG.TRACE(postData.toString());
			reqHash.setProperty("request-body", postData.toString());
		}
		
		RubyValue res = RhoRuby.processRequest( reqHash, reqHeaders, resHeaders);
		processResponse(res);
		
		if ( actionid !=null && actionid.length() > 2 && 
			 actionid.charAt(0)=='{' && actionid.charAt(actionid.length()-1)=='}' )
			SyncThread.getInstance().addobjectnotify_bysrcname( model, actionid);

		RhodesApp.getInstance().keepLastVisitedUrl(url_external);
		LOG.INFO("dispatch end");
		return true;
	}
	
	public void processRequest()  throws IOException{
		if (!requestProcessed) {
			String strErr = "";
			
			LOG.TRACE("processRequest: " + getURL() );
			
			String strContType = getContentType();
			if ( this.method.equals("POST") || strContType.length() == 0 )
			{
				if ( dispatch() )
				{
					requestProcessed = true;
					return;
				}
			}
			
			if ( /*this.method == "GET" &&*/ httpGetFile(strContType) ){
				
			//}else if ( dispatch() ){
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
			String strBody = "";
			
			if ( resBody != null && resBody != RubyConstant.QNIL )
				strBody = resBody.toRubyString().toString();
				
			LOG.TRACE(strBody);
			
			try{
				responseData = new ByteArrayInputStream(strBody.getBytes("UTF-8"));
       		}catch(java.io.UnsupportedEncodingException exc)
       		{
       			LOG.ERROR("Error getting utf-8 body :", exc);
       		}
				
			if ( responseData != null )
				contentLength = Integer.parseInt(resHeaders.getPropertyIgnoreCase("Content-Length"));

			try {
				IRhoRubyHelper helper = RhoClassFactory.createRhoRubyHelper();
				helper.wakeUpGeoLocation();
			}catch(Exception exc)
			{
				LOG.ERROR("getGeoLocationText failed", exc);
			}
			
		}
	}
	
}

