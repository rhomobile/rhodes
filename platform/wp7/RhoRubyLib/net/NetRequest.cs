using System;
using System.Net;
using rho.common;
using System.Threading;
using System.Text;
using System.IO;

namespace rho.net
{
    public class NetRequest
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		    new RhoLogger("Net");
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }
	
	    boolean m_bCancel = false;
	    boolean m_sslVerifyPeer = true;
        ManualResetEvent m_respWaitEvent = new ManualResetEvent(true);
        ManualResetEvent m_reqWaitEvent = new ManualResetEvent(false);
        HttpWebRequest m_webRequest;
        IRhoSession m_oSession;
        String m_strRespBody = null;
        String m_strBody = null;
        String m_strUrl = null;
        String m_strCookies = null;
        int m_code = -1;
        Hashtable<String, String> m_headers = null;

	
	    public interface IRhoSession
	    {
		    void logout();
		    String getSession();
		    String getContentType();
	    }
	
	    public class MultipartItem
	    {
	        //mutually exclusive
	        public String m_strFilePath = "";
	        public String m_strBody = "";
	        //

	        public String m_strName = "", m_strFileName = "", m_strContentType = "";

	        public String m_strDataPrefix = "";
	    };
	
	    private Object /*IHttpConnection*/ m_connection = null;
	
	    private boolean m_bIgnoreSuffixOnSim = true;
	    private Hashtable<String, String> m_OutHeaders;
	    public boolean isCancelled(){ return m_bCancel;}
	
	    //TODO: use sslVerifyPeer
        public boolean sslVerifyPeer() {return m_sslVerifyPeer;}
        public void sslVerifyPeer(boolean mode) {m_sslVerifyPeer = mode;}
	
	    public NetResponse pullData(String strUrl, IRhoSession oSession )
        {
		    return doRequest("GET", strUrl, "", oSession, null);
        }

	    public void setIgnoreSuffixOnSim(boolean bset)
	    {
		    m_bIgnoreSuffixOnSim = bset;
	    }

	    private void writeHeaders(Hashtable<String, String> headers)
	    {
		    if (headers != null && headers.size() > 0)
		    {
                Hashtable<String, String>.Enumerator hashEnum = headers.GetEnumerator();
		        while( hashEnum.MoveNext() )
		        {
				    String strName = hashEnum.Current.Key;
				    String strValue = hashEnum.Current.Value;
                    m_webRequest.Headers[strName] = strValue;
		        }
			
		    }
	    }

	    private void readHeaders(Hashtable<String, String> headers)
	    {
		    if ( headers != null )
		    {
			    m_OutHeaders = new Hashtable<String, String>();

			    for (int i = 0; i < m_webRequest.Headers.Count; i++) {
                    String strField = m_webRequest.Headers.AllKeys[i];// m_connection.getHeaderFieldKey(i);
				    if (strField == null && i > 0)
					    break;

				    if (strField != null ) 
				    {
                        String header_field = m_webRequest.Headers[m_webRequest.Headers.AllKeys[i]];// m_connection.getHeaderField(i);
					
					    String strKeyName = strField.toLowerCase();
					    if ( m_OutHeaders.containsKey(strKeyName))
					    {
						    header_field += ";" + m_OutHeaders.get(strKeyName);
						    m_OutHeaders.put(strKeyName, header_field);
					    }
					    else	
						    m_OutHeaders.put(strKeyName, header_field);
				    }
			    }
		    }
	    }
	
	    public static void copyHashtable(Hashtable<String, String> from, Hashtable<String, String> to)
	    {
		    if ( from == null || to == null )
			    return;
            Hashtable<String, String>.Enumerator hashEnum = from.GetEnumerator();
		    while( hashEnum.MoveNext() )
		    {
				String strName = hashEnum.Current.Key;
				String strValue = hashEnum.Current.Value;		
			    to.put(strName, strValue);
	        }
	    }
	
	    private String getResponseEncoding()
	    {
		    if ( m_OutHeaders != null )
			    return (String)m_OutHeaders.get("content-type");
		
		    return "";
	    }
	
	    void handleCookie(IRhoSession oSession)
	    {
		    if ( oSession != null )
		    {
			    String strSession = oSession.getSession();
			    LOG.INFO("Cookie : " + (strSession != null ? strSession : "") );
                if (strSession != null && strSession.length() > 0 && !strSession.equals("rho_empty"))
                    m_webRequest.CookieContainer.SetCookies(new Uri(m_strUrl),strSession);
		    }
	    }

        private void GetResponseCallback(IAsyncResult asyncResult)
        {
            
            HttpWebResponse response = null;
            try
            {
                response = m_webRequest.EndGetResponse(asyncResult) as HttpWebResponse;
            }
            catch (WebException e)
            {
                response = (HttpWebResponse)e.Response;
                m_code = Convert.ToInt32(response.StatusCode);
            }
            
            Stream stream = response.GetResponseStream();
		    LOG.INFO("openInputStream done");

            CookieContainer container = new CookieContainer();
            container.Add(new Uri(m_strUrl), response.Cookies);
            m_strCookies = container.GetCookieHeader(new Uri(m_strUrl));
			
		    m_code = Convert.ToInt32(response.StatusCode);
            LOG.INFO("getResponseCode : " + m_code);

            readHeaders(m_headers);
            copyHashtable(m_OutHeaders, m_headers);

            try
            {
                if (m_code >= 400)
                {
                    LOG.ERROR("Error retrieving data: " + m_code);
                    if (m_code == Convert.ToInt32(HttpStatusCode.Unauthorized) && m_oSession != null)
                    {
                        LOG.ERROR("Unauthorize error.Client will be logged out");
                        m_oSession.logout();
                    }

                    m_strRespBody = readFully(stream, getResponseEncoding());
                    LOG.TRACE("Response body: " + m_strRespBody);
                }
                else
                {
                    long len = response.ContentLength;
                    LOG.INFO("fetchRemoteData data size:" + len);

                    m_strRespBody = readFully(stream, getResponseEncoding());
                    LOG.INFO("fetchRemoteData data readFully.");
                }
            }
            finally
            {
                stream.Close();
                response.Close();
                m_respWaitEvent.Set();
            }
        }

        private void GetRequestStreamCallback(IAsyncResult asyncResult)
        {
            Stream stream = null;
            try
            {
                stream = m_webRequest.EndGetRequestStream(asyncResult);
                stream.Write(new UTF8Encoding().GetBytes(m_strBody), 0, m_strBody.length());//TODO ASCII ???
                LOG.INFO("write body done");
            }
            finally
            {
                if (stream != null)
                    stream.Close();
                m_reqWaitEvent.Set();
            }
        }



	    public NetResponse doRequest(String strMethod, String strUrl, String strBody, IRhoSession oSession, Hashtable<String, String> headers )
        {
            m_respWaitEvent.WaitOne();
            m_respWaitEvent.Reset();


            m_oSession = oSession;
            m_strBody = strBody;
            m_strUrl = strUrl;
            m_headers = headers;
		
		    m_bCancel = false;

            m_webRequest = WebRequest.Create(strUrl) as HttpWebRequest; // TODO m_bIgnoreSuffixOnSim ???
            LOG.INFO("connection done");

            m_webRequest.CookieContainer = new CookieContainer();
            handleCookie(oSession);

			if ( strBody != null && strBody.length() > 0 )
			{
		        if ( oSession != null )
                    m_webRequest.ContentType = oSession.getContentType();
				else
                    m_webRequest.ContentType = "application/x-www-form-urlencoded";

				writeHeaders(headers);
				LOG.INFO("writeHeaders done");
                m_webRequest.Method = strMethod;
				
                m_webRequest.BeginGetRequestStream(GetRequestStreamCallback, null);
                m_reqWaitEvent.WaitOne();
			}else
			{
				writeHeaders(headers);
                m_webRequest.Method = strMethod;
			}

            m_webRequest.BeginGetResponse(GetResponseCallback, null);
            m_respWaitEvent.WaitOne();

            m_bIgnoreSuffixOnSim = true;

            return makeResponse(m_strRespBody, m_code);
        }
	
	    private NetResponse makeResponse(String strRespBody, int nErrorCode)
	    {
		    NetResponse pResp = new NetResponse(strRespBody != null ? strRespBody : "", nErrorCode );
		    if (pResp.isSuccess())
                pResp.setCookies(m_strCookies);
		
		    return pResp;
	    }
	
	    public NetResponse pushData(String strUrl, String strBody, IRhoSession oSession)
        {
		    m_bCancel = false;
		
		    if ( RHODESAPP().isRhodesAppUrl(strUrl) )
                return RHODESAPP().processCallback(strUrl, strBody);
		
		    return doRequest("POST", strUrl, strBody, oSession, null);
        }
	
        public NetResponse pushMultipartData(String strUrl, MultipartItem oItem, IRhoSession oSession, Hashtable<String,String> pHeaders)
        {
            Vector<MultipartItem> arItems = new Vector<MultipartItem>();
            arItems.addElement(oItem);

            return pushMultipartData(strUrl, arItems, oSession, pHeaders);
        }
	
	    public NetResponse pullCookies(String strUrl, String strBody, IRhoSession oSession)
	    {
		    Hashtable<String,String> headers = new Hashtable<String,String>();
		    m_bIgnoreSuffixOnSim = false;
		    m_bCancel = false;
    	
		    NetResponse resp = doRequest/*Try*/("POST", strUrl, strBody, oSession, headers);
		    if ( resp.isOK() )
		    {
			    String strCookie = resp.getCookies();
			    if ( strCookie == null || strCookie.length() == 0 )
				    strCookie = "rho_empty";
			
			    resp.setCharData(strCookie);
			    LOG.INFO("pullCookies: " + resp.getCharData() );
		    }
		
		    return resp;
	    }
	
	    //static String szMultipartPostfix = 
	    //    "\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

	    //static String szMultipartContType = 
	    //    "multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5";

	    void processMultipartItems( Vector<MultipartItem> arItems )
	    {
	        for( int i = 0; i < (int)arItems.size(); i++ )
	        {
	            MultipartItem oItem = (MultipartItem)arItems.elementAt(i); 

	            if ( oItem.m_strName.length() == 0 )
	                oItem.m_strName = "blob";

	            if ( oItem.m_strFileName.length() == 0 )
	            {
	                if ( oItem.m_strFilePath.length() > 0 )
	                {
	                    oItem.m_strFileName = CFilePath.getBaseName(oItem.m_strFilePath);
	                }
	                //else
	                //    oItem.m_strFileName = "doesnotmatter.txt";
	            }

	            oItem.m_strDataPrefix = i > 0 ? "\r\n" : "";
	            oItem.m_strDataPrefix += 
	                "------------A6174410D6AD474183FDE48F5662FCC5\r\n"+
	                "Content-Disposition: form-data; name=\"";
	            oItem.m_strDataPrefix += oItem.m_strName + "\"";
	            if (oItem.m_strFileName.length()>0)
	                oItem.m_strDataPrefix += "; filename=\"" + oItem.m_strFileName + "\"";
	            oItem.m_strDataPrefix += "\r\n";
	            if ( oItem.m_strContentType != null && oItem.m_strContentType.length() > 0 )
	                oItem.m_strDataPrefix += "Content-Type: " + oItem.m_strContentType + "\r\n";

	            long nContentSize = 0;
	            if ( oItem.m_strFilePath.length() > 0 )
	            {
                    //TODO: process file
/*	        	    SimpleFile file = null;
	    		    try{
	    			    file = RhoClassFactory.createFile();
	    			    file.open(oItem.m_strFilePath, true, true);
	    			    nContentSize = file.length();
	    			    if ( !file.isOpened() ){
	    				    LOG.ERROR("File not found: " + oItem.m_strFilePath);
	    				    throw new RuntimeException("File not found:" + oItem.m_strFilePath);
	    			    }
	    		    }finally{
	    			    if ( file != null )
	    				    try{ file.close(); }catch(IOException e){}
	    		    }*/
	            }
	            else
	                nContentSize = oItem.m_strBody.length();

	            if ( oItem.m_strContentType != null && oItem.m_strContentType.length() > 0 )
	                oItem.m_strDataPrefix += "Content-Length: " + nContentSize + "\r\n";

	            oItem.m_strDataPrefix += "\r\n";
	        }

	    }
	
        public NetResponse pushMultipartData(String strUrl, Vector<MultipartItem> arItems, IRhoSession oSession, Hashtable<String,String> headers)
        {
/*		    String strRespBody = null;
		    InputStream is = null;
		    OutputStream os = null;
		    int code  = -1;
		
		    m_bCancel = false;
    	
		    try{
			    closeConnection();
			    m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl, false);
			
			    handleCookie(oSession);

			    m_connection.setRequestProperty("Connection", "keep-alive");
			    m_connection.setRequestProperty("content-type", szMultipartContType);
			    writeHeaders(headers);
			    m_connection.setRequestMethod(IHttpConnection.POST);
			
			    //PUSH specific
			    processMultipartItems( arItems );
			    os = m_connection.openOutputStream();
	            //write all items
	            for( int i = 0; i < (int)arItems.size(); i++ )
	            {
	                MultipartItem oItem = (MultipartItem)arItems.elementAt(i); 
    			    os.write(oItem.m_strDataPrefix.getBytes(), 0, oItem.m_strDataPrefix.length());

	                if ( oItem.m_strFilePath.length() > 0 )
	                {

		        	    SimpleFile file = null;
		    		    InputStream fis = null;
		        	
		    		    try{
		    			    file = RhoClassFactory.createFile();
		    			    file.open(oItem.m_strFilePath, true, true);
		        		
		    			    if ( !file.isOpened() ){
		    				    LOG.ERROR("File not found: " + oItem.m_strFilePath);
		    				    throw new RuntimeException("File not found:" + oItem.m_strFilePath);
		    			    }
		    			
		    			    fis = file.getInputStream();
		    			    byte[]  byteBuffer = new byte[1024*4]; 
		    			    int nRead = 0;
		        		    do{
		        			    nRead = fis.read(byteBuffer);	    			
		        			    if ( nRead > 0 )
		        				    os.write(byteBuffer, 0, nRead);
		        		    }while( nRead > 0 );
		    		    }finally{
						    if (fis != null)
							    try{ fis.close(); }catch(IOException e){}
		    			
		    			    if ( file != null )
		    				    try{ file.close(); }catch(IOException e){}
		    		    }
	    			
	                }else
	                {
	    			    os.write(oItem.m_strBody.getBytes(), 0, oItem.m_strBody.length());
	                }
	            }
			    os.write(szMultipartPostfix.getBytes(), 0, szMultipartPostfix.length());
			    //os.flush();
			    //PUSH specific
			
			    is = m_connection.openInputStream();
			    code = m_connection.getResponseCode();
		
			    LOG.INFO("getResponseCode : " + code);

			    readHeaders(headers);
			    copyHashtable(m_OutHeaders, headers);
			
			    if (code >= 400 ) 
			    {
				    LOG.ERROR("Error retrieving data: " + code);
				    if (code == IHttpConnection.HTTP_UNAUTHORIZED)
				    {
					    LOG.ERROR("Unauthorize error.Client will be logged out");
					    oSession.logout();
				    }
				
				    //if ( code != IHttpConnection.HTTP_INTERNAL_ERROR )
					    strRespBody = readFully(is, getResponseEncoding());
					    LOG.TRACE("Response body: " + strRespBody );
				
			    }else
			    {
				    long len = m_connection.getLength();
				    LOG.INFO("fetchRemoteData data size:" + len );
		
				    strRespBody = readFully(is, getResponseEncoding());
				
				    LOG.INFO("fetchRemoteData data readFully.");
			    }
			
		    }finally{
			    try{
				    if ( is != null )
					    is.close();
				    if (os != null)
					    os.close();
				
				    closeConnection();
				
			    }catch(IOException exc2){}
		    }
		
		    return makeResponse(strRespBody, code );  */
            return makeResponse("", 200 );
        }
	
	    //long m_nMaxPacketSize = 0;
	    //int m_nCurDownloadSize = 0;
	    boolean m_bFlushFileAfterWrite = false;
	    public NetResponse pullFile( String strUrl, String strFileName, IRhoSession oSession, Hashtable<String, String> headers )
	    {
/*		    IRAFile file = null;
		    NetResponse resp = null;
		
		    m_nMaxPacketSize = RhoClassFactory.getNetworkAccess().getMaxPacketSize();
		    m_bFlushFileAfterWrite = RhoConf.getInstance().getBool("use_persistent_storage");
		    m_bCancel = false;
    	
		    try{

	            if (!strFileName.startsWith("file:")) { 
            	    try{
	            	    strFileName = FilePath.join(RhoClassFactory.createFile().getDirPath(""), strFileName);
            	    } catch (IOException x) { 
                 	    LOG.ERROR("getDirPath failed.", x);
                        throw x;
                    }              	
	            }
			
			    file = RhoClassFactory.createFSRAFile();
			    file.open(strFileName, "rw");
			    file.seek(file.size());
			
			    do{
				    resp = pullFile1( strUrl, file, file.size(), oSession, headers );
			    }while( !m_bCancel && (resp == null || resp.isOK()) && m_nCurDownloadSize > 0 && m_nMaxPacketSize > 0 );
			
		    }finally{
			    if ( file != null )
			    {
				    file.close();
				    file = null;
			    }
		    }
		
		    copyHashtable(m_OutHeaders, headers);
		
		    return resp != null && !m_bCancel ? resp : makeResponse("", IHttpConnection.HTTP_INTERNAL_ERROR );*/
            return makeResponse("",200);
	    }
/*	
	    NetResponse pullFile1( String strUrl, IRAFile file, long nStartPos, IRhoSession oSession, Hashtable headers )throws Exception
	    {
		    String strRespBody = null;
		    InputStream is = null;
		    int code = -1;
		
		    try{
			    closeConnection();
			    m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl, true);
			
			    handleCookie(oSession);

			    m_connection.setRequestProperty("Connection", "keep-alive");
			
			    if ( nStartPos > 0 || m_nMaxPacketSize > 0 )
			    {
				    if ( m_nMaxPacketSize > 0 )
					    m_connection.setRequestProperty("Range", "bytes="+nStartPos+"-" + (nStartPos + m_nMaxPacketSize-1));
				    else
					    m_connection.setRequestProperty("Range", "bytes="+nStartPos+"-");
			    }
			    writeHeaders(headers);
			
			    m_connection.setRequestMethod(IHttpConnection.GET);
			
			    is = m_connection.openInputStream();
			
			    code = m_connection.getResponseCode();
			
			    LOG.INFO("getResponseCode : " + code);
			
			    m_nCurDownloadSize = 0;
			    readHeaders(headers);

			    if ( code == IHttpConnection.HTTP_OK )
				    file.seek(0);
			
			    if ( code == IHttpConnection.HTTP_RANGENOTSATISFY )
				    code = IHttpConnection.HTTP_PARTIAL_CONTENT;
			    else
			    {
				    if (code >= 400 && code != IHttpConnection.HTTP_PARTIAL_CONTENT ) 
				    {
					    LOG.ERROR("Error retrieving data: " + code);
					    if (code == IHttpConnection.HTTP_UNAUTHORIZED)
					    {
						    LOG.ERROR("Unauthorize error.Client will be logged out");
						    oSession.logout();
					    }
					
					    strRespBody = readFully(is, getResponseEncoding());
				    }else
				    {
					    int nRead = 0;
					
					    byte[]  byteBuffer = new byte[1024*20]; 

		    		    do{
		    			    nRead = is.read(byteBuffer);
		    			    if ( nRead > 0 )
		    			    {
		    				    file.write(byteBuffer, 0, nRead);
		    				
		    				    if (m_bFlushFileAfterWrite)
		    					    file.sync();

		    				    m_nCurDownloadSize += nRead;
		    			    }
		    		    }while( !m_bCancel && nRead >= 0 );
		    		
		    		    if ( code == IHttpConnection.HTTP_OK || (code == IHttpConnection.HTTP_PARTIAL_CONTENT && isFinishDownload()) )
		    			    m_nCurDownloadSize = 0;
				    }
				
			    }
		    }finally
		    {
			    if ( is != null )
				    try{ is.close(); }catch(IOException exc){}
			
			    closeConnection();
		    }
		
		    return makeResponse(strRespBody != null ? strRespBody : "", code );
	    }*/
	
	    private boolean isFinishDownload()
	    {
            String strContRange = "";// m_connection.getHeaderField("Content-Range");
		    if ( strContRange != null )
		    {
			    int nMinus = strContRange.indexOf('-');
			    if ( nMinus > 0 )
			    {
				    int nSep = strContRange.indexOf('/', nMinus);
				    if ( nSep > 0 )
				    {
					    String strHigh = strContRange.substring(nMinus+1,nSep);
					    String strTotal = strContRange.substring(nSep+1);
					
					    if ( int.Parse(strHigh) + 1 >= int.Parse(strTotal) )
						    return true;
				    }
			    }
		    }
		
		    return false;
	    }
	
	    public String resolveUrl(String strUrl)
        {
	        return RHODESAPP().canonicalizeRhoUrl(strUrl);
        }

	    public void cancel()
        {
		    m_bCancel = true;
		    closeConnection();
        }

	    /*static{
		    TEST();
	    }
	    public static void TEST()
	    {
		    //ParsedCookie cookie = new ParsedCookie();
		    String strClientCookie = "";
		    strClientCookie = URI.parseCookie("auth_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT, auth_token=887b2ffd30a7b97be9a0986d7746a934421eec7d; path=/; expires=Sat, 24 Oct 2009 20:56:55 GMT, rhosync_session=BAh7BzoMdXNlcl9pZGkIIgpmbGFzaElDOidBY3Rpb25Db250cm9sbGVyOjpGbGFzaDo6Rmxhc2hIYXNoewAGOgpAdXNlZHsA--f9b67d99397fc534107fb3b7483ccdae23b4a761; path=/; expires=Sun, 10 Oct 2010 19:10:58 GMT; HttpOnly");
		    strClientCookie = URI.parseCookie("auth_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT");
		    strClientCookie = URI.parseCookie("rhosync_session=BAh7CToNcGFzc3dvcmQiFTiMYru1W11zuoAlN%2FPtgjc6CmxvZ2luIhU4jGK7tVtdc7qAJTfz7YI3Ogx1c2VyX2lkaQYiCmZsYXNoSUM6J0FjdGlvbkNvbnRyb2xsZXI6OkZsYXNoOjpGbGFzaEhhc2h7AAY6CkB1c2VkewA%3D--a7829a70171203d72cd4e83d07b18e8fcf5e2f78; path=/; expires=Thu, 02 Sep 2010 23:51:31 GMT; HttpOnly");
	    }*/
	
	    private static String makeClientCookie(Hashtable<String, String> headers)
	    {
		    if ( headers == null )
			    return "";
		
		    //ParsedCookie cookie = new ParsedCookie();
		    String strRes = "";
            Hashtable<String, String>.Enumerator hashEnum = headers.GetEnumerator();
		    while( hashEnum.MoveNext() )
		    {
				String strName = hashEnum.Current.Key;
				String strValue = hashEnum.Current.Value;

			    if (strName.equalsIgnoreCase("Set-Cookie")) 
			    {
				    LOG.INFO("Set-Cookie: " + strValue);
				
				    strRes += URI.parseCookie(strValue);
			    }
		    }

		    return strRes;
	    }

	    public static String readFully(Stream stream, String strContType) 
	    {
		    String strRes = "";
            byte[]  byteBuffer = new byte[1024*4];
		    boolean bUTF8 = false;
            StreamReader reader = null;
		
		    if ( strContType != null )
		    {
			    int nCharsetPos = strContType.lastIndexOf('=');
			    if ( nCharsetPos > 0 )
			    {
				    String strEnc = strContType.substring(nCharsetPos+1);
				    bUTF8 = strEnc.equalsIgnoreCase("UTF-8");
			    }
		    }

            try
            {
                if(bUTF8)
                    reader = new StreamReader(stream, Encoding.UTF8);
                else
                    reader = new StreamReader(stream);

                strRes = reader.ReadToEnd();
            }
            finally
            {
                if(reader != null)
                    reader.Close();
            }

            return strRes;
	    }
	
	    public void closeConnection(){
		    if ( m_connection != null ){
			    try{
                    //TODO: close connection
				    //m_connection.close();
			    }catch(Exception exc){
				    LOG.ERROR("There was an error close connection", exc);
			    }
		    }
		
		    m_connection = null;
	    }

    }
}
