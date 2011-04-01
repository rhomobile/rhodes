using System;
using rho.common;

namespace rho.net
{
    public class URI
    {
        public static String parseCookie(String value)
        {
            String strRes = "";
            Tokenizer stringtokenizer = new Tokenizer(value, ";");
            while (stringtokenizer.hasMoreTokens())
            {
                String tok = stringtokenizer.nextToken();
                tok = tok.trim();
                if (tok.length() == 0)
                {
                    continue;
                }

                //expires=Thu, 01 Jan 1970 00:00:00 GMT, auth_token=
                int nExp = tok.indexOf("expires=");
                if (nExp >= 0)
                {
                    int nEnd = tok.indexOf(',', nExp);
                    if (nEnd >= 0)
                    {
                        int nEnd1 = tok.indexOf(',', nEnd + 1);
                        if (nEnd1 >= 0)
                            nEnd = nEnd1;
                        else
                            nEnd = tok.length() - 1;
                    }

                    tok = tok.substring(0, nExp) + tok.substring(nEnd + 1);
                    tok = tok.trim();
                }

                int nEq = tok.indexOf('=');
                if (nEq < 0)
                    continue;

                strRes += tok + ";";
            }

            return strRes;
        }

        public static String urlEncode(String fullPath)
        {
            System.Text.StringBuilder sb = new System.Text.StringBuilder();
            int len = fullPath.length();

            char c;
            for (int index = 0; index < len; index++)
            {
                c = fullPath.charAt(index);
                if (c == '^' || c == '_'
                       || c == '\\' || c == '-'
                       || c == '.'
                       || (c >= 'A' && c <= 'Z')
                       || (c >= 'a' && c <= 'z')
                       || (c >= '0' && c <= '9'))
                {
                    sb.Append(c);
                }
                else
                {
                    sb.Append('%');
                    sb.Append(String.Format("{0:X2}", (int)c));
                }

            }
            return sb.ToString();
        }

        public static String urlDecode(String fullPath)
        {
            System.Text.StringBuilder sb = new System.Text.StringBuilder();
            int len = fullPath.length();

            for (int index = 0; index < len; index++)
            {
                char c1 = fullPath.charAt(index);
                if (c1 != '%')
                {
                    sb.Append(c1);
                    continue;
                }
                index++;
                c1 = fullPath.charAt(index);

                if (c1 >= '0' && c1 <= '9')
                    c1 = (char)(c1 - '0');
                else if (c1 >= 'a' && c1 <= 'f')
                    c1 = (char)(c1 - 'a' + 10);
                else if (c1 >= 'A' && c1 <= 'F')
                    c1 = (char)(c1 - 'A' + 10);
                else
                    break;

                index++;
                char c2 = fullPath.charAt(index);
                if (c2 >= '0' && c2 <= '9')
                    c2 = (char)(c2 - '0');
                else if (c2 >= 'a' && c2 <= 'f')
                    c2 = (char)(c2 - 'a' + 10);
                else if (c2 >= 'A' && c2 <= 'F')
                    c2 = (char)(c2 - 'A' + 10);
                else
                    break;

                char c = (char)((c1 << 4) | c2);
                sb.Append(c);
            }

            return sb.ToString();
        }

        static public String urlEscapeSymbols(String fullPath)
        {
            System.Text.StringBuilder sb = new System.Text.StringBuilder();
            int len = fullPath.length();

            char c;
            for (int index = 0; index < len; index++)
            {
                c = fullPath.charAt(index);
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
                     c == '_' || c == '.')
                {
                    sb.Append(c);
                }
                else
                {
                    sb.Append('_');
                }
            }

            return sb.ToString();
        }

        static public String ampEncode(String fullPath)
        {
            System.Text.StringBuilder sb = new System.Text.StringBuilder();
            int len = fullPath.length();

            char c;
            for (int index = 0; index < len; index++)
            {
                c = fullPath.charAt(index);
                if (c == '&')
                    sb.Append("&amp;");
                else
                    sb.Append(c);
            }

            return sb.ToString();
        }

        public static String getQueryString(String strUrl)
        {
            int nQuest = strUrl.indexOf('?');
            if (nQuest < 0 )
                return "";

            return strUrl.substring(nQuest + 1);
        }

        public static String getLastNamePart(String strUrl)
        {
            int nQuest = strUrl.lastIndexOf('?');
            String strRes = strUrl;
            if (nQuest>=0)
                strRes = strUrl.substring(0, nQuest);

            int nSlash = strRes.lastIndexOf('/');
            if ( nSlash < 0 )
                nSlash = strRes.lastIndexOf('\\');

            if ( nSlash >= 0 )
                strRes = strRes.substring(nSlash+1);
    
            return strRes;
        }
    }
}
