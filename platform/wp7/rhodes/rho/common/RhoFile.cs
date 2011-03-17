using System;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Resources;
using System.IO;

namespace rho.common
{
    public class CRhoFile
    {
        public static bool isResourceFileExist(String path)
        {
            if (path.StartsWith("/"))
                path = path.Substring(1);

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(path, UriKind.Relative));
            if (sr == null)
                return false;

            return sr != null;
        }

        public static bool isFileExist(String path)
        {
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            return isoStore.FileExists(path);
        }
    }
}
