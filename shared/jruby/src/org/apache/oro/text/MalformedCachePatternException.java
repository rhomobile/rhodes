/* 
 * $Id: MalformedCachePatternException.java 124053 2005-01-04 01:24:35Z dfs $
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


/**
 * An exception used to indicate errors in a regular expression fetched
 * from a PatternCache.
 * It is derived from RuntimeException, and therefore does not have to be
 * caught.  You should generally make an effort to catch
 * MalformedCachePatternException whenever you use dynamically generated
 * patterns (from user input or some other source).  Static expressions
 * represented as strings in your source code don't require exception
 * handling because as you write and test run your program you will
 * correct any errors in those expressions when you run into an uncaught
 * MalformedCachePatternException.  By the time you complete your
 * project, those static expressions will be guaranteed to be correct.
 * However, pieces of code with expressions that you cannot guarantee to
 * be correct should catch MalformedCachePatternException to ensure
 * reliability.
 *
 * @version @version@
 * @since 1.0
 * @see PatternCache
 */
public class MalformedCachePatternException extends RuntimeException {
  /**
   * Simply calls the corresponding constructor of its superclass.
   */
  public MalformedCachePatternException() {
    super();
  }

  /**
   * Simply calls the corresponding constructor of its superclass.
   * <p>
   * @param message  A message indicating the nature of the error.
   */
  public MalformedCachePatternException(String message) {
    super(message);                                
  }

}

