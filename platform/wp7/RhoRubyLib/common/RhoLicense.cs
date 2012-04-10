using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Text;
using System.Runtime.InteropServices;


namespace rho.common
{
    public class RhoLicense
    {
        [DllImport("user32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int wsprintf(
          [Out] char[] buffer,
          string format,
          params object[] args);

        [DllImport("user32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int wcsncmp(
          char[] string1,
          char[] string2,
          int count);

        [DllImport("user32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int wcscmp(
          char[] string1,
          char[] string2);

        
        private static String LIC_KEY = "MOTNEON";
        private static ulong dwVer1 = 2;
        private static ulong dwVer2 = 0;
        private static ulong dwVer3 = 0;
        private static ulong dwVer4 = 5;
        private static String wtDIN;
        
        public static void DecryptPwd(ref char[] pwtPwd, ref char[] pwtDecrypted)
        {
            int x;
	        char[] wtHex = new char[3];
	        char wtChar;
	        int x1 = 0;
            
	        int l = wtDIN.Length;
	        int l1 = pwtPwd.Length;

	        //decode
	        for(x=0; x < l1; x+=2){
                wtHex[0] = pwtPwd[x];
                wtHex[1] = pwtPwd[x+1];
		        pwtPwd[x1++] = Convert.ToChar(Convert.ToInt64(wtHex.ToString(), 16));
	        }

	        //decrypt
	        char[] wtTemp = new char[1000];
	        pwtDecrypted[0] = '\0';
	        for(x=0; x < x1; x++){
		        wtChar = wtDIN[(x % l) - l * Convert.ToInt32((x+1 % l)==0)];
                wtTemp[x] = Convert.ToChar(pwtPwd[x] ^ wtChar);
                pwtDecrypted = System.String.Concat(pwtDecrypted, wtTemp).ToCharArray() ;
	        }
        }

        int MotorolaLicence_check(String company, String licence)
        {	
            char[] wtCoName = company.ToCharArray();
	        char[] wtReg = licence.ToCharArray();

	
	        int iChecksum = 0;
	        for(uint iCS=0; iCS<wtCoName.Length; iCS++)
		        iChecksum+=wtCoName[iCS];
	
	        if(iChecksum > 0xEE2)
		        return 0;
	
	        char[] wtDecrypted = new char[50];
	
	        char[] wtCS = new char[5];
	        char[] wtLicenseKey = new char[16];

	        wsprintf(wtCS, "%03X:", iChecksum);
		    wtDIN = "CORPERATELICENSE";
		    DecryptPwd(ref wtReg, ref wtDecrypted);
		    if(wcsncmp(wtCS, wtDecrypted, 4) != 0)
			    return 0;

		    wsprintf(wtLicenseKey, "%s%d_%dO0", LIC_KEY, dwVer1, dwVer2, dwVer3);
		
	        
            if(wcscmp(wtDecrypted,wtLicenseKey ) != 0)//todo!!!
		        return 0;
  
	        return 1;
        }

    }
}
