/* 
 * $Id: Cache.java 124053 2005-01-04 01:24:35Z dfs $
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
 * An interface defining the basic functions of a cache.
 *
 * @version @version@
 * @since 1.0
 */
public interface Cache {

  public void addElement(Object key, Object value);

  public Object getElement(Object key);

  /**
   * Returns the number of elements in the cache, not to be confused with
   * the {@link #capacity()} which returns the number
   * of elements that can be held in the cache at one time.
   * <p>
   * @return  The current size of the cache (i.e., the number of elements
   *          currently cached).
   */
  public int size();


  /**
   * Returns the maximum number of elements that can be cached at one time.
   * <p>
   * @return The maximum number of elements that can be cached at one time.
   */
  public int capacity();

}
