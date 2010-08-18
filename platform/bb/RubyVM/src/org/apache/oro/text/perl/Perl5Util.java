/* 
 * $Id: Perl5Util.java 390702 2006-04-01 17:31:03Z dfs $
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


package org.apache.oro.text.perl;

import j2me.util.*;

import org.apache.oro.text.*;
import org.apache.oro.text.regex.*;
import org.apache.oro.util.*;
import j2me.lang.CharacterMe;
import j2me.lang.ThreadLocal;
import j2me.util.ArrayList;
/**
 * This is a utility class implementing the 3 most common Perl5 operations
 * involving regular expressions:
 * <ul>
 * <li> [m]/pattern/[i][m][s][x],
 * <li> s/pattern/replacement/[g][i][m][o][s][x],
 * <li> and split().
 * </ul>
 * As with Perl, any non-alphanumeric character can be used in lieu of
 * the slashes.
 *  <p>
 * The objective of the class is to minimize the amount of code a Java
 * programmer using Jakarta-ORO
 * has to write to achieve the same results as Perl by 
 * transparently handling regular expression compilation, caching, and
 * matching.  A second objective is to use the same Perl pattern matching
 * syntax to ease the task of Perl programmers transitioning to Java
 * (this also reduces the number of parameters to a method).
 * All the state affecting methods are synchronized to avoid
 * the maintenance of explicit locks in multithreaded programs.  This
 * philosophy differs from the
 * {@link org.apache.oro.text.regex} package, where
 * you are expected to either maintain explicit locks, or more preferably
 * create separate compiler and matcher instances for each thread.
 * <p>
 * To use this class, first create an instance using the default constructor
 * or initialize the instance with a PatternCache of your choosing using
 * the alternate constructor.  The default cache used by Perl5Util is a
 * PatternCacheLRU of capacity GenericPatternCache.DEFAULT_CAPACITY.  You may
 * want to create a cache with a different capacity, a different
 * cache replacement policy, or even devise your own PatternCache
 * implementation.  The PatternCacheLRU is probably the best general purpose
 * pattern cache, but your specific application may be better served by
 * a different cache replacement policy.  You should remember that you can
 * front-load a cache with all the patterns you will be using before
 * initializing a Perl5Util instance, or you can just let Perl5Util
 * fill the cache as you use it.
 * <p>
 * You might use the class as follows:
 * <pre>
 * Perl5Util util = new Perl5Util();
 * String line;
 * DataInputStream input;
 * PrintStream output;
 * 
 * // Initialization of input and output omitted
 * while((line = input.readLine()) != null) {
 *     // First find the line with the string we want to substitute because
 *     // it is cheaper than blindly substituting each line.
 *     if(util.match("/HREF=\"description1.html\"/", line)) {
 *        line = util.substitute("s/description1\\.html/about1.html/", line);
 *     }
 *    output.println(line);
 * }
 * </pre>
 * <p>
 * A couple of things to remember when using this class are that the
 * {@link #match match()} methods have the same meaning as
 * {@link org.apache.oro.text.regex.Perl5Matcher#contains
 *  Perl5Matcher.contains()}
 * and <code>=~ m/pattern/</code> in Perl.  The methods are named match
 * to more closely associate them with Perl and to differentiate them
 * from {@link org.apache.oro.text.regex.Perl5Matcher#matches
 * Perl5Matcher.matches()}.
 * A further thing to keep in mind is that the
 * {@link MalformedPerl5PatternException} class is derived from
 * RuntimeException which means you DON'T have to catch it.  The reasoning
 * behind this is that you will detect your regular expression mistakes
 * as you write and debug your program when a MalformedPerl5PatternException
 * is thrown during a test run.  However, we STRONGLY recommend that you
 * ALWAYS catch MalformedPerl5PatternException whenever you deal with a
 * DYNAMICALLY created pattern.  Relying on a fatal
 * MalformedPerl5PatternException being thrown to detect errors while
 * debugging is only useful for dealing with static patterns, that is, actual
 * pregenerated strings present in your program.  Patterns created from user
 * input or some other dynamic method CANNOT be relied upon to be correct
 * and MUST be handled by catching MalformedPerl5PatternException for your
 * programs to be robust.
 * <p>
 * Finally, as a convenience Perl5Util implements 
 * the {@link org.apache.oro.text.regex.MatchResult MatchResult} interface.
 * The methods are merely wrappers which call the corresponding method of
 * the last {@link org.apache.oro.text.regex.MatchResult MatchResult}
 * found (which can be accessed with {@link #getMatch()}) by a match or
 * substitution (or even a split, but this isn't particularly useful).
 * The
 * {@link org.apache.oro.text.regex.MatchResult MatchResult} returned
 * by {@link #getMatch()} is stored in a thread-local variable.  Therefore
 * concurrent calls to {@link #getMatch()} will produce the last match
 * found by the calling thread.
 *
 * @version @version@
 * @since 1.0
 * @see MalformedPerl5PatternException
 * @see org.apache.oro.text.PatternCache
 * @see org.apache.oro.text.PatternCacheLRU
 * @see org.apache.oro.text.regex.MatchResult
 */
public final class Perl5Util implements MatchResult {
  /** The regular expression to use to parse match expression. */
  private static final String __matchExpression = "m?(\\W)(.*)\\1([imsx]*)";

  /** The pattern cache to compile and store patterns */
  private PatternCache __patternCache;
  /** The hashtable to cache higher-level expressions */
  private Cache __expressionCache;
  /** The pattern matcher to perform matching operations. */
  private Perl5Matcher __matcher;
  /** The compiled match expression parsing regular expression. */
  private Pattern __matchPattern;

  static class ThreadState {
    /** The last match from a successful call to a matching method. */
    MatchResult lastMatch = null;

    /**
     * Keeps track of the original input (for postMatch() and preMatch())
     * methods.  This will be discarded if the preMatch() and postMatch()
     * methods are moved into the MatchResult interface.
     */
    Object originalInput = null;

    /**
     * Keeps track of the begin and end offsets of the original input for
     * the postMatch() and preMatch() methods.
     */
    private int inputBeginOffset = 0, inputEndOffset = 0;
  }

  private ThreadLocal __threadState;

  /**
   * A container for temporarily holding the results of a split before
   * deleting trailing empty fields.
   */
  private ArrayList __splitList;

  /** Used for default return value of post and pre Match() */
  private static final String __nullString = "";

  /**
   * A constant passed to the {@link #split split()} methods indicating
   * that all occurrences of a pattern should be used to split a string. 
   */
  public static final int SPLIT_ALL = Util.SPLIT_ALL;

  /**
   * A secondary constructor for Perl5Util.  It initializes the Perl5Matcher
   * used by the class to perform matching operations, but requires the
   * programmer to provide a PatternCache instance for the class
   * to use to compile and store regular expressions.  You would want to
   * use this constructor if you want to change the capacity or policy 
   * of the cache used.  Example uses might be:
   * <pre>
   * // We know we're going to use close to 50 expressions a whole lot, so
   * // we create a cache of the proper size.
   * util = new Perl5Util(new PatternCacheLRU(50));
   * </pre>
   * or
   * <pre>
   * // We're only going to use a few expressions and know that second-chance
   * // fifo is best suited to the order in which we are using the patterns.
   * util = new Perl5Util(new PatternCacheFIFO2(10));
   * </pre>
   */
  public Perl5Util(PatternCache cache) {
    __splitList    = new ArrayList();
    __matcher      = new Perl5Matcher();
    __threadState  = new ThreadLocal() {
        protected synchronized Object initialValue() {
          return new ThreadState();
        }
      };
    __patternCache = cache;
    __expressionCache = new CacheLRU(cache.capacity());
    __compilePatterns();
  }

  /**
   * Default constructor for Perl5Util.  This initializes the Perl5Matcher
   * used by the class to perform matching operations and creates a
   * default PatternCacheLRU instance to use to compile and cache regular
   * expressions.  The size of this cache is 
   * GenericPatternCache.DEFAULT_CAPACITY.
   */
  public Perl5Util() {
    this(new PatternCacheLRU());
  }


  /**
   * A convenience method to take care of typecasting.  Wouldn't be
   * necessary if we we could use Java Generics.
   **/
  private ThreadState __getState() {
    return (ThreadState)__threadState.get();
  }

  /**
   * Compiles the patterns (currently only the match expression) used to
   * parse Perl5 expressions.  Right now it initializes __matchPattern.
   */
  private void __compilePatterns() {
    Perl5Compiler compiler = new Perl5Compiler();

    try {
      __matchPattern = 
        compiler.compile(__matchExpression, Perl5Compiler.SINGLELINE_MASK);
    } catch(MalformedPatternException e) {
      // This should only happen during debugging.
      //e.printStackTrace();
      throw new RuntimeException(e.getMessage());
    }
  }

  /**
   * Parses a match expression and returns a compiled pattern.
   * First checks the expression cache and if the pattern is not found,
   * then parses the expression and fetches a compiled pattern from the
   * pattern cache.  Otherwise, just uses the pattern found in the
   * expression cache.  __matchPattern is used to parse the expression.
   * <p>
   * @param pattern  The Perl5 match expression to parse.
   * @exception MalformedPerl5PatternException If there is an error parsing
   *            the expression.
   */
  private Pattern __parseMatchExpression(String pattern)
       throws MalformedPerl5PatternException 
  {
    int index, compileOptions;
    String options, regex;
    MatchResult result;
    Object obj;
    Pattern ret;

    obj = __expressionCache.getElement(pattern);

    // Must catch ClassCastException because someone might incorrectly 
    // pass an s/// expression.  try block is cheaper than checking
    // instanceof
    try {
      if(obj != null)
        return (Pattern)obj;
    } catch(ClassCastException e) {
      // Fall through and parse expression
    }

    if(!__matcher.matches(pattern, __matchPattern))
      throw new
        MalformedPerl5PatternException("Invalid expression: " +
                                       pattern);

    result = __matcher.getMatch();

    regex = result.group(2);
    compileOptions = Perl5Compiler.DEFAULT_MASK;

    options = result.group(3);

    if(options != null) {
      index = options.length();

      while(index-- > 0) {
        switch(options.charAt(index)) {
        case 'i' :
          compileOptions |= Perl5Compiler.CASE_INSENSITIVE_MASK;
          break;
        case 'm' : compileOptions |= Perl5Compiler.MULTILINE_MASK; break;
        case 's' : compileOptions |= Perl5Compiler.SINGLELINE_MASK; break;
        case 'x' : compileOptions |= Perl5Compiler.EXTENDED_MASK; break;
        default  :
          throw new
            MalformedPerl5PatternException("Invalid options: " + options);
        }
      }
    }

    ret = __patternCache.getPattern(regex, compileOptions);
    __expressionCache.addElement(pattern, ret);

    return ret;
  }

  /**
   * Searches for the first pattern match somewhere in a character array
   * taking a pattern specified in Perl5 native format:
   * <blockquote><pre>
   * [m]/pattern/[i][m][s][x]
   * </pre></blockquote>
   * The <code>m</code> prefix is optional and the meaning of the optional
   * trailing options are:
   * <dl compact> 
   * <dt> i <dd> case insensitive match
   * <dt> m <dd> treat the input as consisting of multiple lines
   * <dt> s <dd> treat the input as consisting of a single line
   * <dt> x <dd> enable extended expression syntax incorporating whitespace
   *             and comments
   * </dl>
   * As with Perl, any non-alphanumeric character can be used in lieu of
   * the slashes.
   * <p>
   * If the input contains the pattern, the org.apache.oro.text.regex.MatchResult
   * can be obtained by calling {@link #getMatch()}.
   * However, Perl5Util implements the MatchResult interface as a wrapper
   * around the last MatchResult found, so you can call its methods to
   * access match information.
   * <p>
   * @param pattern  A Perl5 regular expression to compile and search for.
   * @param input    The char[] input to search.
   * @return True if the input contains the pattern, false otherwise.
   * @exception MalformedPerl5PatternException  If there is an error in
   *            the pattern.  You are not forced to catch this exception
   *            because it is derived from RuntimeException.
   */
  public synchronized boolean match(String pattern, char[] input) 
       throws MalformedPerl5PatternException
  {
    boolean result;
    __parseMatchExpression(pattern);

    result = __matcher.contains(input, __parseMatchExpression(pattern));
                         
    if(result) {
      ThreadState state      = __getState();
      state.lastMatch        = __matcher.getMatch();
      state.originalInput    = input;
      state.inputBeginOffset = 0;
      state.inputEndOffset   = input.length;
    }

    return result;
  }


  /**
   * Searches for the first pattern match in a String taking
   * a pattern specified in Perl5 native format:
   * <blockquote><pre>
   * [m]/pattern/[i][m][s][x]
   * </pre></blockquote>
   * The <code>m</code> prefix is optional and the meaning of the optional
   * trailing options are:
   * <dl compact> 
   * <dt> i <dd> case insensitive match
   * <dt> m <dd> treat the input as consisting of multiple lines
   * <dt> s <dd> treat the input as consisting of a single line
   * <dt> x <dd> enable extended expression syntax incorporating whitespace
   *             and comments
   * </dl>
   * As with Perl, any non-alphanumeric character can be used in lieu of
   * the slashes.
   * <p>
   * If the input contains the pattern, the
   * {@link org.apache.oro.text.regex.MatchResult MatchResult}
   * can be obtained by calling {@link #getMatch()}.
   * However, Perl5Util implements the MatchResult interface as a wrapper
   * around the last MatchResult found, so you can call its methods to
   * access match information.
   * <p>
   * @param pattern  A Perl5 regular expression to compile and search for.
   * @param input    The String input to search.
   * @return True if the input contains the pattern, false otherwise.
   * @exception MalformedPerl5PatternException  If there is an error in
   *            the pattern.  You are not forced to catch this exception
   *            because it is derived from RuntimeException.
   */
  public synchronized boolean match(String pattern, String input)
       throws MalformedPerl5PatternException
  {
    return match(pattern, input.toCharArray());
  }


  /**
   * Searches for the next pattern match somewhere in a
   * org.apache.oro.text.regex.PatternMatcherInput instance, taking
   * a pattern specified in Perl5 native format:
   * <blockquote><pre>
   * [m]/pattern/[i][m][s][x]
   * </pre></blockquote>
   * The <code>m</code> prefix is optional and the meaning of the optional
   * trailing options are:
   * <dl compact> 
   * <dt> i <dd> case insensitive match
   * <dt> m <dd> treat the input as consisting of multiple lines
   * <dt> s <dd> treat the input as consisting of a single line
   * <dt> x <dd> enable extended expression syntax incorporating whitespace
   *             and comments
   * </dl>
   * As with Perl, any non-alphanumeric character can be used in lieu of
   * the slashes.
   * <p>
   * If the input contains the pattern, the
   * {@link org.apache.oro.text.regex.MatchResult MatchResult}
   * can be obtained by calling {@link #getMatch()}.
   * However, Perl5Util implements the MatchResult interface as a wrapper
   * around the last MatchResult found, so you can call its methods to
   * access match information.
   * After the call to this method, the PatternMatcherInput current offset
   * is advanced to the end of the match, so you can use it to repeatedly
   * search for expressions in the entire input using a while loop as
   * explained in the {@link org.apache.oro.text.regex.PatternMatcherInput
   * PatternMatcherInput} documentation.
   * <p>
   * @param pattern  A Perl5 regular expression to compile and search for.
   * @param input    The PatternMatcherInput to search.
   * @return True if the input contains the pattern, false otherwise.
   * @exception MalformedPerl5PatternException  If there is an error in
   *            the pattern.  You are not forced to catch this exception
   *            because it is derived from RuntimeException.
   */
  public synchronized boolean match(String pattern, PatternMatcherInput input)
       throws MalformedPerl5PatternException
  {
    boolean result;

    result = __matcher.contains(input, __parseMatchExpression(pattern));

    if(result) {
      ThreadState state      = __getState();
      state.lastMatch        = __matcher.getMatch();
      state.originalInput    = input.getInput();
      state.inputBeginOffset = input.getBeginOffset();
      state.inputEndOffset   = input.getEndOffset();
    }

    return result;
  }


  /**
   * Returns the last match found by a call to a match(), substitute(), or
   * split() method.  This method is only intended for use to retrieve a match
   * found by the last match found by a match() method.  This method should
   * be used when you want to save MatchResult instances.  Otherwise, for
   * simply accessing match information, it is more convenient to use the
   * Perl5Util methods implementing the MatchResult interface.
   * <p>
   * @return The org.apache.oro.text.regex.MatchResult instance containing the
   *         last match found.
   */
  public synchronized MatchResult getMatch() {
    return __getState().lastMatch;
  }


  /**
   * Substitutes a pattern in a given input with a replacement string.
   * The substitution expression is specified in Perl5 native format:
   * <blockquote><pre>
   * s/pattern/replacement/[g][i][m][o][s][x]
   * </pre></blockquote>
   * The <code>s</code> prefix is mandatory and the meaning of the optional
   * trailing options are:
   * <dl compact> 
   * <dt> g <dd> Substitute all occurrences of pattern with replacement.
   *             The default is to replace only the first occurrence.
   * <dt> i <dd> perform a case insensitive match
   * <dt> m <dd> treat the input as consisting of multiple lines
   * <dt> o <dd> If variable interopolation is used, only evaluate the
   *             interpolation once (the first time).  This is equivalent
   *             to using a numInterpolations argument of 1 in
   * {@link org.apache.oro.text.regex.Util#substitute Util.substitute()}.
   *             The default is to compute each interpolation independently.
   *             See
   * {@link org.apache.oro.text.regex.Util#substitute Util.substitute()}
   * and {@link org.apache.oro.text.regex.Perl5Substitution Perl5Substitution}
   *             for more details on variable interpolation in
   *             substitutions.
   * <dt> s <dd> treat the input as consisting of a single line
   * <dt> x <dd> enable extended expression syntax incorporating whitespace
   *             and comments
   * </dl>
   * As with Perl, any non-alphanumeric character can be used in lieu of
   * the slashes.  This is helpful to avoid backslashing.  For example,
   * using slashes you would have to do:
   * <blockquote><pre>
   * numSubs = util.substitute(result, "s/foo\\/bar/goo\\/\\/baz/", input);
   * </pre></blockquote>
   * when you could more easily write:
   * <blockquote><pre>
   * numSubs = util.substitute(result, "s#foo/bar#goo//baz#", input);
   * </pre></blockquote>
   * where the hashmarks are used instead of slashes.
   * <p>
   * There is a special case of backslashing that you need to pay attention
   * to.  As demonstrated above, to denote a delimiter in the substituted
   * string it must be backslashed.  However, this can be a problem
   * when you want to denote a backslash at the end of the substituted
   * string.  As of PerlTools 1.3, a new means of handling this
   * situation has been implemented.
   * In previous versions, the behavior was that
   * <blockquote>
   * "... a double backslash (quadrupled in the Java String) always
   * represents two backslashes unless the second backslash is followed
   * by the delimiter, in which case it represents a single backslash."
   * </blockquote>
   * <p>
   * The new behavior is that a backslash is always a backslash
   * in the substitution portion of the expression unless it is used to
   * escape a delimiter.  A backslash is considered to escape a delimiter
   * if an even number of contiguous backslashes preceed the backslash
   * and the delimiter following the backslash is not the FINAL delimiter
   * in the expression.  Therefore, backslashes preceding final delimiters
   * are never considered to escape the delimiter.  The following, which
   * used to be an invalid expression and require a special-case extra
   * backslash, will now replace all instances of / with \:
   * <blockquote><pre>
   * numSubs = util.substitute(result, "s#/#\\#g", input);
   * </pre></blockquote>
   * <p>
   * @param result     The StringBuffer in which to store the result of the
   *                   substitutions. The buffer is only appended to.
   * @param expression The Perl5 substitution regular expression.
   * @param input      The input on which to perform substitutions.
   * @return The number of substitutions made.
   * @exception MalformedPerl5PatternException  If there is an error in
   *            the expression.  You are not forced to catch this exception
   *            because it is derived from RuntimeException.
   * @since 2.0.6
   */
  // Expression parsing will have to be moved into a separate method if
  // there are going to be variations of this method.
  public synchronized int substitute(StringBuffer result, String expression,
                                     String input)
       throws MalformedPerl5PatternException 
  {
    boolean backslash, finalDelimiter;
    int index, compileOptions, numSubstitutions, numInterpolations;
    int firstOffset, secondOffset, thirdOffset, subCount;
    StringBuffer replacement;
    Pattern compiledPattern;
    char exp[], delimiter;
    ParsedSubstitutionEntry entry;
    Perl5Substitution substitution;
    Object obj;

    obj = __expressionCache.getElement(expression);

  __nullTest:
    if(obj != null) {
      // Must catch ClassCastException because someone might incorrectly 
      // pass an m// expression.  try block is cheaper than checking
      // instanceof.  We want to go ahead with parsing just in case so
      // we break.
      try {
        entry = (ParsedSubstitutionEntry)obj;
      } catch(ClassCastException e) {
        break __nullTest;
      }


      subCount =
        Util.substitute(result, __matcher, entry._pattern, entry._substitution,
                        input, entry._numSubstitutions);

      __getState().lastMatch = __matcher.getMatch();

      return subCount;
    }

    exp = expression.toCharArray();

    // Make sure basic conditions for a valid substitution expression hold.
    if(exp.length < 4 || exp[0] != 's' || CharacterMe.isLetterOrDigit(exp[1])
       || exp[1] == '-')
      throw new
        MalformedPerl5PatternException("Invalid expression: " + expression);
    delimiter    = exp[1];
    firstOffset  = 2;
    secondOffset = thirdOffset = -1;
    backslash    = false;

    // Parse pattern
    for(index = firstOffset; index < exp.length; index++) {
      if(exp[index] == '\\')
        backslash = !backslash;
      else if(exp[index] == delimiter && !backslash) {
        secondOffset = index;
        break;
      } else if(backslash) 
        backslash = !backslash;
    }

    if(secondOffset == -1 || secondOffset == exp.length - 1)
      throw new
        MalformedPerl5PatternException("Invalid expression: " + expression);

    // Parse replacement string

    backslash = false;
    finalDelimiter = true;
    replacement = new StringBuffer(exp.length - secondOffset);
    for(index = secondOffset + 1; index < exp.length; index++) {
      if(exp[index] == '\\') {
        backslash = !backslash;

        // 05/05/99 dfs
        // We unbackslash backslashed delimiters in the replacement string
        // only if we're on an odd backslash and there is another occurrence
        // of a delimiter later in the string.
        if(backslash && index + 1 < exp.length && exp[index + 1] == delimiter
          && expression.lastIndexOf(delimiter, exp.length - 1) != (index + 1))
        {
          finalDelimiter = false;
          continue;
        }
      } else if(exp[index] == delimiter && finalDelimiter) {
        thirdOffset = index;
        break;
      } else {
        backslash      = false;
        finalDelimiter = true;
      }

      replacement.append(exp[index]);
    }

    if(thirdOffset == -1)
      throw new
        MalformedPerl5PatternException("Invalid expression: " + expression);

    compileOptions    = Perl5Compiler.DEFAULT_MASK;
    numSubstitutions  = 1;

    // Single quotes cause no interpolations to be performed in replacement
    if(delimiter != '\'')
      numInterpolations = Perl5Substitution.INTERPOLATE_ALL;
    else
      numInterpolations = Perl5Substitution.INTERPOLATE_NONE;

    // Parse options
    for(index = thirdOffset + 1; index < exp.length; index++) {
      switch(exp[index]) {
      case 'i' :
        compileOptions |= Perl5Compiler.CASE_INSENSITIVE_MASK;
        break;
      case 'm' : compileOptions |= Perl5Compiler.MULTILINE_MASK; break;
      case 's' : compileOptions |= Perl5Compiler.SINGLELINE_MASK; break;
      case 'x' : compileOptions |= Perl5Compiler.EXTENDED_MASK; break;
      case 'g' : numSubstitutions = Util.SUBSTITUTE_ALL; break;
      case 'o' : numInterpolations = 1; break;
      default  :
        throw new
          MalformedPerl5PatternException("Invalid option: " + exp[index]);
      }
    }

    compiledPattern =
      __patternCache.getPattern(new String(exp, firstOffset,
                                           secondOffset - firstOffset),
                                compileOptions);
    substitution =
      new Perl5Substitution(replacement.toString(), numInterpolations);
    entry = new ParsedSubstitutionEntry(compiledPattern, substitution,
                                        numSubstitutions);
    __expressionCache.addElement(expression, entry);

    subCount =
      Util.substitute(result, __matcher, compiledPattern, substitution,
                      input, numSubstitutions);

    __getState().lastMatch = __matcher.getMatch();

    return subCount;
  }

  /**
   * Substitutes a pattern in a given input with a replacement string.
   * The substitution expression is specified in Perl5 native format.
   * <dl compact>
   *   <dt>Calling this method is the same as:</dt>
   *   <dd>
   *     <blockquote><pre>
   *      String result;
   *      StringBuffer buffer = new StringBuffer();
   *      perl.substitute(buffer, expression, input);
   *      result = buffer.toString();
   *     </pre></blockquote>
   *   </dd>
   * </dl>
   * @param expression The Perl5 substitution regular expression.
   * @param input      The input on which to perform substitutions.
   * @return  The input as a String after substitutions have been performed.
   * @exception MalformedPerl5PatternException  If there is an error in
   *            the expression.  You are not forced to catch this exception
   *            because it is derived from RuntimeException.
   * @since 1.0
   * @see #substitute
   */
  public synchronized String substitute(String expression, String input)
    throws MalformedPerl5PatternException
  {
    StringBuffer result = new StringBuffer();
    substitute(result, expression, input);
    return result.toString();
  }
 
  /**
   * Splits a String into strings that are appended to a List, but no more
   * than a specified limit.  The String is split using a regular expression
   * as the delimiter.  The regular expression is a pattern specified
   * in Perl5 native format:
   * <blockquote><pre>
   * [m]/pattern/[i][m][s][x]
   * </pre></blockquote>
   * The <code>m</code> prefix is optional and the meaning of the optional
   * trailing options are:
   * <dl compact> 
   * <dt> i <dd> case insensitive match
   * <dt> m <dd> treat the input as consisting of multiple lines
   * <dt> s <dd> treat the input as consisting of a single line
   * <dt> x <dd> enable extended expression syntax incorporating whitespace
   *             and comments
   * </dl>
   * As with Perl, any non-alphanumeric character can be used in lieu of
   * the slashes.
   * <p>
   * The limit parameter causes the string to be split on at most the first
   * <b>limit - 1</b> number of pattern occurences.
   * <p>
   * Of special note is that this split method performs EXACTLY the same
   * as the Perl split() function.  In other words, if the split pattern
   * contains parentheses, additional Vector elements are created from
   * each of the matching subgroups in the pattern.  Using an example
   * similar to the one from the Camel book:
   * <blockquote><pre>
   * split(list, "/([,-])/", "8-12,15,18")
   * </pre></blockquote>
   * produces the Vector containing:
   * <blockquote><pre>
   * { "8", "-", "12", ",", "15", ",", "18" }
   * </pre></blockquote>
   * Furthermore, the following Perl behavior is observed: "leading empty
   * fields are preserved, and empty trailing one are deleted."  This
   * has the effect that a split on a zero length string returns an empty
   * list.
   * The {@link org.apache.oro.text.regex.Util#split Util.split()} method
   * does NOT implement these behaviors because it is intended to
   * be a general self-consistent and predictable split function usable
   * with Pattern instances other than Perl5Pattern.
   * <p>
   * @param results 
   *    A <code> Collection </code> to which the substrings of the input
   *    that occur between the regular expression delimiter occurences
   *    are appended. The input will not be split into any more substrings
   *    than the specified 
   *    limit. A way of thinking of this is that only the first
   *    <b>limit - 1</b>
   *    matches of the delimiting regular expression will be used to split the
   *    input.  The Collection must support the
   *    <code>addAll(Collection)</code> operation.
   * @param pattern The regular expression to use as a split delimiter.
   * @param input The String to split.
   * @param limit The limit on the size of the returned <code>Vector</code>.
   *   Values <= 0 produce the same behavior as the SPLIT_ALL constant which
   *   causes the limit to be ignored and splits to be performed on all
   *   occurrences of the pattern.  You should use the SPLIT_ALL constant
   *   to achieve this behavior instead of relying on the default behavior
   *   associated with non-positive limit values.
   * @exception MalformedPerl5PatternException  If there is an error in
   *            the expression.  You are not forced to catch this exception
   *            because it is derived from RuntimeException.
   */
  public synchronized void split(Collection results, String pattern,
                                 String input, int limit)
       throws MalformedPerl5PatternException 
  {
    int beginOffset, groups, index;
    String group;
    MatchResult currentResult = null;
    PatternMatcherInput pinput;
    Pattern compiledPattern;

    compiledPattern = __parseMatchExpression(pattern);

    pinput = new PatternMatcherInput(input);
    beginOffset = 0;

    while(--limit != 0 && __matcher.contains(pinput, compiledPattern)) {
      currentResult = __matcher.getMatch();

      __splitList.add(input.substring(beginOffset,
                                      currentResult.beginOffset(0)));

      if((groups = currentResult.groups()) > 1) {
        for(index = 1; index < groups; ++index) {
          group = currentResult.group(index);
          if(group != null && group.length() > 0)
            __splitList.add(group);
        }
      }

      beginOffset = currentResult.endOffset(0);
    }

    __splitList.add(input.substring(beginOffset, input.length()));

    // Remove all trailing empty fields.
    for(int i = __splitList.size() - 1; i >= 0; --i) {
      String str;

      str = (String)__splitList.get(i);
      if(str.length() == 0)
        __splitList.remove(i);
      else
        break;
    }

    results.addAll(__splitList);
    __splitList.clear();

    // Just for the sake of completeness
    __getState().lastMatch = currentResult;
  }

  /**
   * This method is identical to calling:
   * <blockquote><pre>
   * split(results, pattern, input, SPLIT_ALL);
   * </pre></blockquote>
   */
  public synchronized void split(Collection results, String pattern,
                                 String input)
       throws MalformedPerl5PatternException 
  {
    split(results, pattern, input, SPLIT_ALL);
  }

  /**
   * Splits input in the default Perl manner, splitting on all whitespace.
   * This method is identical to calling:
   * <blockquote><pre>
   * split(results, "/\\s+/", input);
   * </pre></blockquote>
   */
  public synchronized void split(Collection results, String input)
       throws MalformedPerl5PatternException
  {
    split(results, "/\\s+/", input);
  }

  /**
   * Splits a String into strings contained in a Vector of size no greater
   * than a specified limit.  The String is split using a regular expression
   * as the delimiter.  The regular expression is a pattern specified
   * in Perl5 native format:
   * <blockquote><pre>
   * [m]/pattern/[i][m][s][x]
   * </pre></blockquote>
   * The <code>m</code> prefix is optional and the meaning of the optional
   * trailing options are:
   * <dl compact> 
   * <dt> i <dd> case insensitive match
   * <dt> m <dd> treat the input as consisting of multiple lines
   * <dt> s <dd> treat the input as consisting of a single line
   * <dt> x <dd> enable extended expression syntax incorporating whitespace
   *             and comments
   * </dl>
   * As with Perl, any non-alphanumeric character can be used in lieu of
   * the slashes.
   * <p>
   * The limit parameter causes the string to be split on at most the first
   * <b>limit - 1</b> number of pattern occurences.
   * <p>
   * Of special note is that this split method performs EXACTLY the same
   * as the Perl split() function.  In other words, if the split pattern
   * contains parentheses, additional Vector elements are created from
   * each of the matching subgroups in the pattern.  Using an example
   * similar to the one from the Camel book:
   * <blockquote><pre>
   * split("/([,-])/", "8-12,15,18")
   * </pre></blockquote>
   * produces the Vector containing:
   * <blockquote><pre>
   * { "8", "-", "12", ",", "15", ",", "18" }
   * </pre></blockquote>
   * The {@link org.apache.oro.text.regex.Util#split Util.split()} method
   * does NOT implement this particular behavior because it is intended to
   * be usable with Pattern instances other than Perl5Pattern.
   * <p>
   * @deprecated Use
   * {@link #split(Collection results, String pattern, String input, int limit)}
   *  instead.
   * @param pattern The regular expression to use as a split delimiter.
   * @param input The String to split.
   * @param limit The limit on the size of the returned <code>Vector</code>.
   *   Values <= 0 produce the same behavior as the SPLIT_ALL constant which
   *   causes the limit to be ignored and splits to be performed on all
   *   occurrences of the pattern.  You should use the SPLIT_ALL constant
   *   to achieve this behavior instead of relying on the default behavior
   *   associated with non-positive limit values.
   * @return A <code> Vector </code> containing the substrings of the input
   *    that occur between the regular expression delimiter occurences. The
   *    input will not be split into any more substrings than the specified 
   *    limit. A way of thinking of this is that only the first
   *    <b>limit - 1</b>
   *    matches of the delimiting regular expression will be used to split the
   *    input. 
   * @exception MalformedPerl5PatternException  If there is an error in
   *            the expression.  You are not forced to catch this exception
   *            because it is derived from RuntimeException.
   */
  public synchronized ArrayList split(String pattern, String input, int limit)
       throws MalformedPerl5PatternException 
  {
        ArrayList results = new ArrayList(20);
    split(results, pattern, input, limit);
    return results;
  }

  /**
   * This method is identical to calling:
   * <blockquote><pre>
   * split(pattern, input, SPLIT_ALL);
   * </pre></blockquote>
   * @deprecated Use
   * {@link #split(Collection results, String pattern, String input)} instead.
   */
  public synchronized ArrayList split(String pattern, String input)
       throws MalformedPerl5PatternException 
  {
    return split(pattern, input, SPLIT_ALL);
  }

  /**
   * Splits input in the default Perl manner, splitting on all whitespace.
   * This method is identical to calling:
   * <blockquote><pre>
   * split("/\\s+/", input);
   * </pre></blockquote>
   * @deprecated Use
   * {@link #split(Collection results, String input)} instead.
   */
  public synchronized ArrayList split(String input)
       throws MalformedPerl5PatternException 
  {
    return split("/\\s+/", input);
  }

  //
  // MatchResult interface methods.
  //

  /**
   * Returns the length of the last match found.
   * <p>
   * @return The length of the last match found.
   */
  public synchronized int length() {
    return getMatch().length();
  }

  /**
   * @return The number of groups contained in the last match found.
   *         This number includes the 0th group.  In other words, the
   *         result refers to the number of parenthesized subgroups plus
   *         the entire match itself.          
   */
  public synchronized int groups() {
    return getMatch().groups();
  }


  /**
   * Returns the contents of the parenthesized subgroups of the last match
   * found according to the behavior dictated by the MatchResult interface.
   * <p>
   * @param group The pattern subgroup to return.
   * @return A string containing the indicated pattern subgroup.  Group
   *         0 always refers to the entire match.  If a group was never
   *         matched, it returns null.  This is not to be confused with
   *         a group matching the null string, which will return a String
   *         of length 0.
   */                       
  public synchronized String group(int group) {
    return getMatch().group(group);
  }

  /**
   * Returns the begin offset of the subgroup of the last match found 
   * relative the beginning of the match.
   * <p>
   * @param group The pattern subgroup.
   * @return The offset into group 0 of the first token in the indicated
   *         pattern subgroup.  If a group was never matched or does
   *         not exist, returns -1.  Be aware that a group that matches
   *         the null string at the end of a match will have an offset
   *         equal to the length of the string, so you shouldn't blindly
   *         use the offset to index an array or String.
   */                                                                 
  public synchronized int begin(int group) {
    return getMatch().begin(group);
  }


  /**
   * Returns the end offset of the subgroup of the last match found 
   * relative the beginning of the match.
   * <p>
   * @param group The pattern subgroup.
   * @return Returns one plus the offset into group 0 of the last token in
   *         the indicated pattern subgroup.  If a group was never matched
   *         or does not exist, returns -1.  A group matching the null
   *         string will return its start offset.
   */
  public synchronized int end(int group) {
    return getMatch().end(group);
  }


  /**
   * Returns an offset marking the beginning of the last pattern match
   * found relative to the beginning of the input from which the match
   * was extracted.
   * <p>
   * @param group The pattern subgroup.
   * @return The offset of the first token in the indicated
   *         pattern subgroup.  If a group was never matched or does
   *         not exist, returns -1.          
   */
  public synchronized int beginOffset(int group) {
    return getMatch().beginOffset(group);
  }

  /**
   * Returns an offset marking the end of the last pattern match found
   * relative to the beginning of the input from which the match was
   * extracted.
   * <p>
   * @param group The pattern subgroup.
   * @return Returns one plus the offset of the last token in
   *         the indicated pattern subgroup.  If a group was never matched
   *         or does not exist, returns -1.  A group matching the null
   *         string will return its start offset.
   */                   
  public synchronized int endOffset(int group) {
    return getMatch().endOffset(group);
  }

  /**
   * Returns the same as group(0).
   * <p>
   * @return A string containing the entire match.
   */  
  public synchronized String toString() {
    MatchResult match = getMatch();
    if(match == null)
      return null;
    return match.toString();
  }


  /**
   * Returns the part of the input preceding the last match found.
   * <p>
   * @return The part of the input following the last match found.
   */
  public synchronized String preMatch() {
    ThreadState state = __getState();

    if(state.originalInput == null)
      return __nullString;

    int begin = beginOffset(0);

    if(begin <= 0)
      return __nullString;

    if(state.originalInput instanceof char[]) {
      char[] input;

      input = (char[])state.originalInput;

      // Just in case we make sure begin offset is in bounds.  It should
      // be but we're paranoid.
      if(begin > input.length)
        begin = input.length;

      return new String(input, state.inputBeginOffset, begin);
    } else if(state.originalInput instanceof String) {
      String input;

      input = (String)state.originalInput;

      // Just in case we make sure begin offset is in bounds.  It should
      // be but we're paranoid.
      if(begin > input.length())
        begin = input.length();

      return input.substring(state.inputBeginOffset, begin);
    }

    return __nullString;
  }


  /**
   * Returns the part of the input following the last match found.
   * <p>
   * @return The part of the input following the last match found.
   */
  public synchronized String postMatch() {
    ThreadState state = __getState();

    if(state.originalInput == null)
      return __nullString;

    int end = endOffset(0);

    if(end < 0)
      return __nullString;

    if(state.originalInput instanceof char[]) {
      char[] input;

      input = (char[])state.originalInput;
      // Just in case we make sure begin offset is in bounds.  It should
      // be but we're paranoid.
      if(end >= input.length)
        return __nullString;

      return new String(input, end, state.inputEndOffset - end);
    } else if(state.originalInput instanceof String) {
      String input;

      input = (String)state.originalInput;

      // Just in case we make sure begin offset is in bounds.  It should
      // be but we're paranoid.
      if(end >= input.length())
        return __nullString;

      return input.substring(end, state.inputEndOffset);
    }

    return __nullString;
  }


  /**
   * Returns the part of the input preceding the last match found as a
   * char array.  This method eliminates the extra
   * buffer copying caused by preMatch().toCharArray().
   * <p>
   * @return The part of the input preceding the last match found as a char[].
   *         If the result is of zero length, returns null instead of a zero
   *         length array.
   */
  public synchronized char[] preMatchCharArray() {
    ThreadState state = __getState();
    int begin;
    char[] result = null;

    if(state.originalInput == null)
      return null;

    begin = beginOffset(0);

    if(begin <= 0)
      return null;

    if(state.originalInput instanceof char[]) {
      char[] input;

      input = (char[])state.originalInput;

      // Just in case we make sure begin offset is in bounds.  It should
      // be but we're paranoid.
      if(begin >= input.length)
        begin = input.length;

      result = new char[begin - state.inputBeginOffset];
      System.arraycopy(input, state.inputBeginOffset, result, 0,
                       result.length);
    } else if(state.originalInput instanceof String) {
      String input;

      input = (String)state.originalInput;

      // Just in case we make sure begin offset is in bounds.  It should
      // be but we're paranoid.
      if(begin >= input.length())
        begin = input.length();

      result = new char[begin - state.inputBeginOffset];
      input.getChars(state.inputBeginOffset, begin, result, 0);
    }

    return result;
  }


  /**
   * Returns the part of the input following the last match found as a char
   * array.  This method eliminates the extra buffer copying caused by
   * preMatch().toCharArray().
   * <p>
   * @return The part of the input following the last match found as a char[].
   *         If the result is of zero length, returns null instead of a zero
   *         length array.
   */
  public synchronized char[] postMatchCharArray() {
    ThreadState state = __getState();
    int end;
    char[] result = null;

    if(state.originalInput == null)
      return null;

    end = endOffset(0);

    if(end < 0)
      return null;

    if(state.originalInput instanceof char[]) {
      int length;
      char[] input;

      input = (char[])state.originalInput;
      // Just in case we make sure begin offset is in bounds.  It should
      // be but we're paranoid.
      if(end >= input.length)
        return null;

      length = state.inputEndOffset - end;
      result = new char[length];
      System.arraycopy(input, end, result, 0, length);
    } else if(state.originalInput instanceof String) {
      String input;

      input = (String)state.originalInput;

      // Just in case we make sure begin offset is in bounds.  It should
      // be but we're paranoid.
      if(end >= state.inputEndOffset)
        return null;

      result = new char[state.inputEndOffset - end];
      input.getChars(end, state.inputEndOffset, result, 0);
    }

    return result;
  }

}

