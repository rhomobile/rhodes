/* 
 * $Id: Pattern.java 124053 2005-01-04 01:24:35Z dfs $
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


package org.apache.oro.text.regex;


/**
 * The Pattern interface allows multiple representations of a regular
 * expression to be defined.  In general, different regular expression
 * compilers will produce different types of pattern representations.
 * Some will produce state transition tables derived from syntax trees,
 * others will produce byte code representations of an NFA, etc.  The
 * Pattern interface does not impose any specific internal pattern
 * representation, and consequently, Pattern implementations are not meant
 * to be interchangeable among differing PatternCompiler and PatternMatcher
 * implementations.  The documentation accompanying a specific implementation
 * will define what other classes a Pattern can interact with.
 *
 * @version @version@
 * @since 1.0
 * @see PatternCompiler
 * @see PatternMatcher
 */
public interface Pattern {

  /**
   * This method returns the string representation of the pattern.  Its
   * purpose is to allow a pattern to be reconstructed after compilation.
   * In other words, when you compile a pattern, the resulting data 
   * structures bear no relation to the string defining the pattern.
   * It is often useful to be able to access the string defining a pattern 
   * after it has been compiled.
   * <p>
   * @return The original string representation of the regular expression
   *         pattern.
   */
  public String getPattern();



  /**
   * This method returns an integer containing the compilation options used
   * to compile this pattern.
   * <p>
   * @return The compilation options used to compile the pattern.
   */
  public int getOptions();
}
