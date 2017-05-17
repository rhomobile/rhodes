using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.IsolatedStorage;
using Windows.Security.Cryptography;
using Windows.Security.Cryptography.Core;
using Windows.Security.Cryptography.DataProtection;
using Windows.Foundation;
using Windows.Storage.Streams;
using System.Runtime.InteropServices.WindowsRuntime;


namespace rhodes.common
{
    public class RhoCrypt
    {
        //private byte[] mKey;
        private uint mLastError = 0;
        //private String mDBPartition;

        public int dbDecrypt(String partition, int size, String data, out String decryptedData)
        {
            SymmetricKeyAlgorithmProvider SAP = SymmetricKeyAlgorithmProvider.OpenAlgorithm(SymmetricAlgorithmNames.AesEcbPkcs7);
            CryptographicKey AES;
            HashAlgorithmProvider HAP = HashAlgorithmProvider.OpenAlgorithm(HashAlgorithmNames.Md5);
            CryptographicHash Hash_AES = HAP.CreateHash();
            try
            {
                byte[] hash = new byte[32];
                Hash_AES.Append(CryptographicBuffer.CreateFromByteArray(System.Text.Encoding.UTF8.GetBytes(data)));
                byte[] temp;
                CryptographicBuffer.CopyToByteArray(Hash_AES.GetValueAndReset(), out temp);

                Array.Copy(temp, 0, hash, 0, 16);
                Array.Copy(temp, 0, hash, 15, 16);

                AES = SAP.CreateSymmetricKey(CryptographicBuffer.CreateFromByteArray(hash));

                Windows.Storage.Streams.IBuffer Buffer = CryptographicBuffer.CreateFromByteArray(System.Text.Encoding.UTF8.GetBytes(partition));
                decryptedData = CryptographicBuffer.EncodeToBase64String(CryptographicEngine.Encrypt(AES, Buffer, null));

            }
            catch (Exception ex)
            {
                decryptedData = "";
                return getErrorCode() == 0 ? 1 : 0; 
            }
            return getErrorCode() == 0 ? 1 : 0;
        }

        public int dbEncrypt(String partition, int size, String data, out String dataOut)
        {
            SymmetricKeyAlgorithmProvider SAP = SymmetricKeyAlgorithmProvider.OpenAlgorithm(SymmetricAlgorithmNames.AesEcbPkcs7);
            CryptographicKey AES;
            HashAlgorithmProvider HAP = HashAlgorithmProvider.OpenAlgorithm(HashAlgorithmNames.Md5);
            CryptographicHash Hash_AES = HAP.CreateHash();

            try{
                byte[] hash = new byte[32];
                Hash_AES.Append(CryptographicBuffer.CreateFromByteArray(System.Text.Encoding.UTF8.GetBytes(partition)));
                byte[] temp;
                CryptographicBuffer.CopyToByteArray(Hash_AES.GetValueAndReset(), out temp);

                Array.Copy(temp, 0, hash, 0, 16);
                Array.Copy(temp, 0, hash, 15, 16);

                AES = SAP.CreateSymmetricKey(CryptographicBuffer.CreateFromByteArray(hash));

                Windows.Storage.Streams.IBuffer Buffer = CryptographicBuffer.DecodeFromBase64String(data);
                byte[] Decrypted;
                CryptographicBuffer.CopyToByteArray(CryptographicEngine.Decrypt(AES, Buffer, null), out Decrypted);
                dataOut = System.Text.Encoding.UTF8.GetString(Decrypted, 0, Decrypted.Length);

            }
            catch (Exception ex)
            {
                dataOut = "";
                return getErrorCode() == 0 ? 1 : 0;
            }

            return getErrorCode() == 0 ? 1 : 0;
        }

        public int setDbCryptKey(String partition, String key, bool bPersistent)
        {
            Windows.Storage.Streams.IBuffer keyByte = Encoding.UTF8.GetBytes(key).AsBuffer();
            DataProtectionProvider protecotor = new DataProtectionProvider();
            IAsyncOperation<IBuffer> task = protecotor.ProtectAsync(keyByte);
            task.AsTask().Wait();
            Windows.Storage.Streams.IBuffer protectedKeyByte = task.GetResults(); 
            this.writeKeyToFile(partition, protectedKeyByte.ToArray());        
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
            using (IsolatedStorageFileStream writestream = new IsolatedStorageFileStream(filePath, System.IO.FileMode.Create, System.IO.FileAccess.Write, file))
            {
                using (Stream writer = new StreamWriter(writestream).BaseStream)
                {
                    writer.Write(key, 0, key.Length);
                    writer.Flush();
                }    
            }
        }

        private byte[] readKeyFromFile(String filePath)
        {
            IsolatedStorageFile file = IsolatedStorageFile.GetUserStoreForApplication();
            byte[] key = null;
            using (IsolatedStorageFileStream readstream = new IsolatedStorageFileStream(filePath, System.IO.FileMode.Open, FileAccess.Read, file))
            {
                using (Stream reader = new StreamReader(readstream).BaseStream)
                {
                    key = new byte[reader.Length];
                    reader.Read(key, 0, key.Length);
                }
            }

            return key;
        }
    }
}
