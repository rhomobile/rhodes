package com.motorolasolutions.rho.push.local;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.rho.push.local.ILocal;
import com.rho.push.local.ILocalSingleton;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

/**
 * 
 * @author Ben Kennedy (NCVT73)
 */
public class Local implements ILocal
{   
	protected static String TAG = "Push.Local";
	protected static String ERROR_ALREADY_REGISTERED = "Port & Path combination already registered.";
	protected static String START_SUCCESS = "Push started successfully: port %i, path %s";
	
	private String id;
	private int port = 8081;
	private String passKey = "";
	private Map<String,String> response;
	private String path = "";
	private MethodResult pushCallback;
	private boolean isStarted = false;
	private String[] corsOrigins = {""};
	private LocalWebServer localWebServer;
	
    public Local(String id, Map<String, Object> propertyMap)
    {
    	Logger.D(TAG, "Constructor+");
    	this.id = id;
    	this.response = new HashMap<String, String>();
    	this.response.put(ILocalSingleton.HK_FAIL, "");
    	this.response.put(ILocalSingleton.HK_PASS, "");
    	if(propertyMap != null)
    	{
    		setPassKey(propertyMap.get(LocalSingleton.HK_PASS_KEY));  		
    		setPort(propertyMap.get(LocalSingleton.HK_PORT));
    		setPath(propertyMap.get(LocalSingleton.HK_PATH));
    		setResponse(propertyMap.get(LocalSingleton.HK_RESPONSE));
    		setCorsOrigins(propertyMap.get(LocalSingleton.HK_CORS_ORIGINS));
    	}      
    	localWebServer = LocalWebServer.getLocalWebServer();
    	Logger.D(TAG, "Constructor-");
    }

    /**
     * Parses and stores the corsOrigins object if the object is valid. This is an array of URLs|[*]|null which describes the acceptable
     * Origins HTTP header value which this Push Object accepts.
     * @param corsOrigins a List of the corsOrigins headers
     * @return true if this was set correctly, false otherwise.
     * @author Ben Kennedy (NCVT73)
     */
    @SuppressWarnings("unchecked")
	private boolean setCorsOrigins(Object object)
	{
		if(object == null || !(object instanceof List<?>)) return false;
		return setCorsOrigins((List<String>) object);
	}
    
    /**
     * Parses and stores the corsOrigins array. This is an array of URLs|[*]|null which describes the acceptable
     * Origins HTTP header value which this Push Object accepts.
     * @param corsOrigins a List of the corsOrigins headers
     * @return true if this was set correctly, false otherwise.
     * @author Ben Kennedy (NCVT73)
     */
    private boolean setCorsOrigins(List<String> corsOrigins)
	{
    	try
    	{
    		this.corsOrigins = LocalWebServer.parseCorsOrigins(corsOrigins.toArray(new String[corsOrigins.size()]));
    	}
    	catch(ArrayStoreException e)
    	{
    		Logger.E(TAG, "CorsOrigins type incorrect");
    		return false;
    	}
    	return true;
	}

	/**
     * Sets the response field to the value of the passed object if the object is valid
     * @param object the response object taken from the propertyMap
     * @return true if the value was changed, false otherwise
     * @author Ben Kennedy (NCVT73)
     */
	private boolean setResponse(Object object)
	{
		if(object == null || !(object instanceof String)) return false;
		return setResponse((String) object);
	}
	
    /**
     * Sets the response field to the value of the passed String
     * @param response the response String value to set
     * @return true if the value was changed, false otherwise
     * @author Ben Kennedy (NCVT73)
     */
	private boolean setResponse(Map<String,String> response)
	{
		if(this.response.equals(response)) return false;
		this.response = response;
		return true;
	}

    /**
     * Sets the path field to the value of the passed object if the object is valid
     * @param object the path object taken from the propertyMap
     * @return true if the value was changed, false otherwise
     * @author Ben Kennedy (NCVT73)
     */
	private boolean setPath(Object object)
	{
		if(object == null || !(object instanceof String)) return false;
		return setPath((String) object);
	}
	
    /**
     * Sets the path field to the value of the passed String
     * @param path the path String value to set
     * @return true if the value was changed, false otherwise
     * @author Ben Kennedy (NCVT73)
     */
	private boolean setPath(String path)
	{
		if(this.path.equals(path)) return false;
		this.path = path;
		return true;
	}
	
    /**
     * Sets the port field to the value of the passed object if the object is valid
     * @param object the port object taken from the propertyMap
     * @return true if the value was changed, false otherwise
     * @author Ben Kennedy (NCVT73)
     */
	private boolean setPort(Object object)
	{
		if(object == null || !(object instanceof Integer)) return false;
		return setPort(((Integer) object).intValue());
	}
	
    /**
     * Sets the port field to the value of the passed int
     * @param port the port int value to set
     * @return true if the value was changed, false otherwise
     * @author Ben Kennedy (NCVT73)
     */
	private boolean setPort(int port)
	{
		if(this.port == port || !validatePort(port)) return false;
		this.port = port;
		return true;
	}

	/**
     * Sets the passKey field to the value of the passed object if the object is valid
     * @param object the passKey object taken from the propertyMap
     * @return true if the value was changed, false otherwise
     * @author Ben Kennedy (NCVT73)
     */
	private boolean setPassKey(Object object)
	{
		if(object == null || !(object instanceof String)) return false;
		return setPassKey((String) object);
	}

    /**
     * Sets the passKey field to the value of the passed String
     * @param passKey the passKey String value to set
     * @return true if the value was changed, false otherwise
     * @author Ben Kennedy (NCVT73)
     */
	private boolean setPassKey(String passKey)
	{
		if(this.passKey != null && this.passKey.equals(passKey) || this.passKey == null && this.passKey == passKey) return false;
		this.passKey = passKey;
		return true;
	}
	
	/**
	 * Validates that the parameter is within the range of 1025-65535 (inclusive)
	 * @param newPort the value to check
	 * @return true if valid, false otherwise
	 * @author Ben Kennedy (NCVT73)
	 */
	private boolean validatePort(Integer newPort)
	{
		if(newPort <= 65535 && newPort >= 1025) return true;
		return false;
	}

	@Override
	public String getId()
	{
		Logger.D(TAG, "getId: " + id);
		return id;
	}

	@Override
	public void getPort(IMethodResult result)
	{
		Logger.D(TAG, "getPort: " + port);
		result.set(port);
	}

	@Override
	public void getPassKey(IMethodResult result)
	{
		Logger.D(TAG, "getPassKey: " + passKey);
		result.set(passKey);
	}
	
	@SuppressWarnings("unchecked")
	@Override
	public void getResponse(IMethodResult result)
	{
		Logger.D(TAG, "getResponse: " + response);
		result.set((Map<String, Object>)(Object)response);
	}

	@Override
	public void getPath(IMethodResult result)
	{
		Logger.D(TAG, "getPath: " + path);
		result.set(path);
	}
	
	@Override
	public void getCorsOrigins(IMethodResult result)
	{
		Logger.D(TAG, "getCorsOrigins");
		result.set(new ArrayList<Object>(Arrays.asList(corsOrigins)));	
	}

	@Override
	public void start(IMethodResult result)
	{
		Logger.D(TAG, "start+");
		if(result != null)
		{
			if(!localWebServer.registerPushAccount(this))
			{
				Logger.E(TAG, ERROR_ALREADY_REGISTERED + "Push server not started");
				result.setError(ERROR_ALREADY_REGISTERED + "Push server not started");
			}
			else
			{
				Logger.I(TAG, String.format(START_SUCCESS, this.port, this.path));
				pushCallback = ((MethodResult) result);
				pushCallback.keepAlive();
				isStarted = true;
			}
		}
		Logger.D(TAG, "start-");
	}

	@Override
	public void stop(IMethodResult result)
	{
		Logger.D(TAG, "stop+");
		if(isStarted)
		{
			localWebServer.unregisterPushAccount(this);
			isStarted  = false;
		}
		else
		{
			Logger.I(TAG, "Call to stop Push server ignored. Push server is already stopped");
		}
		if(pushCallback != null) pushCallback.release();
		Logger.D(TAG, "stop-");
	}

	//@Override
	public void getAllProperties(IMethodResult result)
	{
		HashMap<String, Object> map = new HashMap<String, Object>(4);
		map.put(LocalSingleton.HK_PASS_KEY, passKey);
		map.put(LocalSingleton.HK_PATH, path);
		map.put(LocalSingleton.HK_PORT, port);
		map.put(LocalSingleton.HK_RESPONSE, response);
		result.set(map);
	}

	//@Override
	public void getProperty(String name, IMethodResult result)
	{
		try
		{
			Object returnObj = getProperty(name);
			if(returnObj instanceof String) result.set((String) returnObj);
			if(returnObj instanceof Integer) result.set((Integer) returnObj);
		}
		catch(IllegalArgumentException e)
		{
			result.setError("Property " + name + " not recognised");
		}
	}
	
	/**
	 * Returns the property corresponding to the given name
	 * @param name the name of the property to get
	 * @return the field corresponding to the name parameter
	 * @throws IllegalArgumentException thrown if the name is not recognised
	 * @author Ben Kennedy (NCVT73)
	 */
	public Object getProperty(String name) throws IllegalArgumentException
	{
		if(name.equals(LocalSingleton.HK_PASS_KEY)) 	 return passKey;
		else if(name.equals(LocalSingleton.HK_PATH))	 return path;
		else if(name.equals(LocalSingleton.HK_PORT))	 return port;
		else if(name.equals(LocalSingleton.HK_RESPONSE)) return response;
		throw new IllegalArgumentException(name + " not found");
	}

//	//@Override
//	public void getProperties(List<String> names, IMethodResult result)
//	{
//		HashMap<String, Object> map = new HashMap<String, Object>(4);
//		
//		for(String name: names)
//		{
//			try
//			{
//				map.put(name, getProperty(name));
//			}
//			catch(IllegalArgumentException e)
//			{
//				Logger.W(TAG, e.getMessage());
//			}
//		}
//		result.set(map);
//	}
//
//	//@Override
//	public void setProperty(String name, String val, IMethodResult result)
//	{
//		try
//		{
//			setProperty(name, val);
//		}
//		catch(IllegalArgumentException e)
//		{
//			result.setError("Property " + name + " not recognised");
//		}
//	}
	
//	/**
//	 * Sets the property corresponding to the given name
//	 * @param name the name of the property to set
//	 * @param val the value of the property to set
//	 * @return if the property has changed or not
//	 * @throws IllegalArgumentException thrown if the name is not recognised
//	 * @author Ben Kennedy (NCVT73)
//	 */
//	public boolean setProperty(String name, String val) throws IllegalArgumentException
//	{
//		if(name.equals(LocalSingleton.HK_PASS_KEY)) 	 return setPassKey(val);
//		else if(name.equals(LocalSingleton.HK_PATH))	 return setPath(val);
//		else if(name.equals(LocalSingleton.HK_PORT))	 return setPort((Object) port); 		//TODOO all this is wrong
//		else if(name.equals(LocalSingleton.HK_RESPONSE)) return setResponse(response);			//TODOO all this is wrong
//		else if(name.equals(LocalSingleton.HK_CORS_ORIGINS)) return setCorsOrigins(response);	//TODOO all this is wrong
//		throw new IllegalArgumentException(name + " not found");
//	}
	
//	@Override
//	public void setProperties(Map<String, String> props, IMethodResult result)
//	{
//		for(String name: props.keySet())
//		{
//			try
//			{
//				setProperty(name, props.get(name));
//			}
//			catch(IllegalArgumentException e)
//			{
//				Logger.W(TAG, e.getMessage());
//			}
//		}
//	}

	@Override
	public void setPort(int port, IMethodResult result)
	{
		int oldPort = this.port;
		if(!setPort(port) && !validatePort(port))
		{
			result.setError("Requested port is invalid. Port range is 1024-65535");
		}
		else if(isStarted)
		{
			localWebServer.unregisterPushAccount(this, oldPort);
			if(localWebServer.registerPushAccount(port, getPath(), this)) return;
			else
			{
				Logger.E(TAG, ERROR_ALREADY_REGISTERED + " Push server stopped.");
				result.setError(ERROR_ALREADY_REGISTERED + " Push server stopped.");
				isStarted = false;
			}
		}
	}

	@Override
	public void setPassKey(String passKey, IMethodResult result)
	{
		if(!setPassKey(passKey)) Logger.I(TAG, "PassKey setting failed. Old passKey is the same as the new passKey");
	}

	@Override
	public void setResponse(Map<String, String> response, IMethodResult result)
	{
		if(!setResponse(response)) Logger.I(TAG, "Response setting failed. Old response is the same as the new response");
	}

	@Override
	public void setPath(String path, IMethodResult result)
	{
		String oldPath = this.path;
		if(!setPath(path)) Logger.I(TAG, "Response setting failed. Old path is the same as the new path");
		else if(isStarted)
		{
			localWebServer.unregisterPushAccount(this, oldPath);
			if(localWebServer.registerPushAccount(getPort(), path, this)) return;
			else
			{
				Logger.E(TAG, "Port & Path combination already registered. Push server stopped.");
				result.setError("Port & Path combination already registered. Push server stopped.");
				isStarted = false;
			}
		}
	}

	@Override
	public void setCorsOrigins(List<String> corsOrigins, IMethodResult result)
	{
		if(!setCorsOrigins(corsOrigins))  Logger.I(TAG, "Setting corsOrigins failed. CORS Origins have not changed");
	}

	public int getPort()
	{
		return port;
	}

	public String getPath()
	{
		return path;
	}
	
	public String[] getCorsOrigins()
	{
		return corsOrigins;
	}

	public String getResponseFail()
	{
		return response.get(ILocalSingleton.HK_FAIL);
	}
	
	public String getResponsePass()
	{
		return response.get(ILocalSingleton.HK_PASS);
	}

	public String getPassKey()
	{
		return passKey;
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	public void sendCallback(Map<String, String> queryStrings)
	{
		if(pushCallback != null)
		{
			pushCallback.set((Map) queryStrings);
		}
	}
	//TODO push close (pushCallbackRelease/Push server destroy)
}