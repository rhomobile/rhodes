using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.IsolatedStorage;
using System.Security.Cryptography;

namespace rhodes.common
{
    public class RhoCrypt
    {
        //private byte[] mKey;
        private uint mLastError;
        //private String mDBPartition;

        public int dbDecrypt(String partition, int size, String data, out String decryptedData)
        {
            AesManaged aes = null;
            MemoryStream memoryStream = null;

            try
            {
                aes = new AesManaged();
                aes.Key = readKeyFromFile(partition);

                memoryStream = new MemoryStream();
                CryptoStream cryptoStream = new CryptoStream(memoryStream, aes.CreateDecryptor(), CryptoStreamMode.Write);

                byte[] buf = Convert.FromBase64String(data);
                cryptoStream.Write(buf, 0, buf.Length);
                cryptoStream.FlushFinalBlock();

                byte[] decryptBytes = memoryStream.ToArray();

                if (cryptoStream != null)
                    cryptoStream.Dispose();

                decryptedData = Encoding.UTF8.GetString(decryptBytes, 0, decryptBytes.Length);
            }
            finally
            {
                if (memoryStream != null)
                    memoryStream.Dispose();

                if (aes != null)
                    aes.Clear();
            }


            return getErrorCode() == 0 ? 1 : 0;
        }

        public int dbEncrypt(String partition, int size, String data, out String dataOut)
        {
            AesManaged aes = null;
            MemoryStream memoryStream = null;
            CryptoStream cryptoStream = null;

            try
            {
                aes = new AesManaged();
                aes.Key = readKeyFromFile(partition);

                memoryStream = new MemoryStream();
                cryptoStream = new CryptoStream(memoryStream, aes.CreateEncryptor(), CryptoStreamMode.Write);

                byte[] buf = Encoding.UTF8.GetBytes(data);
                cryptoStream.Write(buf, 0, buf.Length);
                cryptoStream.FlushFinalBlock();

                dataOut = Convert.ToBase64String(memoryStream.ToArray());
            }
            finally
            {
                if (cryptoStream != null)
                    cryptoStream.Close();

                if (memoryStream != null)
                    memoryStream.Close();

                if (aes != null)
                    aes.Clear();
            }

            return getErrorCode() == 0 ? 1 : 0;
        }

        public int setDbCryptKey(String partition, String key, bool bPersistent)
        {
            byte[] keyByte = Encoding.UTF8.GetBytes(key);
            //mKey = keyByte;
            byte[] protectedKeyByte = ProtectedData.Protect(keyByte, null);

            this.writeKeyToFile(partition, protectedKeyByte);        
            
            return getErrorCode() == 0 ? 1 : 0;
        }

        private bool checkError(bool bRes, String func)
        {
            return true;
        }

        private uint getErrorCode() { return mLastError; }

        private void writeKeyToFile(String filePath, byte[] key)
        {
            IsolatedStorageFile file = IsolatedStorageFile.GetUserStoreForApplication();
            IsolatedStorageFileStream writestream = new IsolatedStorageFileStream(filePath, System.IO.FileMode.Create, System.IO.FileAccess.Write, file);

            Stream writer = new StreamWriter(writestream).BaseStream;
            writer.Write(key, 0, key.Length);
            writer.Close();
            writestream.Close();
        }

        private byte[] readKeyFromFile(String filePath)
        {
            IsolatedStorageFile file = IsolatedStorageFile.GetUserStoreForApplication();
            IsolatedStorageFileStream readstream = new IsolatedStorageFileStream(filePath, System.IO.FileMode.Open, FileAccess.Read, file);

            Stream reader = new StreamReader(readstream).BaseStream;
            byte[] key = new byte[reader.Length];

            reader.Read(key, 0, key.Length);
            reader.Close();
            readstream.Close();

            return key;
        }
    }
}
