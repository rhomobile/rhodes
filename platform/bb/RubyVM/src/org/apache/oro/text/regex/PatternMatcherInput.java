/* 
 * $Id: PatternMatcherInput.java 124053 2005-01-04 01:24:35Z dfs $
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
 * The PatternMatcherInput class is used to preserve state across
 * calls to the <code>contains()</code> methods of PatternMatcher instances.
 * It is also used to specify that only a subregion of a string
 * should be used as input when looking for a pattern match.  All that
 * is meant by preserving state is that the end offset of the last match
 * is remembered, so that the next match is performed from that point
 * where the last match left off.  This offset can be accessed from
 * the {@link #getCurrentOffset()} method and can be set with the
 * {@link #setCurrentOffset(int)} method.
 * <p>
 * You would use a PatternMatcherInput object when you want to search for
 * more than just the first occurrence of a pattern in a string, or when
 * you only want to search a subregion of the string for a match.  An
 * example of its most common use is:
 * <blockquote><pre>
 * PatternMatcher matcher;
 * PatternCompiler compiler;
 * Pattern pattern;
 * PatternMatcherInput input;
 * MatchResult result;
 *
 * compiler = new Perl5Compiler();
 * matcher  = new Perl5Matcher();
 *
 * try {
 *   pattern = compiler.compile(somePatternString);
 * } catch(MalformedPatternException e) {
 *   System.out.println("Bad pattern.");
 *   System.out.println(e.getMessage());
 *   return;
 * }
 *
 * input   = new PatternMatcherInput(someStringInput);
 *
 * while(matcher.contains(input, pattern)) {
 *   result = matcher.getMatch();  
 *   // Perform whatever processing on the result you want.
 * }
 * // Suppose we want to start searching from the beginning again with
 * // a different pattern.
 * // Just set the current offset to the begin offset.
 * input.setCurrentOffset(input.getBeginOffset());
 *
 * // Second search omitted
 *
 * // Suppose we're done with this input, but want to search another string.
 * // There's no need to create another PatternMatcherInput instance.
 * // We can just use the setInput() method.
 * input.setInput(aNewInputString);
 *
 * </pre></blockquote>
 *
 * @version @version@
 * @since 1.0
 * @see PatternMatcher
 */
public final class PatternMatcherInput {
  String _originalStringInput;
  char[] _originalCharInput, _originalBuffer, _toLowerBuffer;
  int _beginOffset, _endOffset, _currentOffset;
  int _matchBeginOffset = -1, _matchEndOffset = -1;

  /**
   * Creates a PatternMatcherInput object, associating a region of a String
   * as input to be used for pattern matching by PatternMatcher objects.
   * A copy of the string is not made, therefore you should not modify
   * the string unless you know what you are doing.
   * The current offset of the PatternMatcherInput is set to the begin
   * offset of the region.
   * <p>
   * @param input  The input to associate with the PatternMatcherInput.
   * @param begin  The offset into the char[] to use as the beginning of
   *               the input.
   * @param length The length of the reegion starting from the begin offset
   *               to use as the input for pattern matching purposes.
   */
  public PatternMatcherInput(String input, int begin, int length) {
    setInput(input, begin, length);
  }

  /**
   * Like calling
   * <blockquote><pre>
   * PatternMatcherInput(input, 0, input.length());
   * </pre></blockquote>
   * <p>
   * @param input  The input to associate with the PatternMatcherInput.
   */
  public PatternMatcherInput(String input) {
    this(input, 0, input.length());
  }


  /**
   * Creates a PatternMatcherInput object, associating a region of a string
   * (represented as a char[]) as input
   * to be used for pattern matching by PatternMatcher objects.
   * A copy of the string is not made, therefore you should not modify
   * the string unless you know what you are doing.
   * The current offset of the PatternMatcherInput is set to the begin
   * offset of the region.
   * <p>
   * @param input  The input to associate with the PatternMatcherInput.
   * @param begin  The offset into the char[] to use as the beginning of
   *               the input.
   * @param length The length of the reegion starting from the begin offset
   *               to use as the input for pattern matching purposes.
   */
  public PatternMatcherInput(char[] input, int begin, int length) {
    setInput(input, begin, length);
  } 

  /**
   * Like calling:
   * <blockquote><pre>
   * PatternMatcherInput(input, 0, input.length);
   * </pre></blockquote>
   * <p>
   * @param input  The input to associate with the PatternMatcherInput.
   */
  public PatternMatcherInput(char[] input) {
    this(input, 0, input.length);
  }


  /**
   * @return The length of the region to be considered input for pattern
   *         matching purposes.  Essentially this is then end offset minus
   *         the begin offset.
   */
  public int length()        {
    return (_endOffset - _beginOffset);
    //return _originalBuffer.length;
  }


  /**
   * Associates a region of a String as input
   * to be used for pattern matching by PatternMatcher objects.
   * The current offset of the PatternMatcherInput is set to the begin
   * offset of the region.
   * <p>
   * @param input  The input to associate with the PatternMatcherInput.
   * @param begin  The offset into the String to use as the beginning of
   *               the input.
   * @param length The length of the reegion starting from the begin offset
   *               to use as the input for pattern matching purposes.
   */
  public void setInput(String input, int begin, int length) {
    _originalStringInput = input;
    _originalCharInput = null;
    _toLowerBuffer = null;
    _originalBuffer = input.toCharArray();
    setCurrentOffset(begin);
    setBeginOffset(begin);
    setEndOffset(_beginOffset + length);
  }

  /**
   * This method is identical to calling:
   * <blockquote><pre>
   * setInput(input, 0, input.length());
   * </pre></blockquote>
   * <p>
   * @param input  The input to associate with the PatternMatcherInput.
   */
  public void setInput(String input) {
    setInput(input, 0, input.length());
  }


  /**
   * Associates a region of a string (represented as a char[]) as input
   * to be used for pattern matching by PatternMatcher objects.
   * A copy of the string is not made, therefore you should not modify
   * the string unless you know what you are doing.
   * The current offset of the PatternMatcherInput is set to the begin
   * offset of the region.
   * <p>
   * @param input  The input to associate with the PatternMatcherInput.
   * @param begin  The offset into the char[] to use as the beginning of
   *               the input.
   * @param length The length of the reegion starting from the begin offset
   *               to use as the input for pattern matching purposes.
   */
  public void setInput(char[] input, int begin, int length) {
    _originalStringInput = null;
    _toLowerBuffer  = null;
    _originalBuffer = _originalCharInput = input;
    setCurrentOffset(begin);
    setBeginOffset(begin);
    setEndOffset(_beginOffset + length);
  }


  /**
   * This method is identical to calling:
   * <blockquote><pre>
   * setInput(input, 0, input.length);
   * </pre></blockquote>
   * <p>
   * @param input  The input to associate with the PatternMatcherInput.
   */
  public void setInput(char[] input) {
    setInput(input, 0, input.length);
  }


  /**
   * Returns the character at a particular offset relative to the begin
   * offset of the input.
   * <p>
   * @param offset  The offset at which to fetch a character (relative to
   *                the beginning offset.
   * @return The character at a particular offset.
   * @exception ArrayIndexOutOfBoundsException If the offset does not occur
   *            within the bounds of the input.
   */
  public char charAt(int offset) {
    return _originalBuffer[_beginOffset + offset];
  }

  /**
   * Returns a new string that is a substring of the PatternMatcherInput
   * instance. The substring begins at the specified beginOffset relative
   * to the begin offset and extends to the specified endOffset - 1
   * relative to the begin offset of the PatternMatcherInput instance.
   * <p>
   * @param beginOffset  The offset relative to the begin offset of the
   *        PatternMatcherInput at which to start the substring (inclusive).
   * @param endOffset  The offset relative to the begin offset of the
   *        PatternMatcherInput at which to end the substring (exclusive).
   * @return The specified substring.
   * @exception ArrayIndexOutOfBoundsException If one of the offsets does
   *        not occur within the bounds of the input.
   */
  public String substring(int beginOffset, int endOffset) {
    return new String(_originalBuffer, _beginOffset+beginOffset,
		      endOffset - beginOffset);
  }

  /**
   * Returns a new string that is a substring of the PatternMatcherInput
   * instance. The substring begins at the specified beginOffset relative
   * to the begin offset and extends to the end offset of the
   * PatternMatcherInput.
   * <p>
   * @param beginOffset  The offset relative to the begin offset of the
   *        PatternMatcherInput at which to start the substring.
   * @return The specified substring.
   * @exception ArrayIndexOutOfBoundsException If the offset does not occur
   *            within the bounds of the input.
   */
  public String substring(int beginOffset) {
    beginOffset+=_beginOffset;
    return new String(_originalBuffer, beginOffset, _endOffset - beginOffset);
  }


  /**
   * Retrieves the original input used to initialize the PatternMatcherInput
   * instance.  If a String was used, the String instance will be returned.
   * If a char[] was used, a char instance will be returned.  This violates
   * data encapsulation and hiding principles, but it is a great convenience
   * for the programmer.
   * <p>
   * @return The String or char[] input used to initialize the
   *         PatternMatcherInput instance.
   */
  public Object getInput(){
    if(_originalStringInput == null)
      return _originalCharInput;
    return _originalStringInput;
  }

  /**
   * Retrieves the char[] buffer to be used used as input by PatternMatcher
   * implementations to look for matches.  This array should be treated
   * as read only by the programmer.
   * <p>
   * @return The char[] buffer to be used as input by PatternMatcher
   *         implementations.
   */
  public char[] getBuffer() { return _originalBuffer;  }

  /**
   * Returns whether or not the end of the input has been reached.
   * <p>
   * @return True if the current offset is greater than or equal to the
   *         end offset.
   */
  public boolean endOfInput(){ return (_currentOffset >= _endOffset); }


  /**
   * @return The offset of the input that should be considered the start
   *         of the region to be considered as input by PatternMatcher
   *         methods.
   */
  public int getBeginOffset()   { return _beginOffset; }

  /**
   * @return The offset of the input that should be considered the end
   *         of the region to be considered as input by PatternMatcher
   *         methods.  This offset is actually 1 plus the last offset
   *         that is part of the input region.
   */
  public int getEndOffset()     { return _endOffset;  }

  /**
   * @return The offset of the input that should be considered the current
   *         offset where PatternMatcher methods should start looking for
   *         matches.
   */
  public int getCurrentOffset() { return _currentOffset; }

  /**
   * Sets the offset of the input that should be considered the start
   * of the region to be considered as input by PatternMatcher
   * methods.  In other words, everything before this offset is ignored
   * by a PatternMatcher.
   * <p>
   * @param offset  The offset to use as the beginning of the input.
   */
  public void setBeginOffset(int offset)   { _beginOffset = offset; }

  /**
   * Sets the offset of the input that should be considered the end
   * of the region to be considered as input by PatternMatcher
   * methods.  This offset is actually 1 plus the last offset
   * that is part of the input region.
   * <p>
   * @param offset  The offset to use as the end of the input.
   */
  public void setEndOffset(int offset)     { _endOffset = offset; }

  /**
   * Sets the offset of the input that should be considered the current
   * offset where PatternMatcher methods should start looking for
   * matches.  Also resets all match offset information to -1.  By calling
   * this method, you invalidate all previous match information.  Therefore
   * a PatternMatcher implementation must call this method before setting
   * match offset information.
   * <p>
   * @param offset  The offset to use as the current offset.
   */
  public void setCurrentOffset(int offset) {
    _currentOffset    = offset;
    setMatchOffsets(-1, -1);
  }

  /**
   * Returns the string representation of the input, where the input is
   * considered to start from the begin offset and end at the end offset.
   * <p>
   * @return The string representation of the input.
   */
  public String toString() {
    return new String(_originalBuffer, _beginOffset, length());
  }


  /**
   * A convenience method returning the part of the input occurring before
   * the last match found by a call to a Perl5Matcher
   * {@link Perl5Matcher#contains contains} method.
   * <p>
   * @return The input preceeding a match.
   */
  public String preMatch() {
    return new String(_originalBuffer, _beginOffset,
		      _matchBeginOffset - _beginOffset);
  }


  /**
   * A convenience method returning the part of the input occurring after
   * the last match found by a call to a Perl5Matcher
   * {@link Perl5Matcher#contains contains} method.
   * <p>
   * @return The input succeeding a contains() match.
   */
  public String postMatch() {
    return new String(_originalBuffer, _matchEndOffset,
		      _endOffset - _matchEndOffset);
  }


  /**
   * A convenience method returning the part of the input corresponding
   * to the last match found by a call to a Perl5Matcher
   * {@link Perl5Matcher#contains contains} method.
   * The method is not called getMatch() so as not to confuse it
   * with Perl5Matcher's getMatch() which returns a MatchResult instance
   * and also for consistency with preMatch() and postMatch().
   * <p>
   * @return The input consisting of the match found by contains().
   */
  public String match() {
    return new String(_originalBuffer, _matchBeginOffset,
		      _matchEndOffset - _matchBeginOffset);
  }


  /**
   * This method is intended for use by PatternMatcher implementations.
   * It is necessary to record the location of the previous match so that
   * consecutive contains() matches involving null string matches are
   * properly handled.  If you are not implementing a PatternMatcher, forget
   * this method exists.  If you use it outside of its intended context, you
   * will only disrupt the stored state.
   * <p>
   * As a note, the preMatch(), postMatch(), and match() methods are provided
   * as conveniences because PatternMatcherInput must store match offset
   * information to completely preserve state for consecutive PatternMatcher
   * contains() matches.
   * <p>
   * @param matchBeginOffset  The begin offset of a match found by contains().
   * @param matchEndOffset    The end offset of a match found by contains().
   */
  public void setMatchOffsets(int matchBeginOffset, int matchEndOffset) {
    _matchBeginOffset    = matchBeginOffset;
    _matchEndOffset      = matchEndOffset;
  }

  /**
   * Returns the offset marking the beginning of the match found by
   * contains().
   * <p>
   * @return The begin offset of a contains() match.
   */
  public int getMatchBeginOffset()    { return _matchBeginOffset; }

  /**
   * Returns the offset marking the end of the match found by contains().
   * <p>
   * @return The end offset of a contains() match.
   */
  public int getMatchEndOffset()      { return _matchEndOffset; }
}
