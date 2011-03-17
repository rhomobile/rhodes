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

        public static void recursiveCreateDir(String strPath)
        {
            char[] sep = { '/' };
            string[] dirsPath = strPath.Split(sep, StringSplitOptions.RemoveEmptyEntries);
            string strBaseDir = string.Empty;

            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            for (int i = 0; i < dirsPath.Length - 1; i++)
            {
                strBaseDir = System.IO.Path.Combine(strBaseDir, dirsPath[i]);
                isoStore.CreateDirectory(strBaseDir);
            }
        }

        public static void writeStringToFile(String strPath, String strData)
        {
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();

            //Write the file
            using (BinaryWriter bw = new BinaryWriter(isoStore.OpenFile(strPath, FileMode.Create, FileAccess.Write, FileShare.Read)))
            {
                bw.Write(strData);
                bw.Close();
            }
        }
    }
}
