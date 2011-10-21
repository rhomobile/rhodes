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
using Microsoft.Scripting;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Resources;
using System.IO;
using rho.common;

namespace rho
{
    public class WP_PlatformAdaptationLayer : PlatformAdaptationLayer
    {
        public override bool FileExists(string path)
        {
            return CRhoFile.isResourceFileExist(path) || CRhoFile.isFileExist(path);
        }

        public override string GetDirectoryName(string path)
        {
            return Path.GetDirectoryName(CFilePath.removeLastSlash(path));
        }

        public override void CreateDirectory(string path)
        {
            CRhoFile.createDirectory(path);
        }

        public override bool DirectoryExists(string path)
        {
            return CRhoFile.isDirectoryExist(path);
        }

        public override bool IsAbsolutePath(string path)
        {
            return path.StartsWith(CurrentDirectory);
        }

        public override string CurrentDirectory
        {
            get
            {
                return "/";
            }
            set
            {
                throw new NotImplementedException();
            }
        }
        public override string GetFullPath(string path)
        {
            return Path.Combine(CurrentDirectory, path);
        }

        public override Stream OpenInputFileStream(string path)
        {
            Stream st = null;

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(CFilePath.removeFirstSlash(path), UriKind.Relative));
            if (sr != null)
                st = sr.Stream;

            if (st == null)
            {
                try
                {
                    CRhoFile file = new CRhoFile();
                    file.open(path, CRhoFile.EOpenModes.OpenReadOnly);
                    st = file.getStream();
                }
                catch (Exception)
                {
                    throw new System.IO.FileNotFoundException();
                }
            }

            if (st == null)
                throw new System.IO.FileNotFoundException();

            return st;
        }

        public override Stream OpenInputFileStream(string path, FileMode mode, FileAccess access, FileShare share)
        {
            //TODO: OpenInputFileStream with params

            Stream st = null;
            if (access == FileAccess.Read)
                return OpenInputFileStream(path);
            else
            {
                CRhoFile file = new CRhoFile();
                file.open(path, CRhoFile.EOpenModes.OpenForReadWrite);
                st = file.getStream();
            }

            if (st == null)
                throw new System.IO.FileNotFoundException();

            return st;
        }
    }

}