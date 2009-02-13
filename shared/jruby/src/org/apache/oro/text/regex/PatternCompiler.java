/* 
 * $Id: PatternCompiler.java 124053 2005-01-04 01:24:35Z dfs $
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
 * The PatternCompiler interface defines the operations a regular
 * expression compiler must implement.  However, the types of
 * regular expressions recognized by a compiler and the Pattern 
 * implementations produced as a result of compilation are not
 * restricted.
 * <p>
 * A PatternCompiler instance is used to compile the string representation
 * (either as a String or char[]) of a regular expression into a Pattern
 * instance.  The Pattern can then be used in conjunction with the appropriate
 * PatternMatcher instance to perform pattern searches.  A form
 * of use might be:
 * <p>
 * <blockquote><pre>
 * PatternCompiler compiler;
 * PatternMatcher matcher;
 * Pattern pattern;
 * String input;
 *
 * // Initialization of compiler, matcher, and input omitted;
 *
 * try {
 *   pattern = compiler.compile("\\d+");
 * } catch(MalformedPatternException e) {
 *   System.out.println("Bad pattern.");
 *   System.out.println(e.getMessage());
 *   System.exit(1);
 * }
 * 
 *
 * if(matcher.matches(input, pattern))
 *    System.out.println(input + " is a number");
 * else
 *    System.out.println(input + " is not a number");
 *
 * </pre></blockquote>
 * <p>
 * Specific PatternCompiler implementations such as Perl5Compiler may have
 * variations of the compile() methods that take extra options affecting
 * the compilation of a pattern.  However, the PatternCompiler method
 * implementations should provide the default behavior of the class.
 * 
 * @version @version@
 * @since 1.0
 * @see Pattern
 * @see PatternMatcher
 * @see MalformedPatternException
 */
public interface PatternCompiler {
  /**
   * Compiles a regular expression into a data structure that can be used
   * by a PatternMatcher implementation to perform pattern matching.
   * <p>
   * @param pattern  A regular expression to compile.
   * @return A Pattern instance constituting the compiled regular expression.
   * @exception MalformedPatternException  If the compiled expression
   *  does not conform to the grammar understood by the PatternCompiler or
   *  if some other error in the expression is encountered.
   */
  public Pattern compile(String pattern) throws MalformedPatternException;


  /**
   * Compiles a regular expression into a data structure that can be
   * used by a PatternMatcher implementation to perform pattern matching.
   * Additional regular expression syntax specific options can be passed
   * as a bitmask of options.
   * <p>
   * @param pattern  A regular expression to compile.
   * @param options  A set of flags giving the compiler instructions on
   *                 how to treat the regular expression.  The flags
   *                 are a logical OR of any number of the allowable
   *                 constants permitted by the PatternCompiler
   *                 implementation.
   * @return A Pattern instance constituting the compiled regular expression.
   * @exception MalformedPatternException  If the compiled expression
   *  does not conform to the grammar understood by the PatternCompiler or
   *  if some other error in the expression is encountered.
   */
  public Pattern compile(String pattern, int options)
       throws MalformedPatternException;


  /**
   * Compiles a regular expression into a data structure that can be used
   * by a PatternMatcher implementation to perform pattern matching.
   * <p>
   * @param pattern  A regular expression to compile.
   * @return A Pattern instance constituting the compiled regular expression.
   * @exception MalformedPatternException  If the compiled expression
   *  does not conform to the grammar understood by the PatternCompiler or
   *  if some other error in the expression is encountered.
   */
  public Pattern compile(char[] pattern) throws MalformedPatternException;


  /**
   * Compiles a regular expression into a data structure that can be
   * used by a PatternMatcher implementation to perform pattern matching.
   * Additional regular expression syntax specific options can be passed
   * as a bitmask of options.
   * <p>
   * @param pattern  A regular expression to compile.
   * @param options  A set of flags giving the compiler instructions on
   *                 how to treat the regular expression.  The flags
   *                 are a logical OR of any number of the allowable
   *                 constants permitted by the PatternCompiler
   *                 implementation.
   * @return A Pattern instance constituting the compiled regular expression.
   * @exception MalformedPatternException  If the compiled expression
   *  does not conform to the grammar understood by the PatternCompiler or
   *  if some other error in the expression is encountered.
   */
  public Pattern compile(char[] pattern, int options)
       throws MalformedPatternException;

}
