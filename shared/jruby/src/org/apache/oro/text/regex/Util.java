/* 
 * $Id: Util.java 124053 2005-01-04 01:24:35Z dfs $
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

import j2me.util.*;
/**
 * The Util class is a holder for useful static utility methods that can
 * be generically applied to Pattern and PatternMatcher instances.
 * This class cannot and is not meant to be instantiated.
 * The Util class currently contains versions of the split() and substitute()
 * methods inspired by Perl's split function and <b>s</b> operation
 * respectively, although they are implemented in such a way as not to
 * rely on the Perl5 implementations of the OROMatcher packages regular
 * expression interfaces.  They may operate on any interface implementations
 * conforming to the OROMatcher API specification for the PatternMatcher,
 * Pattern, and MatchResult interfaces. Future versions of the class may
 * include additional utility methods.
 * <p>
 * A grep method is not included for two reasons:
 * <ol>
 *     <li> The details of reading a line at a time from an input stream
 *          differ in JDK 1.0.2 and JDK 1.1, making it difficult to
 *          retain compatibility across both Java releases.
 *     <li> Grep style processing is trivial for the programmer to implement
 *          in a while loop.  Rarely does anyone want to retrieve all
 *          occurences of a pattern and then process them.  More often a
 *          programmer will retrieve pattern matches and process them as they
 *          are retrieved, which is more efficient than storing them all in a
 *          Vector and then accessing them.
 * </ol>
 *
 * @version @version@
 * @since 1.0
 * @see Pattern
 * @see PatternMatcher
 */
public final class Util {
  /**
   * A constant passed to the {@link #substitute substitute()}
   * methods indicating that all occurrences of a pattern should be 
   * substituted.
   */
  public static final int SUBSTITUTE_ALL = -1;

  /**
   * A constant passed to the {@link #split split()} methods
   * indicating that all occurrences of a pattern should be used to
   * split a string.
   */
  public static final int SPLIT_ALL = 0;

  /**
   * The default destructor for the Util class.  It is made private
   * to prevent the instantiation of the class.
   */
  private Util() { }


  /**
   * Splits up a <code>String</code> instance and stores results as a
   * <code>List</code> of substrings numbering no more than a specified
   * limit.  The string is split with a regular expression as the delimiter. 
   * The <b>limit</b> parameter essentially says to split the
   * string only on at most the first <b>limit - 1</b> number of pattern
   * occurences.
   * <p>
   * This method is inspired by the Perl split() function and behaves 
   * identically to it when used in conjunction with the Perl5Matcher and
   * Perl5Pattern classes except for the following difference:
   * <ul><p>
   * In Perl, if the split expression contains parentheses, the split()
   * method creates additional list elements from each of the matching
   * subgroups in the pattern.  In other words:
   * <ul><p>
   * <code>split(list, "/([,-])/", "8-12,15,18", Util.SPLIT_ALL)</code></ul>
   * <p> produces the list containing:
   * <ul><p><code> { "8", "-", "12", ",", "15", ",", "18" } </code> </ul>
   * <p> The OROMatcher split method does not follow this behavior.  The
   * following list would be produced by OROMatcher:
   * <ul><p><code> { "8", "12",  "15", "18" } </code> </ul>
   * <p> To obtain the Perl behavior, use
   * {@link org.apache.oro.text.perl.Perl5Util#split}.
   * </ul>
   * <p>
   * @param results A Collection to which the split results are appended.
   *         After the method returns, it contains the substrings of the input
   *         that occur between the regular expression delimiter occurences.
   *         The input will not be split into any more substrings than the
   *         specified <code>limit</code>.  A way of thinking of this is that
   *         only the first <code>limit - 1</code> matches of the delimiting
   *         regular expression will be used to split the input.
   * @param matcher The regular expression matcher to execute the split.
   * @param pattern The regular expression to use as a split delimiter.
   * @param input   The <code>String</code> to split.
   * @param limit  The limit on the number of resulting split elements.
   *               Values <= 0 produce the same behavior as using the
   *               <b>SPLIT_ALL</b> constant which causes the limit to be 
   *               ignored and splits to be performed on all occurrences of
   *               the pattern.  You should use the <b>SPLIT_ALL</b> constant
   *               to achieve this behavior instead of relying on the default
   *               behavior associated with non-positive limit values.
   * @since 2.0
   */
  public static void split(Collection results, PatternMatcher matcher,
                           Pattern pattern, String input, int limit)
  {
    int beginOffset;
    MatchResult currentResult;
    PatternMatcherInput pinput;

    pinput = new PatternMatcherInput(input);
    beginOffset = 0;

    while(--limit != 0 && matcher.contains(pinput, pattern)) {
      currentResult = matcher.getMatch();
      results.add(input.substring(beginOffset,
                                  currentResult.beginOffset(0)));
      beginOffset = currentResult.endOffset(0);
    }

    results.add(input.substring(beginOffset, input.length()));
  }


  /**
   * Splits up a <code>String</code> instance and stores results as a
   * <code>Collection</code> of all its substrings using a regular expression
   * as the delimiter.
   * This method is inspired by the Perl split() function and behaves 
   * identically to it when used in conjunction with the Perl5Matcher and
   * Perl5Pattern classes except for the following difference:
   * <p>
   * <ul>
   * In Perl, if the split expression contains parentheses, the split()
   * method creates additional list elements from each of the matching
   * subgroups in the pattern.  In other words:
   * <ul><p><code>split(list, "/([,-])/", "8-12,15,18")</code></ul>
   * <p> produces the list containing: 
   * <ul><p><code> { "8", "-", "12", ",", "15", ",", "18" } </code> </ul>
   * <p> The OROMatcher split method does not follow this behavior.  The
   * following list would be produced by OROMatcher:
   * <ul><p><code> { "8", "12",  "15", "18" } </code> </ul>
   * <p> To obtain the Perl behavior, use
   * {@link org.apache.oro.text.perl.Perl5Util#split}.
   * </ul>
   * <p>
   * This method is identical to calling:
   * <blockquote><pre>
   * split(matcher, pattern, input, Util.SPLIT_ALL);
   * </pre></blockquote>
   * <p>
   * @param results A <code>Collection</code> to which all the substrings of
   *         the input that occur between the regular expression delimiter
   *         occurences are appended.
   * @param matcher The regular expression matcher to execute the split.
   * @param pattern The regular expression to use as a split delimiter.
   * @param input   The <code>String</code> to split.
   * @since 2.0
   */
  public static void split(Collection results,  PatternMatcher matcher,
                           Pattern pattern, String input)
  {
    split(results, matcher, pattern, input, SPLIT_ALL);
  }

  /**
   * Splits up a <code>String</code> instance into strings contained in a
   * <code>Vector</code> of size not greater than a specified limit.  The
   * string is split with a regular expression as the delimiter. 
   * The <b>limit</b> parameter essentially says to split the
   * string only on at most the first <b>limit - 1</b> number of pattern
   * occurences.
   * <p>
   * This method is inspired by the Perl split() function and behaves 
   * identically to it when used in conjunction with the Perl5Matcher and
   * Perl5Pattern classes except for the following difference:
   * <ul><p>
   * In Perl, if the split expression contains parentheses, the split()
   * method creates additional list elements from each of the matching
   * subgroups in the pattern.  In other words:
   * <ul><p><code>split("/([,-])/", "8-12,15,18")</code></ul>
   * <p> produces the Vector containing:
   * <ul><p><code> { "8", "-", "12", ",", "15", ",", "18" } </code> </ul>
   * <p> The OROMatcher split method does not follow this behavior.  The
   * following Vector would be produced by OROMatcher:
   * <ul><p><code> { "8", "12",  "15", "18" } </code> </ul>
   * <p> To obtain the Perl behavior, use
   * {@link org.apache.oro.text.perl.Perl5Util#split}.
   * </ul>
   * <p>
   * @deprecated Use 
   *  {@link #split(Collection, PatternMatcher, Pattern, String, int)} instead.
   * @param matcher The regular expression matcher to execute the split.
   * @param pattern The regular expression to use as a split delimiter.
   * @param input  The <code>String</code> to split.
   * @param limit  The limit on the size of the returned <code>Vector</code>.
   *               Values <= 0 produce the same behavior as using the
   *               <b>SPLIT_ALL</b> constant which causes the limit to be 
   *               ignored and splits to be performed on all occurrences of
   *               the pattern.  You should use the <b>SPLIT_ALL</b> constant
   *               to achieve this behavior instead of relying on the default
   *               behavior associated with non-positive limit values.
   * @return A <code>Vector</code> containing the substrings of the input
   *         that occur between the regular expression delimiter occurences.
   *         The input will not be split into any more substrings than the
   *         specified <code>limit</code>.  A way of thinking of this is that
   *         only the first <code>limit - 1</code> matches of the delimiting
   *         regular expression will be used to split the input.
   * @since 1.0
   */
  public static ArrayList split(PatternMatcher matcher, Pattern pattern,
                             String input, int limit)
  {
    ArrayList results = new ArrayList(20); 

    split(results, matcher, pattern, input, limit);

    return results;
  }


  /**
   * Splits up a <code>String</code> instance into a <code>Vector</code>
   * of all its substrings using a regular expression as the delimiter.
   * This method is inspired by the Perl split() function and behaves 
   * identically to it when used in conjunction with the Perl5Matcher and
   * Perl5Pattern classes except for the following difference:
   * <p>
   * <ul>
   * In Perl, if the split expression contains parentheses, the split()
   * method creates additional list elements from each of the matching
   * subgroups in the pattern.  In other words:
   * <ul><p><code>split("/([,-])/", "8-12,15,18")</code></ul>
   * <p> produces the Vector containing: 
   * <ul><p><code> { "8", "-", "12", ",", "15", ",", "18" } </code> </ul>
   * <p> The OROMatcher split method does not follow this behavior.  The
   * following Vector would be produced by OROMatcher:
   * <ul><p><code> { "8", "12",  "15", "18" } </code> </ul>
   * <p> To obtain the Perl behavior, use
   * {@link org.apache.oro.text.perl.Perl5Util#split}.
   * </ul>
   * <p>
   * This method is identical to calling:
   * <blockquote><pre>
   * split(matcher, pattern, input, Util.SPLIT_ALL);
   * </pre></blockquote>
   * <p>
   * @deprecated Use 
   * {@link #split(Collection, PatternMatcher, Pattern, String)} instead.
   * @param matcher The regular expression matcher to execute the split.
   * @param pattern The regular expression to use as a split delimiter.
   * @param input   The <code>String</code> to split.
   * @return A <code>Vector</code> containing all the substrings of the input
   *         that occur between the regular expression delimiter occurences.
   * @since 1.0
   */
  public static ArrayList split( PatternMatcher matcher, Pattern pattern,
                              String input)
  {
    return split(matcher, pattern, input, SPLIT_ALL);
  }


  /**
   * Searches a string for a pattern and replaces the first occurrences
   * of the pattern with a Substitution up to the number of
   * substitutions specified by the <b>numSubs</b> parameter.  A 
   * <b>numSubs</b> value of <b>SUBSTITUTE_ALL</b> will cause all occurrences
   * of the pattern to be replaced.
   * <p>
   * @param matcher The regular expression matcher to execute the pattern
   *                search.
   * @param pattern The regular expression to search for and substitute
   *                occurrences of.
   * @param sub     The Substitution used to substitute pattern occurences.
   * @param input   The <code>String</code> on which to perform substitutions.
   * @param numSubs The number of substitutions to perform.  Only the
   *                first <b> numSubs </b> patterns encountered are
   *                substituted.  If you want to substitute all occurences
   *                set this parameter to <b> SUBSTITUTE_ALL </b>.
   * @return A String comprising the input string with the substitutions,
   *         if any, made.  If no substitutions are made, the returned String
   *         is the original input String.
   * @since 1.0
   */
  public static String substitute(PatternMatcher matcher, Pattern pattern,
                                  Substitution sub, String input, int numSubs)
  {
    StringBuffer buffer = new StringBuffer(input.length());
    PatternMatcherInput pinput = new PatternMatcherInput(input);
    
    // Users have indicated that they expect the result to be the
    // original input string, rather than a copy, if no substitutions
    // are performed, 
    if(substitute(buffer, matcher, pattern, sub, pinput, numSubs) != 0)
      return buffer.toString();
    return input;
  }

  /**
   * Searches a string for a pattern and substitutes only the first
   * occurence of the pattern.
   * <p>
   * This method is identical to calling:
   * <blockquote><pre>
   * substitute(matcher, pattern, sub, input, 1);
   * </pre></blockquote>
   * <p>
   * @param matcher The regular expression matcher to execute the pattern
   *                search.
   * @param pattern The regular expression to search for and substitute
   *                occurrences of.
   * @param sub     The Substitution used to substitute pattern occurences.
   * @param input   The <code>String</code> on which to perform substitutions.
   * @return A String comprising the input string with the substitutions,
   *         if any, made.  If no substitutions are made, the returned String
   *         is the original input String.
   * @since 1.0
   */
  public static String substitute(PatternMatcher matcher, Pattern pattern,
                                  Substitution sub, String input)
  {
    return substitute(matcher, pattern, sub, input, 1);
  }

  /**
   * Searches a string for a pattern and replaces the first occurrences
   * of the pattern with a Substitution up to the number of
   * substitutions specified by the <b>numSubs</b> parameter.  A 
   * <b>numSubs</b> value of <b>SUBSTITUTE_ALL</b> will cause all occurrences
   * of the pattern to be replaced.  The number of substitutions made
   * is returned.
   * <p>
   * @param result  The StringBuffer in which to store the result of the
   *                substitutions.  The buffer is only appended to.
   * @param matcher The regular expression matcher to execute the pattern
   *                search.
   * @param pattern The regular expression to search for and substitute
   *                occurrences of.
   * @param sub     The Substitution used to substitute pattern occurences.
   * @param input   The input on which to perform substitutions.
   * @param numSubs The number of substitutions to perform.  Only the
   *                first <b> numSubs </b> patterns encountered are
   *                substituted.  If you want to substitute all occurences
   *                set this parameter to <b> SUBSTITUTE_ALL </b>.
   * @return The number of substitutions made.
   * @since 2.0.6
   */
  public static int substitute(StringBuffer result,
                               PatternMatcher matcher, Pattern pattern,
                               Substitution sub, String input,
                               int numSubs)
  {
    PatternMatcherInput pinput = new PatternMatcherInput(input);
    return substitute(result, matcher, pattern, sub, pinput, numSubs);
  }

  /**
   * Searches a string for a pattern and replaces the first occurrences
   * of the pattern with a Substitution up to the number of
   * substitutions specified by the <b>numSubs</b> parameter.  A 
   * <b>numSubs</b> value of <b>SUBSTITUTE_ALL</b> will cause all occurrences
   * of the pattern to be replaced.  The number of substitutions made
   * is returned.
   * <p>
   * @param result  The StringBuffer in which to store the result of the
   *                substitutions.  The buffer is only appended to.
   * @param matcher The regular expression matcher to execute the pattern
   *                search.
   * @param pattern The regular expression to search for and substitute
   *                occurrences of.
   * @param sub     The Substitution used to substitute pattern occurences.
   * @param input   The input on which to perform substitutions.
   * @param numSubs The number of substitutions to perform.  Only the
   *                first <b> numSubs </b> patterns encountered are
   *                substituted.  If you want to substitute all occurences
   *                set this parameter to <b> SUBSTITUTE_ALL </b>.
   * @return The number of substitutions made.
   * @since 2.0.3
   */
  public static int substitute(StringBuffer result,
                               PatternMatcher matcher, Pattern pattern,
                               Substitution sub, PatternMatcherInput input,
                               int numSubs)
  {
    int beginOffset, subCount;
    char[] inputBuffer;

    subCount    = 0;
    beginOffset = input.getBeginOffset(); 
    inputBuffer = input.getBuffer();

    // Must be != 0 because SUBSTITUTE_ALL is represented by -1.
    // Do NOT change to numSubs > 0.
    while(numSubs != 0 && matcher.contains(input, pattern)) {
      --numSubs;
      ++subCount;
      result.append(inputBuffer, beginOffset,
                    input.getMatchBeginOffset() - beginOffset);
      sub.appendSubstitution(result, matcher.getMatch(), subCount,
                             input, matcher, pattern);
      beginOffset = input.getMatchEndOffset();
    }

    result.append(inputBuffer, beginOffset, input.length() - beginOffset);
    return subCount;
  }
}
