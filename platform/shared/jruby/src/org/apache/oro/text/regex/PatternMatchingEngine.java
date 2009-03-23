/* 
 * $Id: PatternMatchingEngine.java 124053 2005-01-04 01:24:35Z dfs $
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
 * PatternMatchingEngine is an interface that abstracts a regular
 * expression implementation into a {@link PatternCompiler}, a
 * {@link PatternMatcher}, and {@link PatternCompilerOptions}.
 * Implementations of PatternMatchingEngine can wrap any regular
 * expression implementation and make them usable using the
 * jakarta-oro interfaces.
 * 
 * @version @version@
 * @since 2.1
 */
public interface PatternMatchingEngine {

  /**
   * Creates a new instance of a PatternCompiler that can compile
   * patterns for a particular regular expression engine.
   *
   * @return A new PatternCompiler.
   */

  public PatternCompiler createCompiler();

  /**
   * Creates a new instance of a PatternMatcher that can match
   * patterns for a particular regular expression engine.
   *
   * @return A new PatternMatcher.
   */

  public PatternMatcher createMatcher();

  /**
   * Returns a {@link PatternCompilerOptions} object that can map
   * generic {@link PatternCompilerOptions} option constants into
   * constants specific to a particular regular expression engine.
   *
   * @return A PatternCompilerOptions object corresponding to an engine.
   */

  public PatternCompilerOptions getOptions();

}
