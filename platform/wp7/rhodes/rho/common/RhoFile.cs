using System;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Resources;
using System.IO;

namespace rho.common
{
    public class CRhoFile
    {
        public static void deleteFile(String path)
        {
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            isoStore.DeleteFile(path);
        }

        public static bool isDirectoryExist(String path)
        {
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            return isoStore.DirectoryExists(path);
        }

        public static bool isFileExist(String path)
        {
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            return isoStore.FileExists(path);
        }

        public static bool isResourceFileExist(String path)
        {
            if (path.StartsWith("/"))
                path = path.Substring(1);

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(path, UriKind.Relative));
            if (sr == null)
                return false;

            return sr != null;
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

        public static String readFiletoString(String path)
        {
            string content = "";

            if (path.StartsWith("/"))
                path = path.Substring(1);

            if (!isFileExist(path)) return content; 

            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            Stream st = isoStore.OpenFile(path, FileMode.Open, FileAccess.Read, FileShare.None);
            using (System.IO.BinaryReader br = new BinaryReader(st))
            {
                //char[] str = br.ReadChars((int)st.Length);
                content = br.ReadString();
            }

            return content;
        }

        public static String readResourceFiletoString(String path)
        {
            string content = "";

            if (path.StartsWith("/"))
                path = path.Substring(1);

            if (!CRhoFile.isResourceFileExist(path)) return content;

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(path, UriKind.Relative));

            using (System.IO.BinaryReader br = new BinaryReader(sr.Stream))
            {
                char[] str = br.ReadChars((int)sr.Stream.Length);
                content = new string(str);
                br.Close();
            }

            return content;
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
