/* 
 * $Id: GenericCacheEntry.java 124053 2005-01-04 01:24:35Z dfs $
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
 * A structure used to store values in a GenericCache.  It
 * is declared with default access to limit it to use only within the
 * package.
 *
 * @version @version@
 * @since 1.0
 */
final class GenericCacheEntry implements j2me.io.Serializable {
  /** The cache array index of the entry. */
  int _index;
  /** The value stored at this entry. */
  Object _value;
  /** The key used to store the value. */
  Object _key;

  GenericCacheEntry(int index) {
    _index = index;
    _value = null;
    _key   = null;
  }
}
