using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System.Threading;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Windows.Media.Imaging;
using System.Windows.Resources;
using Microsoft.Devices;
using Microsoft.Phone.Tasks;
using rhoruntime;
using ZXing;

namespace rhodes
{
    public partial class CameraNavigatePage : PhoneApplicationPage
    {
        public CameraNavigatePage()
        {
            InitializeComponent();
        }

        private PhotoCamera     _phoneCamera;
        private IBarcodeReader  _barcodeReader;
        private DispatcherTimer _scanTimer;
        private WriteableBitmap _previewBuffer;

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            // Initialize the camera object
            _phoneCamera = new PhotoCamera();
            _phoneCamera.Initialized += Camera_Initialized;
            CameraButtons.ShutterKeyHalfPressed += CameraButtons_ShutterKeyHalfPressed;

            //Display the camera feed in the UI
            viewfinderBrush.SetSource(_phoneCamera);
            
            // This timer will be used to scan the camera buffer every 250ms and scan for any barcodes
            _scanTimer          = new DispatcherTimer();
            _scanTimer.Interval = TimeSpan.FromMilliseconds(250);
            _scanTimer.Tick += (o, arg) => ScanForBarcode();

            base.OnNavigatedTo(e);
        }

        void CameraButtons_ShutterKeyHalfPressed(object sender, EventArgs e)
        {
            _phoneCamera.Focus();
        }

        protected override void OnNavigatingFrom(System.Windows.Navigation.NavigatingCancelEventArgs e)
        {
            //we're navigating away from this page, we won't be scanning any barcodes
            _scanTimer.Stop();

            if (_phoneCamera != null)
            {
                // Cleanup
                _phoneCamera.Dispose();
                _phoneCamera.Initialized -= Camera_Initialized;
                CameraButtons.ShutterKeyHalfPressed -= CameraButtons_ShutterKeyHalfPressed;
            }

            if (OpticalReaderTask.TaskPending)
            {
                if (e.NavigationMode == NavigationMode.Back)
                {
                    OpticalReaderTask.CancelTask(true);
                }
                else
                {
                    OpticalReaderTask.CancelTask(false);
                }
            }
        }

        void Camera_Initialized(object sender, Microsoft.Devices.CameraOperationCompletedEventArgs e)
        {
            if (e.Succeeded)
            {
                this.Dispatcher.BeginInvoke(delegate()
                {
                    _phoneCamera.FlashMode = FlashMode.Off;

                    _previewBuffer         = new WriteableBitmap((int)_phoneCamera.PreviewResolution.Width, 
                                                                 (int)_phoneCamera.PreviewResolution.Height);

                    _barcodeReader = new BarcodeReader();

                    _barcodeReader.TryHarder = true;
                    _barcodeReader.ResultFound += BarcodeReader_ResultFound;
                    _scanTimer.Start();
                });
            }
            else
            {
                Dispatcher.BeginInvoke(() =>
                {
                    MessageBox.Show("Unable to initialize the camera");
                });
            }
        }

        void BarcodeReader_ResultFound(Result obj)
        {
            ProcessResult processResult = new ProcessResult();
            processResult.Data   = obj.RawBytes;
            processResult.Format = obj.BarcodeFormat.ToString();
            processResult.Text   = obj.Text;

            OpticalReaderTask.CompleteTask(processResult, null);
        }

        private void ScanForBarcode()
        {
            //grab a camera snapshot
            _phoneCamera.GetPreviewBufferArgb32(_previewBuffer.Pixels);
            _previewBuffer.Invalidate();

            //scan the captured snapshot for barcodes
            //if a barcode is found, the ResultFound event will fire
            _barcodeReader.Decode(_previewBuffer);
        }
    }
}