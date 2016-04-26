package com.rho.rhoelements.activex;

import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

public class ActiveXFactory
{

	private static Map<String, ActiveX> myObjects;

	static
	{
		myObjects = new HashMap<String, ActiveX>();
	}

	public static ActiveX create(String name, String ID)
	{
		ActiveX temp = null;
		try
		{
			Class cls = Class.forName("com.rho.rhoelements.activex." + name);
			Method meth = cls.getMethod("getInstance");

			temp = (ActiveX) meth.invoke(null, null);
			myObjects.put(ID, temp);

		}
		catch (Exception E)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "error :  " + E.getMessage()));
		}
		return temp;
	}

	public static ActiveX getObjectByID(String ID)
	{
		return myObjects.get(ID);
	}

	public static void deleteObjectByID(String ID)
	{
		myObjects.remove(ID);
	}
}
