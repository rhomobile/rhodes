/* 
 * $Id: PatternCacheLRU.java 124053 2005-01-04 01:24:35Z dfs $
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


package org.apache.oro.text;

import org.apache.oro.text.regex.*;
import org.apache.oro.util.*;

/**
 * This class is a GenericPatternCache subclass implementing an LRU
 * (Least Recently Used) cache replacement policy.  In other words,
 * patterns are added to the cache until it becomes full.  Once the
 * cache is full, when a new pattern is added to the cache, it replaces
 * the least recently used pattern currently in the cache.  This is probably
 * the best general purpose pattern cache replacement policy.
 *
 * @version @version@
 * @since 1.0
 * @see GenericPatternCache
 */
public final class PatternCacheLRU extends GenericPatternCache {

  /**
   * Creates a PatternCacheLRU instance with a given cache capacity,
   * and initialized to use a given PatternCompiler instance as a pattern
   * compiler.
   * <p>
   * @param capacity  The capacity of the cache.
   * @param compiler  The PatternCompiler to use to compile patterns.
   */
  public PatternCacheLRU(int capacity, PatternCompiler compiler) {
    super(new CacheLRU(capacity), compiler);
  }

  /**
   * Same as:
   * <blockquote><pre>
   * PatternCacheLRU(GenericPatternCache.DEFAULT_CAPACITY, compiler);
   * </pre></blockquote>
   */
  public PatternCacheLRU(PatternCompiler compiler) {
    this(GenericPatternCache.DEFAULT_CAPACITY, compiler);
  }


  /**
   * Same as:
   * <blockquote><pre>
   * PatternCacheLRU(capacity, new Perl5Compiler());
   * </pre></blockquote>
   */
  public PatternCacheLRU(int capacity) {
    this(capacity, new Perl5Compiler());
  }

  /**
   * Same as:
   * <blockquote><pre>
   * PatternCacheLRU(GenericPatternCache.DEFAULT_CAPACITY);
   * </pre></blockquote>
   */
  public PatternCacheLRU() {
    this(GenericPatternCache.DEFAULT_CAPACITY);
  }

}



