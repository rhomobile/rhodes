package com.rho.rhoelements.activex;

import java.lang.reflect.Method;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

public abstract class ActiveX
{
	public String[] Execute(String name, String[] args)
	{
		String[] result = null;
		try
		{
			String lcName = name.toLowerCase();
			Class<? extends ActiveX> cls = this.getClass();
			Method meth = cls.getMethod(lcName, args.getClass());
			result = (String[]) meth.invoke(this, (Object) args);
		}
		catch (Exception E)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "error :  " + E.getMessage()));
		}
		return result;
	};
}
