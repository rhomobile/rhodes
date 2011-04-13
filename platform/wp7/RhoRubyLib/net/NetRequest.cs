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
        bool m_isMultiPart = false;
        bool m_isPullFile = false;
        int m_code = -1;
        CRhoFile m_pulledFile = null;
        Vector<MultipartItem> m_arItems = null;
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
	
	    private Hashtable<String, String> m_OutHeaders;
	    public boolean isCancelled(){ return m_bCancel;}
	
	    //TODO: use sslVerifyPeer
        public boolean sslVerifyPeer() {return m_sslVerifyPeer;}
        public void sslVerifyPeer(boolean mode) {m_sslVerifyPeer = mode;}
	
	    public NetResponse pullData(String strUrl, IRhoSession oSession )
        {
		    return doRequest("GET", strUrl, "", oSession, null);
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

            m_webRequest.UserAgent = "rhodes-wp7";
	    }

        static String addUniqueParam(String strUrl)
        {
            //Random RND = new Random();
            //RND.Next(10000);
            String strUnique = DateTime.Now.ToFileTime().ToString();

            int nQuest = strUrl.indexOf('?');
            if (nQuest >= 0)
                strUrl += "&";
            else
                strUrl += "?";

            strUrl += "wp7_nocache_param=" + strUnique;

            return strUrl; 
        }

	    private void readHeaders(Hashtable<String, String> headers, HttpWebResponse response)
	    {
		    if ( headers != null )
		    {
			    m_OutHeaders = new Hashtable<String, String>();

			    for (int i = 0; i < response.Headers.Count; i++) 
                {
                    String strField = response.Headers.AllKeys[i];
				    if (strField != null ) 
				    {
                        String header_field = response.Headers[strField];
					
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

        private String makeClientCookie(HttpWebResponse response)
        {
            String strRes = "";

            if (response.Cookies != null)
            {
                CookieContainer container = new CookieContainer();
                container.Add(new Uri(m_strUrl), response.Cookies);
                strRes = container.GetCookieHeader(new Uri(m_strUrl));
            }

            if (strRes != null && strRes.Length > 0)
                return strRes;

            for (int i = 0; i < response.Headers.Count; i++) 
            {
                String strName = response.Headers.AllKeys[i];
                String strValue = response.Headers[strName];

                if (strName.equalsIgnoreCase("Set-Cookie"))
                {
                    LOG.INFO("Set-Cookie: " + strValue);

                    int nSep = strValue.IndexOf(';');
                    String strVal = strValue;
                    if (nSep > 0)
                        strVal = strVal.substring(0, nSep);

                    strRes += strVal;//URI.parseCookie(strVal);
                }
            }

            return strRes;
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
                    m_webRequest.CookieContainer.SetCookies(new Uri(m_strUrl), strSession);
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
                LOG.ERROR("EndGetResponse", e);
                response = (HttpWebResponse)e.Response;
                m_code = Convert.ToInt32(response.StatusCode);
            }

            
            Stream stream = response.GetResponseStream();

		    m_code = Convert.ToInt32(response.StatusCode);
            LOG.INFO("getResponseCode : " + m_code);

            m_strCookies = makeClientCookie(response);

            readHeaders(m_headers, response);
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

        private void GetPullFileCallback(IAsyncResult asyncResult)
        {
            HttpWebResponse response = null;
            try
            {
                response = m_webRequest.EndGetResponse(asyncResult) as HttpWebResponse;
            }
            catch (WebException e)
            {
                LOG.ERROR("EndGetResponse", e);
                response = (HttpWebResponse)e.Response;
                m_code = Convert.ToInt32(response.StatusCode);
            }


            Stream stream = response.GetResponseStream();

            m_code = Convert.ToInt32(response.StatusCode);
            LOG.INFO("getResponseCode : " + m_code);

            m_strCookies = makeClientCookie(response);

            readHeaders(m_headers, response);
            copyHashtable(m_OutHeaders, m_headers);

            try
            {
                if (m_code == Convert.ToInt32(HttpStatusCode.OK) )
				    m_pulledFile.setPosTo(0);

                if ( m_code == Convert.ToInt32(HttpStatusCode.RequestedRangeNotSatisfiable) )
				    m_code = Convert.ToInt32(HttpStatusCode.PartialContent);
			    else
			    {
                    if (m_code >= 400 && m_code != Convert.ToInt32(HttpStatusCode.PartialContent) ) 
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
                        int nRead = 0;
					
					    byte[]  byteBuffer = new byte[1024*20]; 

		    		    do{
		    			    nRead = stream.Read(byteBuffer, 0, 1024*20);
		    			    if ( nRead > 0 )
		    			    {
		    				    m_pulledFile.write(byteBuffer, nRead);
		    				    m_pulledFile.flush();
		    				    m_nCurDownloadSize += nRead;
		    			    }
		    		    }while( !m_bCancel && nRead > 0 );

                        if (m_code == Convert.ToInt32(HttpStatusCode.OK) || (m_code == Convert.ToInt32(HttpStatusCode.PartialContent) && isFinishDownload(response.Headers["Content-Range"])))
		    			    m_nCurDownloadSize = 0;
                    }
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
            }
            finally
            {
                if (stream != null)
                    stream.Close();
                m_reqWaitEvent.Set();
            }
        }

	    public NetResponse doRequest(String strMethod, String strUrl, String strBody, IRhoSession oSession, Hashtable<String, String> headers, long nRangePos = -1)
        {
            LOG.INFO("Url: " + strUrl + "; Body: " + strBody);
            m_respWaitEvent.WaitOne();
            m_respWaitEvent.Reset();

            m_oSession = oSession;
            m_strBody = strBody;
            m_strUrl = addUniqueParam(strUrl);
            m_headers = headers;
		
		    m_bCancel = false;

            m_webRequest = WebRequest.Create(m_strUrl) as HttpWebRequest;

            m_webRequest.CookieContainer = new CookieContainer();
            handleCookie(oSession);

            if ((strBody != null && strBody.length() > 0) || m_isPullFile || m_isMultiPart)
			{
                if (oSession != null)
                    m_webRequest.ContentType = oSession.getContentType();
                else if (m_isMultiPart)
                    m_webRequest.ContentType = "multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5";
                else if (m_isPullFile)
                {
                    if (nRangePos > 0)
                        m_webRequest.Headers["Range"] = "bytes=" + nRangePos.ToString() + "-";
                }
                else
                    m_webRequest.ContentType = "application/x-www-form-urlencoded";

				writeHeaders(headers);
                m_webRequest.Method = strMethod;

                if (!m_isPullFile)
                {
                    m_webRequest.BeginGetRequestStream(GetRequestStreamCallback, null);
                    m_reqWaitEvent.Reset();
                    m_reqWaitEvent.WaitOne();
                }
			}else
			{
				writeHeaders(headers);
                m_webRequest.Method = strMethod;
			}

            if(!m_isPullFile)
                m_webRequest.BeginGetResponse(GetResponseCallback, null);
            else
                m_webRequest.BeginGetResponse(GetPullFileCallback, null);

            m_respWaitEvent.Reset();
            m_respWaitEvent.WaitOne();

            m_isMultiPart = false;
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
	
	    static String szMultipartPostfix = 
	        "\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

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
	        	    CRhoFile file = null;
	    		    try{
	    			    file = RhoClassFactory.createFile();
	    			    file.open(oItem.m_strFilePath, CRhoFile.EOpenModes.OpenReadOnly);
	    			    nContentSize = file.size();
	    			    if ( !file.isOpened() ){
	    				    LOG.ERROR("File not found: " + oItem.m_strFilePath);
	    				    throw new Exception("File not found:" + oItem.m_strFilePath);
	    			    }
	    		    }finally{
	    			    if ( file != null )
	    				    try{ file.close(); }catch(IOException e)
                            {
                                LOG.ERROR("file closing failed.", e);
                            }
	    		    }
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
            m_arItems = arItems;
            m_isMultiPart = true;
            processMultipartItems( arItems );
            return doRequest("POST", strUrl, null, oSession, headers);
        }
	
	    int m_nCurDownloadSize = 0;
	    public NetResponse pullFile( String strUrl, String strFileName, IRhoSession oSession, Hashtable<String, String> headers )
	    {
		    NetResponse resp = null;
            m_isPullFile = true;

		    m_bCancel = false;
    	
		    try{

	            if (!strFileName.startsWith("file:")) { 
            	    try{
	            	    strFileName = CFilePath.join(CRhodesApp.getRhoRootPath(), strFileName);
            	    } catch (IOException e) { 
                 	    LOG.ERROR("getDirPath failed.", e);
                    }              	
	            }

                m_pulledFile = RhoClassFactory.createFile();
                m_pulledFile.open(strFileName, CRhoFile.EOpenModes.OpenForReadWrite);
                m_pulledFile.setPosTo(m_pulledFile.size());
			
			    do{
                    resp = doRequest("GET", strUrl, null, oSession, headers, m_pulledFile.size());
			    }while( !m_bCancel && (resp == null || resp.isOK()) && m_nCurDownloadSize > 0);
			
		    }finally{
                if (m_pulledFile != null)
			    {
                    try { m_pulledFile.close(); }
                    catch (IOException e)
                    {
                        LOG.ERROR("file closing failed.", e);
                    }
                    m_pulledFile = null;
			    }
		    }
		
		    copyHashtable(m_OutHeaders, headers);

            m_isPullFile = false;
            m_nCurDownloadSize = 0;
            return resp != null && !m_bCancel ? resp : makeResponse("", Convert.ToInt32(HttpStatusCode.InternalServerError));
	    }
	
	    private boolean isFinishDownload(String strContentRange)
	    {
            String strContRange = strContentRange;// m_connection.getHeaderField("Content-Range");
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
