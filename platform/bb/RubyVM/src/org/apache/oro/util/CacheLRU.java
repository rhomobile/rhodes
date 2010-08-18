/* 
 * $Id: CacheLRU.java 124053 2005-01-04 01:24:35Z dfs $
 *
 * Copyright 2000-2005 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


package org.apache.oro.util;

/**
 * This class is a GenericCache subclass implementing an LRU
 * (Least Recently Used) cache replacement policy.  In other words,
 * values are added to the cache until it becomes full.  Once the
 * cache is full, when a new value is added to the cache, it replaces
 * the least recently used value currently in the cache.  This is probably
 * the best general purpose cache replacement policy.
 * 
 * @version @version@
 * @since 1.0
 * @see GenericCache
 */
public final class CacheLRU extends GenericCache {
  private int __head = 0, __tail = 0;
  private int[] __next, __prev;

  /**
   * Creates a CacheLRU instance with a given cache capacity.
   * <p>
   * @param capacity  The capacity of the cache.
   */
  public CacheLRU(int capacity) { 
    super(capacity);

    int i;

    __next = new int[_cache.length];
    __prev = new int[_cache.length];

    for(i=0; i < __next.length; i++)
      __next[i] = __prev[i] = -1;
  }


  /**
   * Same as:
   * <blockquote><pre>
   * CacheLRU(GenericCache.DEFAULT_CAPACITY);
   * </pre></blockquote>
   */
  public CacheLRU(){
    this(GenericCache.DEFAULT_CAPACITY);
  }


  private void __moveToFront(int index) {
    int next, prev;

    if(__head != index) {
      next = __next[index];
      prev = __prev[index];

      // Only the head has a prev entry that is an invalid index so
      // we don't check.
      __next[prev] = next;

      // Make sure index is valid.  If it isn't, we're at the tail
      // and don't set __prev[next].
      if(next >= 0)
	__prev[next] = prev;
      else
	__tail = prev;

      __prev[index] = -1;
      __next[index] = __head;
      __prev[__head] = index;
      __head        = index;
    }
  }


  public synchronized Object getElement(Object key) { 
    Object obj;

    obj = _table.get(key);

    if(obj != null) {
      GenericCacheEntry entry;

      entry = (GenericCacheEntry)obj;
      // Maintain LRU property
      __moveToFront(entry._index);

      return entry._value;
    }

    return null;
  }


  /**
   * Adds a value to the cache.  If the cache is full, when a new value
   * is added to the cache, it replaces the least recently used value
   * in the cache (i.e., LRU).
   * <p>
   * @param key   The key referencing the value added to the cache.
   * @param value The value to add to the cache.
   */
  public final synchronized void addElement(Object key, Object value) {
    Object obj;

    obj = _table.get(key);

    if(obj != null) {
      GenericCacheEntry entry;

      // Just replace the value, but move it to the front.
      entry = (GenericCacheEntry)obj;
      entry._value = value;
      entry._key   = key;

      __moveToFront(entry._index);

      return;
    }

    // If we haven't filled the cache yet, place in next available spot
    // and move to front.
    if(!isFull()) {
      if(_numEntries > 0) {
	__prev[_numEntries] = __tail;
	__next[_numEntries] = -1;
	__moveToFront(_numEntries);
      }
      ++_numEntries;
    } else {
      // We replace the tail of the list.
      _table.remove(_cache[__tail]._key);
      __moveToFront(__tail);
    }

    _cache[__head]._value = value;
    _cache[__head]._key   = key;
    _table.put(key, _cache[__head]);
  }
}
