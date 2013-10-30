/* 
    Copyright (c) 2011 Microsoft Corporation.  All rights reserved.
    Use of this sample source code is subject to the terms of the Microsoft license 
    agreement under which you licensed this sample source code and is provided AS-IS.
    If you did not accept the terms of the license agreement, you are not authorized 
    to use this sample source code.  For the terms of the license, please see the 
    license agreement between you and Microsoft.
  
    To see all Code Samples for Windows Phone, visit http://go.microsoft.com/fwlink/?LinkID=219604 
  
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

// Directives
using Microsoft.Devices;
using System.IO;
using System.IO.IsolatedStorage;
using Microsoft.Xna.Framework.Media;

namespace rho
{
    public partial class CameraPage : PhoneApplicationPage
    {
        // Variables
        private int savedCounter = 0;
        PhotoCamera cam;
        MediaLibrary library = new MediaLibrary();

        // Holds the current flash mode.
        private string currentFlashMode;

        // Holds the current resolution index.
        int currentResIndex = 0;

        // Constructor
        public CameraPage()
        {
            InitializeComponent();
        }

        //Code for initialization, capture completed, image availability events; also setting the source for the viewfinder.
        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {

            // Check to see if the camera is available on the device.
            if ((PhotoCamera.IsCameraTypeSupported(CameraType.Primary) == true) ||
                 (PhotoCamera.IsCameraTypeSupported(CameraType.FrontFacing) == true))
            {
                // Initialize the camera, when available.
                if (PhotoCamera.IsCameraTypeSupported(CameraType.FrontFacing))
                {
                    // Use front-facing camera if available.
                    cam = new Microsoft.Devices.PhotoCamera(CameraType.FrontFacing);
                }
                else
                {
                    // Otherwise, use standard camera on back of device.
                    cam = new Microsoft.Devices.PhotoCamera(CameraType.Primary);
                }

                // Event is fired when the PhotoCamera object has been initialized.
                cam.Initialized += new EventHandler<Microsoft.Devices.CameraOperationCompletedEventArgs>(cam_Initialized);

                // Event is fired when the capture sequence is complete.
                cam.CaptureCompleted += new EventHandler<CameraOperationCompletedEventArgs>(cam_CaptureCompleted);

                // Event is fired when the capture sequence is complete and an image is available.
                cam.CaptureImageAvailable += new EventHandler<Microsoft.Devices.ContentReadyEventArgs>(cam_CaptureImageAvailable);

                // Event is fired when the capture sequence is complete and a thumbnail image is available.
                cam.CaptureThumbnailAvailable += new EventHandler<ContentReadyEventArgs>(cam_CaptureThumbnailAvailable);

                // The event is fired when auto-focus is complete.
                cam.AutoFocusCompleted += new EventHandler<CameraOperationCompletedEventArgs>(cam_AutoFocusCompleted);

                // The event is fired when the viewfinder is tapped (for focus).
                viewfinderCanvas.Tap += new EventHandler<GestureEventArgs>(focus_Tapped);

                // The event is fired when the shutter button receives a half press.
                CameraButtons.ShutterKeyHalfPressed += OnButtonHalfPress;

                // The event is fired when the shutter button receives a full press.
                CameraButtons.ShutterKeyPressed += OnButtonFullPress;

                // The event is fired when the shutter button is released.
                CameraButtons.ShutterKeyReleased += OnButtonRelease;

                //Set the VideoBrush source to the camera.
                viewfinderBrush.SetSource(cam);
            }
            else
            {
                // The camera is not supported on the device.
                this.Dispatcher.BeginInvoke(delegate()
                {
                    // Write message.
                    txtDebug.Text = "A Camera is not available on this device.";
                });

                // Disable UI.
                ShutterButton.IsEnabled = false;
                FlashButton.IsEnabled = false;
                AFButton.IsEnabled = false;
                ResButton.IsEnabled = false;
            }
        }

        protected override void OnNavigatingFrom(System.Windows.Navigation.NavigatingCancelEventArgs e)
        {
            if (cam != null)
            {
                // Dispose camera to minimize power consumption and to expedite shutdown.
                cam.Dispose();

                // Release memory, ensure garbage collection.
                cam.Initialized -= cam_Initialized;
                cam.CaptureCompleted -= cam_CaptureCompleted;
                cam.CaptureImageAvailable -= cam_CaptureImageAvailable;
                cam.CaptureThumbnailAvailable -= cam_CaptureThumbnailAvailable;
                cam.AutoFocusCompleted -= cam_AutoFocusCompleted;
                CameraButtons.ShutterKeyHalfPressed -= OnButtonHalfPress;
                CameraButtons.ShutterKeyPressed -= OnButtonFullPress;
                CameraButtons.ShutterKeyReleased -= OnButtonRelease;
            }
        }

        // Update the UI if initialization succeeds.
        void cam_Initialized(object sender, Microsoft.Devices.CameraOperationCompletedEventArgs e)
        {
            if (e.Succeeded)
            {
                this.Dispatcher.BeginInvoke(delegate()
                {
                    // Write message.
                    txtDebug.Text = "Camera initialized.";

                    // Set flash button text.
                    FlashButton.Content = "Fl:" + cam.FlashMode.ToString();
                });
            }
        }

        // Ensure that the viewfinder is upright in LandscapeRight.
        protected override void OnOrientationChanged(OrientationChangedEventArgs e)
        {
            if (cam != null)
            {
                // LandscapeRight rotation when camera is on back of device.
                int landscapeRightRotation = 180;

                // Change LandscapeRight rotation for front-facing camera.
                if (cam.CameraType == CameraType.FrontFacing) landscapeRightRotation = -180;

                // Rotate video brush from camera.
                if (e.Orientation == PageOrientation.LandscapeRight)
                {
                    // Rotate for LandscapeRight orientation.
                    viewfinderBrush.RelativeTransform =
                        new CompositeTransform() { CenterX = 0.5, CenterY = 0.5, Rotation = landscapeRightRotation };
                }
                else
                {
                    // Rotate for standard landscape orientation.
                    viewfinderBrush.RelativeTransform =
                        new CompositeTransform() { CenterX = 0.5, CenterY = 0.5, Rotation = 0 };
                }
            }

            base.OnOrientationChanged(e);
        }

        private void ShutterButton_Click(object sender, RoutedEventArgs e)
        {
            if (cam != null)
            {
                try
                {
                    // Start image capture.
                    cam.CaptureImage();
                }
                catch (Exception ex)
                {
                    this.Dispatcher.BeginInvoke(delegate()
                    {
                        // Cannot capture an image until the previous capture has completed.
                        txtDebug.Text = ex.Message;
                    });
                }
            }
        }

        void cam_CaptureCompleted(object sender, CameraOperationCompletedEventArgs e)
        {
            // Increments the savedCounter variable used for generating JPEG file names.
            savedCounter++;
        }


        // Informs when full resolution picture has been taken, saves to local media library and isolated storage.
        void cam_CaptureImageAvailable(object sender, Microsoft.Devices.ContentReadyEventArgs e)
        {
            string fileName = savedCounter + ".jpg";

            try
            {   // Write message to the UI thread.
                Deployment.Current.Dispatcher.BeginInvoke(delegate()
                {
                    txtDebug.Text = "Captured image available, saving picture.";
                });

                // Save picture to the library camera roll.
                library.SavePictureToCameraRoll(fileName, e.ImageStream);

                // Write message to the UI thread.
                Deployment.Current.Dispatcher.BeginInvoke(delegate()
                {
                    txtDebug.Text = "Picture has been saved to camera roll.";

                });

                // Set the position of the stream back to start
                e.ImageStream.Seek(0, SeekOrigin.Begin);

                // Save picture as JPEG to isolated storage.
                using (IsolatedStorageFile isStore = IsolatedStorageFile.GetUserStoreForApplication())
                {
                    using (IsolatedStorageFileStream targetStream = isStore.OpenFile(fileName, FileMode.Create, FileAccess.Write))
                    {
                        // Initialize the buffer for 4KB disk pages.
                        byte[] readBuffer = new byte[4096];
                        int bytesRead = -1;

                        // Copy the image to isolated storage. 
                        while ((bytesRead = e.ImageStream.Read(readBuffer, 0, readBuffer.Length)) > 0)
                        {
                            targetStream.Write(readBuffer, 0, bytesRead);
                        }
                    }
                }

                // Write message to the UI thread.
                Deployment.Current.Dispatcher.BeginInvoke(delegate()
                {
                    txtDebug.Text = "Picture has been saved to isolated storage.";

                });
            }
            finally
            {
                // Close image stream
                e.ImageStream.Close();
            }

        }

        // Informs when thumbnail picture has been taken, saves to isolated storage
        // User will select this image in the pictures application to bring up the full-resolution picture. 
        public void cam_CaptureThumbnailAvailable(object sender, ContentReadyEventArgs e)
        {
            string fileName = savedCounter + "_th.jpg";

            try
            {
                // Write message to UI thread.
                Deployment.Current.Dispatcher.BeginInvoke(delegate()
                {
                    txtDebug.Text = "Captured image available, saving thumbnail.";
                });

                // Save thumbnail as JPEG to isolated storage.
                using (IsolatedStorageFile isStore = IsolatedStorageFile.GetUserStoreForApplication())
                {
                    using (IsolatedStorageFileStream targetStream = isStore.OpenFile(fileName, FileMode.Create, FileAccess.Write))
                    {
                        // Initialize the buffer for 4KB disk pages.
                        byte[] readBuffer = new byte[4096];
                        int bytesRead = -1;

                        // Copy the thumbnail to isolated storage. 
                        while ((bytesRead = e.ImageStream.Read(readBuffer, 0, readBuffer.Length)) > 0)
                        {
                            targetStream.Write(readBuffer, 0, bytesRead);
                        }
                    }
                }

                // Write message to UI thread.
                Deployment.Current.Dispatcher.BeginInvoke(delegate()
                {
                    txtDebug.Text = "Thumbnail has been saved to isolated storage.";

                });
            }
            finally
            {
                // Close image stream
                e.ImageStream.Close();
            }
        }

        // Activate a flash mode.
        // Cycle through flash mode options when the flash button is pressed.
        private void changeFlash_Clicked(object sender, RoutedEventArgs e)
        {

            switch (cam.FlashMode)
            {
                case FlashMode.Off:
                    if (cam.IsFlashModeSupported(FlashMode.On))
                    {
                        // Specify that flash should be used.
                        cam.FlashMode = FlashMode.On;
                        FlashButton.Content = "Fl:On";
                        currentFlashMode = "Flash mode: On";
                    }
                    break;
                case FlashMode.On:
                    if (cam.IsFlashModeSupported(FlashMode.RedEyeReduction))
                    {
                        // Specify that the red-eye reduction flash should be used.
                        cam.FlashMode = FlashMode.RedEyeReduction;
                        FlashButton.Content = "Fl:RER";
                        currentFlashMode = "Flash mode: RedEyeReduction";
                    }
                    else if (cam.IsFlashModeSupported(FlashMode.Auto))
                    {
                        // If red-eye reduction is not supported, specify automatic mode.
                        cam.FlashMode = FlashMode.Auto;
                        FlashButton.Content = "Fl:Auto";
                        currentFlashMode = "Flash mode: Auto";
                    }
                    else
                    {
                        // If automatic is not supported, specify that no flash should be used.
                        cam.FlashMode = FlashMode.Off;
                        FlashButton.Content = "Fl:Off";
                        currentFlashMode = "Flash mode: Off";
                    }
                    break;
                case FlashMode.RedEyeReduction:
                    if (cam.IsFlashModeSupported(FlashMode.Auto))
                    {
                        // Specify that the flash should be used in the automatic mode.
                        cam.FlashMode = FlashMode.Auto;
                        FlashButton.Content = "Fl:Auto";
                        currentFlashMode = "Flash mode: Auto";
                    }
                    else
                    {
                        // If automatic is not supported, specify that no flash should be used.
                        cam.FlashMode = FlashMode.Off;
                        FlashButton.Content = "Fl:Off";
                        currentFlashMode = "Flash mode: Off";
                    }
                    break;
                case FlashMode.Auto:
                    if (cam.IsFlashModeSupported(FlashMode.Off))
                    {
                        // Specify that no flash should be used.
                        cam.FlashMode = FlashMode.Off;
                        FlashButton.Content = "Fl:Off";
                        currentFlashMode = "Flash mode: Off";
                    }
                    break;
            }

            // Display current flash mode.
            this.Dispatcher.BeginInvoke(delegate()
            {
                txtDebug.Text = currentFlashMode;
            });
        }

        // Provide auto-focus in the viewfinder.
        private void focus_Clicked(object sender, System.Windows.RoutedEventArgs e)
        {
            if (cam.IsFocusSupported == true)
            {
                //Focus when a capture is not in progress.
                try
                {
                    cam.Focus();
                }
                catch (Exception focusError)
                {
                    // Cannot focus when a capture is in progress.
                    this.Dispatcher.BeginInvoke(delegate()
                    {
                        txtDebug.Text = focusError.Message;
                    });
                }
            }
            else
            {
                // Write message to UI.
                this.Dispatcher.BeginInvoke(delegate()
                {
                    txtDebug.Text = "Camera does not support programmable auto focus.";
                });
            }
        }

        void cam_AutoFocusCompleted(object sender, CameraOperationCompletedEventArgs e)
        {
            Deployment.Current.Dispatcher.BeginInvoke(delegate()
            {
                // Write message to UI.
                txtDebug.Text = "Auto focus has completed.";

                // Hide the focus brackets.
                focusBrackets.Visibility = Visibility.Collapsed;

            });
        }

        // Provide touch focus in the viewfinder.
        void focus_Tapped(object sender, GestureEventArgs e)
        {
            if (cam != null)
            {
                if (cam.IsFocusAtPointSupported == true)
                {
                    try
                    {
                        // Determine location of tap.
                        Point tapLocation = e.GetPosition(viewfinderCanvas);

                        // Position focus brackets with estimated offsets.
                        focusBrackets.SetValue(Canvas.LeftProperty, tapLocation.X - 30);
                        focusBrackets.SetValue(Canvas.TopProperty, tapLocation.Y - 28);

                        // Determine focus point.
                        double focusXPercentage = tapLocation.X / viewfinderCanvas.Width;
                        double focusYPercentage = tapLocation.Y / viewfinderCanvas.Height;

                        // Show focus brackets and focus at point
                        focusBrackets.Visibility = Visibility.Visible;
                        cam.FocusAtPoint(focusXPercentage, focusYPercentage);

                        // Write a message to the UI.
                        this.Dispatcher.BeginInvoke(delegate()
                        {
                            txtDebug.Text = String.Format("Camera focusing at point: {0:N2} , {1:N2}", focusXPercentage, focusYPercentage);
                        });
                    }
                    catch (Exception focusError)
                    {
                        // Cannot focus when a capture is in progress.
                        this.Dispatcher.BeginInvoke(delegate()
                        {
                            // Write a message to the UI.
                            txtDebug.Text = focusError.Message;
                            // Hide focus brackets.
                            focusBrackets.Visibility = Visibility.Collapsed;
                        });
                    }
                }
                else
                {
                    // Write a message to the UI.
                    this.Dispatcher.BeginInvoke(delegate()
                    {
                        txtDebug.Text = "Camera does not support FocusAtPoint().";
                    });
                }
            }
        }

        private void changeRes_Clicked(object sender, System.Windows.RoutedEventArgs e)
        {
            // Variables
            IEnumerable<Size> resList = cam.AvailableResolutions;
            int resCount = resList.Count<Size>();
            Size res;

            // Poll for available camera resolutions.
            for (int i = 0; i < resCount; i++)
            {
                res = resList.ElementAt<Size>(i);
            }

            // Set the camera resolution.
            res = resList.ElementAt<Size>((currentResIndex + 1) % resCount);
            cam.Resolution = res;
            currentResIndex = (currentResIndex + 1) % resCount;

            // Update the UI.
            txtDebug.Text = String.Format("Setting capture resolution: {0}x{1}", res.Width, res.Height);
            ResButton.Content = "R" + res.Width;
        }


        // Provide auto-focus with a half button press using the hardware shutter button.
        private void OnButtonHalfPress(object sender, EventArgs e)
        {
            if (cam != null)
            {
                // Focus when a capture is not in progress.
                try
                {
                    this.Dispatcher.BeginInvoke(delegate()
                    {
                        txtDebug.Text = "Half Button Press: Auto Focus";
                    });

                    cam.Focus();
                }
                catch (Exception focusError)
                {
                    // Cannot focus when a capture is in progress.
                    this.Dispatcher.BeginInvoke(delegate()
                    {
                        txtDebug.Text = focusError.Message;
                    });
                }
            }
        }

        // Capture the image with a full button press using the hardware shutter button.
        private void OnButtonFullPress(object sender, EventArgs e)
        {
            if (cam != null)
            {
                cam.CaptureImage();
            }
        }

        // Cancel the focus if the half button press is released using the hardware shutter button.
        private void OnButtonRelease(object sender, EventArgs e)
        {

            if (cam != null)
            {
                cam.CancelFocus();
            }
        }


    }
}
