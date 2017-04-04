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
using System.Windows.Controls.Primitives;
//using rhoruntime;
using ZXing;

namespace BarcodeReaderLib
{
    public partial class CameraNavigatePage : PhoneApplicationPage
    {
        public CameraNavigatePage()
        {
            InitializeComponent();
        }

        private Popup           _scanConfirmation = new Popup();
        private PhotoCamera     _phoneCamera;
        private IBarcodeReader  _barcodeReader;
        private DispatcherTimer _scanTimer;
        private WriteableBitmap _previewBuffer;
        static EventWaitHandle  _waitSendResultHandle = new AutoResetEvent(false);
         
        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            // Initialize the camera object
            _phoneCamera = new PhotoCamera();
            _phoneCamera.Initialized += Camera_Initialized;
            
            //Display the camera feed in the UI
            viewfinderBrush.SetSource(_phoneCamera);
            
            // This timer will be used to scan the camera buffer every 250ms and scan for any barcodes
            _scanTimer          = new DispatcherTimer();
            _scanTimer.Interval = TimeSpan.FromMilliseconds(250);
            _scanTimer.Tick += (o, arg) => ScanForBarcode();

            CameraButtons.ShutterKeyHalfPressed += CameraButtons_ShutterKeyHalfPressed;

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

            if (_scanConfirmation.IsOpen)
                _scanConfirmation.IsOpen = false;

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

                    OpticalReaderTask.TaskPending = true;
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

        void ShowPopupTextDialog()
        {
            if (_processResult.Text == null || _processResult.Text.Length == 0)
                return;

            const int dialogHeight = 300;
            const int borderThickness = 7;

            //Stack panel with a black background.
            StackPanel panelConfirmation = new StackPanel();
            panelConfirmation.Background = new SolidColorBrush(Colors.Black);
            panelConfirmation.Width  = Application.Current.Host.Content.ActualWidth - borderThickness*2;
            panelConfirmation.Height = dialogHeight;

            //Create a white border.
            Border border          = new Border();
            border.BorderBrush     = new SolidColorBrush(Colors.White);
            border.BorderThickness = new Thickness(borderThickness);

            //Create a close button to exit popup.
            Button cancelButton  = new Button();
            cancelButton.Content = "Close";
            cancelButton.Margin  = new Thickness(5.0);
            cancelButton.Height  = 120;
            cancelButton.Click += new RoutedEventHandler(cancel_ConfirmationClick);


            Button okButton  = new Button();
            okButton.Content = "Done";
            okButton.Margin  = new Thickness(5.0);
            okButton.Height = 120;
            okButton.Click += new RoutedEventHandler(ok_ConfirmationClick);

            //Create helper text
            TextBlock textblockHelp = new TextBlock();
            textblockHelp.FontSize      = 24;
            textblockHelp.TextAlignment = TextAlignment.Center;
            textblockHelp.Foreground    = new SolidColorBrush(Colors.White);
            textblockHelp.TextWrapping  = TextWrapping.Wrap;
            textblockHelp.Text          = "Barcode: " + _processResult.Text;
            textblockHelp.Margin        = new Thickness(5.0);

            //Add controls to stack panel
            panelConfirmation.Children.Add(textblockHelp);
            panelConfirmation.Children.Add(okButton);
            panelConfirmation.Children.Add(cancelButton);

            border.Child = panelConfirmation;

            // Set the Child property of Popup to the border
            // that contains a stackpanel, textblock and button.
            _scanConfirmation.Child = border;

            // Set where the popup will show up on the screen.  
            _scanConfirmation.VerticalOffset = Application.Current.Host.Content.ActualHeight - dialogHeight - borderThickness*2;
            _scanConfirmation.HorizontalOffset = 0;

            // Open the popup.
            _scanConfirmation.IsOpen = true;
        }

        ProcessResult _processResult = new ProcessResult();

        void BarcodeReader_ResultFound(Result obj)
        {
            System.Diagnostics.Debug.WriteLine("BarcodeReader_ResultFound");

            if (_processResult.Text == null)
            {
                _processResult.Data   = obj.RawBytes;
                _processResult.Format = obj.BarcodeFormat.ToString();
                _processResult.Text   = obj.Text;

                ShowPopupTextDialog();
            }
        }

        void ok_ConfirmationClick(object sender, RoutedEventArgs e)
        {
            OpticalReaderTask.CompleteTask(_processResult, null);
            OpticalReaderTask.TaskPending = false;

            _scanConfirmation.IsOpen = false;

            NavigationService.GoBack();
        }

        void cancel_ConfirmationClick(object sender, RoutedEventArgs e)
        {
            _processResult.Data   = null;
            _processResult.Format = null;
            _processResult.Text   = null;

            _scanConfirmation.IsOpen = false;
        }

        private void ScanForBarcode()
        {
            _phoneCamera.Focus();

            Thread.Sleep(50);

            //grab a camera snapshot
            _phoneCamera.GetPreviewBufferArgb32(_previewBuffer.Pixels);
            _previewBuffer.Invalidate();

            //scan the captured snapshot for barcodes
            //if a barcode is found, the ResultFound event will fire
            _barcodeReader.Decode(_previewBuffer);
        }
    }
}