/* 
 * $Id: PatternCache.java 124053 2005-01-04 01:24:35Z dfs $
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

/**
 * An interface defining the basic functions of a regular expression
 * cache.
 * <p>
 * A PatternCache is an object that takes care of compiling, storing, and
 * retrieving regular expressions so that the programmer does not have to
 * explicitly manage these operation himself.  The main benefit derived
 * is the ease of use from only having to express regular expressions
 * by their String representations.
 *
 * @version @version@
 * @since 1.0
 * @see MalformedCachePatternException
 */

public interface PatternCache {

  /**
   * Adds a pattern to the cache and returns the compiled pattern.  This
   * method is in principle almost identical to
   * {@link #getPattern(String)} except for the fact that
   * it throws a MalformedPatternException if an expression cannot be
   * compiled.
   * <p>
   * addPattern() is meant to be used when you expressly intend to add
   * an expression to a cache and is useful for front-loading a cache
   * with expressions before use.  If the expression added does not
   * already exist in the cache, it is compiled, added to the cache,
   * and returned.  If the compiled expression is already in the cache, it
   * is simply returned.
   * <p>
   * The expected behavior of this method should be to start replacing
   * patterns in the cache only after the cache has been filled to capacity.
   * <p>
   * @param expression  The regular expression to add to the cache.
   * @return The Pattern corresponding to the String representation of the
   *         regular expression.
   * @exception MalformedPatternException  If there is an error in compiling
   *         the regular expression.
   */
  public Pattern addPattern(String expression)
       throws MalformedPatternException;


  /**
   * Adds a pattern to the cache and returns the compiled pattern.  This
   * method is in principle almost identical to
   * {@link #getPattern(String)} except for the fact that
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
  public Pattern addPattern(String expression, int options)
       throws MalformedPatternException;


  /**
   * This method fetches a pattern from the cache.  It is nearly identical
   * to {@link #addPattern addPattern()} except that it doesn't
   * throw a MalformedPatternException.  If the pattern is not in the
   * cache, it is compiled, placed in the cache, and returned.  If
   * the pattern cannot be compiled successfully, the implementation must
   * throw an exception derived from MalformedCachePatternException.
   * Note that this exception is derived from RuntimeException, which means
   * you are NOT forced to catch it by the compiler.  Please refer to
   * {@link MalformedCachePatternException} for a discussion of when you
   * should and shouldn't catch this exception.
   * <p>
   * @param expression  The regular expression to fetch from the cache in
   *        compiled form.
   * @return The Pattern corresponding to the String representation of the
   *         regular expression.
   * @exception MalformedCachePatternException  If there is an error in
   *     compiling the regular expression.
   */
  public Pattern getPattern(String expression)
       throws MalformedCachePatternException;


  /**
   * This method fetches a pattern from the cache.  It is nearly identical
   * to {@link #addPattern addPattern()} except that it doesn't
   * throw a MalformedPatternException.  If the pattern is not in the
   * cache, it is compiled, placed in the cache, and returned.  If
   * the pattern cannot be compiled successfully, it
   * throws a MalformedCachePatternException.
   * Note that this exception is derived from RuntimeException, which means
   * you are NOT forced to catch it by the compiler.  Please refer to
   * {@link MalformedCachePatternException} for a discussion of when you
   * should and shouldn't catch this exception.
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
  public Pattern getPattern(String expression, int options)
       throws MalformedCachePatternException;


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
   * Returns the maximum number of patterns that can be cached at one time.
   * <p>
   * @return The maximum number of patterns that can be cached at one time.
   */
  public int capacity();
}

