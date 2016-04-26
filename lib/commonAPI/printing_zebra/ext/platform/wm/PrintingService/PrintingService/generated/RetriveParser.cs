using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO;

using ZSDK_API.ApiException;
using ZSDK_API.Comm;
using ZSDK_API.Sgd;

using rho.collections;
using System.Diagnostics;
using System.Threading;

namespace rho
{
    class RetriveParser
    {
        private ZebraPrinterConnection m_connection = null;

        public RetriveParser(ZebraPrinterConnection connection)
        {
            m_connection = connection;
        }

        public string getAnswerFromPrinter()
        {
            if (m_connection != null && m_connection.IsConnected())
            {
                
                string result = null;

                for (int i = 0; i < 5 ; i++)
                {
                    result = SGD.GET("file.dir", m_connection);

                    Thread.Sleep(500);

                    if (result != null && result.Length != 0)
                        break;
                }

                return result;
            }

            return null;
        }

        public List<string> getAllFileNames()
        {
            List<string> result = new List<string>();

            string answerStr = getAnswerFromPrinter();

            Debug.WriteLine("printer files: " + answerStr);

            if (answerStr != null)
            {
                List<string> rawList = answerStr.Split('\n').ToList<string>();

                foreach (string fileRecord in rawList)
                {
                    if (fileRecord.Length <= 0)
                        continue;

                    string fileNameRow = fileRecord.Remove(fileRecord.IndexOf("\r"), 1).Trim();

                    if (fileNameRow.IndexOf(" ") < 0)
                        continue;

                    int fNameSize = fileNameRow.Length - fileNameRow.IndexOf(" ");

                    string fileName = fileNameRow.Remove(fileNameRow.IndexOf(" "), fNameSize);

                    if (fileName.IndexOf(".") < 0)
                        continue;

                    result.Add(fileName);
                }
            }

            return result;
        }

        public List<string> getAllFileNames(IReadOnlyList<string> extensions)
        {
            List<string> result = new List<string>();
            List<string> fList  = getAllFileNames();
              
            foreach (string fileName in fList)
            {
                Debug.WriteLine("file - " + fileName);

                foreach (string ext in extensions)
                {
                    string needExt = "." + ext;

                    if (Path.GetExtension(fileName) == needExt)
                    {
                        Debug.WriteLine("ext - " + fileName + " file - " + fileName);

                        result.Add(fileName);
                        break;
                    }
                }
            }

            return result;
        }
    }
}
