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
        return false;
    }

    public override string CurrentDirectory
    {
        get
        {
            return "/rho";
        }
        set
        {
            throw new NotImplementedException();
        }
    }
    public override string GetFullPath(string path)
    {
        return Path.Combine(CurrentDirectory,path);
    }

    public override Stream OpenInputFileStream(string path)
    {
        System.Diagnostics.Debug.WriteLine("open_file: " + path, "");

        if (!path.StartsWith("/rho"))
            path = "rho/" + path;

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
