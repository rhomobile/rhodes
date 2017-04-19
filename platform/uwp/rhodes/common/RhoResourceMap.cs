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
using System.Net;
using System.Windows;
using System.Windows.Input;


namespace rho.common
{
    public sealed class CRhoResourceMap
    {
        private static void clearMap(String strMap)
        {
            string[] files = strMap.Split('\n');

            for (int i = 0; i < files.Length - 1; i++)
            {
                String[] values = files[i].Split('|');

                String strFile = values[0];
                if (CRhoFile.isFileExist(strFile))
                {
                    CRhoFile.deleteFile(strFile);
                }
            }
        }

        private static void copyMap(String strMap)
        {
            string[] files = strMap.Split('\n');

            for (int i = 0; i < files.Length - 1; i++)
            {
                String[] values = files[i].Split('|');

                String strFile    = "rho/" + values[0];
                String strDstFile = strFile;

                if (values[1] == "dir")
                   continue;

                //System.Diagnostics.Debugger.Log(0, "", strDstFile + "\n");

                CRhoFile.recursiveCreateDir(strDstFile);
                CRhoFile.writeDataToFile(strDstFile, CRhoFile.readResourceFile(strFile));
            }

            CRhoFile.writeStringToFile("RhoBundleMap.txt", strMap);
        }

        public static void deployContent()
        {
            String newMap = CRhoFile.readStringFromResourceFile("RhoBundleMap.txt");
            String curMap = CRhoFile.readStringFromFile("RhoBundleMap.txt");

            if (curMap == "")
            {
                copyMap(newMap);
            }
            else if (curMap != newMap && newMap != "")
            {
                clearMap(curMap);
                copyMap(newMap);
            }
        }
    }
}
