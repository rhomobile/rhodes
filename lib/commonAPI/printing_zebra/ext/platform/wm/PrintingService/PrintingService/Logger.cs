using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Reflection;

using rho.collections;

namespace rho
{
    public static class Logger
    {
        const string logFileName = "log.txt";

        private static readonly TextWriter g_tw;
        private static readonly object g_syncObject = new object();

        static Logger()
        {
            string pathToServiceDir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);

            TextWriter writer = File.CreateText(pathToServiceDir + "\\" + logFileName);
            g_tw = TextWriter.Synchronized(writer);
        }

        
        public static void Write(string logMessage, IReadOnlyList<string> list)
        {
            try
            {
                Log(logMessage, g_tw);

                foreach (string item in list)
                {
                    Log("\t-: " + item, g_tw);
                }
            }
            catch (IOException)
            {
                g_tw.Close();
            }
        }

        public static void Write(string logMessage, IReadOnlyDictionary<string, string> hash)
        {
            try
            {
                Log(logMessage, g_tw);

                foreach (KeyValuePair<string, string> kvItem in hash)
                {
                    Log("\tkey: " + kvItem.Key + " value: " + kvItem.Value, g_tw);
                }
            }
            catch (IOException)
            {
                g_tw.Close();
            }
        }

        public static void Write(string logMessage)
        {
            try
            {
                Log(logMessage, g_tw);
            }
            catch (IOException)
            {
                g_tw.Close();
            }
        }

        public static void Log(string logMessage, TextWriter w)
        {
            lock (g_syncObject)
            {
                w.WriteLine("{0} {1} : {2}", DateTime.Now.ToLongTimeString(), DateTime.Now.ToLongDateString(), logMessage);
                w.Flush();
            }
        }
    }
}
