/** 
 * Copyright 2005-2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

//import j2me.util.Iterator;

//import com.xruby.runtime.builtin.RubyProc;

class MethodCache {
	private static int CACHE_SIZE = 0x800;
	private static int CACHE_MASK = 0x7ff;
	
	private CacheEntry[] cache = clearCache();
	
	public void reset() {
		if (!RubyRuntime.running) {
			return;
		}
		
		for (int i = 0; i < CACHE_SIZE; i++) {
			cache[i].klass = null;
			cache[i].mid = null;
			cache[i].method = null;
		}
	}
	
	private CacheEntry[] clearCache() {
		CacheEntry[] cache = new CacheEntry[CACHE_SIZE];
		for (int i = 0; i < CACHE_SIZE; i++) {
			cache[i] = new CacheEntry();
			cache[i].klass = null;
			cache[i].mid = null;
			cache[i].method = null;
		}
		
		return cache;
	}
	
	private int cacheIndex(RubyClass c, RubyID id) {		
		return (((c.objectAddress() >> 3) ^ ((int)id.getId())) & CACHE_MASK);
	}
	
	public CacheEntry getMethod(RubyClass c, RubyID id) {		
		int index = cacheIndex(c, id);
		return cache[index];
	}
	
	public void putMethod(RubyClass c, RubyID id, RubyMethod m) {
		int index = cacheIndex(c, id);
		
		cache[index].klass = c;
		cache[index].mid = id;
		cache[index].method = m;
	}
	
	public void removeMethod(RubyClass c, RubyID id) {
		if (!RubyRuntime.running) {
			return;
		}
		
//		for (CacheEntry entry : cache) {
		for(int i = 0; i<cache.length;i++){
			CacheEntry entry = cache[i]; 
			if (entry.mid == id && entry.klass == c) {
				// FIXME: method table should be compared
				entry.mid = null;
			}
		}
	}
	
	public void removeMethod(RubyID id) {
		if (!RubyRuntime.running) {
			return;
		}
		
//		for (CacheEntry entry : cache) {
		for(int i = 0; i<cache.length;i++){
			CacheEntry entry = cache[i]; 
		
			if (entry.mid == id) {
				entry.mid = null;
			}
		}
	}
	
	public void removeClass(RubyClass c) {
		if (!RubyRuntime.running) {
			return;
		}
		
//		for (CacheEntry entry : cache) {
		for(int i = 0; i<cache.length;i++){
			CacheEntry entry = cache[i]; 
		
			if (entry.klass == c) {
				entry.mid = null;
			}
		}
	}
	
	static class CacheEntry {
		public RubyClass klass;
		public RubyID mid;
		public RubyMethod method;
	}
}
