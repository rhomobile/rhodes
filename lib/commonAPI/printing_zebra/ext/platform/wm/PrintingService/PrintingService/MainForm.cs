using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

using ZSDK_API;
using ZSDK_API.Comm;
using ZSDK_API.Printer;

using rho;

namespace PrintingService
{
    public partial class MainForm : Form
    {
        const int SW_MINIMIZE = 6;

        [System.Runtime.InteropServices.DllImport("coredll.dll")]
        static extern int ShowWindow(IntPtr hWnd, int nCmdShow);

        private rho.server.ClientsManager m_clientManager = null; 

        public delegate PrinterStatus GetPrinterStatusCallback(rho.PrinterZebraImpl.PrinterZebra.EPrinterConnectionType connType, string address, Int32 port);

        public PrinterStatus getPrinterStatus(rho.PrinterZebraImpl.PrinterZebra.EPrinterConnectionType connType, string address, Int32 port)
        {
            Logger.Write("MainWindow.getPrinterStatus call, address=" + address + " port=" + port.ToString());

            PrinterStatus          status = null;
            ZebraPrinterConnection connection = null;
            ZebraPrinter           printer = null;

            if (connType == rho.PrinterZebraImpl.PrinterZebra.EPrinterConnectionType.eTCP)
            {
                connection = new TcpPrinterConnection(address, port);
            }
            else if (connType == rho.PrinterZebraImpl.PrinterZebra.EPrinterConnectionType.eBluetooth)
            {
                connection = new BluetoothPrinterConnection(address);
            }
            else if (connType == rho.PrinterZebraImpl.PrinterZebra.EPrinterConnectionType.eUSB)
            {
                connection = new UsbPrinterConnection(address);
            }
            else
            {
                Logger.Write("MainWindow.getPrinterStatus undifined connection type");
                return null;
            }
            
            connection.Open();

            if (connection.IsConnected())
            {
                printer = ZebraPrinterFactory.GetInstance(PrinterLanguage.ZPL, connection);

                status = printer.GetCurrentStatus();
            }

            connection.Close();

            Logger.Write("MainWindow.getPrinterStatus call finished");

            return status;
        }

        public MainForm()
        {
            InitializeComponent();

            System.Net.ServicePointManager.DefaultConnectionLimit = 10;

            m_clientManager = new rho.server.ClientsManager(this);
            m_clientManager.startThread();
        }
        private void MainForm_Load(object sender, EventArgs e)
        {

            System.Windows.Forms.Timer starttimer = new System.Windows.Forms.Timer();
            starttimer.Interval = 900;
            starttimer.Tick += new EventHandler(starttimer_Tick);
            starttimer.Enabled = true;
           
        }

        void starttimer_Tick(object sender, EventArgs e)
        {
            ((System.Windows.Forms.Timer)sender).Enabled = false;
            this.Hide();
        }
    }
}
