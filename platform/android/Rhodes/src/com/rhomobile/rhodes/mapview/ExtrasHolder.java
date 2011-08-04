package com.rhomobile.rhodes.mapview;



import java.util.HashMap;
import java.util.Map;

public class ExtrasHolder {
	
		public ExtrasHolder() {
			mHash = new HashMap<String, String>();
		}
		
		public void clear() {
			mHash.clear();
		}
		
		public String getString(String key) {
			return mHash.get(key);
		}
		
		public boolean getBoolean(String key) {
			String value = getString(key);
			if (value == null) {
				return false;
			}
			return Boolean.parseBoolean(value);
		}
		
		public int getInt(String key) {
			String value = getString(key);
			if (value == null) {
				return 0;
			}
			return Integer.parseInt(value);
		}

		public void putExtra(String key, String value) {
			mHash.put(key, value);
		}
		
		public void putExtra(String key, boolean value) {
			mHash.put(key, String.valueOf(value));
		}
		
		public void putExtra(String key, int value) {
			mHash.put(key, String.valueOf(value));
		}

		private	Map<String, String> mHash;
	}
