/* ****************************************************************************
 *
 * Copyright (c) Microsoft Corporation. 
 *
 * This source code is subject to terms and conditions of the Apache License, Version 2.0. A 
 * copy of the license can be found in the License.html file at the root of this distribution. If 
 * you cannot locate the  Apache License, Version 2.0, please send an email to 
 * dlr@microsoft.com. By using this source code in any fashion, you are agreeing to be bound 
 * by the terms of the Apache License, Version 2.0.
 *
 * You must not remove this notice, or any other, from this software.
 *
 *
 * ***************************************************************************/

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
            System.Diagnostics.Debug.WriteLine("open_file: " + path, "");

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(CFilePath.removeFirstSlash(path), UriKind.Relative));
            if (sr == null)
                throw new System.IO.FileNotFoundException();

            return sr.Stream;
        }

        public override Stream OpenInputFileStream(string path, FileMode mode, FileAccess access, FileShare share)
        {
            //TODO: OpenInputFileStream with params

            Stream st = null;
            if (access == FileAccess.Read)
            {
                st = OpenInputFileStream(path);
                if (st == null)
                {
                    CRhoFile file = new CRhoFile();
                    file.open(path, CRhoFile.EOpenModes.OpenReadOnly);
                    st = file.getStream();
                }

            }
            else
            {
                CRhoFile file = new CRhoFile();
                file.open(path, CRhoFile.EOpenModes.OpenForReadWrite);
                st = file.getStream();
            }

            return st;
        }
    }

}