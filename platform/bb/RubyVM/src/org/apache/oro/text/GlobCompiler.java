/* 
 * $Id: GlobCompiler.java 124053 2005-01-04 01:24:35Z dfs $
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

import org.apache.oro.text.regex.*;

/**
 * The GlobCompiler class will compile a glob expression into a Perl5Pattern
 * that may be used to match patterns in conjunction with Perl5Matcher.
 * Rather than create extra GlobMatcher and GlobPattern classes tailored
 * to the task of matching glob expressions, we have simply reused the
 * Perl5 regular expression classes from org.apache.oro.text.regex by
 * making GlobCompiler translate a glob expression into a Perl5 expression
 * that is compiled by a Perl5Compiler instance internal to the GlobCompiler.
 * <p>
 * Because there are various similar glob expression syntaxes, GlobCompiler
 * tries to provide a small amount of customization by providing the
 * {@link #STAR_CANNOT_MATCH_NULL_MASK}
 * and  {@link #QUESTION_MATCHES_ZERO_OR_ONE_MASK} compilation options.
 * <p>
 * The GlobCompiler expression syntax is based on Unix shell glob expressions
 * but should be usable to simulate Win32 wildcards.  The following syntax
 * is supported:
 * <ul>
 * <li> <b>*</b> - Matches zero or more instances of any character.  If the
 *          STAR_CANNOT_MATCH_NULL_MASK option is used, <b>*</b> matches
 *          one or more instances of any character.
 * <li> <b>?</b> - Matches one instance of any character. If the
 *          QUESTION_MATCHES_ZERO_OR_ONE_MASK option is used, <b>?</b>
 *          matches zero or one instances of any character.
 * <li> <b>[...]</b> - Matches any of characters enclosed by the brackets.
 *          <b> * </b> and <b>?</b> lose their special meanings within a
 *          character class.  Additionaly if the first character following
 *          the opening bracket is a <b>!</b> or a <b>^</b>, then any
 *          character not in the character class is matched.  A <b>-</b>
 *          between two characters can be used to denote a range.  A
 *          <b>-</b> at the beginning or end of the character class matches
 *          itself rather than referring to a range.  A <b>]</b> immediately
 *          following the opening <b>[</b> matches itself rather than
 *          indicating the end of the character class, otherwise it must be
 *          escaped with a backslash to refer to itself.
 * <li> <b>\</b> - A backslash matches itself in most situations.  But
 *          when a special character such as a <b>*</b> follows it, a
 *          backslash <em> escapes </em> the character, indicating that
 *          the special chracter should be interpreted as a normal character
 *          instead of its special meaning.
 * <li> All other characters match themselves.
 * </ul>
 * <p>
 * Please remember that the when you construct a Java string in Java code,
 * the backslash character is itself a special Java character, and it must
 * be double backslashed to represent single backslash in a regular
 * expression.
 *
 * @version @version@
 * @since 1.0
 * @see org.apache.oro.text.regex.PatternCompiler
 * @see org.apache.oro.text.regex.Perl5Matcher
 */
public final class GlobCompiler implements PatternCompiler {
  /**
   * The default mask for the {@link #compile compile} methods.
   * It is equal to 0.  The default behavior is for a glob expression to
   * be case sensitive unless it is compiled with the CASE_INSENSITIVE_MASK
   * option.
   */
  public static final int DEFAULT_MASK = 0;

  /**
   * A mask passed as an option to the {@link #compile compile} methods
   * to indicate a compiled glob expression should be case insensitive.
   */ 
  public static final int CASE_INSENSITIVE_MASK             = 0x0001;

  /**
   * A mask passed as an option to the {@link #compile compile} methods
   * to indicate that a * should not be allowed to match the null string.
   * The normal behavior of the * metacharacter is that it may match any
   * 0 or more characters.  This mask causes it to match 1 or more 
   * characters of anything.
   */ 
  public static final int STAR_CANNOT_MATCH_NULL_MASK       = 0x0002;

  /**
   * A mask passed as an option to the {@link #compile compile} methods
   * to indicate that a ? should not be allowed to match the null string.
   * The normal behavior of the ? metacharacter is that it may match any 1
   * character.  This mask causes it to match 0 or 1 characters.
   */ 
  public static final int QUESTION_MATCHES_ZERO_OR_ONE_MASK = 0x0004;

  /**
   * A mask passed as an option to the {@link #compile compile} methods
   * to indicate that the resulting Perl5Pattern should be treated as a
   * read only data structure by Perl5Matcher, making it safe to share
   * a single Perl5Pattern instance among multiple threads without needing
   * synchronization.  Without this option, Perl5Matcher reserves the right
   * to store heuristic or other information in Perl5Pattern that might
   * accelerate future matches.  When you use this option, Perl5Matcher will
   * not store or modify any information in a Perl5Pattern.  Use this option
   * when you want to share a Perl5Pattern instance among multiple threads
   * using different Perl5Matcher instances.
   */
  public static final int READ_ONLY_MASK        = 0x0008;

  private Perl5Compiler __perl5Compiler;

  private static boolean __isPerl5MetaCharacter(char ch) {
    return ("'*?+[]()|^$.{}\\".indexOf(ch) >= 0);
  }

  private static boolean __isGlobMetaCharacter(char ch) {
    return ("*?[]".indexOf(ch) >= 0);
  }

  /**
   * This static method is the basic engine of the Glob PatternCompiler
   * implementation.  It takes a glob expression in the form of a character
   * array and converts it into a String representation of a Perl5 pattern.
   * The method is made public so that programmers may use it for their
   * own purposes.  However, the GlobCompiler compile methods work by
   * converting the glob pattern to a Perl5 pattern using this method, and
   * then invoking the compile() method of an internally stored Perl5Compiler
   * instance.
   * <p>
   * @param pattern  A character array representation of a Glob pattern.
   * @return A String representation of a Perl5 pattern equivalent to the
   *         Glob pattern.
   */
  public static String globToPerl5(char[] pattern, int options) {
    boolean inCharSet, starCannotMatchNull = false, questionMatchesZero;
    int ch;
    StringBuffer buffer;

    buffer = new StringBuffer(2*pattern.length);
    inCharSet = false;

    questionMatchesZero = ((options & QUESTION_MATCHES_ZERO_OR_ONE_MASK) != 0);
    starCannotMatchNull = ((options & STAR_CANNOT_MATCH_NULL_MASK) != 0);

    for(ch=0; ch < pattern.length; ch++) {
      switch(pattern[ch]) {
      case '*':
	if(inCharSet)
	  buffer.append('*');
	else {
	  if(starCannotMatchNull)
	      buffer.append(".+");
	  else
	    buffer.append(".*");
	}
	break;
      case '?':
	if(inCharSet)
	  buffer.append('?');
	else {
	  if(questionMatchesZero)
	    buffer.append(".?");
	  else
	    buffer.append('.');
	}
	break;
      case '[':
	inCharSet = true;
	buffer.append(pattern[ch]);

	if(ch + 1 < pattern.length) {
	  switch(pattern[ch + 1]) {
	  case '!':
	  case '^':
	    buffer.append('^');
	    ++ch;
	    continue;
	  case ']':
	    buffer.append(']');
	    ++ch;
	    continue;
	  }
	}
	break;
      case ']':
	inCharSet = false;
	buffer.append(pattern[ch]);
	break;
      case '\\':
	buffer.append('\\');
	if(ch == pattern.length - 1) {
	  buffer.append('\\');
	} else if(__isGlobMetaCharacter(pattern[ch + 1]))
	  buffer.append(pattern[++ch]);
	else
	  buffer.append('\\');
	break;
      default:
	if(!inCharSet && __isPerl5MetaCharacter(pattern[ch]))
	  buffer.append('\\');
	buffer.append(pattern[ch]);
	break;
      }
    }

    return buffer.toString();
  }


  /**
   * The default GlobCompiler constructor.  It initializes an internal
   * Perl5Compiler instance to compile translated glob expressions.
   */
  public GlobCompiler() {
    __perl5Compiler = new Perl5Compiler();
  }


  /**
   * Compiles a Glob expression into a Perl5Pattern instance that
   * can be used by a Perl5Matcher object to perform pattern matching.
   * <p>
   * @param pattern  A Glob expression to compile.
   * @param options  A set of flags giving the compiler instructions on
   *                 how to treat the glob expression.  The flags
   *                 are a logical OR of any number of the 3 <b>MASK</b>
   *                 constants.  For example:
   *                 <pre>
   * regex =
   *   compiler.compile(pattern, GlobCompiler.
   *                    CASE_INSENSITIVE_MASK |
   *                    GlobCompiler.STAR_CANNOT_MATCH_NULL_MASK);
   *                 </pre>
   *                  This says to compile the pattern so that *
   *                  cannot match the null string and to perform
   *                  matches in a case insensitive manner.
   * @return A Pattern instance constituting the compiled expression.
   *         This instance will always be a Perl5Pattern and can be reliably
   *         casted to a Perl5Pattern.
   * @exception MalformedPatternException  If the compiled expression
   *  is not a valid Glob expression.
   */
  public Pattern compile(char[] pattern, int options)
       throws MalformedPatternException
  {
    int perlOptions = 0;
    if((options & CASE_INSENSITIVE_MASK) != 0)
      perlOptions |= Perl5Compiler.CASE_INSENSITIVE_MASK;
    if((options & READ_ONLY_MASK) != 0)
      perlOptions |= Perl5Compiler.READ_ONLY_MASK;
    return __perl5Compiler.compile(globToPerl5(pattern, options), perlOptions);
  }

  /**
   * Same as calling <b>compile(pattern, GlobCompiler.DEFAULT_MASK);</b>
   * <p>
   * @param pattern  A regular expression to compile.
   * @return A Pattern instance constituting the compiled regular expression.
   *         This instance will always be a Perl5Pattern and can be reliably
   *         casted to a Perl5Pattern.
   * @exception MalformedPatternException  If the compiled expression
   *  is not a valid Glob expression.
   */
  public Pattern compile(char[] pattern) throws MalformedPatternException {
    return compile(pattern, DEFAULT_MASK);
  }

  /**
   * Same as calling <b>compile(pattern, GlobCompiler.DEFAULT_MASK);</b>
   * <p>
   * @param pattern  A regular expression to compile.
   * @return A Pattern instance constituting the compiled regular expression.
   *         This instance will always be a Perl5Pattern and can be reliably
   *         casted to a Perl5Pattern.
   * @exception MalformedPatternException  If the compiled expression
   *  is not a valid Glob expression.
   */
  public Pattern compile(String pattern) throws MalformedPatternException {
    return compile(pattern.toCharArray(), DEFAULT_MASK);
  }

  /**
   * Compiles a Glob expression into a Perl5Pattern instance that
   * can be used by a Perl5Matcher object to perform pattern matching.
   * <p>
   * @param pattern  A Glob expression to compile.
   * @param options  A set of flags giving the compiler instructions on
   *                 how to treat the glob expression.  The flags
   *                 are a logical OR of any number of the 3 <b>MASK</b>
   *                 constants.  For example:
   *                 <pre>
   * regex =
   *   compiler.compile("*.*", GlobCompiler.
   *                    CASE_INSENSITIVE_MASK |
   *                    GlobCompiler.STAR_CANNOT_MATCH_NULL_MASK);
   *                 </pre>
   *                  This says to compile the pattern so that *
   *                  cannot match the null string and to perform
   *                  matches in a case insensitive manner.
   * @return A Pattern instance constituting the compiled expression.
   *         This instance will always be a Perl5Pattern and can be reliably
   *         casted to a Perl5Pattern.
   * @exception MalformedPatternException  If the compiled expression
   *  is not a valid Glob expression.
   */
  public Pattern compile(String pattern, int options)
       throws MalformedPatternException
  {
    return compile(pattern.toCharArray(), options);
  }

}

