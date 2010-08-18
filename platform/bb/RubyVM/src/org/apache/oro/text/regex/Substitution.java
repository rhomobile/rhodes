/* 
 * $Id: Substitution.java 124053 2005-01-04 01:24:35Z dfs $
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
 * The Substitution interface provides a means for you to control how
 * a substitution is performed when using the
 * {@link Util#substitute Util.substitute} method.  Two standard
 * implementations are provided,
 * {@link StringSubstitution} and {@link Perl5Substitution}.  To
 * achieve custom control over the behavior of substitutions, you can 
 * create your own implementations.  A common use for customization is
 * to make a substitution a function of a match.
 *
 * @version @version@
 * @since 1.1
 * @see Util
 * @see Util#substitute
 * @see StringSubstitution
 * @see Perl5Substitution
 */
public interface Substitution {

  /**
   * Appends the substitution to a buffer containing the original input
   * with substitutions applied for the pattern matches found so far.
   * For maximum flexibility, the original input as well as the
   * PatternMatcher and Pattern used to find the match are included as
   * arguments.  However, you will almost never find a need to use those
   * arguments when creating your own Substitution implementations.
   * <p>
   * For performance reasons, rather than provide a getSubstitution method
   * that returns a String used by Util.substitute, we have opted to pass
   * a StringBuffer argument from Util.substitute to which the Substitution
   * must append data.  The contract that an appendSubstitution 
   * implementation must abide by is that the appendBuffer may only be
   * appended to.  appendSubstitution() may not alter the appendBuffer in
   * any way other than appending to it.
   * <p>
   * This method is invoked by Util.substitute every time it finds a match.
   * After finding a match, Util.substitute appends to the appendBuffer
   * all of the original input occuring between the end of the last match
   * and the beginning of the current match.  Then it invokes 
   * appendSubstitution(), passing the appendBuffer, current match, and
   * other information as arguments.  The substitutionCount keeps track
   * of how many substitutions have been performed so far by an invocation
   * of Util.substitute.  Its value starts at 1 when the first substitution
   * is found and appendSubstitution is invoked for the first time.  It
   * will NEVER be zero or a negative value.
   * <p>
   * @param appendBuffer The buffer containing the new string resulting
   * from performing substitutions on the original input.
   * @param match The current match causing a substitution to be made. 
   * @param substitutionCount  The number of substitutions that have been
   *  performed so far by Util.substitute.
   * @param originalInput The original input upon which the substitutions are
   * being performed.  The Substitution must treat this parameter as read only.
   * @param matcher The PatternMatcher used to find the current match.
   * @param pattern The Pattern used to find the current match.
   */
  public void appendSubstitution(StringBuffer appendBuffer, MatchResult match,
				 int substitutionCount,
				 PatternMatcherInput originalInput, 
				 PatternMatcher matcher, Pattern pattern);
}
