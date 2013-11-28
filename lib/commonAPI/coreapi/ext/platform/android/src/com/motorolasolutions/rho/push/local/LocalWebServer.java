package com.motorolasolutions.rho.push.local;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.URI;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.util.SparseArray;

import com.rhomobile.rhodes.Logger;

public class LocalWebServer
{
	public static class HttpResourceParser
	{
		private String path;
		private Map<String,String> queryStrings;
		
		private HttpResourceParser(String path, Map<String,String> queryStrings)
		{
			this.path = path;
			this.queryStrings = queryStrings;
		}
		
		public String getPath()
		{
			return path;
		}
		
		public String getQueryValue(String queryName)
		{
			return queryStrings.get(queryName);
		}

		public Map<String, String> getQueryStrings()
		{
			return queryStrings;
		}

		public static HttpResourceParser parseGet(String resourceString)
		{
			String[] resourceDivide = resourceString.split("[?]");
			String path = resourceDivide[0];
			String queryString = getQueryString(resourceDivide);
			return parsePost(path, queryString);
		}
		
		public static HttpResourceParser parsePost(String path, String body)
		{
			String[] queryParts = body.split("[&]");
			Map<String,String> queryStrings = new HashMap<String,String>();
			
			for(String queryPart : queryParts)
			{
				String[] splitQueryPart = queryPart.split("[=]");
				String name = splitQueryPart[0];
				String value = "";
				
				if(splitQueryPart.length > 1)
				{
					StringBuilder valueBuilder = new StringBuilder(splitQueryPart[1]);
					for(int i = 1; i < splitQueryPart.length; i++)
					{
						valueBuilder.append('=');
						valueBuilder.append(splitQueryPart[i]);
					}
					value = valueBuilder.toString();
				}
				queryStrings.put(name, value);
			}
			return new HttpResourceParser(path, queryStrings);
		}

		
		/**
		 * Builds the query string, given a String[] created by calling resource.split('[?]') where resource
		 * is the resource string found in the first line of an HTTP request 
		 * @param resourceDivide created by calling resource.split('[?]') where resource
		 * is the resource string found in the first line of an HTTP request
		 * @return The resource string of the given resourceDivide, without the preceding '?' char
		 * @author Ben Kennedy (NCVT73)
		 */
		private static String getQueryString(String[] resourceDivide)
		{
			if(resourceDivide.length > 1)
			{
				StringBuilder builder = new StringBuilder(resourceDivide[1]);
				for(int i = 2; i < resourceDivide.length; i++)
				{
					builder.append('?');
					builder.append(resourceDivide[i]);
				}
				return builder.toString();
			}
			return "";
		}
	}

	protected static String TAG = "Push.LocalServer";
	
	private static final String HTTP_SERVERNAME 		= "Server: RhoElements\r\n";
	private static final String HTTP_ACCEPTRANGES 		= "Accept-Ranges: bytes\r\n";
	private static final String HTTP200_HEADER 			= "HTTP/1.1 200 OK\r\n";
	private static final String HTTP404_HEADER 			= "HTTP/1.1 404 Not Found\r\n";
	private static final String HTTP405_HEADER 			= "HTTP/1.1 405 Method Not Allowed\r\n";
	private static final String HTTP_CACHE_CONTROL 		= "Cache-control: no-cache,must-revalidate\r\nPragma: no-cache\r\n";
	private static final String HTTP_CONNECTION 		= "Connection: close\r\n";
	private static final String HTTP_CORS 				= "Access-Control-Allow-Origin: %s\r\n";
	private static final String HTTP_CONTENT 			= "Content-Type: %s\r\n";
	private static final String HTTP_CONTENT_LEN 		= "Content-Length: %d\r\n";
	private static final String HTML_404_CONTENT 		= "<!DOCTYPE html><html><head><title>RhoElements</title></head><body><h1>404 Page Not Found</h1></body></html>\r\n";
	private static final String HTML_405_CONTENT 		= "<!DOCTYPE html><html><head><title>RhoElements</title></head><body><h1>405 Method Not Allowed</h1></body></html>\r\n";
	private static final String PASSKEY_LOWERCASE 		= "passkey";
	
	// private static final int 	HTTP_DEFAULT_FILE_SIZE 	= 116;
	// private static final int 	HTTP_CORS_FILE_SIZE 	= 121;
	// private static final String HTTP302_HEADER 			= "HTTP/1.1 302 Redirect\r\n";
	// private static final String HTTP400_HEADER 		= "HTTP/1.1 400 Bad Request\r\n";
	// private static final String HTTP403_HEADER 		= "HTTP/1.1 403	Forbidden\r\n";
	// private static final String HTTP500_HEADER 		= "HTTP/1.1 500 Internal Server Error\r\n";
	// private static final String HTTP_DEFAULT_PAGE 	= "/index.html";
	//private static final String HTML_BAD_CORS_CONTENT = "<!DOCTYPE html><html><head><title>RhoElements</title></head><body><h1>CORS origin not allowed</h1></body></html>\r\n";
	
	private static LocalWebServer localWebServer = null;

	private static Pattern originHeaderPattern = Pattern.compile("Origin:\\s*(.*)");
	private SparseArray<RhoPushServerSocket> listeningPorts;

	private LocalWebServer()
	{
		listeningPorts = new SparseArray<RhoPushServerSocket>();
	}
	
	/**
	 * Registers a Local Push object. Starts listening for Push requests on the specified port & path
	 * @param port the TCP port for the push server to listen to. 
	 * @param path the path for the push server to listen to. If "" || null, then listens for all paths.
	 * @param pushObject the Local Push object to inform when the push event occurs.
	 * @return true if the request was successful, false if there is an IOException or if there is
	 * already a Local Push object listening to this port&path combo. Please be aware, that the "all port"
	 * listener overrides specific port listeners.
	 * @author Ben Kennedy (NCVT73) 
	 */
	public boolean registerPushAccount(int port, String path, Local pushObject)
	{
		RhoPushServerSocket currentServerSocket = listeningPorts.get(port);
		if(currentServerSocket == null)
		{
			try
			{
				currentServerSocket = new RhoPushServerSocket(port);
				listeningPorts.put(port, currentServerSocket);
			}
			catch(IOException e)
			{
				Logger.E(TAG, e.getMessage());
				return false;
			}
		}
		return currentServerSocket.registerPushListener(path, pushObject);
	}
	
	/**
	 * Registers a Local Push object. Starts listening for Push requests on the specified port & path
	 * @param pushObject the Local Push object to inform when the push event occurs.
	 * @return true if the request was successful, false if there is an IOException or if there is
	 * already a Local Push object listening to this port&path combo. Please be aware, that the "all port"
	 * listener overrides specific port listeners.
	 * @author Ben Kennedy (NCVT73) 
	 */
	public boolean registerPushAccount(Local pushObject)
	{
		return registerPushAccount(pushObject.getPort(), pushObject.getPath(), pushObject);
	}
	
	/**
	 * Removes a push object from the ServerSocket. If no push object is listening on this port, close the server;
	 * @param pushObject the pushObject to unregister
	 * @param port the port of the Push object which was used to register it to this server
	 * @param path the path of the Push object which was used to register it to this server
	 * @return true if the pushObject was registered. false otherwise.
	 * @author Ben Kennedy (NCVT73)
	 */
	public boolean unregisterPushAccount(Local pushObject, int port, String path)
	{
		RhoPushServerSocket currentServerSocket = listeningPorts.get(port);
		if(currentServerSocket == null) return false;
		if(!currentServerSocket.unregisterPushListener(path, pushObject)) return false;
		
		//If no more Push objects listening on this port...
		if(currentServerSocket.getRegisteredPushCount() < 1)
		{
			try
			{
				currentServerSocket.close();
			}
			catch(IOException e)
			{
				Logger.D(TAG, e);
				Logger.W(TAG, "Error in closing server port: " + port);	
			}
			finally
			{
				listeningPorts.remove(port);
			}
		}
		return true;
	}
	
	/**
	 * Removes a push object from the ServerSocket. If no push object is listening on this port, close the server;
	 * @param pushObject the pushObject to unregister
	 * @param oldPort the port of the Push object which was used to register it to this server
	 * @return true if the pushObject was registered. false otherwise.
	 * @author Ben Kennedy (NCVT73)
	 */
	public boolean unregisterPushAccount(Local pushObject, int oldPort)
	{
		return unregisterPushAccount(pushObject, oldPort, pushObject.getPath());
	}

	/**
	 * Removes a push object from the ServerSocket. If no push object is listening on this port, close the server;
	 * @param pushObject the pushObject to unregister
	 * @param oldPath the path of the Push object which was used to register it to this server
	 * @return true if the pushObject was registered. false otherwise.
	 * @author Ben Kennedy (NCVT73)
	 */
	public boolean unregisterPushAccount(Local pushObject, String oldPath)
	{
		return unregisterPushAccount(pushObject, pushObject.getPort(), oldPath);
	}
	
	/**
	 * Removes a push object from the ServerSocket. If no push object is listening on this port, close the server;
	 * @param pushObject the pushObject to unregister
	 * @return true if the pushObject was registered. false otherwise.
	 * @author Ben Kennedy (NCVT73)
	 */
	public boolean unregisterPushAccount(Local pushObject)
	{
		return unregisterPushAccount(pushObject, pushObject.getPort(), pushObject.getPath());
	}
	
	private class RhoPushServerSocket extends ServerSocket
	{
		private HashMap<String, Local> pathsToCallbacksMaps;
		private boolean isWildcardPath;
		private WebServerThread serverThread;
		
		public RhoPushServerSocket(int port) throws IOException
		{
			super(port);
			pathsToCallbacksMaps = new HashMap<String, Local>();
		}
		
		/**
		 * Adds a new Push listen setting-set to the Server Socket. Succeeds if the path is not already in use.
		 * If a wildcard (any path) value is set, then any following calls will fail unless the wildcard case is unset.
		 * @param path The path to which to listen 
		 * @param pushObject The Calling Push.Local object to send and push events to.
		 * @return true if the path is not already being listened to, false otherwise.
		 * @author Ben Kennedy (NCVT73)
		 * @param corsOriginsAcceptArray 
		 */
		public boolean registerPushListener(String path, Local pushObject)
		{ 
			if(isWildcardPath) return false;
			if(pathsToCallbacksMaps.containsKey(path)) return false;
			if(pathsToCallbacksMaps.put(path, pushObject) != null) return false;
			if("*".equals(path))
			{
				isWildcardPath = true;
			}
			start();
			return true;
		}
		
		/**
		 * Removes a Push listen setting-set from the Server Socket. Succeeds if the given path and pushObject matches
		 * that which was given in registerPushListener.
		 * @param path The path to remove
		 * @param pushObject The Calling Push.Local object used to register
		 * @return true if the given path and pushObject matches that which was given in registerPushListener, false otherwise.
		 * @author Ben Kennedy (NCVT73)
		 */
		public boolean unregisterPushListener(String path, Local pushObject)
		{
			Local pushToUnregister = pathsToCallbacksMaps.get(path);
			if(pushToUnregister == null) return false;
			if(pushToUnregister.equals(pushObject)) return false;
			pathsToCallbacksMaps.remove(path);
			if("*".equals(path))
			{
				isWildcardPath = false;
			}
			return true;
		}
		
		/**
		 * Returns the number of registered push objects
		 * @return the number of registered push objects
		 * @author Ben Kennedy (NCVT73)
		 */
		public int getRegisteredPushCount()
		{
			return pathsToCallbacksMaps.size();
		}
		
		/**
		 * Start listening to requests on the server socket
		 * @author Ben Kennedy (NCVT73)
		 */
		private void start()
		{
			if(serverThread != null) return;
			serverThread = new WebServerThread(this);
			serverThread.start();
		}

		@Override
		public void close() throws IOException
		{
			if(serverThread != null) serverThread.end();
			super.close();
		}
		
		/**
		 * Gets the Local Push object which is associated with the given path
		 * @param path the path of the request
		 * @return the Local object associated with this path (null if none found)
		 */
		public Local getPushObject(String path)
		{
			if(isWildcardPath) return pathsToCallbacksMaps.get("*");
			return pathsToCallbacksMaps.get(path);
		}
		
		private class WebServerThread extends Thread
		{
			private RhoPushServerSocket serverSocket;
			private WebSocketClientThread clientSocketThread;
			private boolean isClosing = false;
			
			public WebServerThread(RhoPushServerSocket serverSocket)
			{
				super();
				this.serverSocket = serverSocket;
			}
			
			@Override
			public void run()
			{
				Logger.D(TAG, "WebServerThread+");
				while (!isClosing)
				{
					try
					{
						//When the ServerSocket is no longer needed, it will be closed and this line will throw SocketException.
						Socket clientSocket = serverSocket.accept();
						clientSocketThread = new WebSocketClientThread(clientSocket, serverSocket);
						clientSocketThread.start();
					}
					catch(SocketException e)
					{
						Logger.I(TAG, "SocketException: This Push Port should be closing...");
					}
					catch (IOException e)
					{
						Logger.E(TAG, e.getMessage());
					}
					catch (NullPointerException e)
					{
						Logger.E(TAG, e.getMessage());
					}
				}
				//TODO clientSocketThread cleanup // I dont think this is needed, the thread will complete then exit?
				Logger.D(TAG, "WebServerThread-");
			}

			public void end()
			{
				isClosing = true;
			}
			
			private class WebSocketClientThread extends Thread
			{
				private RhoPushServerSocket serverSocket;
				private Socket mClientSocket;
				private DataInputStream mSocketInput;
				private DataOutputStream mSocketOutput;

				public WebSocketClientThread(Socket clientSocket, RhoPushServerSocket serverSocket)
				{
					Logger.D(TAG, "WebSocketClientThread+");
					this.mClientSocket = clientSocket;
					this.serverSocket = serverSocket;
					
					try
					{
						mSocketInput = new DataInputStream(new BufferedInputStream(mClientSocket.getInputStream()));
						mSocketOutput = new DataOutputStream(new BufferedOutputStream(mClientSocket.getOutputStream()));
					}
					catch (IOException e)
					{
						Logger.E(TAG, e.getMessage());
					}
					Logger.D(TAG, "WebSocketClientThread-");
				}

				@Override
				public void run()
				{
					Logger.D(TAG, "WebSocketClientThread run+");
					try
					{
						String data = mSocketInput.readLine();
						if (data == null || data.equals("")) return;
						StringTokenizer st = new StringTokenizer(data, " ");
						String method = st.nextToken();
						String fileName = st.nextToken(); // Includes GET query string
						String version = st.nextToken(); // Ignored
						String acceptOriginHeader = null;
						String originHeaderValue = null;
						
						// Parse the additional headers. We currently only look for Origin
						for (String line; (line = mSocketInput.readLine()) != null && !line.isEmpty();)
						{
							Matcher originHeaderMatcher = originHeaderPattern.matcher(line);
							if (originHeaderMatcher.matches())
							{
								originHeaderValue = originHeaderMatcher.group(1);
							}
						}
						
						HttpResourceParser resourceObject;
						
						//Get Query String...
						if("GET".equals(method))
						{
							resourceObject = HttpResourceParser.parseGet(fileName);							
						}
						else if("POST".equals(method))
						{
							String body = mSocketInput.readLine();
							resourceObject = HttpResourceParser.parsePost(fileName, body);
						}
						else
						{
							Logger.E(TAG, "Unsupported HTTP Request Method: " + method);
							output405Response(mSocketOutput);
							mSocketOutput.flush();
							return;
						}
						Logger.D(TAG, "Requested resource: " + resourceObject.getPath());

						//Find related Push.Local object
						Local pushObject = serverSocket.getPushObject(resourceObject.getPath());
						if(pushObject == null)
						{
							//Return 404, no registered push on this path
							Logger.E(TAG, "Failed to get the Push plugin object");
							output404Response(mSocketOutput);
							mSocketOutput.flush();
							return;
						}
						
						//If this is a CORS request...
						if(originHeaderValue != null)
						{
							acceptOriginHeader = getAcceptOriginHeader(originHeaderValue, pushObject.getCorsOrigins());
							if(acceptOriginHeader == null)
							{
								//Origin not acceptable
								output200Response(mSocketOutput, acceptOriginHeader, "text/plain", pushObject.getResponseFail());
								mSocketOutput.flush();
								return;
							}
						}
						
						//Check passKey matches, if it exists in the Push.Local object
						String pushPassKey = pushObject.getPassKey();
						String requestPassKey = resourceObject.getQueryValue(PASSKEY_LOWERCASE);
						
						if(pushPassKey == null || pushPassKey.equals(requestPassKey))
						{
							//remove passKey && send push
							Map<String, String> queryStrings = resourceObject.getQueryStrings();
							queryStrings.remove(PASSKEY_LOWERCASE);
							
							//DO THE PUSH CALLBACK!!
							pushObject.sendCallback(queryStrings);
							
							output200Response(mSocketOutput, acceptOriginHeader, "text/plain", pushObject.getResponsePass());
							mSocketOutput.flush();
							return;
						}
						else
						{
							Logger.W(TAG, "Push request refused. Incorrect passkey value");
							output200Response(mSocketOutput, acceptOriginHeader, "text/plain", pushObject.getResponseFail());
							mSocketOutput.flush();
							return;
						}
					}
					catch (IOException e)
					{
						Logger.E(TAG, e.getMessage());
					}
					catch (InvalidParameterException e)
					{
						Logger.E(TAG, e.getMessage());
					}
					finally
					{
						cleanUp();
						Logger.D(TAG, "WebSocketClientThread run-");
					}
				}
			
				/**
				 * Cleans up the sockets
				 */
				private void cleanUp()
				{
					Logger.D(TAG, "cleanUp+");
					try {
						if(mSocketOutput != null)	mSocketOutput.close();
					}
					catch (IOException e) {
						Logger.E(TAG, e.getMessage());
					}
					try {
						if(mSocketInput != null)	mSocketInput.close();
					}
					catch (IOException e) {
						Logger.E(TAG, e.getMessage());
					}
					try {
						if(mClientSocket != null)	mClientSocket.close();
					}
					catch (IOException e) {
						Logger.E(TAG, e.getMessage());
					}
					Logger.D(TAG, "cleanUp-");
				}
			}
		}
	}
		
	/**
	 * Writes a HTTP 404 response to the Output Stream
	 * @param stream the stream to write the data
	 * @throws IOException if an exception occurs in writing the stream
	 * @author Ben Kennedy (NCVT73)
	 */
	private static void output404Response(DataOutputStream stream) throws IOException
	{
		outputHttpHeaders(stream, null, HTML_404_CONTENT.length(), "text/html", "404");//TODO change to enum
		stream.writeBytes(HTML_404_CONTENT);
	}
	
	/**
	 * Writes a HTTP 405 response to the Output Stream
	 * @param stream the stream to write the data
	 * @throws IOException if an exception occurs in writing the stream
	 * @author Ben Kennedy (NCVT73)
	 */
	private static void output405Response(DataOutputStream stream) throws IOException
	{
		outputHttpHeaders(stream, null, HTML_405_CONTENT.length(), "text/html", "405");//TODO change to enum
		stream.writeBytes(HTML_405_CONTENT);
	}
	
	/**
	 * Writes a HTTP 200 response to the Output Stream
	 * @param stream the stream to write the data
	 * @param originResponse
	 * @param mimeType
	 * @param contentString  
	 * @throws IOException if an exception occurs in writing the stream
	 //TODO 
	 */
	private static void output200Response(DataOutputStream stream, String originResponse, String mimeType, String contentString) throws IOException
	{
		outputHttpHeaders(stream, originResponse, contentString.length(), mimeType, "200");
		stream.writeBytes(contentString);
	}
	
	/**
	 * Writes HTTP response headers to the Output Stream
	 * @param stream the stream to write the data
	 * @param originResponse the origin header if required. null if not required.
	 * @param contentLength the length in bytes of the HTTP content
	 * @param mimeType the mimeType to send
	 * @param responseType "404" or "200" 
	 * @throws IOException if an exception occurs in writing the stream
	 * @author Ben Kennedy (NCVT73)
	 */
	private static void outputHttpHeaders(DataOutputStream stream, String originResponse, long contentLength, String mimeType, String responseType) throws IOException
	{
		if(stream == null) throw new IOException("Stream is not valid");
		
		if("200".equals(responseType))	stream.writeBytes(HTTP200_HEADER);
		else if("404".equals(responseType))	stream.writeBytes(HTTP404_HEADER);
		else if("405".equals(responseType)) stream.writeBytes(HTTP405_HEADER);

		stream.writeBytes(HTTP_SERVERNAME);
		stream.writeBytes(HTTP_ACCEPTRANGES);
		if (originResponse != null)
			stream.writeBytes(originResponse);
		stream.writeBytes(String.format(HTTP_CONTENT_LEN, contentLength));
		stream.writeBytes(HTTP_CACHE_CONTROL);
		stream.writeBytes(HTTP_CONNECTION);
		stream.writeBytes(String.format(HTTP_CONTENT, mimeType));
		stream.writeBytes("\r\n");
	}

	/**
	 * Parses an array of acceptable Origins. If an entry containing only an * is found, it will
	 * return an array with just the '*' string.
	 * @param corsOrigins an array of acceptable Origins.
	 * @return a parsed array of acceptable Origins.
	 * @author Ben Kennedy (NCVT73)
	 */
	public static String[] parseCorsOrigins(String[] corsOrigins)
	{
		String[] returnArray = new String[corsOrigins.length];
		int i = 0;
		for(String corsOrigin: corsOrigins)
		{
			//All origins possible
			if("*".equals(corsOrigin))
			{
				return new String[]{"*"};
			}
			returnArray[i++] = corsOrigin;
		}
		return returnArray;
	}
	
	/**
	 * Given an Origin header value, return a correct Access-Control-Allow-Origin header string (including \r\n)
	 * if the origin corresponds to a value in the given array of acceptable values.
	 * @param origin the origin-or-'null' value from the HTTP Origin header 
	 * @param acceptedList a whitelist of Origins. Origins can be a URL, * or 'null'.  
	 * @return the correct Access-Control-Allow-Origin header, or null if given Origin is refused.
	 * @author Ben Kennedy (NCVT73)
	 */
	private static String getAcceptOriginHeader(String origin, String[] acceptedList)
	{
		if(origin == null || acceptedList == null || acceptedList.length < 1) return null;
		if("*".equals(acceptedList[0])) return String.format(HTTP_CORS, "*");
		
		URI originURI = null;
		try
		{
			originURI = new URI(origin);
		}
		catch(URISyntaxException e)
		{
			Logger.W(TAG, "Unrecognised Origin header");
		}
		
		for(String acceptable: acceptedList)
		{
			if(origin.equals(acceptable))
			{
				return String.format(HTTP_CORS, origin);
			}
			
			if(originURI == null) continue;
			
			//We should accept the request even if they are different, but have the same scheme+host+port
			try
			{
				URI acceptableURI = new URI(acceptable);
				if(	originURI.getScheme().equals(acceptableURI.getScheme()) &&
					originURI.getHost().equals(acceptableURI.getHost()) &&
					(originURI.getPort() == -1 ? 80 : originURI.getPort()) == (acceptableURI.getPort() == -1 ? 80 : acceptableURI.getPort())) // if port is default (80)
				{
					return String.format(HTTP_CORS, origin);
				}
			}
			catch(URISyntaxException e)
			{
				Logger.T(TAG, "corsOrigins entry: '" + acceptable + "' is not a correct URI");
			}
		}
		return null;
	}
	
	/**
	 * Returns the LocalWebServer singleton
	 * @return the singleton LocalWebServer
	 * @author Ben Kennedy (NCVT73)
	 */
	public static synchronized LocalWebServer getLocalWebServer()
	{
		if(localWebServer == null) localWebServer = new LocalWebServer();
		return localWebServer;
	}
} 