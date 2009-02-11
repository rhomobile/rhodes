/* 
 * $Id: PatternMatchingEngineFactory.java 124053 2005-01-04 01:24:35Z dfs $
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

import j2me.util.*;

import org.apache.oro.text.regex.*;
//import org.apache.oro.text.awk.*;

/**
 * 
 * @version @version@
 * @since 2.1
 */
public class PatternMatchingEngineFactory {

  public static final String PERL5_KEY   = "org.apache.oro.text.regex";
  public static final String AWK_KEY     = "org.apache.oro.text.awk";
  public static final String GLOB_KEY    = "org.apache.oro.text";
  public static final String JAVA_KEY    = "java.util.regex";
  public static final String POSIX_KEY   = "org.apache.regexp";
  public static final String DEFAULT_KEY = PERL5_KEY;

  private static final int __KEY    = 0;
  private static final int __CLASS  = 1;
  private static final int __ENGINE = 2;

  private static final String[] __DEFAULT_ENGINES[] = {
    { PERL5_KEY, "org.apache.oro.text.regex.Perl5Matcher",
      "org.apache.oro.text.regex.Perl5Engine" },
    { AWK_KEY, "org.apache.oro.text.awk.AwkMatcher",
      "org.apache.oro.text.awk.AwkEngine" },
    // Yes, we really do want to check for Perl5Matcher because it's
    // required by GlobEngine.
    { GLOB_KEY, "org.apache.oro.text.Perl5Matcher",
      "org.apache.oro.text.GlobEngine" },
    { JAVA_KEY, "java.util.regex.Pattern",
      "org.apache.oro.text.java.JavaEngine" }
  };

  private HashMap __factories;

  public PatternMatchingEngineFactory() {
    __factories = new HashMap();

    for(int i = 0; i < __DEFAULT_ENGINES.length; ++i) {
      try {
        Class.forName(__DEFAULT_ENGINES[i][__CLASS]);
        Class.forName(__DEFAULT_ENGINES[i][__ENGINE]);
        put(__DEFAULT_ENGINES[i][__KEY],
            (PatternMatchingEngine)
            Class.forName(__DEFAULT_ENGINES[i][__ENGINE]).newInstance());
      } catch(ClassNotFoundException cnfe) {
        // Don't do anything.  Package doesn't exist.
      } catch(InstantiationException ie) {
        // This will never happen.  Could use an assertion but that locks us
        // into J2SE 1.4.  We print the stack trace in case a developer made
        // a mistake in one of the class names.
        ie.printStackTrace();
      } catch(IllegalAccessException iae) {
        // This will never happen.  Could use an assertion but that locks us
        // into J2SE 1.4.  Again, we print a stack trace in case of developer
        // error.
        iae.printStackTrace();
      }
    }
  }

  public PatternMatchingEngine get(String key) {
    return (PatternMatchingEngine)__factories.get(key);
  }

  public PatternMatchingEngine put(String key,
                                   PatternMatchingEngine engine)
  {
    return (PatternMatchingEngine)__factories.put(key, engine);
  }

  public PatternMatchingEngine remove(String key) {
    return (PatternMatchingEngine)__factories.remove(key);
  }

  public boolean containsKey(String key) {
    return __factories.containsKey(key);
  }
}
