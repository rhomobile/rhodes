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

namespace rho
{
    public class WP_PlatformAdaptationLayer : PlatformAdaptationLayer
    {
        public override bool FileExists(string path)
        {
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            System.Diagnostics.Debug.WriteLine("exist_file: " + path, "");

            if (path.StartsWith("/"))
                path = path.Substring(1);

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(path, UriKind.Relative));
            if (sr == null)
                return false;

            return sr != null;
        }

        public override bool DirectoryExists(string path)
        {
            //StreamResourceInfo sr1 = Application.GetResourceStream(new Uri("rho/", UriKind.Relative));

            return false;
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

            //if (!path.StartsWith("/rho"))
            //    path = "rho/" + path;

            if (path.StartsWith("/"))
                path = path.Substring(1);

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(path, UriKind.Relative));
            if (sr == null)
                throw new System.IO.FileNotFoundException();

            return sr.Stream;
        }

        public override Stream OpenInputFileStream(string path, FileMode mode, FileAccess access, FileShare share)
        {
            return OpenInputFileStream(path);
        }
    }

}