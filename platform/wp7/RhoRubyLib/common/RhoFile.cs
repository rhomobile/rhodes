using System;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Resources;
using System.Text;
using System.IO;

namespace rho.common
{
    class CFileInputStream : IInputStream
    {
        CRhoFile m_oFile;
        public CFileInputStream(CRhoFile oFile)
        {
            m_oFile = oFile;
        }

        public long available() { return m_oFile.size(); }
        public int read() { return m_oFile.readByte(); }
        public int read(byte[] buffer, int bufOffset, int bytesToRead) { return m_oFile.readData(buffer, bufOffset, bytesToRead); }
        public void reset() { m_oFile.movePosToStart(); }
    };
    
    public class CRhoFile
    {
        public enum EOpenModes{ OpenForAppend = 1, OpenReadOnly = 2, OpenForWrite = 3, OpenForReadWrite = 4 };
        Stream m_st = null;
        IInputStream m_pInputStream = null;

        public bool isOpened() 
        {
            if (m_st == null)
                return false;

            return true; 
        }

        public bool open(String szFilePath, EOpenModes eMode) 
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                szFilePath = CFilePath.removeFirstSlash(szFilePath);
                if (eMode == EOpenModes.OpenForAppend || eMode == EOpenModes.OpenForReadWrite)
                {
                    if (!isFileExist(szFilePath))
                    {
                        m_st = isoStore.OpenFile(szFilePath, FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.Read);
                    }

                    if (eMode == EOpenModes.OpenForAppend)
                        movePosToEnd();
                }
                else if (eMode == EOpenModes.OpenReadOnly)
                    m_st = isoStore.OpenFile(szFilePath, FileMode.Open, FileAccess.Read, FileShare.Read);
                else if (eMode == EOpenModes.OpenForWrite)
                    m_st = isoStore.OpenFile(szFilePath, FileMode.OpenOrCreate, FileAccess.Read, FileShare.Read);
            }

            return isOpened(); 
        }

        public IInputStream getInputStream()
        {
            m_pInputStream = new CFileInputStream(this);
            return m_pInputStream;
        }

        public Stream getStream()
        {
            return m_st;
        }  

        public void write(byte[] data, int len) 
        {
            if (!isOpened())
                return;

            m_st.Write(data, 0, len);
        }

        public void writeString(String data) 
        {
            if (!isOpened())
                return;

            m_st.Write(new UTF8Encoding().GetBytes(data), 0, data.length());
        }

        public void flush()
        {
            if (!isOpened())
                return;

            m_st.Flush();
        }

        public void close()
        {
            if (!isOpened())
                return;

            m_st.Close();
            m_st = null;
        }

        public void movePosToStart()
        {
            if (!isOpened())
                return;

            m_st.Seek(0, SeekOrigin.Begin);
        }

        public void movePosToEnd()
        {
            if (!isOpened())
                return;
            
            m_st.Seek(0, SeekOrigin.End);
        }

        public void setPosTo(long nPos)
        {
            if (!isOpened() || nPos < 0)
                return;

            m_st.Seek(nPos, SeekOrigin.Current);
        }

        public long size() 
        {
            if (!isOpened())
                return -1;

            return m_st.Length > 0 ? m_st.Length : -1; 
        }

        public String readString() { return ""; }

        public int readByte()
        {
            if (!isOpened())
                return -1;

            return m_st.ReadByte();
        }
        public int readData(byte[] buffer, int bufOffset, int bytesToRead)
        {
            if (!isOpened())
                return -1;

            int count = m_st.Read(buffer, bufOffset, bytesToRead);
            return count > 0 ? count : -1;
        }

        public static void deleteFile(String path)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                isoStore.DeleteFile(path);
            }
        }

        public static void renameFile(String oldName, String newName)
        {
            using (var store = IsolatedStorageFile.GetUserStoreForApplication())
            using (var readStream = new IsolatedStorageFileStream(oldName, FileMode.Open, store))
            using (var writeStream = new IsolatedStorageFileStream(newName, FileMode.Create, store))
            using (var reader = new BinaryReader(readStream))
            using (var writer = new BinaryWriter(writeStream))
            {
                writer.Write(reader.ReadBytes((int)readStream.Length));
            }

            using (var store1 = IsolatedStorageFile.GetUserStoreForApplication())
            {
                store1.DeleteFile(oldName);
            }
        }

        public static void deleteDirectory(String path)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                isoStore.DeleteDirectory(CFilePath.removeLastSlash(path));
            }
        }

        public static bool isDirectoryExist(String path)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                return isoStore.DirectoryExists(path);
            }
        }

        public static bool isFileExist(String path)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                return isoStore.FileExists(path);
            }
        }

        public static bool isResourceFileExist(String path)
        {
            StreamResourceInfo sr = Application.GetResourceStream(new Uri(CFilePath.removeFirstSlash(path), UriKind.Relative));
            if (sr == null)
                return false;

            return sr != null;
        }

        public static void recursiveCreateDir(String strPath)
        {
            char[] sep = { '/' };
            string[] dirsPath = strPath.Split(sep, StringSplitOptions.RemoveEmptyEntries);
            string strBaseDir = string.Empty;

            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                for (int i = 0; i < dirsPath.Length - 1; i++)
                {
                    strBaseDir = System.IO.Path.Combine(strBaseDir, dirsPath[i]);
                    isoStore.CreateDirectory(strBaseDir);
                }
            }
        }

        public static string[] enumDirectory(String path)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                return isoStore.GetFileNames(CFilePath.join(path, "*"));
            }
        }

        public static void deleteFilesInFolder(String path)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                string[] arFiles = isoStore.GetFileNames(CFilePath.join(path, "*"));
                foreach (string strFile in arFiles)
                {
                    isoStore.DeleteFile(CFilePath.join(path, strFile));
                }
            }
        }

        public static void createDirectory(string path)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                isoStore.CreateDirectory(CFilePath.removeLastSlash(path));
            }
        }

        public static String readStringFromFile(String path)
        {
            string content = "";
            path = CFilePath.removeFirstSlash(path);

            if (!isFileExist(path)) return content;

            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            using (Stream st = isoStore.OpenFile(path, FileMode.Open, FileAccess.Read, FileShare.None))
            using (System.IO.BinaryReader br = new BinaryReader(st))
            {
                content = br.ReadString();
            }

            return content;
        }

        public static String readStringFromResourceFile(String path)
        {
            string content = "";
            path = CFilePath.removeFirstSlash(path);

            if (!CRhoFile.isResourceFileExist(path)) 
                return content;

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(path, UriKind.Relative));

            using (System.IO.BinaryReader br = new BinaryReader(sr.Stream))
            {
                char[] str = br.ReadChars((int)sr.Stream.Length);
                content = new string(str);
            }

            return content;
        }

        public static byte[] readResourceFile(String path)
        {
            byte[] content = new byte[0];
            path = CFilePath.removeFirstSlash(path);

            if (!CRhoFile.isResourceFileExist(path))
                return content;

            StreamResourceInfo sr = Application.GetResourceStream(new Uri(path, UriKind.Relative));

            using (System.IO.BinaryReader br = new BinaryReader(sr.Stream))
            {
                content = br.ReadBytes((int)sr.Stream.Length);
            }

            return content;
        }

        public static void writeStringToFile(String strPath, String strData)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            using (var file = isoStore.OpenFile(strPath, FileMode.Create, FileAccess.Write, FileShare.Read))
            using (BinaryWriter bw = new BinaryWriter(file))
            {
                bw.Write(strData);
            }
        }

        public static void writeDataToFile(String strPath, byte[] data)
        {
            using (IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            using (var file = isoStore.OpenFile(strPath, FileMode.Create, FileAccess.Write, FileShare.Read))
            using (BinaryWriter bw = new BinaryWriter(file))
            {
                bw.Write(data);
            }
        }
    }
}
