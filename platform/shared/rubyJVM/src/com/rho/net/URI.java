package com.rho.net;

import com.rho.Tokenizer;

 public class URI {

  public static class MalformedURIException extends RuntimeException {

   
    public MalformedURIException(String p_msg) {
      super(p_msg);
    }

	public String getMessage() {	
		return "MalformedURIException: " + super.getMessage();
	}
  }

  
  private static final byte [] fgLookupTable = new byte[128];
  
  /** reserved characters ;/?:@&=+$,[] */
  //RFC 2732 added '[' and ']' as reserved characters
  private static final int RESERVED_CHARACTERS = 0x01;
  
  /** URI punctuation mark characters: -_.!~*'() - these, combined with
      alphanumerics, constitute the "unreserved" characters */
  private static final int MARK_CHARACTERS = 0x02;
  
  /** scheme can be composed of alphanumerics and these characters: +-. */
  private static final int SCHEME_CHARACTERS = 0x04;
  
  /** userinfo can be composed of unreserved, escaped and these
      characters: ;:&=+$, */
//  private static final int USERINFO_CHARACTERS = 0x08;
  
  /** ASCII letter characters */
  private static final int ASCII_ALPHA_CHARACTERS = 0x10;
  
  /** ASCII digit characters */
  private static final int ASCII_DIGIT_CHARACTERS = 0x20;
  
  /** ASCII hex characters */
  private static final int ASCII_HEX_CHARACTERS = 0x40;
  
  /** Path characters */
  private static final int PATH_CHARACTERS = 0x80;

  /** Mask for alpha-numeric characters */
  private static final int MASK_ALPHA_NUMERIC = ASCII_ALPHA_CHARACTERS | ASCII_DIGIT_CHARACTERS;
  
  /** Mask for unreserved characters */
  private static final int MASK_UNRESERVED_MASK = MASK_ALPHA_NUMERIC | MARK_CHARACTERS;
  
  /** Mask for URI allowable characters except for % */
  private static final int MASK_URI_CHARACTER = MASK_UNRESERVED_MASK | RESERVED_CHARACTERS;
  
  /** Mask for scheme characters */
  private static final int MASK_SCHEME_CHARACTER = MASK_ALPHA_NUMERIC | SCHEME_CHARACTERS;
  
  /** Mask for userinfo characters */
//  private static final int MASK_USERINFO_CHARACTER = MASK_UNRESERVED_MASK | USERINFO_CHARACTERS;
  
  /** Mask for path characters */
  private static final int MASK_PATH_CHARACTER = MASK_UNRESERVED_MASK | PATH_CHARACTERS; 

  static {
      // Add ASCII Digits and ASCII Hex Numbers
      for (int i = '0'; i <= '9'; ++i) {
          fgLookupTable[i] |= ASCII_DIGIT_CHARACTERS | ASCII_HEX_CHARACTERS;
      }

      // Add ASCII Letters and ASCII Hex Numbers
      for (int i = 'A'; i <= 'F'; ++i) {
          fgLookupTable[i] |= ASCII_ALPHA_CHARACTERS | ASCII_HEX_CHARACTERS;
          fgLookupTable[i+0x00000020] |= ASCII_ALPHA_CHARACTERS | ASCII_HEX_CHARACTERS;
      }

      // Add ASCII Letters
      for (int i = 'G'; i <= 'Z'; ++i) {
          fgLookupTable[i] |= ASCII_ALPHA_CHARACTERS;
          fgLookupTable[i+0x00000020] |= ASCII_ALPHA_CHARACTERS;
      }

      // Add Reserved Characters
      fgLookupTable[';'] |= RESERVED_CHARACTERS|PATH_CHARACTERS;
      fgLookupTable['/'] |= RESERVED_CHARACTERS|PATH_CHARACTERS;
      fgLookupTable['?'] |= RESERVED_CHARACTERS;
      fgLookupTable[':'] |= RESERVED_CHARACTERS|PATH_CHARACTERS;
      fgLookupTable['@'] |= RESERVED_CHARACTERS|PATH_CHARACTERS;
      fgLookupTable['&'] |= RESERVED_CHARACTERS|PATH_CHARACTERS;
      fgLookupTable['='] |= RESERVED_CHARACTERS|PATH_CHARACTERS;
      fgLookupTable['+'] |= RESERVED_CHARACTERS|PATH_CHARACTERS|SCHEME_CHARACTERS;
      fgLookupTable['$'] |= RESERVED_CHARACTERS|PATH_CHARACTERS;
      fgLookupTable[','] |= RESERVED_CHARACTERS|PATH_CHARACTERS;
      fgLookupTable['['] |= RESERVED_CHARACTERS;
      fgLookupTable[']'] |= RESERVED_CHARACTERS;

      // Add Mark Characters
      fgLookupTable['-'] |= MARK_CHARACTERS|SCHEME_CHARACTERS;
      fgLookupTable['_'] |= MARK_CHARACTERS;
      fgLookupTable['.'] |= MARK_CHARACTERS|SCHEME_CHARACTERS;
      fgLookupTable['!'] |= MARK_CHARACTERS;
      fgLookupTable['~'] |= MARK_CHARACTERS;
      fgLookupTable['*'] |= MARK_CHARACTERS;
      fgLookupTable['\''] |= MARK_CHARACTERS;
      fgLookupTable['('] |= MARK_CHARACTERS;
      fgLookupTable[')'] |= MARK_CHARACTERS;

      fgLookupTable['{'] |= MARK_CHARACTERS;
      fgLookupTable['}'] |= MARK_CHARACTERS;
      
      fgLookupTable['|'] |= MARK_CHARACTERS;
      
      // Add Scheme Characters
//      fgLookupTable['+'] |= SCHEME_CHARACTERS;
//      fgLookupTable['-'] |= SCHEME_CHARACTERS;
//      fgLookupTable['.'] |= SCHEME_CHARACTERS;

      // Add Userinfo Characters
//      fgLookupTable[';'] |= USERINFO_CHARACTERS;
//      fgLookupTable[':'] |= USERINFO_CHARACTERS;
//      fgLookupTable['&'] |= USERINFO_CHARACTERS;
//      fgLookupTable['='] |= USERINFO_CHARACTERS;
//      fgLookupTable['+'] |= USERINFO_CHARACTERS;
//      fgLookupTable['$'] |= USERINFO_CHARACTERS;
//      fgLookupTable[','] |= USERINFO_CHARACTERS;
      
      // Add Path Characters
//      fgLookupTable[';'] |= PATH_CHARACTERS;
//      fgLookupTable['/'] |= PATH_CHARACTERS;
//      fgLookupTable[':'] |= PATH_CHARACTERS;
//      fgLookupTable['@'] |= PATH_CHARACTERS;
//      fgLookupTable['&'] |= PATH_CHARACTERS;
//      fgLookupTable['='] |= PATH_CHARACTERS;
//      fgLookupTable['+'] |= PATH_CHARACTERS;
//      fgLookupTable['$'] |= PATH_CHARACTERS;
//      fgLookupTable[','] |= PATH_CHARACTERS;
  }

  /** Stores the scheme (usually the protocol) for this URI. */
  private String m_scheme = null;

  /** If specified, stores the userinfo for this URI; otherwise null */
  private String m_userinfo = null;

  /** If specified, stores the host for this URI; otherwise null */
  private String m_host = null;

  /** If specified, stores the port for this URI; otherwise -1 */
  private int m_port = -1;
  
  /** If specified, stores the registry based authority for this URI; otherwise -1 */
  private String m_regAuthority = null;

  /** If specified, stores the path for this URI; otherwise null */
  private String m_path = null;

  /** If specified, stores the query string for this URI; otherwise
      null.  */
  private String m_queryString = null;

  /** If specified, stores the fragment for this URI; otherwise null */
  private String m_fragment = null;

  //
  private int mHashCode = -1;
  
  /**
  * Construct a new and uninitialized URI.
  */
//  public URI() {
//  }

 /**
  * Construct a new URI from another URI. All fields for this URI are
  * set equal to the fields of the URI passed in.
  *
  * @param p_other the URI to copy (cannot be null)
  */
//  public URI(URI p_other) {
//    initialize(p_other);
//  }

  public URI(String p_uriSpec) throws MalformedURIException {
    this((URI)null, p_uriSpec);
  }
  
  public URI(URI p_base, String p_uriSpec) throws MalformedURIException {
    initialize(p_base, p_uriSpec!=null?p_uriSpec.trim():null);
    mHashCode=-1;
  }

  private void initialize(URI p_other) {
    m_scheme = p_other.getScheme();
    m_userinfo = p_other.getUserinfo();
    m_host = p_other.getHost();
    m_port = p_other.getPort();
    m_regAuthority = p_other.getRegBasedAuthority();
    m_path = p_other.getPath();
    m_queryString = p_other.getQueryString();
    m_fragment = p_other.getFragment();
  }
  
  private int calcHashCode() {
	  int hash = 0;
	  if (m_scheme!=null) {
		  hash |= m_scheme.hashCode();
	  }
	  if (m_userinfo!=null) {
		  hash ^= m_userinfo.hashCode();
	  }
	  if (m_host!=null) {
		  hash ^= m_host.hashCode();
	  }
//	  if (m_fragment!=null) {
//		  hash ^= m_fragment.hashCode();
//	  }
	  hash ^= m_port;
	  if (m_regAuthority!=null) {
		  hash ^= m_regAuthority.hashCode();
	  }
	  if (m_path!=null) {
		  hash ^= m_path.hashCode();
	  }
	  if (m_queryString!=null) {
		  hash ^= m_queryString.hashCode();
	  }
//	  System.out.println("("+hash+") :"+this.toString());
	  return hash;
  }
  
  public int hashCode() {
	  if (mHashCode==-1)
		  mHashCode = calcHashCode();
	  return mHashCode;	  
  }
  
  private void initialize(URI p_base, String p_uriSpec)
                         throws MalformedURIException {
	  
    String uriSpec = p_uriSpec;
    int uriSpecLen = (uriSpec != null) ? uriSpec.length() : 0;
	
    if (p_base == null && uriSpecLen == 0) {
      throw new MalformedURIException("Url: " + p_uriSpec + 
                   "; Cannot initialize URI with empty parameters.");
    }

    // just make a copy of the base if spec is empty
    if (uriSpecLen == 0) {
      initialize(p_base);
      return;
    }

    int index = 0;

    // Check for scheme, which must be before '/', '?' or '#'.
    int colonIdx = uriSpec.indexOf(':');
    if (colonIdx != -1) {
        final int searchFrom = colonIdx - 1;
        // search backwards starting from character before ':'.
        int slashIdx = uriSpec.lastIndexOf('/', searchFrom);
        int queryIdx = uriSpec.lastIndexOf('?', searchFrom);
        int fragmentIdx = uriSpec.lastIndexOf('#', searchFrom);
       
        if (colonIdx == 0 || slashIdx != -1 || 
            queryIdx != -1 || fragmentIdx != -1) {
            // A standalone base is a valid URI according to spec
            if (colonIdx == 0 || (p_base == null && fragmentIdx != 0)) {
                throw new MalformedURIException("Url: " + p_uriSpec + "; No scheme found in URI.");
            }
        }
        else {
            initializeScheme(uriSpec);
            index = m_scheme.length()+1;
            
            // Neither 'scheme:' or 'scheme:#fragment' are valid URIs.
            if (colonIdx == uriSpecLen - 1 || uriSpec.charAt(colonIdx+1) == '#') {
            	throw new MalformedURIException("Url: " + p_uriSpec +"; Scheme specific part cannot be empty.");	
            }
        }
    }
    
    else if (p_base == null && uriSpec.indexOf('#') != 0) {
        throw new MalformedURIException("Url: " + p_uriSpec + "; No scheme found in URI.");    
    }

    // Two slashes means we may have authority, but definitely means we're either
    // matching net_path or abs_path. These two productions are ambiguous in that
    // every net_path (except those containing an IPv6Reference) is an abs_path. 
    // RFC 2396 resolves this ambiguity by applying a greedy left most matching rule. 
    // Try matching net_path first, and if that fails we don't have authority so 
    // then attempt to match abs_path.
    //
    // net_path = "//" authority [ abs_path ]
    // abs_path = "/"  path_segments
    if (((index+1) < uriSpecLen) &&
        (uriSpec.charAt(index) == '/' && uriSpec.charAt(index+1) == '/')) {
      index += 2;
      int startPos = index;

      // Authority will be everything up to path, query or fragment
      char testChar = '\0';
      while (index < uriSpecLen) {
        testChar = uriSpec.charAt(index);
        if (testChar == '/' || testChar == '?' || testChar == '#') {
          break;
        }
        index++;
      }

      // Attempt to parse authority. If the section is an empty string
      // this is a valid server based authority, so set the host to this
      // value.
      if (index > startPos) {
        // If we didn't find authority we need to back up. Attempt to
        // match against abs_path next.
        if (!initializeAuthority(uriSpec.substring(startPos, index))) {
          index = startPos - 2;
        }
      }
      else {
        m_host = "";
      }
    }

    initializePath(uriSpec, index);

    // Resolve relative URI to base URI - see RFC 2396 Section 5.2
    // In some cases, it might make more sense to throw an exception
    // (when scheme is specified is the string spec and the base URI
    // is also specified, for example), but we're just following the
    // RFC specifications
    if (p_base != null) {
        absolutize(p_base);
    }
  }

  private void absolutize(URI p_base) {

      // check to see if this is the current doc - RFC 2396 5.2 #2
      // note that this is slightly different from the RFC spec in that
      // we don't include the check for query string being null
      // - this handles cases where the urispec is just a query
      // string or a fragment (e.g. "?y" or "#s") -
      // see <http://www.ics.uci.edu/~fielding/url/test1.html> which
      // identified this as a bug in the RFC
      if (m_path.length() == 0 && m_scheme == null &&
          m_host == null && m_regAuthority == null) {
          m_scheme = p_base.m_scheme;
          m_userinfo = p_base.m_userinfo;
          m_host = p_base.m_host;
          m_port = p_base.m_port;
          m_regAuthority = p_base.m_regAuthority;
          m_path = p_base.m_path;
          
          if (m_queryString == null) {
              m_queryString = p_base.getQueryString();
              
              if (m_fragment == null) {
                  m_fragment = p_base.getFragment();
              }
          }
          return;
      }
      
      // check for scheme - RFC 2396 5.2 #3
      // if we found a scheme, it means absolute URI, so we're done
      if (m_scheme == null) {
          m_scheme = p_base.getScheme();
      }
      else {
          return;
      }
      
      // check for authority - RFC 2396 5.2 #4
      // if we found a host, then we've got a network path, so we're done
      if (m_host == null && m_regAuthority == null) {
          m_userinfo = p_base.getUserinfo();
          m_host = p_base.getHost();
          m_port = p_base.getPort();
          m_regAuthority = p_base.getRegBasedAuthority();
      }
      else {
          return;
      }
      
      // check for absolute path - RFC 2396 5.2 #5
      if (m_path.length() > 0 &&
              m_path.startsWith("/")) {
          return;
      }
      
      // if we get to this point, we need to resolve relative path
      // RFC 2396 5.2 #6
      String path = "";
      String basePath = p_base.getPath();
      
      // 6a - get all but the last segment of the base URI path
      if (basePath != null && basePath.length() > 0) {
          int lastSlash = basePath.lastIndexOf('/');
          if (lastSlash != -1) {
              path = basePath.substring(0, lastSlash+1);
          }
      }
      else if (m_path.length() > 0) {
          path = "/";
      }
      
      // 6b - append the relative URI path
      path = path.concat(m_path);
      
      // 6c - remove all "./" where "." is a complete path segment
      int index = -1;
      while ((index = path.indexOf("/./")) != -1) {
          path = path.substring(0, index+1).concat(path.substring(index+3));
      }
      
      // 6d - remove "." if path ends with "." as a complete path segment
      if (path.endsWith("/.")) {
          path = path.substring(0, path.length()-1);
      }
      
      // 6e - remove all "<segment>/../" where "<segment>" is a complete
      // path segment not equal to ".."
      index = 1;
      int segIndex = -1;
      String tempString = null;
      
      while ((index = path.indexOf("/../", index)) > 0) {
          tempString = path.substring(0, path.indexOf("/../"));
          segIndex = tempString.lastIndexOf('/');
          if (segIndex != -1) {
              if (!tempString.substring(segIndex).equals("..")) {
                  path = path.substring(0, segIndex+1).concat(path.substring(index+4));
                  index = segIndex;
              }
              else {
                  index += 4;
              }
          }
          else {
              index += 4;
          }
      }
      
      // 6f - remove ending "<segment>/.." where "<segment>" is a
      // complete path segment
      if (path.endsWith("/..")) {
          tempString = path.substring(0, path.length()-3);
          segIndex = tempString.lastIndexOf('/');
          if (segIndex != -1) {
              path = path.substring(0, segIndex+1);
          }
      }
      m_path = path;
  }

  private void initializeScheme(String p_uriSpec)
                 throws MalformedURIException {
    int uriSpecLen = p_uriSpec.length();
    int index = 0;
    String scheme = null;
    char testChar = '\0';

    while (index < uriSpecLen) {
      testChar = p_uriSpec.charAt(index);
      if (testChar == ':' || testChar == '/' ||
          testChar == '?' || testChar == '#') {
        break;
      }
      index++;
    }
    scheme = p_uriSpec.substring(0, index);

    if (scheme.length() == 0) {
      throw new MalformedURIException("Url: " + p_uriSpec +"; No scheme found in URI.");
    }
    else {
      setScheme(scheme);
    }
  }

  private boolean initializeAuthority(String p_uriSpec) {
    
    int index = 0;
    int start = 0;
    int end = p_uriSpec.length();

//    char testChar = '\0';
    String userinfo = null;

    // userinfo is everything up to @
//    if (p_uriSpec.indexOf('@', start) != -1) {
//      while (index < end) {
//        testChar = p_uriSpec.charAt(index);
//        if (testChar == '@') {
//          break;
//        }
//        index++;
//      }
//      userinfo = p_uriSpec.substring(start, index);
//      index++;
//    }

    // host is everything up to last ':', or up to 
    // and including ']' if followed by ':'.
    String host = null;
    start = index;
    boolean hasPort = false;
    if (index < end) {
      if (p_uriSpec.charAt(start) == '[') {
      	int bracketIndex = p_uriSpec.indexOf(']', start);
      	index = (bracketIndex != -1) ? bracketIndex : end;
      	if (index+1 < end && p_uriSpec.charAt(index+1) == ':') {
      	  ++index;
      	  hasPort = true;
      	}
      	else {
      	  index = end;
      	}
      }
      else {
      	int colonIndex = p_uriSpec.lastIndexOf(':', end);
      	index = (colonIndex > start) ? colonIndex : end;
      	hasPort = (index != end);
      }
    }
    host = p_uriSpec.substring(start, index);
    int port = -1;
    if (host.length() > 0) {
      // port
      if (hasPort) {
        index++;
        start = index;
        while (index < end) {
          index++;
        }
        String portStr = p_uriSpec.substring(start, index);
        if (portStr.length() > 0) {
          // REVISIT: Remove this code.
          /** for (int i = 0; i < portStr.length(); i++) {
            if (!isDigit(portStr.charAt(i))) {
              throw new MalformedURIException(
                   portStr +
                   " is invalid. Port should only contain DIGITS!");
            }
          }**/
          // REVISIT: Remove this code.
          // Store port value as string instead of integer.
          try {
            port = Integer.parseInt(portStr);
            if (port == -1) --port;
          }
          catch (NumberFormatException nfe) {
            port = -2;
          }
        }
      }
    }
    
    if (isValidServerBasedAuthority(host, port, userinfo)) {
      m_host = host;
      m_port = port;
      m_userinfo = userinfo;
      return true;
    }
    // Note: Registry based authority is being removed from a
    // new spec for URI which would obsolete RFC 2396. If the
    // spec is added to XML errata, processing of reg_name
    // needs to be removed. - mrglavas.
//    else if (isValidRegistryBasedAuthority(p_uriSpec)) {
//      m_regAuthority = p_uriSpec;
//      return true;
//    }
    return false;
  }
  
  private boolean isValidServerBasedAuthority(String host, int port, String userinfo) {
    
    // Check if the host is well formed.
    if (!isWellFormedAddress(host)) {
      return false;
    }
    
    // Check that port is well formed if it exists.
    // REVISIT: There's no restriction on port value ranges, but
    // perform the same check as in setPort to be consistent. Pass
    // in a string to this method instead of an integer.
    if (port < -1 || port > 65535) {
      return false;
    }
    
    // Check that userinfo is well formed if it exists.
//    if (userinfo != null) {
//      // Userinfo can contain alphanumerics, mark characters, escaped
//      // and ';',':','&','=','+','$',','
//      int index = 0;
//      int end = userinfo.length();
//      char testChar = '\0';
//      while (index < end) {
//        testChar = userinfo.charAt(index);
//        if (testChar == '%') {
//          if (index+2 >= end ||
//            !isHex(userinfo.charAt(index+1)) ||
//            !isHex(userinfo.charAt(index+2))) {
//            return false;
//          }
//          index += 2;
//        }
//        else if (!isUserinfoCharacter(testChar)) {
//          return false;
//        }
//        ++index;
//      }
//    }
    return true;
  }
  
  private void initializePath(String p_uriSpec, int p_nStartIndex)
                 throws MalformedURIException {
    if (p_uriSpec == null) {
      throw new MalformedURIException("Url: " + p_uriSpec +
                "; Cannot initialize path from null string!");
    }

    int index = p_nStartIndex;
    int start = p_nStartIndex;
    int end = p_uriSpec.length();
    char testChar = '\0';

    // path - everything up to query string or fragment
    if (start < end) {
    	// RFC 2732 only allows '[' and ']' to appear in the opaque part.
    	if (getScheme() == null || p_uriSpec.charAt(start) == '/') {
    	
            // Scan path.
            // abs_path = "/"  path_segments
            // rel_path = rel_segment [ abs_path ]
            while (index < end) {
                testChar = p_uriSpec.charAt(index);
            
                // check for valid escape sequence
                if (testChar == '%') {
                    if (index+2 >= end ||
                    !isHex(p_uriSpec.charAt(index+1)) ||
                    !isHex(p_uriSpec.charAt(index+2))) {
                        throw new MalformedURIException("Url: " + p_uriSpec +
                            "; Path contains invalid escape sequence!");
                    }
                    index += 2;
                }
                // Path segments cannot contain '[' or ']' since pchar
                // production was not changed by RFC 2732.
                else if (!isPathCharacter(testChar)) {
      	            if (testChar == '?' || testChar == '#') {
      	                break;
      	            }
                    throw new MalformedURIException("Url: " + p_uriSpec +
                        "; Path contains invalid character: " + testChar);
                }
                ++index;
            }
        }
        else {
            
            // Scan opaque part.
            // opaque_part = uric_no_slash *uric
            while (index < end) {
                testChar = p_uriSpec.charAt(index);
            
                if (testChar == '?' || testChar == '#') {
                    break;
      	        }
                
                // check for valid escape sequence
                if (testChar == '%') {
                    if (index+2 >= end ||
                    !isHex(p_uriSpec.charAt(index+1)) ||
                    !isHex(p_uriSpec.charAt(index+2))) {
                        throw new MalformedURIException("Url: " + p_uriSpec +
                            "; Opaque part contains invalid escape sequence!");
                    }
                    index += 2;
                }
                // If the scheme specific part is opaque, it can contain '['
                // and ']'. uric_no_slash wasn't modified by RFC 2732, which
                // I've interpreted as an error in the spec, since the 
                // production should be equivalent to (uric - '/'), and uric
                // contains '[' and ']'. - mrglavas
                else if (!isURICharacter(testChar)) {
                    throw new MalformedURIException("Url: " + p_uriSpec +
                        "Opaque part contains invalid character: " + testChar);
                }
                ++index;
            }
        }
    }
    m_path = p_uriSpec.substring(start, index);

    // query - starts with ? and up to fragment or end
    if (testChar == '?') {
      index++;
      start = index;
      while (index < end) {
        testChar = p_uriSpec.charAt(index);
        if (testChar == '#') {
          break;
        }
        if (testChar == '%') {
           if (index+2 >= end ||
              !isHex(p_uriSpec.charAt(index+1)) ||
              !isHex(p_uriSpec.charAt(index+2))) {
            throw new MalformedURIException("Url: " + p_uriSpec +
                    "; Query string contains invalid escape sequence!");
           }
           index += 2;
        }
        else if (!isURICharacter(testChar)) {
          throw new MalformedURIException("Url: " + p_uriSpec +
                "; Query string contains invalid character: " + testChar);
        }
        index++;
      }
      m_queryString = p_uriSpec.substring(start, index);
    }

    // fragment - starts with #
    if (testChar == '#') {
      index++;
      start = index;
      while (index < end) {
        testChar = p_uriSpec.charAt(index);

        if (testChar == '%') {
           if (index+2 >= end ||
              !isHex(p_uriSpec.charAt(index+1)) ||
              !isHex(p_uriSpec.charAt(index+2))) {
            throw new MalformedURIException("Url: " + p_uriSpec +
                    "; Fragment contains invalid escape sequence!");
           }
           index += 2;
        }
        else if (!isURICharacter(testChar)) {
          throw new MalformedURIException("Url: " + p_uriSpec +
                "Fragment contains invalid character: "+testChar);
        }
        index++;
      }
      m_fragment = p_uriSpec.substring(start, index);
    }
  }
  
  public String getScheme() {
    return m_scheme;
  }

  public String getSchemeSpecificPart() {
    StringBuffer schemespec = new StringBuffer();

    if (m_host != null || m_regAuthority != null) {
      schemespec.append("//");
    
      // Server based authority.
      if (m_host != null) {

        if (m_userinfo != null) {
          schemespec.append(m_userinfo);
          schemespec.append('@');
        }
        
        schemespec.append(m_host);
        
        if (m_port != -1) {
          schemespec.append(':');
          schemespec.append(m_port);
        }
      }
      // Registry based authority.
      else {
      	schemespec.append(m_regAuthority);
      }
    }

    if (m_path != null) {
      schemespec.append((m_path));
    }

    if (m_queryString != null) {
      schemespec.append('?');
      schemespec.append(m_queryString);
    }

    if (m_fragment != null) {
      schemespec.append('#');
      schemespec.append(m_fragment);
    }

    return schemespec.toString();
  }

  public String getHostSpecificPart() 
  {
    StringBuffer schemespec = new StringBuffer();

    if (m_scheme != null) {
    	schemespec.append(m_scheme);
    	schemespec.append(':');
      }
    
    if (m_host != null || m_regAuthority != null) {
      schemespec.append("//");
    
      // Server based authority.
      if (m_host != null) {

        if (m_userinfo != null) {
          schemespec.append(m_userinfo);
          schemespec.append('@');
        }
        
        schemespec.append(m_host);
        
        if (m_port != -1) {
          schemespec.append(':');
          schemespec.append(m_port);
        }
      }
      // Registry based authority.
      else {
      	schemespec.append(m_regAuthority);
      }
    }

	return schemespec.toString();	    
  }
  
  public String getUserinfo() {
    return m_userinfo;
  }

  public String getHost() {
    return m_host;
  }
  
  public void setHost(String strHost) {
	m_host = strHost;
  }

  public int getPort() {
    return m_port;
  }
  
  public String getRegBasedAuthority() {
    return m_regAuthority;
  }
  
  public String getPath() {
    return m_path;
  }

  public void setPath(String path) {
	    m_path = path;
  }
  
  public String getQueryString() {
    return m_queryString;
  }

  public String getFragment() {
    return m_fragment;
  }

  public void setScheme(String p_scheme) throws MalformedURIException {
    if (p_scheme == null) {
      throw new MalformedURIException("Url: " + p_scheme +
               "; Cannot set scheme from null string!");
    }
    if (!isConformantSchemeName(p_scheme)) {
      throw new MalformedURIException("Url: " + p_scheme +"; The scheme is not conformant.");
    }
    m_scheme = p_scheme.toLowerCase();
    mHashCode=-1;
  }

  public void setQueryString(String p_queryString) throws MalformedURIException {
    if (p_queryString == null) {
      m_queryString = null;
    }
    else if (!isGenericURI()) {
      throw new MalformedURIException("Url: " + p_queryString +
              "; Query string can only be set for a generic URI!");
    }
    else if (getPath() == null) {
      throw new MalformedURIException("Url: " + p_queryString +
             "; Query string cannot be set when path is null!");
    }
    else if (!isURIString(p_queryString)) {
      throw new MalformedURIException("Url: " + p_queryString +
              "; Query string contains invalid character!");
    }
    else {
      m_queryString = p_queryString;
    }
    mHashCode=-1;
  }

  public boolean equals(Object p_test) {
    if (p_test instanceof URI) {
      URI testURI = (URI) p_test;
      if ( ((m_scheme == testURI.m_scheme) || 
            (m_scheme != null && m_scheme.equals(testURI.m_scheme))) &&
           ((m_userinfo == testURI.m_userinfo) || 
            (m_userinfo != null && m_userinfo.equals(testURI.m_userinfo))) &&
           ((m_host == testURI.m_host) || 
            (m_host != null && m_host.equals(testURI.m_host))) &&
           (m_port == testURI.m_port )&&
           ((m_path == testURI.m_path) || 
            (m_path != null && m_path.equals(testURI.m_path))) &&
           ((m_queryString == testURI.m_queryString) || 
            (m_queryString != null && m_queryString.equals(testURI.m_queryString))) 
//            && ((m_fragment == testURI.m_fragment) || 
//            (m_fragment != null && m_fragment.equals(testURI.m_fragment)))
            ){
          return true;
      }
//      if (((m_scheme == null && testURI.m_scheme == null) ||
//           (m_scheme != null && testURI.m_scheme != null &&
//            m_scheme.equals(testURI.m_scheme))) &&
//          ((m_userinfo == null && testURI.m_userinfo == null) ||
//           (m_userinfo != null && testURI.m_userinfo != null &&
//            m_userinfo.equals(testURI.m_userinfo))) &&
//          ((m_host == null && testURI.m_host == null) ||
//           (m_host != null && testURI.m_host != null &&
//            m_host.equals(testURI.m_host))) &&
//            m_port == testURI.m_port &&
//          ((m_path == null && testURI.m_path == null) ||
//           (m_path != null && testURI.m_path != null &&
//            m_path.equals(testURI.m_path))) &&
//          ((m_queryString == null && testURI.m_queryString == null) ||
//           (m_queryString != null && testURI.m_queryString != null &&
//            m_queryString.equals(testURI.m_queryString))) &&
//          ((m_fragment == null && testURI.m_fragment == null) ||
//           (m_fragment != null && testURI.m_fragment != null &&
//            m_fragment.equals(testURI.m_fragment)))) {
//        return true;
//      }
    }
    return false;
  }

  public String toString() {
    StringBuffer uriSpecString = new StringBuffer();

    if (m_scheme != null) {
      uriSpecString.append(m_scheme);
      uriSpecString.append(':');
    }
    uriSpecString.append(getSchemeSpecificPart());
    return uriSpecString.toString();
  }

  private boolean isGenericURI() {
    // presence of the host (whether valid or empty) means
    // double-slashes which means generic uri
    return (m_host != null);
  }
  
  public static boolean isConformantSchemeName(String p_scheme) {
    if (p_scheme == null || p_scheme.trim().length() == 0) {
      return false;
    }

    if (!isAlpha(p_scheme.charAt(0))) {
      return false;
    }

    char testChar;
    int schemeLength = p_scheme.length();
    for (int i = 1; i < schemeLength; ++i) {
      testChar = p_scheme.charAt(i);
      if (!isSchemeCharacter(testChar)) {
        return false;
      }
    }

    return true;
  }

  public static boolean isWellFormedAddress(String address) {
    if (address == null) {
      return false;
    }

    int addrLength = address.length();
    if (addrLength == 0) {
      return false;
    }
    
    // Check if the host is a valid IPv6reference.
//    if (address.startsWith("[")) {
//      return isWellFormedIPv6Reference(address);
//    }

    // Cannot start with a '.', '-', or end with a '-'.
    if (address.startsWith(".") || 
        address.startsWith("-") || 
        address.endsWith("-")) {
      return false;
    }

    // rightmost domain label starting with digit indicates IP address
    // since top level domain label can only start with an alpha
    // see RFC 2396 Section 3.2.2
    int index = address.lastIndexOf('.');
    if (address.endsWith(".")) {
      index = address.substring(0, index).lastIndexOf('.');
    }

    if (index+1 < addrLength && isDigit(address.charAt(index+1))) {
      return isWellFormedIPv4Address(address);
    }
    else {
      // hostname      = *( domainlabel "." ) toplabel [ "." ]
      // domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
      // toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
      
      // RFC 2396 states that hostnames take the form described in 
      // RFC 1034 (Section 3) and RFC 1123 (Section 2.1). According
      // to RFC 1034, hostnames are limited to 255 characters.
      if (addrLength > 255) {
      	return false;
      }
      
      // domain labels can contain alphanumerics and '-"
      // but must start and end with an alphanumeric
      char testChar;
      int labelCharCount = 0;

      for (int i = 0; i < addrLength; i++) {
        testChar = address.charAt(i);
        if (testChar == '.') {
          if (!isAlphanum(address.charAt(i-1))) {
            return false;
          }
          if (i+1 < addrLength && !isAlphanum(address.charAt(i+1))) {
            return false;
          }
          labelCharCount = 0;
        }
        else if (!isAlphanum(testChar) && testChar != '-') {
          return false;
        }
        // RFC 1034: Labels must be 63 characters or less.
        else if (++labelCharCount > 63) {
          return false;
        }
      }
    }
    return true;
  }
  
  public static boolean isWellFormedIPv4Address(String address) {
      
      int addrLength = address.length();
      char testChar;
      int numDots = 0;
      int numDigits = 0;

      // make sure that 1) we see only DIGITS and dot separators, 2) that
      // any dot separator is preceded and followed by a digit and
      // 3) that we find 3 dots
      //
      // RFC 2732 amended RFC 2396 by replacing the definition 
      // of IPv4address with the one defined by RFC 2373. - mrglavas
      //
      // IPv4address = 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
      //
      // One to three DIGITS must be in each segment.
      for (int i = 0; i < addrLength; i++) {
        testChar = address.charAt(i);
        if (testChar == '.') {
          if ((i > 0 && !isDigit(address.charAt(i-1))) || 
              (i+1 < addrLength && !isDigit(address.charAt(i+1)))) {
            return false;
          }
          numDigits = 0;
          if (++numDots > 3) {
            return false;
          }
        }
        else if (!isDigit(testChar)) {
          return false;
        }
        // Check that that there are no more than three DIGITS
        // in this segment.
        else if (++numDigits > 3) {
          return false;
        }
        // Check that this segment is not greater than 255.
        else if (numDigits == 3) {
          char first = address.charAt(i-2);
          char second = address.charAt(i-1);
          if (!(first < '2' || 
               (first == '2' && 
               (second < '5' || 
               (second == '5' && testChar <= '5'))))) {
            return false;
          }
        }
      }
      return (numDots == 3);
  }
  
  private static boolean isDigit(char p_char) {
    return p_char >= '0' && p_char <= '9';
  }

  private static boolean isHex(char p_char) {
    return (p_char <= 'f' && (fgLookupTable[p_char] & ASCII_HEX_CHARACTERS) != 0);
  }

  private static boolean isAlpha(char p_char) {
      return ((p_char >= 'a' && p_char <= 'z') || (p_char >= 'A' && p_char <= 'Z' ));
  }

  private static boolean isAlphanum(char p_char) {
     return (p_char <= 'z' && (fgLookupTable[p_char] & MASK_ALPHA_NUMERIC) != 0);
  }

  private static boolean isURICharacter (char p_char) {
      return (p_char <= '~' && (fgLookupTable[p_char] & MASK_URI_CHARACTER) != 0);
  }

  private static boolean isSchemeCharacter (char p_char) {
      return (p_char <= 'z' && (fgLookupTable[p_char] & MASK_SCHEME_CHARACTER) != 0);
  }

  private static boolean isPathCharacter (char p_char) {
      return (p_char <= '~' && (fgLookupTable[p_char] & MASK_PATH_CHARACTER) != 0);
  }

  private static boolean isURIString(String p_uric) {
    if (p_uric == null) {
      return false;
    }
    int end = p_uric.length();
    char testChar = '\0';
    for (int i = 0; i < end; i++) {
      testChar = p_uric.charAt(i);
      if (testChar == '%') {
        if (i+2 >= end ||
            !isHex(p_uric.charAt(i+1)) ||
            !isHex(p_uric.charAt(i+2))) {
          return false;
        }
        else {
          i += 2;
          continue;
        }
      }
      if (isURICharacter(testChar)) {
          continue;
      }
      else {
        return false;
      }
    }
    return true;
  }
  
  static public String urlEncode(String fullPath)
  {
	  StringBuffer sb = new StringBuffer();
	  int len = fullPath.length();
	  
	  char c;
	  for  (int index=0; index < len ; index++){
		  c = fullPath.charAt(index);
		  if ( c == '^'  || c == '_'
				 || c == '\\' || c == '-'
				 || c == '.'
				 || (c >= 'A' && c <= 'Z')
				 || (c >= 'a' && c <= 'z')
				 || (c >= '0' && c <= '9') ) {
				sb.append(c);
			} else {
				sb.append('%');
				if (c > 15) { // is it a non-control char, ie. >x0F so 2 chars
					sb.append(Integer.toHexString((int)c)); // just add % and the string
				} else {
					sb.append("0" + Integer.toHexString((int)c));
					// otherwise need to add a leading 0
				}
			}
			  
	  }
	  return sb.toString();
  }

  static public String ampEncode(String fullPath)
  {
	  StringBuffer sb = new StringBuffer();
	  int len = fullPath.length();
	  
	  char c;
	  for( int index=0; index < len ; index++ )
	  {
		  c = fullPath.charAt(index);
		  if ( c == '&' )
			  sb.append("&amp;");
		  else
			  sb.append(c);
	  }
	  
	  return sb.toString();
  }
  
  public static boolean isLocalHost(String strUrl)
  {
	  return strUrl.startsWith("http://localhost") ||
	  	strUrl.startsWith("http://127.0.0.1");
  }

  public static boolean isLocalData(String strUrl)
  {
	  return strUrl.startsWith("data:");
  }
  /*
  public String getEscapedURL(){
	  String fullPath = m_scheme + "://" + m_host + ":" + m_port + m_path;
	  StringBuffer sb = new StringBuffer();
	  int len = fullPath.length();
	  
	  char c;
	  for  (int index=0; index < len ; index++){
		  c = fullPath.charAt(index);
		  if (c == '&') {
				sb.append("&");
			} else if (c == ' ') {
				sb.append('+');
			} else if (
				    c == ','  || c == ';'
				    || c == ' '  || c == '-'
				    || c == '.'  || c == '*'
					|| (c >= 'A' && c <= 'Z')
					|| (c >= 'a' && c <= 'z')
					|| (c >= '0' && c <= '9')
					|| c == '_' || c == '=' 
					|| c == '?') {
				sb.append(c);
			} else {
				sb.append('%');
				if (c > 15) { // is it a non-control char, ie. >x0F so 2 chars
					sb.append(Integer.toHexString((int)c)); // just add % and the string
				} else {
					sb.append("0" + Integer.toHexString((int)c));
					// otherwise need to add a leading 0
				}
			}
			  
	  }
	  return sb.toString();
  }
  */
  
  public void appendFragment(String fragment) {
	  this.m_fragment = fragment;
  }

	//"auth_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT, auth_token=887b2ffd30a7b97be9a0986d7746a934421eec7d; path=/; expires=Sat, 24 Oct 2009 20:56:55 GMT, rhosync_session=BAh7BzoMdXNlcl9pZGkIIgpmbGFzaElDOidBY3Rpb25Db250cm9sbGVyOjpGbGFzaDo6Rmxhc2hIYXNoewAGOgpAdXNlZHsA--f9b67d99397fc534107fb3b7483ccdae23b4a761; path=/; expires=Sun, 10 Oct 2010 19:10:58 GMT; HttpOnly");
	//"auth_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT");
	//"rhosync_session=BAh7CToNcGFzc3dvcmQiFTiMYru1W11zuoAlN%2FPtgjc6CmxvZ2luIhU4jGK7tVtdc7qAJTfz7YI3Ogx1c2VyX2lkaQYiCmZsYXNoSUM6J0FjdGlvbkNvbnRyb2xsZXI6OkZsYXNoOjpGbGFzaEhhc2h7AAY6CkB1c2VkewA%3D--a7829a70171203d72cd4e83d07b18e8fcf5e2f78; path=/; expires=Thu, 02 Sep 2010 23:51:31 GMT; HttpOnly");
  
	public static String parseCookie(String value) 
	{
		String strRes = "";
		Tokenizer stringtokenizer = new Tokenizer(value, ";");
		while (stringtokenizer.hasMoreTokens()) {
			String tok = stringtokenizer.nextToken();
			tok = tok.trim();
			if (tok.length() == 0) {
				continue;
			}

			//expires=Thu, 01 Jan 1970 00:00:00 GMT, auth_token=
			int nExp = tok.indexOf("expires=");
			if ( nExp >= 0 )
			{
				int nEnd = tok.indexOf(',', nExp);
				if ( nEnd >= 0 )
				{
					int nEnd1 = tok.indexOf(',', nEnd+1);
					if ( nEnd1 >= 0 )
						nEnd = nEnd1;
					else
						nEnd = tok.length()-1;
				}
				
				tok = tok.substring(0,nExp) + tok.substring(nEnd+1);
				tok = tok.trim();
			}
			
			int nEq = tok.indexOf('=');
			if ( nEq < 0 )
				continue;
			
			strRes += tok + ";";  
		}
		
		return strRes;
	}
  
 }
