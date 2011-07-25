/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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

        public static String changeBaseName( String path, String szFileName )
        {
            int basePos = findLastSlash(path);
            if (basePos >= 0 && basePos < path.length() - 1)
            {
                String res = path.substring(0, basePos + 1);
                res += szFileName;

                return res;
            }

            return join(path, szFileName);
        }

        static public String getRelativePath(String path1, String path2)
        {
            if (!path1.startsWith(path2))
                return path1;

            return path1.substring(path2.length());
        }

    }
}
