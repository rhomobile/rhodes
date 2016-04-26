using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

using rho.protocol.shared;
using rho.PrinterZebraImpl;

namespace rho
{
    class PrinterManager
    {
        private static PrinterManager instance;

        private Dictionary<string, PrinterZebra> m_printersCache = new Dictionary<string, PrinterZebra>();

        private PrinterZebraSingleton zebraSingleton = new PrinterZebraSingleton();

        private PrinterManager() {}

        public static PrinterManager Instance
        {
            get 
            {
                if (instance == null)
                {
                    instance = new PrinterManager();
                }

                return instance;
            }
        }

        public PrinterZebra getPrinterByID(string ID)
        {
            if (m_printersCache.ContainsKey(ID))
            {
                return m_printersCache[ID];
            }

            return null;
        }

        public PrinterZebra addPrinterWithID(string ID, Int32 port, PrinterZebra.EPrinterConnectionType type)
        {
            if (m_printersCache.ContainsKey(ID))
            {
                return m_printersCache[ID];
            }

            PrinterZebraImpl.PrinterZebra newPrinter = new PrinterZebraImpl.PrinterZebra();

            newPrinter.ID             = ID;
            newPrinter.Port           = port;
            newPrinter.connectionType = type;

            m_printersCache.Add(ID, newPrinter);

            return newPrinter;
        }

        public bool hasPrinter(string printerAddress)
        {
            return m_printersCache.ContainsKey(printerAddress);
        }

        public PrinterZebra getPrinter(string printerAddress)
        {
            return m_printersCache[printerAddress];
        }

        public List<string> getPrintersKeys()
        {
            List<string> printerKeys = new List<string>();

            foreach (KeyValuePair<string, PrinterZebra> kvItem in m_printersCache)
            {
                printerKeys.Add(kvItem.Key);
            }

            return printerKeys;
        }

        public void disconnectAll()
        {
            IMethodResult result = new SleepMethodResult(500);

            foreach (KeyValuePair<string, PrinterZebra> kvPrinter in m_printersCache)
            {
                PrinterZebra printer = (PrinterZebra)kvPrinter.Value;
                printer.disconnect(result);
            }
        }

        public PrinterZebraSingleton getPrinterSingleton()
        {
            return zebraSingleton;
        }

        public void removePrinters(List<string> printers)
        {
            foreach (string key in printers)
            {
                m_printersCache.Remove(key);
            }  
        }
    }
}
