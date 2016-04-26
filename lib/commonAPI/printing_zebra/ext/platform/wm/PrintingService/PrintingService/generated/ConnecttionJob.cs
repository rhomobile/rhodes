using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.Text;

using ZSDK_API.ApiException;
using ZSDK_API.Comm;
using ZSDK_API.Printer;
using ZSDK_API.Sgd;

namespace rho 
{
    namespace PrinterZebraImpl
    {
        public class ConnecttionJob
        {
            public string Address { get; set; }
            public Int32 Port { get; set; }
            public ZebraPrinterConnection Connection { get; set; }
            public ZebraPrinter Printer { get; set; }
            public PrinterZebra.EPrinterConnectionType ConnectionType { get; set; }
            public int MaxTimeoutForRead { get; set; }
            public int TimeToWaitForMoreData { get; set; }
            public string FriendlyName { get; set; }

            public bool Connect(Int32 waitTimeout)
            {
                Logger.Write("start connect job with timeout");
                Logger.Write("Address: " + Address);
                Logger.Write("Port: " + Port.ToString());
                Logger.Write("Timeout: " + waitTimeout);

                Thread thread = new Thread(Run);

                thread.Start();

                if (!thread.Join(waitTimeout))
                {
                    Close();
                    return false;
                }

                Logger.Write("end connect job with timeout");

                return true;
            }

            public void Connect()
            {
                Logger.Write("start connect job with timeout");
                Logger.Write("Address: " + Address);
                Logger.Write("Port: " + Port.ToString());

                Thread thread = new Thread(Run);

                thread.Start();
                thread.Join();

                Logger.Write("end connect job without timeout");
            }

            private void Run()
            {
                try
                {
                    if (ConnectionType == PrinterZebra.EPrinterConnectionType.eTCP)
                    {
                        Connection = new TcpPrinterConnection(Address, Port, MaxTimeoutForRead, TimeToWaitForMoreData);
                    }
                    else if (ConnectionType == PrinterZebra.EPrinterConnectionType.eBluetooth)
                    {
                        Connection = new BluetoothPrinterConnection(Address, MaxTimeoutForRead, TimeToWaitForMoreData);
                    }
                    else if (ConnectionType == PrinterZebra.EPrinterConnectionType.eUSB)
                    {
                        Connection = new UsbPrinterConnection(Address);
                    }

                    if (Connection == null)
                    {
                        return;
                    }

                    Connection.Open();

                    if (Connection != null && !Connection.IsConnected())
                    {
                        Close();
                        return;
                    }

                    if (Connection != null)
                    {
                        if (ConnectionType != PrinterZebra.EPrinterConnectionType.eUSB)
                        {
                            SGD.SET("device.languages", "ZPL", Connection);
                        }
                        else
                        {
                            //Do nothing for USB
                        }

                        Printer = ZebraPrinterFactory.GetInstance(PrinterLanguage.ZPL, Connection);

                        if (ConnectionType != PrinterZebra.EPrinterConnectionType.eUSB)
                        {
                            FriendlyName = SGD.GET("device.friendly_name", Connection);
                        }
                        else
                        {
                            FriendlyName = "USB Printer";
                        }

                        if (FriendlyName.Length == 0)
                        {
                            Logger.Write("friendly name is empty, return device name");
                            FriendlyName = Address;
                        }
                    }
                }
                catch (ZebraPrinterConnectionException e)
                {
                    Logger.Write("Connect exception [connection]: " + e.Message);
                    Close();
                }
                catch (ZebraGeneralException e)
                {
                    Logger.Write("Connect exception [general]: " + e.Message);
                    Close();
                }
                catch (Exception e)
                {
                    Logger.Write("Connect exception [system]: " + e.Message);
                    Close();
                }
            }

            public void Close()
            {
                Thread.Sleep(1000);

                if (Connection != null)
                    Connection.Close();

                Thread.Sleep(1000);

                Printer = null;
                Connection = null;
            }
        };
    }
}
