/* 
 * $Id: Perl5Repetition.java 124053 2005-01-04 01:24:35Z dfs $
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
 * Perl5Repetition is a support class for Perl5Matcher.  It was originally
 * defined as a top-level class rather than as an inner class to allow
 * compilation for JDK 1.0.2.
 *
 * @version @version@
 * @since 1.0
 * @see Perl5Matcher
 */
final class Perl5Repetition {
  int _parenFloor;
  int _numInstances, _min, _max;
  boolean _minMod;
  int _scan;
  int _next;
  int _lastLocation;
  Perl5Repetition _lastRepetition;
}
