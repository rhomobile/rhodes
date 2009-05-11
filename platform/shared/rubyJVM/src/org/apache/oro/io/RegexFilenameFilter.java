/* 
 * $Id: RegexFilenameFilter.java 124053 2005-01-04 01:24:35Z dfs $
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


package org.apache.oro.io;

import j2me.io.*;

import org.apache.oro.text.regex.*;
import org.apache.oro.text.*;

/**
 * RegexFilenameFilter is the base class for a set of FilenameFilter
 * implementations that filter based on a regular expression.  It
 * is also a generic filter implementation that can use any regular
 * expression engine represented by a {@link PatternMatchingEngine}.
 *
 * @version @version@
 * @since 1.0
 * @see Perl5FilenameFilter
 * @see AwkFilenameFilter
 * @see GlobFilenameFilter
 */
public class RegexFilenameFilter implements FilenameFilter, FileFilter {
  PatternCache   _cache;
  PatternMatcher _matcher;
  Pattern _pattern;

  protected RegexFilenameFilter(PatternCache cache, PatternMatcher matcher,
                                String regex)
  {
    _cache   = cache;
    _matcher = matcher;
    setFilterExpression(regex);
  }

  protected RegexFilenameFilter(PatternCache cache, PatternMatcher matcher,
                                String regex, int options)
  {
    _cache   = cache;
    _matcher = matcher;
    setFilterExpression(regex, options);
  }

  protected RegexFilenameFilter(PatternCache cache, PatternMatcher matcher) {
    this(cache, matcher, "");
  }

  /**
   * Creates a new RegexFilenameFilter using the supplied
   * {@link PatternMatchingEngine}.
   *
   * @param engine The {@link PatternMatchingEngine} to use for pattern
   *               matching.
   */
  public RegexFilenameFilter(PatternMatchingEngine engine) {
    _cache   = new PatternCacheLRU(engine.createCompiler());
    _matcher = engine.createMatcher();
  }

  /**
   * Creates a new RegexFilenameFilter using the supplied engine key to
   * create a {@link PatternMatchingEngine} using
   * the {@link PatternMatchingEngineFactory} class.
   *
   * @param engineKey One of the {@link PatternMatchingEngineFactory} engine
   *                  keys.
   * @throws IllegalArgumentException
   */
  public RegexFilenameFilter(String engineKey) 
    throws IllegalArgumentException
  {
    PatternMatchingEngineFactory factory = new PatternMatchingEngineFactory();
    PatternMatchingEngine engine = factory.get(engineKey);

    if(engine == null)
      throw
        new IllegalArgumentException("Unsupported PatternMatchingEngine type: "
                                     + engineKey);
    
    _cache   = new PatternCacheLRU(engine.createCompiler());
    _matcher = engine.createMatcher();
  }

  /**
   * Set the regular expression on which to filter.
   * <p>
   * @param regex  The regular expression on which to filter.
   * @exception MalformedCachePatternException  If there is an error in
   *     compiling the regular expression.  This need not be caught if
   *     you are using a hard-coded expression that you know is correct.
   *     But for robustness and reliability you should catch this exception
   *     for dynamically entered expressions determined at runtime.
   */
  public void setFilterExpression(String regex)
       throws MalformedCachePatternException
  {
    _pattern = _cache.getPattern(regex);
  }

  /**
   * Set the regular expression on which to filter along with any
   * special options to use when compiling the expression.
   * <p>
   * @param regex  The regular expression on which to filter.
   * @param options A set of compilation options specific to the regular
   *        expression grammar being used.
   * @exception MalformedCachePatternException  If there is an error in
   *     compiling the regular expression.  This need not be caught if
   *     you are using a hard-coded expression that you know is correct.
   *     But for robustness and reliability you should catch this exception
   *     for dynamically entered expressions determined at runtime.
   */
  public void setFilterExpression(String regex, int options)
       throws MalformedCachePatternException
  {
    _pattern = _cache.getPattern(regex, options);
  }

  /**
   * Filters a filename.  Tests if the filename EXACTLY matches the pattern
   * contained by the filter.  The directory argument is not examined.
   * Conforms to the java.io.FilenameFilter interface.
   * <p>
   * @param dir  The directory containing the file.
   * @param filename  The name of the file.
   * @return True if the filename EXACTLY matches the pattern, false if not.
   */
  public boolean accept(File dir, String filename) {
    synchronized(_matcher) {
      return _matcher.matches(filename, _pattern);
    }
  }

  /**
   * Filters a filename.  Tests if the filename EXACTLY matches the pattern
   * contained by the filter.  The filename is defined as pathname.getName().
   * Conforms to the java.io.FileFilter interface.
   * <p>
   * @param pathname  The file pathname.
   * @return True if the filename EXACTLY matches the pattern, false if not.
   */
  public boolean accept(File pathname) {
    synchronized(_matcher) {
      return _matcher.matches(pathname.getName(), _pattern);
    }
  }
}
