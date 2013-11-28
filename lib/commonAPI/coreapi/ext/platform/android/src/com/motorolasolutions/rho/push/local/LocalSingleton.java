package com.motorolasolutions.rho.push.local;

import java.util.HashMap;
import java.util.Map;

import com.rho.push.local.ILocal;
import com.rho.push.local.ILocalSingleton;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;

class LocalSingleton implements ILocalSingleton
{
	protected static String TAG = "Push.LocalSingleton";
	protected String defaultId = "0";
	protected int lastKey = 0;
	protected LocalFactory factory;
	protected HashMap<String, Local> localMap;
	protected static Map<String, Object> defaultSettings = new HashMap<String, Object>(4);

	static
	{
		defaultSettings.put(HK_PASS_KEY, "");
		defaultSettings.put(HK_PATH, "");
		defaultSettings.put(HK_PORT, 8081);
		defaultSettings.put(HK_RESPONSE, "");
	}
	
	public LocalSingleton(LocalFactory factory)
	{
		this.factory = factory;
		localMap = new HashMap<String, Local>();
		localMap.put(defaultId, new Local(defaultId, defaultSettings));
	}

	@Override
	public String getDefaultID()
	{
		Logger.D(TAG, "getDefaultID");
		return defaultId;
	}

	@Override
	public void setDefaultID(String id)
	{
		Logger.D(TAG, "setDefaultID " + id);
		defaultId = id;
	}

	@Override
	public void createNew(Map<String, Object> propertyMap, IMethodResult result)
	{
		Logger.D(TAG, "createNew+");
		String id = getNewId();
		Local newLocal = new Local(id, propertyMap);
		localMap.put(id, newLocal);
		Logger.D(TAG, "createNew-");
	}

	/**
	 * Returns the next id corresponding to an empty slot in
	 * @return
	 */
	private String getNewId()
	{
		do
		{
			lastKey++;
		} while(localMap.containsKey(String.valueOf(lastKey)));
		return String.valueOf(lastKey);
	}

	public ILocal getLocal(String id)
	{
		Logger.D(TAG, "getLocal");
		return localMap.get(id);
	}
}