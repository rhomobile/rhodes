package com.rho.rhoelements;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URISyntaxException;
import java.net.UnknownHostException;
import java.security.InvalidParameterException;
import java.util.StringTokenizer;

import com.rho.rhoelements.plugins.PushPlugin;

public class LocalWebServer
{
    private static final String HTTP_SERVERNAME		= "Server: RhoElements\r\n";
    private static final String HTTP_ACCEPTRANGES	= "Accept-Ranges: bytes\r\n";
    private static final String HTTP200_HEADER 		= "HTTP/1.1 200 OK\r\n";
    private static final String HTTP302_HEADER 		= "HTTP/1.1 302 Redirect\r\n";
    //private static final String HTTP400_HEADER	= "HTTP/1.1 400 Bad Request\r\n";
    private static final String HTTP403_HEADER 		= "HTTP/1.1 403	Forbidden\r\n";
    private static final String HTTP404_HEADER 		= "HTTP/1.1 404 Not Found\r\n";
    //private static final String HTTP500_HEADER       	= "HTTP/1.1 500 Internal Server Error\r\n";
    private static final String HTTP_CACHE_CONTROL     	= "Cache-control: no-cache,must-revalidate\r\nPragma: no-cache\r\n";
    private static final String HTTP_CONNECTION	       	= "Connection: close\r\n";
    private static final String HTTP_CORS	       	= "Access-Control-Allow-Origin: ";
    private static final String HTTP_CONTENT		= "Content-Type: %s\r\n";
    private static final String HTTP_CONTENT_LEN	= "Content-Length: %d\r\n";
    private static final int HTTP_DEFAULT_FILE_SIZE = 116;
    private static final int HTTP_CORS_FILE_SIZE = 121;
    //private static final String HTTP_DEFAULT_PAGE	= "/index.html";
    
    private ServerSocket mWebServerSocket;
    private boolean isPublic;
    private int port = 8080;  
    private String path;
    private boolean mRunning = false; //false if server thread has to run, false otherwise (ie. stop is called)
    private Thread mThread = null;
    private boolean mInitialisedOk = false; //true if the web server could be initialised correctly, false otherwise (ie. invalid port)
    private boolean isPushServer = false; //Flag to know whether this is a local web server or a push web server    
    private String corsValueConfig = null; //Config file cors values
    private String originResponse = null; //Response to be sent to the client (just echo the same cors value sent by the browser)

    
    public LocalWebServer(int port, String path, boolean isPublic, boolean isPushServer)
    {
	this.port = port;
	this.path = path;
	this.isPushServer = isPushServer; //Save the source for the web server
	if (!this.path.endsWith("/"))
	    this.path += "/";
	this.isPublic = isPublic;
	try
	{
	    mWebServerSocket = new ServerSocket(this.port);
	    mInitialisedOk = true;
	} 
	catch (IOException e)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	    mInitialisedOk = false;
	}
    }
    
    public void start()
    {  
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	if (mInitialisedOk == false)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Web Server cannot start as it was not initialized correctly"));
	    return;
	}
	mRunning = true;
	if (mThread == null)
	    mThread = new Thread(new WebServerThread());
	mThread.start();
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
    }
    
    public void stop()
    {  
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	try {
		mWebServerSocket.close();
	} catch (IOException e) {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	}
	mRunning = false;
	mThread = null;
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
    }
    
    private boolean isRemote(Socket socket)
    {
	boolean result = false;
	try
	{
	    String addressString = socket.getRemoteSocketAddress().toString();
	    addressString = addressString.substring(0, addressString.indexOf("/"));
	    InetAddress requestAddress = InetAddress.getByName(addressString);
	    // if the web server is not set to publish resources externally 
	    result = ( (requestAddress.isAnyLocalAddress()) || (requestAddress.isLoopbackAddress()) ) ? false : true;
	}
	catch (UnknownHostException e)
	{
	    result = false;
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Returning false"));
	}
	return result;
    }
    
    private class WebServerThread implements Runnable
    {

	/** (non-Javadoc)
	 * @see java.lang.Runnable#run()
	 */
	@Override
	public void run()
	{  
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	    while (mRunning)
	    {
		try
		{
		    Socket clientSocket = mWebServerSocket.accept();
		    if ( (!isPublic) && (isRemote(clientSocket)) )
		    {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Connection from remote address not accepted as server is not set to Public"));
			clientSocket.getOutputStream().write(HTTP403_HEADER.getBytes());
			clientSocket.getOutputStream().flush();
			clientSocket.close();
			continue;
		    }
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Connection accepted"));
		    
		    //read and store the config value                    	
        	corsValueConfig = Common.config.getSetting(Config.SETTING_CORS); 
        	
		    new Thread(new WebSocketClientThread(clientSocket)).start();
		}
		catch (IOException e)
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
		}
		catch (NullPointerException e)
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
		}
	    }
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	}
    }
    
    private class WebSocketClientThread implements Runnable
    {
	private Socket mClientSocket;
	private DataInputStream mSocketInput;
	private DataOutputStream mSocketOutput;
	
	public WebSocketClientThread(Socket clientSocket)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	    mClientSocket = clientSocket;
	    try
	    {
		mSocketInput = new DataInputStream(new BufferedInputStream(mClientSocket.getInputStream()));
		mSocketOutput = new DataOutputStream(new BufferedOutputStream(mClientSocket.getOutputStream()));
	    } 
	    catch (IOException e)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	    }
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	}
	
	private void cleanUp()
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	    try
	    {
        	mSocketOutput.close();
        	mSocketInput.close();
        	mClientSocket.close();
	    }
	    catch (IOException e)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	    }
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	}
	
	private String mimeType(String fileName)
	{
	    fileName = fileName.toLowerCase();
	    if (fileName.endsWith(".html") || fileName.endsWith(".htm")) 
		return "text/html";
	    if (fileName.endsWith(".css"))
		return "text/css";
	    if (fileName.endsWith(".js"))
		return "text/jacascript";
	    else if (fileName.endsWith(".gif")) 
		return "image/gif";
	    else if (fileName.endsWith(".bmp")) 
		return "image/bmp";
	    else if (fileName.endsWith(".png")) 
		return "image/png";
	    else if (fileName.endsWith(".class"))
		return "application/octet-stream";
	    else if (fileName.endsWith(".jpg") || fileName.endsWith(".jpeg"))
		return "image/jpeg";
	    else    
		return "text/plain";
	}
	
	/**
	 * Check for client origin in the values provided by config option
	 * @param config - Config file origin allowed values
	 * @param client - Origin value provided by the client browser
	 * @return true if the origin matches with the client value
	 */
	private boolean CheckOrigin(String config, String client)
	{			
		String corsConfigValues[] = config.split("\\s+|;"); // ";", " " are the separators
		
		// For all the allowed strings specified in the config file
		for (String corsConfig : corsConfigValues)
		{			
			// Split client and then match with cors
			String corsConfigFields[] = corsConfig.split(":|\\."); // ':", "." are the separators in the URL. Should we do "//" as well>			
			
			boolean bResult = true; //resets for each string in the cors values
						
			// TODO: This doesn't check the order of fields in the origin to match with Windows behavior
			// So if we have 192.168.*.* then the origin as 168.34.45.192 would also match incorrectly
			
			// For each field (www, google, co, uk etc.) check if the field is present in the client origin
			for (String corsConfigField : corsConfigFields)
			{
				// Wild-character, dont compare
				if (corsConfigField.compareToIgnoreCase("*") != 0)
				{					
					 // If not just check if the client contains the field
					 if (!client.contains(corsConfigField))
					 {
						 bResult = false;
						 break; //Fail case, no need to check further
					 }						 
				}
			}
			
			// Found the match, no failures. return
			if (bResult == true)
				return true;
			// Regular expression matching is not yet supported. Only wild-card matching is supported for now.
			//if (client.matches(cors))
				//return true;
			//}
			//catch(PatternSyntaxException e)
			//{	
				//Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Regular expression specified is not valid"));
			//}
		}
		
		return false;
	}
	
	/** (non-Javadoc)
	 * @see java.lang.Runnable#run()
	 */
	@Override
	public void run()
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	    try
	    {	    		
        	    String data = mSocketInput.readLine();
        	    if( data == null || data.equals("") )
        		return;
        	    StringTokenizer st = new StringTokenizer(data, " " );
        	    String method = st.nextToken();
        	    String fileName = st.nextToken();
        	    String version = st.nextToken();        	    
        	    String resourceName = Common.parseAndroidURI(path + fileName).getPath();
        	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Requested resource: " + resourceName));      
        	    
        	    
            	
        	    //Parse the additional headers
        	    for (String line; (line = mSocketInput.readLine()) != null;) 
        	    {
                    if (line.isEmpty()) 
                    	break; // Stop when headers are completed. We're not interested in all the HTML.
                    
                    // Check the CORS header
                    if (line.startsWith("Origin"))
                    {
                    	String corsValueClient = line.substring(8); //eg. Origin: 192.168.4.128 or www.google.com 
                    	
                    	// Define the CORS response header
                    	originResponse = HTTP_CORS + corsValueClient + "\r\n";
                    	
                    	if (corsValueConfig != null)
                    	{
                    		if (corsValueConfig == "*")
                    		{ 
                    			//Allow all        
                    		}
                    		else
                    		{
                    			if ((corsValueClient.compareToIgnoreCase("null") == 0) ||                    					
                    				!(CheckOrigin(corsValueConfig, corsValueClient)))		                    	
		                    	{
                    				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Origin not allowed"));
		        					mSocketOutput.writeBytes(HTTP404_HEADER);
		        	        		mSocketOutput.writeBytes(HTTP_SERVERNAME);
		                	        mSocketOutput.writeBytes(HTTP_ACCEPTRANGES);		                	        
		                	        if (originResponse != null)
		                	        {
		                	        	mSocketOutput.writeBytes(originResponse);
		                	        	mSocketOutput.writeBytes(String.format(HTTP_CONTENT_LEN, HTTP_CORS_FILE_SIZE));
		                	        }
		                	        else
		                	        {
		                	        	mSocketOutput.writeBytes(String.format(HTTP_CONTENT_LEN, HTTP_DEFAULT_FILE_SIZE));
		                	        }
		                	        mSocketOutput.writeBytes(HTTP_CACHE_CONTROL);
		                	        mSocketOutput.writeBytes(HTTP_CONNECTION);		                	       
		                	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT, "text/html"));
		                	        mSocketOutput.writeBytes("\r\n");
		                	        
		                	        mSocketOutput.writeBytes("<html>\r\n");
		                	    	mSocketOutput.writeBytes("\t<head>\r\n");  
		                	    	mSocketOutput.writeBytes("\t\t<title>RhoElements</title>\r\n");
		                	    	mSocketOutput.writeBytes("\t</head>\r\n"); 
		                	    	mSocketOutput.writeBytes("\t<body>\r\n");  
		                	    	mSocketOutput.writeBytes("\t\t<h1>CORS origin not allowed</h1>\r\n");
		                	    	mSocketOutput.writeBytes("\t</body>\r\n"); 
		                	    	mSocketOutput.writeBytes("</html>\r\n");  
		                	    	
		                	    	mSocketOutput.flush();	 
		        					return;
		                    	} 
		                    	else
		                    	{
		                    		// Success		                    		
		                    	}
                    		}                    		
                    	}
                    	else
                    	{
                    		// If the server doesn't have CORS setting then no need to check
                    	}
                    }
                    else
                    {
                    	// Currently we dont check the headers other than "Origin"
                    }
                }
        	    
        	    //Push plugin
        	    if (isPushServer)
        	    {
        	    	PushPlugin push = (PushPlugin) Common.pluginManager.getPlugin("Push");
        			if (push != null)
        			{
        				boolean handlePush = push.handle(fileName); //This handles the arguments supplied in the URL
        				if (handlePush == false)
        				{
        					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Failed to handle the Push response"));
        					mSocketOutput.writeBytes(HTTP404_HEADER);
        	        		mSocketOutput.writeBytes(HTTP_SERVERNAME);
                	        mSocketOutput.writeBytes(HTTP_ACCEPTRANGES);
                	        if (originResponse != null) mSocketOutput.writeBytes(originResponse);                	                       	        	
                	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT_LEN, HTTP_DEFAULT_FILE_SIZE));                	        
                	        mSocketOutput.writeBytes(HTTP_CACHE_CONTROL);
                	        mSocketOutput.writeBytes(HTTP_CONNECTION);
                	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT, "text/html"));
                	        mSocketOutput.writeBytes("\r\n");
                	        
                	        mSocketOutput.writeBytes("<html>\r\n");
                	    	mSocketOutput.writeBytes("\t<head>\r\n");  
                	    	mSocketOutput.writeBytes("\t\t<title>RhoElements</title>\r\n");
                	    	mSocketOutput.writeBytes("\t</head>\r\n"); 
                	    	mSocketOutput.writeBytes("\t<body>\r\n");  
                	    	mSocketOutput.writeBytes("\t\t<h1>404 Page Not Found</h1>\r\n");
                	    	mSocketOutput.writeBytes("\t</body>\r\n"); 
                	    	mSocketOutput.writeBytes("</html>\r\n");  
                	    	
                	    	mSocketOutput.flush();	 
        					return;
        				}
        				resourceName = push.getResponsePage(); //Get the response page parameter from the Push plugin
        				if (resourceName == null)        				
        				{
        					// The user hasn't provided any response page, so construct a default response message
        					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "HTTP construct new response"));
        					mSocketOutput.writeBytes(HTTP200_HEADER);
                	        mSocketOutput.writeBytes(HTTP_SERVERNAME);
                	        mSocketOutput.writeBytes(HTTP_ACCEPTRANGES);
                	        if (originResponse != null) mSocketOutput.writeBytes(originResponse); 
                	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT_LEN, HTTP_DEFAULT_FILE_SIZE));                	        
                	        mSocketOutput.writeBytes(HTTP_CACHE_CONTROL);
                	        mSocketOutput.writeBytes(HTTP_CONNECTION);                	        
                	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT, "text/html"));
                	        mSocketOutput.writeBytes("\r\n");
                	        
            	    		mSocketOutput.writeBytes("<html>\r\n");
                	    	mSocketOutput.writeBytes("\t<head>\r\n");  
                	    	mSocketOutput.writeBytes("\t\t<title>RhoElements</title>\r\n");
                	    	mSocketOutput.writeBytes("\t</head>\r\n"); 
                	    	mSocketOutput.writeBytes("\t<body>\r\n");  
                	    	mSocketOutput.writeBytes("\t\t<h1>REPush received OK</h1>\r\n");
                	    	mSocketOutput.writeBytes("\t</body>\r\n"); 
                	    	mSocketOutput.writeBytes("</html>\r\n");                	    	  
                	        mSocketOutput.flush(); 
                	        return;
        				}
        			}
        			else
        			{
        				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Failed to get the Push plugin object"));
        				mSocketOutput.writeBytes(HTTP404_HEADER);
    	        		mSocketOutput.writeBytes(HTTP_SERVERNAME);
            	        mSocketOutput.writeBytes(HTTP_ACCEPTRANGES);
            	        if (originResponse != null) mSocketOutput.writeBytes(originResponse); 
            	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT_LEN, HTTP_DEFAULT_FILE_SIZE));            	       
            	        mSocketOutput.writeBytes(HTTP_CACHE_CONTROL);
            	        mSocketOutput.writeBytes(HTTP_CONNECTION);            	       
            	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT, "text/html"));
            	        mSocketOutput.writeBytes("\r\n");
            	        
            	        mSocketOutput.writeBytes("<html>\r\n");
            	    	mSocketOutput.writeBytes("\t<head>\r\n");  
            	    	mSocketOutput.writeBytes("\t\t<title>RhoElements</title>\r\n");
            	    	mSocketOutput.writeBytes("\t</head>\r\n"); 
            	    	mSocketOutput.writeBytes("\t<body>\r\n");  
            	    	mSocketOutput.writeBytes("\t\t<h1>404 Page Not Found</h1>\r\n");
            	    	mSocketOutput.writeBytes("\t</body>\r\n"); 
            	    	mSocketOutput.writeBytes("</html>\r\n");  
            	    	
            	    	mSocketOutput.flush();	 
        				return;
        			}
        	    }        	            	    
        	    File resource = new File(resourceName);        	   
        	    
        	    if( !resource.exists() )
        	    {        	    	
	        		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "HTTP 404 Not found"));
	        		mSocketOutput.writeBytes(HTTP404_HEADER);
	        		mSocketOutput.writeBytes(HTTP_SERVERNAME);
        	        mSocketOutput.writeBytes(HTTP_ACCEPTRANGES);
        	        if (originResponse != null) mSocketOutput.writeBytes(originResponse);
        	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT_LEN, HTTP_DEFAULT_FILE_SIZE));        	        
        	        mSocketOutput.writeBytes(HTTP_CACHE_CONTROL);
        	        mSocketOutput.writeBytes(HTTP_CONNECTION);        	       
        	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT, "text/html"));
        	        mSocketOutput.writeBytes("\r\n");
        	        
        	        mSocketOutput.writeBytes("<html>\r\n");
        	    	mSocketOutput.writeBytes("\t<head>\r\n");  
        	    	mSocketOutput.writeBytes("\t\t<title>RhoElements</title>\r\n");
        	    	mSocketOutput.writeBytes("\t</head>\r\n"); 
        	    	mSocketOutput.writeBytes("\t<body>\r\n");  
        	    	mSocketOutput.writeBytes("\t\t<h1>404 Page Not Found</h1>\r\n");
        	    	mSocketOutput.writeBytes("\t</body>\r\n"); 
        	    	mSocketOutput.writeBytes("</html>\r\n");  
        	    	
        	    	mSocketOutput.flush();	        	            	    	
        	    }
        	    else if (resource.isDirectory())
        	    {
	        		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "HTTP 302 redirect"));
	        		mSocketOutput.writeBytes(HTTP302_HEADER);
	        		mSocketOutput.writeBytes("Location: index.html");
        	    }
        	    else
        	    {
	        		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "HTTP 200 OK"));
	        		FileInputStream resourceFileReader = new FileInputStream(resource);
        	        mSocketOutput.writeBytes(HTTP200_HEADER);
        	        mSocketOutput.writeBytes(HTTP_SERVERNAME);
        	        mSocketOutput.writeBytes(HTTP_ACCEPTRANGES);
        	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT_LEN, resource.length()));
        	        mSocketOutput.writeBytes(HTTP_CACHE_CONTROL);
        	        if (originResponse != null) mSocketOutput.writeBytes(originResponse);
        	        mSocketOutput.writeBytes(HTTP_CONNECTION);
        	        mSocketOutput.writeBytes(String.format(HTTP_CONTENT, mimeType(resourceName)));
        	        mSocketOutput.writeBytes("\r\n");
        	        
        	        byte[] buffer = new byte[10240];
        	        int length;
        	        while ((length = resourceFileReader.read(buffer)) != -1)
        	        {
        	            mSocketOutput.write(buffer, 0, length);
        	        }
        	        mSocketOutput.flush();
        	        resourceFileReader.close();
        	    }
	    }	
	    catch (IOException e)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	    } 
	    catch (InvalidParameterException e)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	    }
	    catch (URISyntaxException e)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	    }
	    finally
	    {
		cleanUp();
	    }
	}   
    }
}
