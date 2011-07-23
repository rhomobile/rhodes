package com.rho.net;

public class URI {

	public URI(String strUrl) {
		// TODO Auto-generated constructor stub
	}

	public String getHostSpecificPart() {
		// TODO Auto-generated method stub
		return null;
	}

	public String getLastNamePart() {
		// TODO Auto-generated method stub
		return null;
	}

	public String getQueryString() {
		// TODO Auto-generated method stub
		return null;
	}

	public static native String parseCookie(String value) 
	/*
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
 	*/
	/*-{
	}-*/;
}
