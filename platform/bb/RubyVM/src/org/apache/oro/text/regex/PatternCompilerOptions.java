/* 
 * $Id: PatternCompilerOptions.java 124053 2005-01-04 01:24:35Z dfs $
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

import j2me.lang.UnsupportedOperationException;
/**
 * PatternCompilerOptions abstracts the options used by regular
 * expression engines into a common set.  These options are
 * represented as integer constants that must be mapped to a value
 * native to the regular expression implementation.
 *
 * @version @version@
 * @since 2.1
 */
public interface PatternCompilerOptions {

  /**
   * The default set of options used by a {@link PatternCompiler}.
   */
  public int DEFAULT          = 0;

  /**
   * An option indicating a compiled regular expression should be case
   * insensitive.
   */
  public int CASE_INSENSITIVE = 1;

  /**
   * An option indicating a compiled regular expression should treat
   * input as having multiple lines.
   */
  public int MULTILINE        = 2;

  /**
   * Maps one of the PatternCompilerOptions constants to a mask value
   * that can be passed to a {@link PatternCompiler} implementation to
   * compile a pattern.
   *
   * @param option A PatternCompilerOptions option constant.
   * @return The mask value specific to the regular expression engine
   *         that corresponds to the option parameter.
   */

  public int getMask(int option)
    throws UnsupportedOperationException;

}
