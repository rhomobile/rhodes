using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.Text;

using rho.protocol.shared;
using rho.collections;

using ZSDK_API.ApiException;
using ZSDK_API.Comm;
using ZSDK_API.Printer;
using Newtonsoft.Json;

using PrintingService;

namespace rho 
{
namespace PrinterZebraImpl
{   
    public class PrinterZebra
    {
        public enum EPrinterConnectionType
        {
            eBluetooth,
            eTCP,
            eUSB,
            eOnBoard,
        }

        private ZebraPrinterConnection m_connection = null;
        private ZebraPrinter           m_printer = null;

        private int m_maxTimeoutForRead     = 0;
        private int m_timeToWaitForMoreData = 0;

        public string ID { get; set; }

        public Int32 Port { get; set; }

        public EPrinterConnectionType connectionType { get; set; }

        public MainForm applicationForm { get; set; }

        private PrinterStatus getStatus()
        {
            object[] args = new object[] { connectionType, ID, Port };

            object result = applicationForm.Invoke(new PrintingService.MainForm.GetPrinterStatusCallback(applicationForm.getPrinterStatus), args);

            return (PrinterStatus)result;
        }

        /// common functionality 
        public void connect(IMethodResult oResult)
        {
            Logger.Write("connect call");

            IReadOnlyDictionary<string, string> options = new IReadOnlyDictionary<string, string>();
            connectWithOptions(options, oResult);
        }

        public void connectWithOptions(IReadOnlyDictionary<string, string> options, IMethodResult oResult)
        {
            Logger.Write("connectWithOptions call");
            Logger.Write("options:", options);

            string valueObj = null;
            Int32  connectionTimeout = 0;

            if (m_connection != null && m_connection.IsConnected() && m_printer != null)
            {
                m_connection.Close();

                Thread.Sleep(1000);

                m_connection = null;
                m_printer    = null;
            }

            if (options.ContainsKey(ZebraConstants.HK_TIMEOUT))
            {
                valueObj = options[ZebraConstants.HK_TIMEOUT];

                if ((valueObj != null) && (valueObj is String))
                {
                    try
                    {
                        connectionTimeout = Int32.Parse(valueObj.ToString());
                    }
                    catch (System.FormatException)
                    {
                        m_maxTimeoutForRead = 0;
                    }
                }
            }

            if (options.ContainsKey(ZebraConstants.PROPERTY_MAX_TIMEOUT_FOR_READ))
            {
                valueObj = options[ZebraConstants.PROPERTY_MAX_TIMEOUT_FOR_READ];
                
                if ((valueObj != null) && (valueObj is String))
                {
                    try
                    {
                        m_maxTimeoutForRead = Int32.Parse(valueObj.ToString());
                    }
                    catch (System.FormatException)
                    {
                        m_maxTimeoutForRead = 0;
                    }
                }
            }

            if (options.ContainsKey(ZebraConstants.PROPERTY_TIME_TO_WAIT_FOR_MORE_DATA))
            {
                valueObj = options[ZebraConstants.PROPERTY_TIME_TO_WAIT_FOR_MORE_DATA];

                if ((valueObj != null) && (valueObj is String))
                {
                    try
                    {
                        m_timeToWaitForMoreData = Int32.Parse(valueObj.ToString());
                    }
                    catch (System.FormatException)
                    {
                        m_maxTimeoutForRead = 0;
                    }
                }
            }
            
            ConnecttionJob job = new ConnecttionJob();

            job.Address               = ID;
            job.Port                  = Port;
            job.ConnectionType        = connectionType;
            job.MaxTimeoutForRead     = m_maxTimeoutForRead;
            job.TimeToWaitForMoreData = m_timeToWaitForMoreData;

            if (connectionTimeout == 0)
            {
                job.Connect();
            }
            else
            {
                if (!job.Connect(connectionTimeout))
                {
                    oResult.set(ZebraConstants.PRINTER_STATUS_ERR_TIMEOUT);
                    return;
                }
            }
            
            if (job.Connection != null && job.Printer != null)
            {
                m_connection = job.Connection;
                m_printer    = job.Printer;

                oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
                return;
            }

            oResult.set(ZebraConstants.PRINTER_STATUS_ERR_NOT_CONNECTED);
        }

        public void disconnect(IMethodResult oResult)
        {
            Logger.Write("disconnect call");

            if (m_connection != null && m_connection.IsConnected())
            {
                Thread.Sleep(1000);
                m_connection.Close();
                Thread.Sleep(1000);
            }

            m_connection = null;
            m_printer = null;

            oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
        }
        
        public void getIsConnected(IMethodResult oResult)
        {
            Logger.Write("getIsConnected call");

            if (m_connection != null)
            {
                oResult.set(m_connection.IsConnected());
            }
            else
            {
                oResult.set(false);
            }
        }

        public void printFile(string fileURI, IReadOnlyDictionary<string, string> options, IMethodResult oResult)
        {
            Logger.Write("printFile call");
            Logger.Write("fileURI: " + fileURI);
            Logger.Write("options:", options);

            if (m_connection != null && m_printer != null)
            {
                try
                {
                    if (Path.GetExtension(fileURI) == ".jpg" || Path.GetExtension(fileURI) == ".png")
                    {
                        m_printer.GetGraphicsUtil().PrintImage(fileURI, 0, 0);
                        oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
                        return;
                    }
                }
                catch (Exception ex)
                {
                }
            }

            oResult.set(ZebraConstants.PRINTER_STATUS_ERR_NOT_CONNECTED);
        }

        public void printRawString(string command, IReadOnlyDictionary<string, string> options, IMethodResult oResult)
        {
            Logger.Write("printRawString call: " + command);
            Logger.Write("command: " + command);
            Logger.Write("options:", options);

            if (m_connection != null && m_printer != null)
            {
                try
                {                    
                    byte[] commandStr = Encoding.UTF8.GetBytes(command);
                    m_connection.Write(commandStr);
                    
                    Thread.Sleep(500);
                                        
                    byte[] printerResponse = m_connection.Read();
                    string printerResponseStr = Encoding.UTF8.GetString(printerResponse, 0, printerResponse.Length);

                    IReadOnlyDictionary<string, string> response = new IReadOnlyDictionary<string, string>();

                    response.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_SUCCESS);
                    response.Add(ZebraConstants.HK_STRING_RESPONCE, printerResponseStr);

                    oResult.set(response);
                }
                catch (Exception ex)
                {
                    IReadOnlyDictionary<string, string> errResponse = new IReadOnlyDictionary<string, string>();

                    errResponse.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_ERROR);
                    errResponse.Add(ZebraConstants.HK_MESSAGE, ex.Message.ToString());

                    oResult.set(errResponse);
                }
            }
            else
            {
                oResult.set(ZebraConstants.PRINTER_STATUS_ERR_NOT_CONNECTED);
            }
        }

        public void getDeviceName(IMethodResult oResult)
        {
            Logger.Write("getDeviceName call");

            if (m_connection != null && m_connection is BluetoothPrinterConnection)
            {
                BluetoothPrinterConnection conn = (BluetoothPrinterConnection)m_connection;
                oResult.set(conn.GetFriendlyName());
            }
        }

        public void enumerateSupportedControlLanguages(IMethodResult oResult)
        {
            Logger.Write("enumerateSupportedControlLanguages call");

            if (m_printer != null)
            {
                IReadOnlyDictionary<string, string> controlLanguage = new IReadOnlyDictionary<string, string>();

                switch (m_printer.GetPrinterControlLanguage())
                {
                    case PrinterLanguage.CPCL:
                        controlLanguage.Add(ZebraConstants.PRINTER_LANGUAGE_CPCL, "");
                        break;
                    case PrinterLanguage.ZPL:
                        controlLanguage.Add(ZebraConstants.PRINTER_LANGUAGE_ZPL, "");
                        controlLanguage.Add(ZebraConstants.PRINTER_LANGUAGE_CPCL, "");
                        break;
                }

                oResult.set(controlLanguage);
                return;
            }

            oResult.set(ZebraConstants.PRINTER_STATUS_ERR_NOT_CONNECTED);
        }

        public void printImageFromFile(string path, int x, int y, IReadOnlyDictionary<string, string> options, IMethodResult oResult)
        {
            Logger.Write("printImageFromFile call");
            Logger.Write("path: " + path);
            Logger.Write("x: " + x);
            Logger.Write("y: " + y);
            Logger.Write("options:", options);

            object valueObj = null;
            int imgWitdh = -1;
            int imgHeight = -1;
            bool insideFormat = false;

            if (m_connection != null && m_connection.IsConnected() && m_printer != null)
            {
                if (options.ContainsKey(ZebraConstants.HK_WIDTH))
                {
                    valueObj = options[ZebraConstants.HK_WIDTH];

                    if ((valueObj != null) && (valueObj is String))
                    {
                        try
                        {
                            imgWitdh = Int32.Parse(valueObj.ToString());
                        }
                        catch (System.FormatException) 
                        {
                        }
                    }
                }

                if (options.ContainsKey(ZebraConstants.HK_HEIGHT))
                {
                    valueObj = options[ZebraConstants.HK_HEIGHT];

                    if ((valueObj != null) && (valueObj is String))
                    {
                        try 
                        {
                            imgHeight = Int32.Parse(valueObj.ToString());
                        }
                        catch (System.FormatException) 
                        {
                        }
                    }
                }

                if (options.ContainsKey(ZebraConstants.HK_IS_INSIDE_FORMAT))
                {
                    valueObj = options[ZebraConstants.HK_IS_INSIDE_FORMAT];

                    if ((valueObj != null) && (valueObj is String))
                    {
                        try
                        {
                            insideFormat = Boolean.Parse(valueObj.ToString());
                        }
                        catch(System.FormatException)
                        {
                        }
                    }
                }

                if (Path.GetExtension(path) == ".jpg" || Path.GetExtension(path) == ".png")
                {
                    if (imgHeight == -1 && imgWitdh == -1)
                    {
                        m_printer.GetGraphicsUtil().PrintImage(path, x, y);
                        
                        Logger.Write("printImageFromFile call PRINTER_STATUS_SUCCESS");

                        oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
                        return;
                    }
                    else if (imgHeight != 0 && imgWitdh != 0)
                    {
                        m_printer.GetGraphicsUtil().PrintImage(path, x, y, imgWitdh, imgHeight, insideFormat);

                        Logger.Write("printImageFromFile call PRINTER_STATUS_SUCCESS");

                        oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
                        return;
                    }
                }
            }
            else
            {
                Logger.Write("printImageFromFile call PRINTER_STATUS_ERR_NOT_CONNECTED");

                oResult.set(ZebraConstants.PRINTER_STATUS_ERR_NOT_CONNECTED);
                return;
            }

            Logger.Write("printImageFromFile call PRINTER_STATUS_ERROR");

            oResult.set(ZebraConstants.PRINTER_STATUS_ERROR);
        }
        
        public void sendFileContents(string filePath, IMethodResult oResult)
        {
            Logger.Write("sendFileContents call");
            Logger.Write("fileParh: " + filePath);

            if (m_connection != null && m_connection.IsConnected() && m_printer != null)
            {
                m_printer.GetFileUtil().SendFileContents(filePath);

                oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
            }
            else
            {
                oResult.set(ZebraConstants.PRINTER_STATUS_ERR_NOT_CONNECTED);
            }
        }

        public void retrieveFileNames(IMethodResult oResult)
        {
            Logger.Write("call retrieveFileNames ");

            IReadOnlyDictionary<string, object> resultHash = new IReadOnlyDictionary<string, object>();

            if (m_connection != null && m_connection.IsConnected())
            {
                RetriveParser parser = new RetriveParser(m_connection);

                List<string> resultNames = parser.getAllFileNames();
                
                resultHash.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_SUCCESS);
                resultHash.Add(ZebraConstants.HK_FILE_NAMES, resultNames);
                oResult.set(resultHash);

                return;
            }

            resultHash.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_ERROR);
            resultHash.Add(ZebraConstants.HK_FILE_NAMES, new List<string>());
            oResult.set(resultHash);
        }

        public void retrieveFileNamesWithExtensions(IReadOnlyList<string> extensions, IMethodResult oResult)
        {
            Logger.Write("retrieveFileNamesWithExtensions call");
            Logger.Write("extensions:", extensions);

            IReadOnlyDictionary<string, object> resultHash = new IReadOnlyDictionary<string, object>();

            if (m_printer != null)
            {
                RetriveParser parser = new RetriveParser(m_connection);

                List<string> resultNames = parser.getAllFileNames(extensions);

                resultHash.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_SUCCESS);
                resultHash.Add(ZebraConstants.HK_FILE_NAMES, resultNames);
                oResult.set(resultHash);

                return;
            }

            resultHash.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_ERROR);
            resultHash.Add(ZebraConstants.HK_FILE_NAMES, new List<string>());
            oResult.set(resultHash);
        }

        public void printStoredFormatWithHash(string formatPathOnPrinter, IReadOnlyDictionary<string, string> vars, IMethodResult oResult)
        {
            Logger.Write("printStoredFormatWithHash call");
            Logger.Write("formatPathOnPrinter: " + formatPathOnPrinter);
            Logger.Write("vars: ", vars);

            if (m_printer != null)
            {
                Dictionary<int, string> _params = new Dictionary<int, string>();

                foreach (KeyValuePair<string, string> pair in vars)
                {
                    try 
                    {
                        _params.Add(Int32.Parse(pair.Key), pair.Value);
                    }
                    catch(Exception e) 
                    {
                    }
                }

                m_printer.GetFormatUtil().PrintStoredFormat(formatPathOnPrinter, _params);
                oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
                return;
            }

            oResult.set(ZebraConstants.PRINTER_STATUS_ERROR);
        }

        public void printStoredFormatWithArray(string formatPathOnPrinter, IReadOnlyList<string> vars, IMethodResult oResult)
        {
            Logger.Write("printStoredFormatWithArray call");
            Logger.Write("formatPathOnPrinter: " + formatPathOnPrinter);
            Logger.Write("vars: ", vars);

            if (m_printer != null)
            {
                m_printer.GetFormatUtil().PrintStoredFormat(formatPathOnPrinter, vars.ToArray());
                oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
                return;
            }

            oResult.set(ZebraConstants.PRINTER_STATUS_ERROR);
        }

        public void storeImage(string printerDriveAndFileName, string imageFilePathOnDevice, int width, int height, IMethodResult oResult)
        {
            Logger.Write("storeImage call");
            Logger.Write("printerDriveAndFileName: " + printerDriveAndFileName);
            Logger.Write("imageFilePathOnDevice: " + imageFilePathOnDevice);
            Logger.Write("width: " + width.ToString());
            Logger.Write("height: " + height.ToString());

            try
            {
                if (m_printer != null && width != 0 && height != 0 &&
                    Path.GetExtension(imageFilePathOnDevice) != ".bmp" && Path.GetExtension(imageFilePathOnDevice) != ".gif")
                {
                    m_printer.GetGraphicsUtil().StoreImage(printerDriveAndFileName, imageFilePathOnDevice, width, height);
                    oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
                    return;
                }
            }
            catch(ZebraPrinterConnectionException)
            {
                oResult.set(ZebraConstants.PRINTER_STATUS_ERR_NETWORK);
                return;
            }
            catch(ZebraIllegalArgumentException)
            {
                oResult.set(ZebraConstants.PRINTER_STATUS_ERR_IO);
                return;
            }
            catch(ArgumentException)
            {
                oResult.set(ZebraConstants.PRINTER_STATUS_ERR_PARAM);
                return;
            }
            catch (Exception)
            {
                oResult.set(ZebraConstants.PRINTER_STATUS_ERROR);
                return;
            }

            oResult.set(ZebraConstants.PRINTER_STATUS_ERROR);
        }

        public void requestState(IReadOnlyList<string> listOfParameters, IMethodResult oResult)
        {
            Logger.Write("requestState call");
            Logger.Write("listOfParameters:", listOfParameters);

            IReadOnlyDictionary<string, object> resultHash = new IReadOnlyDictionary<string, object>();

            if (m_connection != null && m_connection.IsConnected() && m_printer != null)
            {
                PrinterStatus currStatus = getStatus();
                
                if (currStatus != null)
                {
                    resultHash.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_SUCCESS);
                    resultHash.Add(ZebraConstants.HK_MESSAGE, "");

                    foreach (string parameter in listOfParameters)
                    {
                        switch(parameter)
                        {
                        case ZebraConstants.PRINTER_STATE_IS_HEAD_COLD:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_HEAD_COLD, currStatus.IsHeadCold);
                            break;
                        case ZebraConstants.PRINTER_STATE_IS_HEAD_OPEN:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_HEAD_OPEN, currStatus.IsHeadOpen);
                            break;
                        case ZebraConstants.PRINTER_STATE_IS_HEAD_TOO_HOT:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_HEAD_TOO_HOT, currStatus.IsHeadTooHot);
                            break;
                        case ZebraConstants.PRINTER_STATE_IS_PAPER_OUT:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_PAPER_OUT, currStatus.IsPaperOut);
                            break;
                        case ZebraConstants.PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS, currStatus.IsPartialFormatInProgress);
                            break;
                        case ZebraConstants.PRINTER_STATE_IS_PAUSED:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_BATTERY_LOW, currStatus.IsPaused);
                            break;
                        case ZebraConstants.PRINTER_STATE_IS_READY_TO_PRINT:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_READY_TO_PRINT, currStatus.IsReadyToPrint);
                            break;
                        case ZebraConstants.PRINTER_STATE_IS_RECEIVE_BUFFER_FULL:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_RECEIVE_BUFFER_FULL, currStatus.IsReceiveBufferFull);
                            break;
                        case ZebraConstants.PRINTER_STATE_IS_RIBBON_OUT:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_IS_RIBBON_OUT, currStatus.IsRibbonOut);
                            break;
                        case ZebraConstants.PRINTER_STATE_LABELS_REMAINING_IN_BATCH:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_LABELS_REMAINING_IN_BATCH, currStatus.LabelsRemainingInBatch);
                            break;
                        case ZebraConstants.PRINTER_STATE_LABEL_LENGTH_IN_DOTS:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_LABEL_LENGTH_IN_DOTS, currStatus.LabelLengthInDots);
                            break;
                        case ZebraConstants.PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER:
                            resultHash.Add(ZebraConstants.PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER, currStatus.NumberOfFormatsInReceiveBuffer);
                            break;
                        case ZebraConstants.PRINTER_STATE_PRINT_MODE:
                            switch (currStatus.PrintMode)
                            {
                                case ZplPrintMode.REWIND:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_REWIND);
                                    break;
                                case ZplPrintMode.PEEL_OFF:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_PEEL_OFF);
                                    break;
                                case ZplPrintMode.TEAR_OFF:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_TEAR_OFF);
                                    break;
                                case ZplPrintMode.CUTTER:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_CUTTER);
                                    break;
                                case ZplPrintMode.APPLICATOR:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_APPLICATOR);
                                    break;
                                case ZplPrintMode.DELAYED_CUT:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_DELAYED_CUT);
                                    break;
                                case ZplPrintMode.LINERLESS_PEEL:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_LINERLESS_PEEL);
                                    break;
                                case ZplPrintMode.LINERLESS_REWIND:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_REWIND);
                                    break;
                                case ZplPrintMode.PARTIAL_CUTTER:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_PARTIAL_CUTTER);
                                    break;
                                case ZplPrintMode.RFID:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_RFID);
                                    break;
                                case ZplPrintMode.KIOSK:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_KIOSK);
                                    break;
                                case ZplPrintMode.UNKNOWN:
                                    resultHash.Add(ZebraConstants.PRINTER_STATE_PRINT_MODE, ZebraConstants.PRINT_MODE_UNKNOWN);
                                    break;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    resultHash.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_ERROR);
                    resultHash.Add(ZebraConstants.HK_MESSAGE, "");
                }
            }
            else
            {
                resultHash.Add(ZebraConstants.HK_STATUS, ZebraConstants.PRINTER_STATUS_ERR_NOT_CONNECTED);
                resultHash.Add(ZebraConstants.HK_MESSAGE, "");
            }

            oResult.set(resultHash);
        }
    }  

    public class PrinterZebraSingleton
    {
        private Int32 getTimeout(IReadOnlyDictionary<string, string> options)
        {
            Object valueObj = null;

            if (options.ContainsKey("timeout"))
            {
                valueObj = options["timeout"];

                if ((valueObj != null) && (valueObj is String))
                {
                    try
                    {
                        return Int32.Parse(valueObj.ToString());
                    }
                    catch (System.FormatException ex)
                    {
                    }
                }
            }

            return 0;
        }

        private string getConnectionType(IReadOnlyDictionary<string, string> options)
        {
            Object valueObj = null;

            if (options.ContainsKey("connectionType"))
            {
                valueObj = options["connectionType"];
                if ((valueObj != null) && (valueObj is String))
                {
                    return (String)valueObj;
                }
            }

            return ZebraConstants.CONNECTION_TYPE_ANY;
        }

        private string getDeviceAdress(IReadOnlyDictionary<string, string> options)
        {
            Object valueObj = null;

            if (options.ContainsKey("deviceAddress"))
            {
                valueObj = options["deviceAddress"];
                if ((valueObj != null) && (valueObj is String))
                {
                    return (String)valueObj;
                }
            }

            return null;
        }

        private Int32 getDevicePort(IReadOnlyDictionary<string, string> options)
        {
            Object valueObj = null;

            if (options.ContainsKey("devicePort"))
            {
                valueObj = options["devicePort"];
                if (valueObj == null && options.ContainsKey("port"))
                {
                    valueObj = options["port"];
                }

                try
                {
                    return Int32.Parse(valueObj.ToString());
                }
                catch (System.FormatException)
                {
                }
            }

            return 6101;
        }

        public void sendConnectFinish(string status, IMethodResult oResult)
        {
            IReadOnlyDictionary<string, string> printerResult = new IReadOnlyDictionary<string, string>();
            printerResult[ZebraConstants.HK_STATUS] = status;
            oResult.set(printerResult);
        }

        public void sendConnectResult(string deviceName, string deviceAdress, Int32 devicePort, PrinterZebra.EPrinterConnectionType connType, IMethodResult oResult)
        {
            IReadOnlyDictionary<string, string> printerResult = new IReadOnlyDictionary<string, string>();

            printerResult[ZebraConstants.HK_STATUS]                = ZebraConstants.PRINTER_STATUS_SUCCESS;
            printerResult[ZebraConstants.HK_PRINTER_ID]            = deviceAdress;
            printerResult[ZebraConstants.PROPERTY_DEVICE_ADDRESS]  = deviceAdress;
            printerResult[ZebraConstants.PROPERTY_DEVICE_PORT]     = devicePort.ToString();
            printerResult[ZebraConstants.PROPERTY_PRINTER_TYPE]    = ZebraConstants.PRINTER_TYPE_ZEBRA;
            printerResult[ZebraConstants.PROPERTY_DEVICE_NAME]     = deviceName;

            if (connType == PrinterZebra.EPrinterConnectionType.eBluetooth)
            {
                printerResult[ZebraConstants.PROPERTY_CONNECTION_TYPE] = ZebraConstants.CONNECTION_TYPE_BLUETOOTH;
            }
            else if (connType == PrinterZebra.EPrinterConnectionType.eTCP)
            {
                printerResult[ZebraConstants.PROPERTY_CONNECTION_TYPE] = ZebraConstants.CONNECTION_TYPE_TCP;
            }
            else if (connType == PrinterZebra.EPrinterConnectionType.eOnBoard)
            {
                printerResult[ZebraConstants.PROPERTY_CONNECTION_TYPE] = ZebraConstants.CONNECTION_TYPE_ON_BOARD;
            }
            else if (connType == PrinterZebra.EPrinterConnectionType.eUSB)
            {
                printerResult[ZebraConstants.PROPERTY_CONNECTION_TYPE] = ZebraConstants.CONNECTION_TYPE_USB;
            }
            
            oResult.set(printerResult);
        }

        public ConnecttionJob tryToConnect(Int32 port, string deviceAdress, int timeout, PrinterZebra.EPrinterConnectionType connType)
        {
            Logger.Write("tryToConnect: " + port.ToString() + ", " + deviceAdress + ", " + timeout.ToString());

            ConnecttionJob job = new ConnecttionJob();

            job.MaxTimeoutForRead     = 0;
            job.TimeToWaitForMoreData = 0;
            job.Port                  = port;
            job.Address               = deviceAdress;
            job.ConnectionType        = connType;

            job.Connect(timeout);

            return job;
        }

        private void tryToConnectInFoundPrinters(IMethodResult oResult)
        {
            IMethodResult result = new SleepMethodResult(500);
            List<string>  badPrinters = new List<string>();

            Logger.Write("tryToConnect start [found printers]");

            List<string> printerKeys = PrinterManager.Instance.getPrintersKeys();

            foreach (string printerKey in printerKeys)
            {
                PrinterZebra printer = PrinterManager.Instance.getPrinter(printerKey);

                string deviceAdress = printer.ID;
                Int32  port         = printer.Port;

                Logger.Write("searching in address [found printers]: " + deviceAdress);

                ConnecttionJob job = tryToConnect(port, deviceAdress, ZebraConstants.connectionTimeout, printer.connectionType);

                if (job.Connection != null)
                {
                    Logger.Write("Found printer on address [found printers]: " + deviceAdress);

                    sendConnectResult(job.FriendlyName, deviceAdress, port, printer.connectionType, oResult);

                    job.Close();
                }
                else
                {
                    Logger.Write("remove printer on address [found printers]: " + deviceAdress + " from cache.");
                    badPrinters.Add(printerKey);
                }
            }

            PrinterManager.Instance.removePrinters(badPrinters);
        }

        public void searchPrinters(IReadOnlyDictionary<string, string> options, IMethodResult oResult)
        {
            Logger.Write("searchPrinters call");
            Logger.Write("options", options);

            tryToConnectInFoundPrinters(oResult);

            DiscoveryPrintersJob job = new DiscoveryPrintersJob();

            Object valueObj       = null;
            int discoveryTimeout  = getTimeout(options);

            Logger.Write("set timeout: " + discoveryTimeout.ToString());

            job.connectionType    = getConnectionType(options);
            job.deviceAdress      = getDeviceAdress(options);
            job.devicePort        = getDevicePort(options);
            job.oResult           = oResult;
            job.zebraSingleton    = this;
            job.isSearchStopped   = false;

            if (options.ContainsKey("printerType"))
            {
                valueObj = options["printerType"];

                if ((valueObj != null) && (valueObj is String))
                {
                    String printerType = (String)valueObj;

                    if (!printerType.Equals(ZebraConstants.PRINTER_TYPE_ZEBRA))
                    {
                        sendConnectFinish(ZebraConstants.PRINTER_STATUS_ERR_UNSUPPORTED, oResult);
                        return;
                    }
                }
            }

            if (discoveryTimeout == 0)
            {
                job.Connect();
            }
            else
            {
                job.Connect(discoveryTimeout);
            }

            job.isSearchStopped = true;

            sendConnectFinish(ZebraConstants.PRINTER_STATUS_SUCCESS, oResult);

            Logger.Write("searchPrinters call end");
        }

        public void stopSearch(IMethodResult oResult)
        {
            oResult.set(ZebraConstants.PRINTER_STATUS_SUCCESS);
        }

        public void closeServiceRequest(IMethodResult oResult)
        {
            Logger.Write("kill printing service");

           // ClientThread.Close();

            //Process.GetCurrentProcess().Kill();
        }
    }
}

}
