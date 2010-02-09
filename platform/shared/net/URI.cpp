#include "URI.h"
#include "common/Tokenizer.h"

namespace rho {
namespace net {

URI::URI(const String& strUrl)
{
    m_strUrl = strUrl;
}

String URI::getPath()
{
    const char* url = m_strUrl.c_str();
    const char* pStartSrv, *pEndSrv;
    const char* pHttp = strstr(url,"://");
    if ( !pHttp )
        pHttp = strstr(url,":\\\\");

    if ( pHttp )
        pStartSrv = pHttp+3;
    else
        pStartSrv = url;

    pEndSrv = strchr( pStartSrv, '/');
    if ( !pEndSrv )
        pEndSrv = strchr( pStartSrv, '\\');

    const char* pStartPath = pEndSrv ? pEndSrv+1 : 0;
    if ( !pStartPath || !*pStartPath)
        return String();

    const char* pEndPath = pStartPath ? strrchr( pStartPath, '?') :0;
    if (!pEndPath)
        pEndPath = pStartPath + strlen(pStartPath);

    return String(pStartPath, pEndPath - pStartPath);
}

String URI::getQueryString()
{
    const char* szQuest = strrchr( m_strUrl.c_str(), '?');
    if ( !szQuest )
        return String();

    return String(szQuest+1);
}

String URI::getScheme()
{
    const char* url = m_strUrl.c_str();
    const char* pHttp = strstr(url,"://");
    if ( !pHttp )
        pHttp = strstr(url,":\\\\");

    if ( !pHttp )
        return String();

    String res(url, pHttp-url);
    return res;
}

String URI::getPathSpecificPart()
{
    const char* szQuest = strrchr( m_strUrl.c_str(), '?');
    if ( !szQuest )
        return m_strUrl;

    return String(m_strUrl.c_str(), szQuest-m_strUrl.c_str());
}

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

/*static*/ void URI::parseCookie(const char* szCookie, CParsedCookie& cookie) 
{
	boolean bAuth = false;
	boolean bSession = false;
    common::CTokenizer stringtokenizer(szCookie, ";");
	while (stringtokenizer.hasMoreTokens()) 
    {
		String tok = stringtokenizer.nextToken();
		tok = trim(tok);
		if (tok.length() == 0) {
			continue;
		}
		
		int i = 0;
		if ( (i=tok.find("auth_token=")) >= 0 )
		{
			String val = trim(tok.substr(i+11));
			if ( val.length() > 0 )
			{
				cookie.strAuth = "auth_token=" + val;
				bAuth = true;
			}
		}else if ( (i=tok.find("path=")) >= 0 )
		{
			String val = trim(tok.substr(i+6));
			if ( val.length() > 0 )
			{
				if (bAuth)
					cookie.strAuth += ";path=" + val;
				else if (bSession)
					cookie.strSession += ";path=" + val;
			}
		}else if ( (i=tok.find("rhosync_session=")) >= 0 )
		{
			String val = trim(tok.substr(i+16));
			if ( val.length() > 0 )
			{
				cookie.strSession = "rhosync_session=" + val;
				bSession = true;
			}
		}
	}
}

}
}
