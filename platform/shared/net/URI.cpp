#include "URI.h"

namespace rho {
namespace net {

/*static*/ void URI::urlEncode(const String& fullPath, String& strRes)
{
    int len = fullPath.length();

    char c;
    char buf[4];
    boolean bFound = false;
    for  (int index=0; index < len ; index++){
      c = fullPath.at(index);
      if ( c == '^'  || c == '_'
		     || c == '\\' || c == '-'
		     || c == '.'
		     || (c >= 'A' && c <= 'Z')
		     || (c >= 'a' && c <= 'z')
		     || (c >= '0' && c <= '9') ) {
            if ( bFound ) 
                strRes += c;
	    } else {
            if ( !bFound )
            {
                strRes += fullPath.substr(0,index);
                bFound = true;
            }

		    strRes += '%';
		    if (c > 15) { // is it a non-control char, ie. >x0F so 2 chars
			    //sb.append(Integer.toHexString((int)c)); // just add % and the string
                _itoa(c, buf, 16);
                strRes += buf;
		    } else {
                buf[0] = '0';
                _itoa(c, buf+1, 16);
                strRes += buf;

			    //sb.append("0" + Integer.toHexString((int)c));
			    // otherwise need to add a leading 0
		    }
	    }
    	  
    }

    if ( !bFound )
        strRes += fullPath;
}

}
}
