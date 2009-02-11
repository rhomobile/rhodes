/* 
 * $Id: MalformedPatternException.java 124053 2005-01-04 01:24:35Z dfs $
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
 * A class used to signify the occurrence of a syntax error in a
 * regular expression that is being compiled.  The class is not
 * declared final so that it may be subclassed for identifying
 * more specific pattern comilation errors.  However, at this point
 * in time, this package does not subclass MalformedPatternException
 * for any purpose.  This does not preclude users and third party
 * implementors of the interfaces of this package from subclassing it
 * for their own purposes.
 *
 * @version @version@
 * @since 1.0
 * @see PatternCompiler
 */
public class MalformedPatternException extends Exception {

  /**
   * Simply calls the corresponding constructor of its superclass.
   */
  public MalformedPatternException() {
    super();
  }

  /**
   * Simply calls the corresponding constructor of its superclass.
   * <p>
   * @param message  A message indicating the nature of the parse error.
   */
  public MalformedPatternException(String message) {
    super(message);
  }
}
