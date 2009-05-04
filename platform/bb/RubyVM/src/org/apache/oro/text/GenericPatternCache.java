/* 
 * $Id: GenericPatternCache.java 124053 2005-01-04 01:24:35Z dfs $
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
 * This is the base class for all cache implementations provided in the
 * org.apache.oro.text package.
 * Although 4 subclasses of GenericPatternCache are provided with this
 * package, users may not derive subclasses from this class.
 * Rather, users should create their own implmentations of the
 * {@link PatternCache} interface.
 *
 * @version @version@
 * @since 1.0
 * @see PatternCache
 * @see PatternCacheLRU
 * @see PatternCacheFIFO
 * @see PatternCacheFIFO2
 * @see PatternCacheRandom
 */
public abstract class GenericPatternCache implements PatternCache {
  PatternCompiler _compiler;
  Cache _cache;

  /**
   * The default capacity to be used by the GenericPatternCache subclasses
   * provided with this package.  Its value is 20.
   */
  public static final int DEFAULT_CAPACITY = 20;

  /**
   * The primary constructor for GenericPatternCache.  It has default
   * access so it will only be used within the package.  It initializes
   * _cache and _compiler to the arguments provided.
   * <p>
   * @param cache The cache with which to store patterns.
   * @param compiler The PatternCompiler that should be used to compile
   *       patterns.
   */
  GenericPatternCache(Cache cache, PatternCompiler compiler) {
    _cache    = cache;
    _compiler = compiler;
  }


  /**
   * Adds a pattern to the cache and returns the compiled pattern.  This
   * method is in principle almost identical to
   * {@link #getPattern getPattern()} except for the fact that
   * it throws a MalformedPatternException if an expression cannot be
   * compiled.
   * <p>
   * addPattern() is meant to be used when you expressly intend to add
   * an expression to the cache and is useful for front-loading a cache
   * with expressions before use.  If the expression added does not
   * already exist in the cache, it is compiled, added to the cache,
   * and returned.  If the compiled expression is already in the cache, it
   * is simply returned.
   * <p>
   * The expected behavior of this method should be to start replacing
   * patterns in the cache only after the cache has been filled to capacity.
   * <p>
   * @param expression  The regular expression to add to the cache.
   * @param options The compilation options to use when compiling the
   *        expression.
   * @return The Pattern corresponding to the String representation of the
   *         regular expression.
   * @exception MalformedPatternException  If there is an error in compiling
   *         the regular expression.
   */
  public final synchronized Pattern addPattern(String expression, int options)
       throws MalformedPatternException
  {
    Object obj;
    Pattern pattern;

    obj = _cache.getElement(expression);

    if(obj != null) {
      pattern = (Pattern)obj;

      if(pattern.getOptions() == options)
	return pattern;
    }

    pattern = _compiler.compile(expression, options);
    _cache.addElement(expression, pattern);

    return pattern;
  }


  /**
   * Same as calling
   * <blockquote><pre>
   * addPattern(expression, 0);
   * </pre></blockquote>
   * @exception MalformedPatternException  If there is an error in compiling
   *         the regular expression.
   */
  public final synchronized Pattern addPattern(String expression)
       throws MalformedPatternException
  {
    return addPattern(expression, 0);
  }


  /**
   * This method fetches a pattern from the cache.  It is nearly identical
   * to {@link #addPattern addPattern()} except that it doesn't
   * throw a MalformedPatternException.  If the pattern is not in the
   * cache, it is compiled, placed in the cache, and returned.  If
   * the pattern cannot be compiled successfully, it
   * throws a MalformedCachePatternException.
   * Note that this exception is derived from RuntimeException, which means
   * you are NOT forced to catch it by the compiler.  Please refer to
   * {@link MalformedCachePatternException} for a discussion of
   * when you should and shouldn't catch this exception.
   * <p>
   * @param expression  The regular expression to fetch from the cache in
   *        compiled form.
   * @param options The compilation options to use when compiling the
   *        expression.
   * @return The Pattern corresponding to the String representation of the
   *         regular expression.
   * @exception MalformedCachePatternException  If there is an error in
   *     compiling the regular expression.
   */
  public final synchronized Pattern getPattern(String expression, int options)
       throws MalformedCachePatternException 
  {
    Pattern result = null;

    try {
      result = addPattern(expression, options);
    } catch(MalformedPatternException e) {
      throw new MalformedCachePatternException("Invalid expression: " +
					       expression + "\n" + 
					       e.getMessage());
    }

    return result;
  }


  /**
   * Same as calling
   * <blockquote><pre>
   * getPattern(expression, 0)
   * </pre></blockquote>
   */
  public final synchronized Pattern getPattern(String expression) 
       throws MalformedCachePatternException 
  {
    return getPattern(expression, 0);
  }


  /**
   * Returns the number of elements in the cache, not to be confused with
   * the {@link #capacity()} which returns the number
   * of elements that can be held in the cache at one time.
   * <p>
   * @return  The current size of the cache (i.e., the number of elements
   *          currently cached).
   */
  public final int size()     { return _cache.size(); }

  /**
   * Returns the maximum number of patterns that can be cached at one time.
   * <p>
   * @return The maximum number of patterns that can be cached at one time.
   */
  public final int capacity() { return _cache.capacity(); }
}
