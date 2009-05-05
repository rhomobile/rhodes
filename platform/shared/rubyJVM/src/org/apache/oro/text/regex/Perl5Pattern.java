/* 
 * $Id: Perl5Pattern.java 124053 2005-01-04 01:24:35Z dfs $
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

import j2me.io.*;
import j2me.lang.Cloneable;

/**
 * An implementation of the Pattern interface for Perl5 regular expressions.
 * This class is compatible with the Perl5Compiler and Perl5Matcher
 * classes.  When a Perl5Compiler instance compiles a regular expression
 * pattern, it produces a Perl5Pattern instance containing internal
 * data structures used by Perl5Matcher to perform pattern matches.
 * This class cannot be subclassed and
 * cannot be directly instantiated by the programmer as it would not
 * make sense.  Perl5Pattern instances should only be created through calls
 * to a Perl5Compiler instance's compile() methods.  The class implements
 * the Serializable interface so that instances may be pre-compiled and
 * saved to disk if desired.
 *
 * @version @version@
 * @since 1.0
 * @see Perl5Compiler
 * @see Perl5Matcher
 */
public final class Perl5Pattern implements Pattern, Serializable, Cloneable {
  static final int
    _OPT_ANCH_BOL  = 0x01,
    _OPT_ANCH_MBOL = 0x02,
    _OPT_SKIP      = 0x04,
    _OPT_IMPLICIT  = 0x08;
  static final int _OPT_ANCH = (_OPT_ANCH_BOL | _OPT_ANCH_MBOL);

  String _expression;
  char[] _program;
  int _mustUtility;
  int _back;
  int _minLength;
  int _numParentheses;
  boolean _isCaseInsensitive, _isExpensive;
  int _startClassOffset;
  int _anchor;
  int _options;
  char[] _mustString, _startString;

  /**
   * A dummy constructor with default visibility to override the default
   * public constructor that would be created otherwise by the compiler.
   */
  Perl5Pattern(){ }

  /*
  private void readObject(ObjectInputStream stream)
       throws IOException, ClassNotFoundException
  {
    stream.defaultReadObject();
    _beginMatchOffsets = new int[_numParentheses + 1];
    _endMatchOffsets   = new int[_numParentheses + 1];
  }
  */

  /**
   * This method returns the string representation of the pattern.
   * <p>
   * @return The original string representation of the regular expression
   *         pattern.
   */
  public String getPattern() { return _expression; }


  /**
   * This method returns an integer containing the compilation options used
   * to compile this pattern.
   * <p>
   * @return The compilation options used to compile the pattern.
   */
  public int getOptions()    { return _options; }

  /*
  // For testing
  public String toString() {
    return "Parens: " + _numParentheses + " " + _beginMatchOffsets.length + " "
      + _endMatchOffsets.length;
  }
  */
}
