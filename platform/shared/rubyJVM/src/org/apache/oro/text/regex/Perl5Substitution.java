/* 
 * $Id: Perl5Substitution.java 124053 2005-01-04 01:24:35Z dfs $
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
 * Perl5Substitution implements a Substitution consisting of a
 * literal string, but allowing Perl5 variable interpolation referencing
 * saved groups in a match.  This class is intended for use with
 * {@link Util#substitute Util.substitute}.
 * <p>
 * The substitution string may contain variable interpolations referring
 * to the saved parenthesized groups of the search pattern.
 * A variable interpolation is denoted by <b>$1</b>, or <b>$2</b>,
 * or <b>$3</b>, etc.  If you want such expressions to be
 * interpreted literally, you should set the <b> numInterpolations </b>
 * parameter to <b> INTERPOLATE_NONE </b>.  It is easiest to explain
 * what an interpolated variable does by giving an example:
 * <ul>
 * Suppose you have the pattern <b>b\d+:</b> and you want to substitute
 * the <b>b</b>'s for <b>a</b>'s and the colon for a dash in parts of
 * your input matching the pattern.  You can do this by changing the
 * pattern to <b>b(\d+):</b> and using the substitution expression
 * <b>a$1-</b>.  When a substitution is made, the <b>$1</b> means
 * "Substitute whatever was matched by the first saved group of the
 *  matching pattern."  An input of <b>b123:</b> after substitution
 * would yield a result of <b>a123-</b>.  But there's a little more
 * to be aware of.  If you set the <b>numInterpolations</b> parameter to
 * <b>INTERPOLATE_ALL</b>, then every time a match is found, the 
 * interpolation variables are computed relative to that match.
 * But if <b>numInterpolations</b> is set to some positive integer, then
 * only the interpolation variables for the first <b>numInterpolations</b>
 * matches are computed relative to the most recent match.  After that,
 * the remaining substitutions have their variable interpolations performed
 * relative to the <b> numInterpolations </b>'th match.  So using the
 * previously mentioned pattern and substitution expression, if you have
 * an input of <pre><b>Tank b123: 85  Tank b256: 32  Tank b78: 22</b></pre>
 * and use a <b> numInterpolations </b> value of <b>INTERPOLATE_ALL</b> and
 * <b> numSubs </b> value (see
 * {@link Util#substitute Util.substitute})
 * of <b> SUBSTITUTE_ALL</b>, then your result  will be:
 * <pre><b>Tank a123- 85  Tank a256- 32  Tank a78- 22</b></pre>
 * But if you set <b> numInterpolations </b> to 2 and keep 
 * <b> numSubs </b> with a value of <b>SUBSTITUTE_ALL</b>, your result is:
 * <pre><b>Tank a123- 85  Tank a256- 32  Tank a256- 22</b></pre>
 * Notice how the last substitution uses the same value for <b>$1</b>
 * as the second substitution.
 * </ul>
 * <p>
 * A final thing to keep in mind is that if you use an interpolation variable
 * that corresponds to a group not contained in the match, then it is
 * interpreted as the empty string.  So given the regular expression from the
 * example, and a substitution expression of <b>a$2-</b>, the result
 * of the last sample input would be:
 * <pre><b>Tank a- 85  Tank a- 32  Tank a- 22</b></pre>
 * The special substitution <b>$&</b> will interpolate the entire portion
 * of the input matched by the regular expression.  <b>$0</b> will
 * do the same, but it is recommended that it be avoided because the
 * latest versions of Perl use <b>$0</b> to store the program name rather
 * than duplicate the behavior of <b>$&</b>.
 * Also, the result of substituting $ followed by a non-positive integer
 * is undefined.  In order to include a $ in a  substitution, it should
 * be escaped with a backslash (e.g., <b>"\\$0"</b>).
 * <p>
 * Perl5 double-quoted string case modification is also supported in
 * the substitution.  The following escape sequences are supported:
 * <dl compact>
 *  <dt> \\U <dd> make substitution uppercase until end of substitution or \\E
 *  <dt> \\u <dd> make next character uppercase
 *  <dt> \\L <dd> make substitution uppercase until end of substitution or \\E
 *  <dt> \\l <dd> make next character uppercase
 *  <dt> \\E <dd> mark the end of the case modification
 * </dl>
 * The double backslashes are shown to remind you that to make a
 * backslash get past Java's string handling and appear as a backslash
 * to the substitution, you must escape the backslash.
 *
 * @version @version@
 * @since 1.1
 * @see Substitution
 * @see Util
 * @see Util#substitute
 * @see Substitution
 * @see StringSubstitution
 */
public class Perl5Substitution extends StringSubstitution {
  /**
   * A constant used when creating a Perl5Substitution indicating that
   * interpolation variables should be computed relative to the most
   * recent pattern match.
   */
  public static final int INTERPOLATE_ALL = 0;

  /**
   * A constant used when creating a Perl5Substitution indicating that 
   * interpolation variables should be interpreted literally, effectively 
   * disabling interpolation.
   */
  public static final int INTERPOLATE_NONE = -1;

  /**
   * The initial size and unit of growth for the
   * {@link #_subOpCodes _subOpCodes} array.
   */
  private static final int __OPCODE_STORAGE_SIZE = 32;

  /**
   * The maximum number of groups supported by interpolation.
   */
  private static final int __MAX_GROUPS = Character.MAX_VALUE;

  /**
   * A constant declaring opcode for copy operation.
   */
  static final int _OPCODE_COPY            = -1;

  /**
   * A constant declaring opcode for lowercase char operation.
   */
  static final int _OPCODE_LOWERCASE_CHAR  = -2;

  /**
   * A constant declaring opcode for uppercase char operation.
   */
  static final int _OPCODE_UPPERCASE_CHAR  = -3;

  /**
   * A constant declaring opcode for lowercase mode operation.
   */
  static final int _OPCODE_LOWERCASE_MODE  = -4;

  /**
   * A constant declaring opcode for lowercase mode operation.
   */
  static final int _OPCODE_UPPERCASE_MODE  = -5;

  /**
   * A constant declaring opcode for lowercase mode operation.
   */
  static final int _OPCODE_ENDCASE_MODE    = -6;
  
  int _numInterpolations;
  int[] _subOpcodes;
  int _subOpcodesCount;
  char[] _substitutionChars;

  transient String _lastInterpolation;

  private static final boolean __isInterpolationCharacter(char ch) {
    return (Character.isDigit(ch) || ch == '&' || ch == '{');
  }

  private void __addElement(int value) {
    int len = _subOpcodes.length;
    if (_subOpcodesCount == len) {
      int[] newarray = new int[len + __OPCODE_STORAGE_SIZE];
      System.arraycopy(_subOpcodes, 0, newarray, 0, len);
      _subOpcodes = newarray;
    }
    _subOpcodes[_subOpcodesCount++] = value;
  }

  private void __parseSubs(String sub) {
    boolean saveDigits, escapeMode, caseMode;
    int posParam;
    int offset;

    char[] subChars = _substitutionChars = sub.toCharArray();
    int subLength = subChars.length;

    _subOpcodes = new int[__OPCODE_STORAGE_SIZE];
    _subOpcodesCount = 0;
    
    posParam = 0;
    offset = -1;
    saveDigits = false;
    escapeMode = false;
    caseMode = false;

    for (int current = 0; current < subLength; current++) {
      char c = subChars[current];
      char nextc;
      int next = current + 1;
        
      // Save digits
      if (saveDigits) {
	int digit = Character.digit(c, 10);

	if (digit > -1) {
	  if (posParam <= __MAX_GROUPS) {
	    posParam *= 10;
	    posParam += digit;
	  }
	  if (next == subLength) {
	    __addElement(posParam);
	  }
	  continue;
	} else if(c == '&') {
	  if(/*current > 0 &&*/subChars[current - 1] == '$') {
	    __addElement(0);
	    posParam = 0;
	    saveDigits = false;
	    continue;
	  }
	} else if(c == '{') {
          continue;
        }

	__addElement(posParam);
	posParam = 0;
	saveDigits = false;

        if(c == '}') {
          continue;
        }
      }

      if ((c != '$' && c != '\\') || escapeMode) {
	escapeMode = false;
	if (offset < 0) {
	  offset = current;
	  __addElement(_OPCODE_COPY);
	  __addElement(offset);
	}
	if (next == subLength) {
	  __addElement(next - offset);
	}
	continue;
      }


      if (offset >= 0) {
	__addElement(current - offset);
	offset = -1;
      }

      // Only do positional and escapes if we have a next char
      if (next == subLength)
	continue;
      nextc = subChars[next];

      // Positional params
      if (c == '$') {
	saveDigits = __isInterpolationCharacter(nextc);
      } else if (c == '\\') { // Escape codes
	if (nextc == 'l') {
	  if (!caseMode){
	    __addElement(_OPCODE_LOWERCASE_CHAR);
	    current++;
	  }
	} else if (nextc == 'u') {
	  if (!caseMode) {
	    __addElement(_OPCODE_UPPERCASE_CHAR);
	    current++;
	  }
	} else if (nextc == 'L') {
	  __addElement(_OPCODE_LOWERCASE_MODE);
	  current++;
	  caseMode = true;
	} else if (nextc == 'U') {
	  __addElement(_OPCODE_UPPERCASE_MODE);
	  current++;
	  caseMode = true;
	} else if (nextc == 'E') {
	  __addElement(_OPCODE_ENDCASE_MODE);
	  current++;
	  caseMode = false;
	} else {
	  escapeMode = true;
	}
      }
    }
  }


  String _finalInterpolatedSub(MatchResult result) {
    StringBuffer buffer = new StringBuffer(10);
    _calcSub(buffer, result);
    return buffer.toString();
  }

  void _calcSub(StringBuffer buffer, MatchResult result) {
    int size, offset, count, caseMode;
    char[] sub, str, match;
    int[] subOpcodes = _subOpcodes;

    caseMode = 0;

    str = _substitutionChars;
    match = result.group(0).toCharArray();

    size = _subOpcodesCount;

    for (int element = 0; element < size; element++) {
      int value = subOpcodes[element];

      // If we have a group, set up interpolation, else
      // interpret op code.
      if(value >= 0 && value < result.groups()) {
	int end, len;
	offset = result.begin(value);
	
	if (offset < 0) continue;

	end = result.end(value);

	if (end < 0) continue;

	len = result.length();

	if (offset >= len || end > len || offset >= end) continue;

	count = end - offset;
	sub = match;
      } else if (value == _OPCODE_COPY) {
	element++;
	if (element >= size) continue;
	offset = subOpcodes[element];
	element++;
	if (element >= size) continue;
	count = subOpcodes[element];
	sub = str;
      } else if (value == _OPCODE_LOWERCASE_CHAR ||
		 value == _OPCODE_UPPERCASE_CHAR) {
	  if (caseMode != _OPCODE_LOWERCASE_MODE &&
	      caseMode != _OPCODE_UPPERCASE_MODE)
	      caseMode = value;
	  continue;
      } else if (value == _OPCODE_LOWERCASE_MODE ||
		 value == _OPCODE_UPPERCASE_MODE) {
	caseMode = value;
	continue;
      } else if (value == _OPCODE_ENDCASE_MODE) {
	caseMode = 0;
	continue;
      } else
	continue;

      // Apply modes to buf
      if (caseMode == _OPCODE_LOWERCASE_CHAR) {
	buffer.append(Character.toLowerCase(sub[offset++]));
	buffer.append(sub, offset, --count);
	caseMode = 0;
      } else if (caseMode == _OPCODE_UPPERCASE_CHAR) {
	buffer.append(Character.toUpperCase(sub[offset++]));
	buffer.append(sub, offset, --count);
	caseMode = 0;
      } else if (caseMode == _OPCODE_LOWERCASE_MODE) {
	for (int end = offset + count; offset < end; ) {
	  buffer.append(Character.toLowerCase(sub[offset++]));
	}
      } else if (caseMode == _OPCODE_UPPERCASE_MODE) {
	for (int end = offset + count; offset < end; ) {
	  buffer.append(Character.toUpperCase(sub[offset++]));
	}
      } else
	buffer.append(sub, offset, count);
        
    }
  }


  /**
   * Default constructor initializing substitution to a zero length
   * String and the number of interpolations to
   * {@link #INTERPOLATE_ALL}.
   */
  public Perl5Substitution() {
    this("", INTERPOLATE_ALL);
  }

  /**
   * Creates a Perl5Substitution using the specified substitution
   * and setting the number of interpolations to
   * {@link #INTERPOLATE_ALL}.
   * <p>
   * @param substitution The string to use as a substitution.
   */
  public Perl5Substitution(String substitution) {
    this(substitution, INTERPOLATE_ALL);
  }

  /**
   * Creates a Perl5Substitution using the specified substitution
   * and setting the number of interpolations to the specified value.
   * <p>
   * @param substitution The string to use as a substitution.
   * @param numInterpolations 
   *            If set to <b>INTERPOLATE_NONE</b>, interpolation variables are
   *            interpreted literally and not as references to the saved
   *            parenthesized groups of a pattern match.  If set to
   *            <b> INTERPOLATE_ALL </b>, all variable interpolations
   *            are computed relative to the pattern match responsible for
   *            the current substitution.  If set to a positive integer,
   *            the first <b> numInterpolations </b> substitutions have
   *            their variable interpolation performed relative to the
   *            most recent match, but the remaining substitutions have
   *            their variable interpolations performed relative to the
   *            <b> numInterpolations </b>'th match.
   */
  public Perl5Substitution(String substitution, int numInterpolations) {
    setSubstitution(substitution, numInterpolations);
  }


  /**
   * Sets the substitution represented by this Perl5Substitution, also
   * setting the number of interpolations to
   * {@link #INTERPOLATE_ALL}.
   * You should use this method in order to avoid repeatedly allocating new
   * Perl5Substitutions.  It is recommended that you allocate a single
   * Perl5Substitution and reuse it by using this method when appropriate.
   * <p>
   * @param substitution The string to use as a substitution.
   */
  public void setSubstitution(String substitution) {
    setSubstitution(substitution, INTERPOLATE_ALL);
  }


  /**
   * Sets the substitution represented by this Perl5Substitution, also
   * setting the number of interpolations to the specified value.
   * You should use this method in order to avoid repeatedly allocating new
   * Perl5Substitutions.  It is recommended that you allocate a single
   * Perl5Substitution and reuse it by using this method when appropriate.
   * <p>
   * @param substitution The string to use as a substitution.
   * @param numInterpolations 
   *            If set to <b>INTERPOLATE_NONE</b>, interpolation variables are
   *            interpreted literally and not as references to the saved
   *            parenthesized groups of a pattern match.  If set to
   *            <b> INTERPOLATE_ALL </b>, all variable interpolations
   *            are computed relative to the pattern match responsible for
   *            the current substitution.  If set to a positive integer,
   *            the first <b> numInterpolations </b> substitutions have
   *            their variable interpolation performed relative to the
   *            most recent match, but the remaining substitutions have
   *            their variable interpolations performed relative to the
   *            <b> numInterpolations </b>'th match.
   */
  public void setSubstitution(String substitution, int numInterpolations) {
    super.setSubstitution(substitution);
    _numInterpolations = numInterpolations;

    if(numInterpolations != INTERPOLATE_NONE && 
       (substitution.indexOf('$') != -1 || substitution.indexOf('\\') != -1))
      __parseSubs(substitution);
    else
      _subOpcodes = null;
    _lastInterpolation = null;
  }


  /**
   * Appends the substitution to a buffer containing the original input
   * with substitutions applied for the pattern matches found so far.
   * See 
   * {@link Substitution#appendSubstitution Substitution.appendSubstition()}
   * for more details regarding the expected behavior of this method.
   * <p>
   * @param appendBuffer The buffer containing the new string resulting
   * from performing substitutions on the original input.
   * @param match The current match causing a substitution to be made. 
   * @param substitutionCount  The number of substitutions that have been
   *  performed so far by Util.substitute.
   * @param originalInput The original input upon which the substitutions are
   * being performed.  This is a read-only parameter and is not modified.
   * @param matcher The PatternMatcher used to find the current match.
   * @param pattern The Pattern used to find the current match.
   */
  public void appendSubstitution(StringBuffer appendBuffer, MatchResult match,
				 int substitutionCount,
				 PatternMatcherInput originalInput, 
				 PatternMatcher matcher, Pattern pattern)
  {
    if(_subOpcodes == null) {
      super.appendSubstitution(appendBuffer, match, substitutionCount,
			       originalInput, matcher, pattern);
      return;
    }

    if(_numInterpolations < 1 || substitutionCount < _numInterpolations)
      _calcSub(appendBuffer, match);
    else {
      if(substitutionCount == _numInterpolations)
	_lastInterpolation = _finalInterpolatedSub(match);
      appendBuffer.append(_lastInterpolation);
    }
  }

}
