using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.IO;

using rho.protocol.shared;

using ZSDK_API.Discovery;
using ZSDK_API.ApiException;
using ZSDK_API.Comm;
using ZSDK_API.Printer;
using ZSDK_API.Sgd;
using Newtonsoft.Json;

using PrintingService;

namespace rho
{
namespace PrinterZebraImpl
{
    public class DiscoveryPrintersJob
    {
        private const int connettionAttempts = 5;

        public String                connectionType { set; get; }
        public String                deviceAdress { set; get; }
        public Int32                 devicePort { set; get; }
        public bool                  isSearchStopped { get; set; }
        public IMethodResult         oResult { get; set; }
        public PrinterZebraSingleton zebraSingleton { get; set; }

        public void Close()
        {
        }

        public void Connect(Int32 waitTimeout)
        {
            Logger.Write("start discovery job with timeout");
            Logger.Write("Timeout: " + waitTimeout);

            Thread thread = new Thread(RunWithCatch);

            thread.Start();

            if (!thread.Join(waitTimeout))
            {
                Logger.Write("exit from discovery job by timeout");
                Close();
            }

            Logger.Write("end discovery job with timeout");
        }

        public void Connect()
        {
            Logger.Write("start discovery job with timeout");

            Thread thread = new Thread(RunWithCatch);

            thread.Start();
            thread.Join();

            Logger.Write("end discovery job without timeout");
        }

        private void RunWithCatch()
        {
            try
            {
                Run();
            }
            catch (ZebraGeneralException ex)
            {
                Logger.Write("discovery exception [general]: " + ex.Message);            	
            }
            catch(ZebraPrinterConnectionException ex)
            {
                Logger.Write("discovery exception [connection]: " + ex.Message);
            }
            catch(Exception ex)
            {
                Logger.Write("discovery exception [system]: " + ex.Message);
            }
        }

        private void searchWiFi()
        {
            Logger.Write("Start search TCP printers");

            // process connection to TCP address	
            PrinterZebra.EPrinterConnectionType connType = PrinterZebra.EPrinterConnectionType.eTCP;

            if (deviceAdress != null && devicePort > 0 && !PrinterManager.Instance.hasPrinter(deviceAdress))
            {
                Logger.Write("searching in address: " + deviceAdress);

                ConnecttionJob job = zebraSingleton.tryToConnect(devicePort, deviceAdress, ZebraConstants.connectionTimeout, connType);

                if (job.Connection != null)
                {
                    Logger.Write("Found printer on address: " + deviceAdress);

                    PrinterManager.Instance.addPrinterWithID(deviceAdress, devicePort, connType);

                    if (!isSearchStopped)
                    {
                        zebraSingleton.sendConnectResult(job.FriendlyName, deviceAdress, devicePort, connType, oResult);
                    }

                    job.Close();
                }
            }
            else
            {
                Logger.Write("Start search in TCP network");

                DiscoveredPrinter[] printers = null;

                for (int attempt = 0; attempt < connettionAttempts; attempt++)
                {
                    printers = NetworkDiscoverer.LocalBroadcast();

                    if (printers.Length > 0)
                    {
                        break;
                    }

                    Thread.Sleep(500);
                }

                if (printers.Length == 0)
                {
                    for (int attempt = 0; attempt < connettionAttempts; attempt++)
                    {
                        printers = NetworkDiscoverer.Multicast(5);

                        if (printers.Length > 0)
                        {
                            break;
                        }

                        Thread.Sleep(500);
                    }
                }

                LogDiscoveryPrinters(printers);

                foreach (DiscoveredPrinter printer in printers)
                {
                    if (isSearchStopped)
                        break;

                    Logger.Write("searching in address: " + printer.Address);

                    if ((printer is DiscoveredPrinterNetwork) && !PrinterManager.Instance.hasPrinter(printer.Address))
                    {
                        DiscoveredPrinterNetwork networkPrinter = (DiscoveredPrinterNetwork)printer;

                        ConnecttionJob job = zebraSingleton.tryToConnect(networkPrinter.Port, networkPrinter.Address, ZebraConstants.connectionTimeout, connType);

                        if (job.Connection != null)
                        {
                            Logger.Write("Found printer on address: " + printer.Address);

                            PrinterManager.Instance.addPrinterWithID(networkPrinter.Address, networkPrinter.Port, connType);

                            zebraSingleton.sendConnectResult(job.FriendlyName, networkPrinter.Address, networkPrinter.Port, connType, oResult);

                            job.Close();
                        }
                    }
                }
            }
        }

        private void searchBluetooth()
        {
            Logger.Write("Start search Bluetooth printers");

            // process connection to Bluetooth
            PrinterZebra.EPrinterConnectionType connType = PrinterZebra.EPrinterConnectionType.eBluetooth;

            if (deviceAdress != null && !PrinterManager.Instance.hasPrinter(deviceAdress))
            {
                Logger.Write("Start search in address: " + deviceAdress);

                ConnecttionJob job = zebraSingleton.tryToConnect(0, deviceAdress, ZebraConstants.connectionTimeout, connType);

                if (job.Connection != null)
                {
                    PrinterManager.Instance.addPrinterWithID(deviceAdress, 0, connType);

                    if (!isSearchStopped)
                    {
                        zebraSingleton.sendConnectResult(job.FriendlyName, deviceAdress, 0, connType, oResult);
                    }

                    job.Close();
                }
            }
            else
            {
                DiscoveredPrinter[] printers = null;

                for (int attempt = 0; attempt < connettionAttempts; attempt++)
                {
                    printers = BluetoothDiscoverer.FindPrinters();

                    if (printers.Length > 0)
                    {
                        break;
                    }

                    Thread.Sleep(500);
                }

                LogDiscoveryPrinters(printers);

                foreach (DiscoveredPrinter printer in printers)
                {
                    Logger.Write("searching in address: " + printer.Address);

                    if (PrinterManager.Instance.hasPrinter(printer.Address))
                        continue;

                    if (isSearchStopped == true)
                        return;

                    Logger.Write("Found printer on address: " + printer.Address);

                    //zebraSingleton.sendConnectResult(job.FriendlyName, printer.Address, 0, connType, oResult);
                    zebraSingleton.sendConnectResult(printer.Address, printer.Address, 0, connType, oResult);
                }
            }
        }

        private void searchUsb()
        {
            Logger.Write("Start search Usb printers");

            // process connection to Usb
            PrinterZebra.EPrinterConnectionType connType = PrinterZebra.EPrinterConnectionType.eUSB;

            if (deviceAdress != null && !PrinterManager.Instance.hasPrinter(deviceAdress))
            {
                Logger.Write("Start search in address: " + deviceAdress);

                ConnecttionJob job = zebraSingleton.tryToConnect(0, deviceAdress, ZebraConstants.connectionTimeout, connType);

                if (job.Connection != null)
                {
                    PrinterManager.Instance.addPrinterWithID(deviceAdress, 0, connType);

                    if (!isSearchStopped)
                    {
                        zebraSingleton.sendConnectResult(job.FriendlyName, deviceAdress, 0, connType, oResult);
                    }

                    job.Close();
                }
            }
            else
            {
                List<String> usbPrinterAddresses = new List<String>();

                usbPrinterAddresses.Add("LPT1:");
                usbPrinterAddresses.Add("LPT2:");

                foreach (String printerAddress in usbPrinterAddresses)
                {
                    if (isSearchStopped)
                        break;                    

                    Logger.Write("searching in address: " + printerAddress);

                    if (!PrinterManager.Instance.hasPrinter(printerAddress))
                    {
                        ConnecttionJob job = zebraSingleton.tryToConnect(0, printerAddress, ZebraConstants.connectionTimeout, connType);

                        if (job.Connection != null)
                        {
                            Logger.Write("Found printer on address: " + printerAddress);

                            PrinterManager.Instance.addPrinterWithID(printerAddress, 0, connType);

                            zebraSingleton.sendConnectResult(job.FriendlyName, printerAddress, 0, connType, oResult);

                            job.Close();
                        }
                    }
                }
            }
        }

        private void Run()
        {
             if (connectionType.Equals(ZebraConstants.CONNECTION_TYPE_USB) || connectionType.Equals(ZebraConstants.CONNECTION_TYPE_ANY))
            {
                searchUsb();
            }
            
            if (connectionType.Equals(ZebraConstants.CONNECTION_TYPE_TCP) || connectionType.Equals(ZebraConstants.CONNECTION_TYPE_ANY))
            {
                searchWiFi();    
            }

            if (connectionType.Equals(ZebraConstants.CONNECTION_TYPE_BLUETOOTH) || connectionType.Equals(ZebraConstants.CONNECTION_TYPE_ANY))
            {
                searchBluetooth();
            }
        }

        private void LogDiscoveryPrinters(DiscoveredPrinter[] printers)
        {
            Logger.Write("Found " + printers.Length.ToString() + " printers");

            foreach (DiscoveredPrinter printer in printers)
            {
                Logger.Write("\t- " + printer.Address);
            }
        }
    }
}
}
