using System;

namespace rho.common
{
    public class CFilePath
    {
        public static String join(String path1, String path2)
        {
            bool bSlash1 = path1.EndsWith("/") || path1.EndsWith("\\");
            bool bSlash2 = path2.StartsWith("/") || path2.StartsWith("\\");

            String res;
            if (bSlash1 && bSlash2)
                res = path1 + path2.Substring(1);
            else if (bSlash1 || bSlash2)
                res = path1 + path2;
            else
                res = path1 + '/' + path2;

            return res;
        }

        public static String getExtension(String path)
        {
            int nDot = path.LastIndexOf('.');
            if (nDot >= 0)
                return path.Substring(0, nDot);

            return "";
        }

        public static String getBaseName(String path)
        { 
            int nSlash = findLastSlash(path);
            if (nSlash >= 0 )
                return path.Substring(nSlash + 1, path.Length - (nSlash + 1));

            return path;
        }

        static int findLastSlash(String path)
        {
            int nPos = path.LastIndexOf('/');
            if (nPos >= 0)
                return nPos;

            return path.LastIndexOf('\\');
        }

        public static String removeFirstSlash(String path)
        {
            if (path.StartsWith("/") || path.StartsWith("\\") )
                return path.Substring(1);

            return path;
        }

        public static String removeLastSlash(String path)
        {
            if (path.EndsWith("/") || path.EndsWith("\\") )
                return path.Substring(1);

            return path;
        }
    }
}
