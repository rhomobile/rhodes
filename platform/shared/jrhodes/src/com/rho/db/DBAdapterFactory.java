package com.rho.db;

import java.lang.reflect.Method;

import com.xruby.runtime.lang.RubyValue;

public class DBAdapterFactory {
	public static IDBAdapter alloc(){
		Class dbAdapterClass;
		try { //android
			dbAdapterClass = Class
					.forName("com.rhomobile.rhodes.db.DBAdapter");
		} catch (ClassNotFoundException x3) {
			return null;
		}
		
		try {
			
			Method mainMethod = dbAdapterClass.getMethod("alloc", new Class[]{RubyValue.class});
			return (IDBAdapter) mainMethod.invoke(null, new Object[]{null});
		} catch (Exception x) {
			return null;
		}
	}
}
