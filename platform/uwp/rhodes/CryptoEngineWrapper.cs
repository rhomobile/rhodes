using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using rhodes.common;

namespace rhodes
{
    public class CryptoEngineWrapper : rhoruntime.ICryptoEngine
    {
        private RhoCrypt _rhoCrypt;
        public CryptoEngineWrapper(RhoCrypt rhoCrypt) { _rhoCrypt = rhoCrypt; }

        public int dbDecrypt(string partition, int size, string data, out string decryptedData)
        {
            return _rhoCrypt.dbDecrypt(partition, size, data, out decryptedData);
        }

        public int dbEncrypt(string partition, int size, string data, out string dataOut)
        {
            return _rhoCrypt.dbEncrypt(partition, size, data, out dataOut);
        }

        public int setDbCryptKey(string partition, string key, bool bPersistent)
        {
            return _rhoCrypt.setDbCryptKey(partition, key, bPersistent);
        }
    }
}
