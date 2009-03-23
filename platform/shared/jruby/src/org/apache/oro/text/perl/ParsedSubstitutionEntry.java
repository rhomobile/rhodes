/* 
 * $Id: ParsedSubstitutionEntry.java 124053 2005-01-04 01:24:35Z dfs $
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

import org.apache.oro.text.regex.*;

/**
 * @version @version@
 * @since 1.0
 */
final class ParsedSubstitutionEntry {
  int _numSubstitutions;
  Pattern _pattern;
  Perl5Substitution _substitution;

  ParsedSubstitutionEntry(Pattern pattern, Perl5Substitution substitution,
			  int numSubstitutions)
  {
    _numSubstitutions  = numSubstitutions;
    _substitution      = substitution;
    _pattern           = pattern;
  }
}

