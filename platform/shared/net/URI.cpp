#include "URI.h"

namespace rho {
namespace net {

static void toHexString(int i, String& strRes, int radix)
{
    char buf[33];
	bool neg= false;
	int f, n;
	if(i<0) { neg= true; i= -i; };
	f= 32;
	buf[f--]= 0;
	do
	{
		n= i%radix;
		if(n<10) buf[f]= '0'+n;
		else buf[f]= 'a'+n-10;
		i= i/radix;
		f--;
	}
	while(i>0);

	if(neg) 
        buf[f--]= '-';

    strRes += (buf+f+1);
}

/*static*/ void URI::urlEncode(const String& fullPath, String& strRes)
{
    int len = fullPath.length();

    char c;
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
                toHexString(c, strRes, 16);
		    } else {
                strRes += '0';
                toHexString(c, strRes, 16);
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
