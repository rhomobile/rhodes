using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Devices;
using System.IO;
using System.IO.IsolatedStorage;
using Microsoft.Xna.Framework.Media;
using System.Windows.Media;
using System.Threading;
using System.Windows.Input;
using Windows.Storage.Pickers;
using Microsoft.Phone.Tasks;
using Microsoft.Phone.Info;
using Windows.Graphics.Display;
using rhoruntime;
using System.Collections.Generic;
using System;
using System.Windows.Threading;
using rhodes;
using System.Linq;
using Windows.Phone.Media.Capture;
using System.Windows.Media.Imaging;
using System.Collections.ObjectModel;
using Microsoft.Phone.BackgroundAudio;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework;
using Windows.Storage; 


namespace rho {

namespace CameraImpl
{
    public class Camera : CameraBase
    {

        PhotoCamera Rho_StillCamera;
        MediaLibrary Rho_Store_Picturelibrary = new MediaLibrary();

        VideoBrush Rho_PhotoCameraBrush;
        Canvas Rho_PhotoCameraCanvas;
        TextBox txtDebug = new TextBox();
        Grid Store_PreviousGridElements;
        // Holds current flash mode.

        PhotoChooserTask Rho_photoChooserTask;

        Dictionary<int, string> Rho_Camera_Type = new Dictionary<int, string>(2);
        int Rho_Camera_selected=0;



        
        int Rho_Selected_Flash_Mode = 2;

        
        MainPage Rho_MainPage;
        Grid LayoutGrid;
        FlashMode Rho_FlashMode;
        UIElementCollection Rho_Control;
        Dictionary<double, Size> Rho_Supported_Resolutions = new Dictionary<double, Size>();
        List<double> Rho_Screen_Resolution_Width = new List<double>();
        List<double> Rho_Screen_Resolution_Height = new List<double>();

        Dictionary<string, double> Rho_Paramenters = new Dictionary<string, double>();
        Dictionary<string, string> Rho_StringParameters = new Dictionary<string, string>();
        Dictionary<string, int> Rho_Flash_Types = new Dictionary<string, int>();
        Dictionary<string, string> Rho_AliasParametersName = new Dictionary<string, string>();
        Dictionary<string, string> Rho_Flashmodes = new Dictionary<string, string>();
        
        Dictionary<string, string> Rho_OutputType = new Dictionary<string, string>();
        Dictionary<string, string> Rho_OutPutFormat = new Dictionary<string, string>();
        IMethodResult m_StoreTakePictureResult;
        Dictionary<string, string> m_Take_Picture_Output = new Dictionary<string, string>();
        Dictionary<string, int> m_CameraTypes = new Dictionary<string, int>();
        string strbase64;

        public Camera()
        {
          
        }
       
         public override void setNativeImpl(string strID, long native)
        {

            base.setNativeImpl(strID, native);
            setCameraType(_strID);
        }
        public void setCameraType(string strId)
        {
            // initialize class instance in C# here
           // Rho_Paramenters.Add("camera_type", (int)CameraType.Primary);
            m_CameraTypes.Clear();
            m_CameraTypes.Add(CameraType.FrontFacing.ToString(), Convert.ToInt32(CameraType.FrontFacing));
            m_CameraTypes.Add(CameraType.Primary.ToString(), Convert.ToInt32(CameraType.Primary));


            Rho_Camera_selected =m_CameraTypes[strId];
            using (PhotoCamera Temp_camera = new PhotoCamera((CameraType)Rho_Camera_selected))
            {
                Rho_Paramenters.Add("desired_height", Temp_camera.Resolution.Height);
                Rho_Paramenters.Add("desired_width", Temp_camera.Resolution.Width);


                Rho_AliasParametersName.Add("desiredHeight", "desired_height");
                Rho_AliasParametersName.Add("desiredWidth", "desired_width");
                Rho_AliasParametersName.Add("flashMode", "flash_mode");
               


                Rho_Paramenters.Add("camera_type", (int)Temp_camera.CameraType);

                Rho_StringParameters.Add("imagename", "Img");
                Rho_StringParameters.Add("imageformat", "jpg");
                Rho_StringParameters.Add("captureSound", string.Empty);


                Rho_OutputType.Add("image", "outputformat");
                Rho_OutputType.Add("datauri", "outputformat");
                Rho_OutPutFormat.Add("outputformat", "image");



                IEnumerable<Size> CameraResolutionhsize = Temp_camera.AvailableResolutions;
                Rho_Paramenters.Add("getmaxwidth", CameraResolutionhsize.Max(x => x.Width));
                Rho_Paramenters.Add("getmaxheight", CameraResolutionhsize.Max(Y => Y.Height));
                int Counter = 0;
                foreach (Size ResolutionStyle in CameraResolutionhsize)
                {
                    Rho_Screen_Resolution_Height.Add(ResolutionStyle.Height);
                    Rho_Screen_Resolution_Width.Add(ResolutionStyle.Width);
                    Rho_Supported_Resolutions.Add(Counter, ResolutionStyle);
                    Counter++;
                }



                Rho_Flash_Types.Add(FLASH_ON, 1);
                Rho_Flash_Types.Add(FLASH_OFF, 2);
                Rho_Flash_Types.Add(FLASH_AUTO, 3);
                Rho_Flash_Types.Add(FLASH_RED_EYE, 4);
                Rho_FlashMode = (FlashMode)(Rho_Flash_Types[FLASH_AUTO]);
                Rho_Flashmodes.Add("flash_mode", FLASH_AUTO);
                Rho_MainPage = MainPage.getInstance();
                LayoutGrid = MainPage.LayoutGrid();

            }
            

        }

        


        public override void getCameraType(IMethodResult oResult)
        {            
           using(PhotoCamera TempCamera=new PhotoCamera((CameraType)Rho_Paramenters["camera_type"]))
           {
               oResult.set(TempCamera.CameraType.ToString());
           }
        }

        public override void getMaxWidth(IMethodResult oResult)
        {
            try
            {
                oResult.set(Rho_Paramenters["getmaxwidth"]);
                return;
            }
            catch (Exception ex)
            {

            }

          
        }

        public override void getMaxHeight(IMethodResult oResult)
        {
            try
            {
                oResult.set(Rho_Paramenters["getmaxheight"]);
                return;
            }
            catch (Exception ex)
            {

            }

           
        }

        public override void getSupportedSizeList(IMethodResult oResult)
        {




            using (PhotoCamera TempCamera = new PhotoCamera((CameraType)Rho_Paramenters["camera_type"]))
            {
                Dictionary<string, string> Store_Resolution = new Dictionary<string, string>();
                List<Dictionary<string, string>> RTypes=new List<Dictionary<string,string>>();
                IReadOnlyList<Dictionary<string, string>> Readonlylists = RTypes;
                Store_Resolution["imageWidth"] = "120";
                Store_Resolution["imageHeight"] = "120";
                oResult.set(Store_Resolution);
                
               
                try
                {
                    int intcounter = 0;
                    
                    /*IEnumerable<Size> objsize=TempCamera.AvailableResolutions;
                    foreach (Size size in objsize)
                    {

                        Store_Resolution[intcounter] = new Dictionary<string, string>();                        
                        Store_Resolution[intcounter].Add("width", size.Width.ToString());
                        Store_Resolution[intcounter].Add("height", size.Height.ToString());
                        RTypes.Add(Store_Resolution[intcounter]);
                        intcounter++;
                        // IReadOnlyDictionary<string, string> StoreDictionary = new ReadOnlyDictionary<string, string>(SupportedCameraResolution);



                    }*/
                }
                catch (Exception ex)
                {
                }
               // oResult.set(Readonlylists);

            }
            

           

        }

        public override void getDesiredWidth(IMethodResult oResult)
        {
            oResult.set(Rho_Paramenters["desired_width"].ToString());
        }

        public override void setDesiredWidth(int desiredWidth, IMethodResult oResult)
        {
            double closestWidth = Rho_Screen_Resolution_Width.Aggregate((x, y) => Math.Abs(x - desiredWidth) < Math.Abs(y - desiredWidth) ? x : y);
           Rho_Paramenters["desired_width"] = closestWidth;
        }

        public override void getDesiredHeight(IMethodResult oResult)
        {
            oResult.set(Rho_Paramenters["desired_height"].ToString());
        }

        public override void setDesiredHeight(int desiredHeight, IMethodResult oResult)
        {
            double closestHeight = Rho_Screen_Resolution_Height.Aggregate((x, y) => Math.Abs(x - desiredHeight) < Math.Abs(y - desiredHeight) ? x : y);
            Rho_Paramenters["desired_height"] = closestHeight;
        }

        public override void getFileName(IMethodResult oResult)
        {
            oResult.set(Rho_StringParameters["imagename"]);
        }

        public override void setFileName(string fileName, IMethodResult oResult)
        {
            Rho_StringParameters["imagename"] = fileName;
        }

        public override void getCompressionFormat(IMethodResult oResult)
        {
            oResult.set(Rho_StringParameters["imageformat"]);          
        }

        public override void setCompressionFormat(string compressionFormat, IMethodResult oResult)
        {
            //AS WP8 does not support any other format apart from jpeg, need to check in 8.1 
           // Rho_StringParameters["ImageFormat"] = compressionFormat;
        }

        public override void getOutputFormat(IMethodResult oResult)
        {
            oResult.set(Rho_OutPutFormat["outputformat"]);
        }

        public override void setOutputFormat(string outputFormat, IMethodResult oResult)
        {
            Rho_OutPutFormat.Clear();
            try
            {
                string storevalue = Rho_OutPutFormat[outputFormat.ToLower().Trim()];
                Rho_OutPutFormat.Add("outputformat", outputFormat.ToLower().Trim());
                
            }

            catch (Exception ex)
            {
                Rho_OutPutFormat.Clear();
                Rho_OutPutFormat.Add("outputformat", "image");

            }


           
           
        }

        public override void getColorModel(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setColorModel(string colorModel, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getEnableEditing(IMethodResult oResult)
        {
          
        }

        public override void setEnableEditing(bool enableEditing, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getFlashMode(IMethodResult oResult)
        {
            IEnumerable<KeyValuePair<string,int>> Flashmode=Rho_Flash_Types.Select(x=>x).Where(k=>k.Value==(int)Rho_FlashMode).ToList();
            string ReturnableValue=string.Empty;
            foreach (KeyValuePair<string, int> obj in Flashmode)
            {
                ReturnableValue = obj.Key;
            }
            oResult.set(ReturnableValue);
        }

        public override void setFlashMode(string flashMode, IMethodResult oResult)
        {
            try
            {
                Rho_FlashMode = (FlashMode)(Rho_Flash_Types[flashMode.ToLower().Trim()]);
                Rho_Flashmodes["flash_mode"] = flashMode.ToLower().Trim();

            }
            catch (Exception ex)
            {
                Rho_FlashMode = (FlashMode)(Rho_Flash_Types[FLASH_AUTO]);
            }

           
        }

      
        public override void getSaveToDeviceGallery(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setSaveToDeviceGallery(bool saveToDeviceGallery, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getCaptureSound(IMethodResult oResult)
        {
            oResult.set(Rho_StringParameters["captureSound"]);
        }

        public override void setCaptureSound(string captureSound, IMethodResult oResult)
        {
            Rho_StringParameters["captureSound"] = captureSound;
        }

        public override void getPreviewLeft(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setPreviewLeft(int previewLeft, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getPreviewTop(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setPreviewTop(int previewTop, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getPreviewWidth(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setPreviewWidth(int previewWidth, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getPreviewHeight(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setPreviewHeight(int previewHeight, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getUseSystemViewfinder(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setUseSystemViewfinder(bool useSystemViewfinder, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getAimMode(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setAimMode(string aimMode, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        private void SetCameraConfiguration(IReadOnlyDictionary<string, string> propertyMap)
        {
            foreach (KeyValuePair<string, string> Parameters in propertyMap)
            {
                string ParametersKeyName = Parameters.Key.Trim().ToLower();
                try
                {
                    ParametersKeyName = Rho_AliasParametersName[ParametersKeyName];
                }
                catch (Exception ex)
                {

                }

                try
                {
                    Rho_Paramenters[ParametersKeyName] = Convert.ToDouble(Parameters.Value);
                }
                catch (Exception ex)
                {
                }
                try
                {
                    
                    Rho_FlashMode = (FlashMode)(Rho_Flash_Types[Rho_Flashmodes[ParametersKeyName]]);
                    Rho_Flashmodes[ParametersKeyName] = Parameters.Value.ToString().Trim();
                }
                catch (Exception ex)
                {
                }
                try
                {
                    Rho_StringParameters[ParametersKeyName] = Parameters.Value.ToString().Trim();
                }
                catch (Exception ex)
                {
                }
                try
                {
                    string value = Rho_OutPutFormat[ParametersKeyName];
                    string tempnumber = Rho_OutputType[Parameters.Value.Trim().ToLower()];
                    Rho_OutPutFormat.Clear();
                    Rho_OutPutFormat.Add(ParametersKeyName, Parameters.Value.Trim().ToLower());

                }
                catch (Exception ex)
                {

                }
                
            }

        }

      
        public override void takePicture(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            try
            {
                
                SetCameraConfiguration(propertyMap);
                Initialize_TakePictureCallBack();
                Store_PreviousGridElements = LayoutGrid;
                Rho_StillCamera = new Microsoft.Devices.PhotoCamera((CameraType)Rho_Camera_selected);
                Rho_MainPage.BackKeyPress += Rho_MainPage_BackKeyPress;
                Rho_Create_Camera_Layout();
                Rho_MainPage.SupportedOrientations = SupportedPageOrientation.Landscape;
                Rho_MainPage.Orientation = PageOrientation.LandscapeLeft;
                Rho_StillCamera.Initialized += new EventHandler<Microsoft.Devices.CameraOperationCompletedEventArgs>(cam_Initialized);



                // Event is fired when the capture sequence is complete.
                // Rho_StillCamera.CaptureCompleted += new EventHandler<CameraOperationCompletedEventArgs>(cam_CaptureCompleted);


                // Event is fired when the capture sequence is complete and an image is available.
                Rho_StillCamera.CaptureImageAvailable += new EventHandler<Microsoft.Devices.ContentReadyEventArgs>(cam_CaptureImageAvailable);

                // Event is fired when the capture sequence is complete and a thumbnail image is available.
                // Rho_StillCamera.CaptureThumbnailAvailable += new EventHandler<ContentReadyEventArgs>(cam_CaptureThumbnailAvailable);

                m_StoreTakePictureResult = oResult;
                Rho_PhotoCameraBrush.SetSource(Rho_StillCamera);
                // implement this method in C# here
            }
            catch (Exception ex)
            {
                m_Take_Picture_Output["status"] = "error";
                m_Take_Picture_Output["message"] = ex.Message;
                 m_Take_Picture_Output["image_format"]=string.Empty;
                 oResult.set(m_Take_Picture_Output);
            }
        }


        void Return_To_Previous_Screen()
        {
            try
            {
                Rho_MainPage.BackKeyPress -= Rho_MainPage_BackKeyPress;


                Rho_StillCamera.Initialized -= new EventHandler<Microsoft.Devices.CameraOperationCompletedEventArgs>(cam_Initialized);

                Rho_StillCamera.CaptureImageAvailable -= new EventHandler<Microsoft.Devices.ContentReadyEventArgs>(cam_CaptureImageAvailable);
                CameraButtons.ShutterKeyPressed -= CameraButtons_ShutterKeyPressed;

                LayoutGrid.Children.Remove(Rho_PhotoCameraCanvas);
                LayoutGrid = Store_PreviousGridElements;

                Rho_PhotoCameraCanvas = null;
                Rho_PhotoCameraBrush = null;



                /*for (int Previous_Control = 0; Previous_Control < Rho_Control.Length; Previous_Control++)
                {
                    LayoutGrid.Children.Add(Rho_Control[Previous_Control]);
                    Rho_Control[Previous_Control] = null;
                }*/
                Rho_MainPage.SupportedOrientations = SupportedPageOrientation.PortraitOrLandscape;
                Rho_MainPage.Orientation = PageOrientation.Portrait;


            }
            catch (AccessViolationException et)
            {
            }
            catch (Exception ex)
            {
            }
        }
        private void Initialize_TakePictureCallBack()
        {
            m_Take_Picture_Output.Clear();
            m_Take_Picture_Output.Add("status", "cancel");
            m_Take_Picture_Output.Add("imageUri", string.Empty);
            m_Take_Picture_Output.Add("imageHeight", string.Empty);
            m_Take_Picture_Output.Add("imageWidth", string.Empty);
            m_Take_Picture_Output.Add("imageFormat", "jpg");
            m_Take_Picture_Output.Add("message", string.Empty);
            m_Take_Picture_Output.Add("image_uri", string.Empty);
            m_Take_Picture_Output.Add("image_height", string.Empty);
            m_Take_Picture_Output.Add("image_width", string.Empty);
            m_Take_Picture_Output.Add("image_format", "jpg");
        }

        void Rho_MainPage_BackKeyPress(object sender, System.ComponentModel.CancelEventArgs e)
        {
            try
            {
                Return_To_Previous_Screen();
                e.Cancel = true;
                 m_Take_Picture_Output["image_format"]=string.Empty;
            }
            catch (Exception ex)
            {
                m_Take_Picture_Output["status"] = "error";
                m_Take_Picture_Output["message"] = ex.Message;
                 m_Take_Picture_Output["image_format"]=string.Empty;
            }
            m_StoreTakePictureResult.set(m_Take_Picture_Output);
        }

        protected AudioVideoCaptureDevice Device { get; set; }


      

        void cam_Initialized(object sender, Microsoft.Devices.CameraOperationCompletedEventArgs e)
        {
            if (e.Succeeded)
            {
                Rho_PhotoCameraCanvas.Dispatcher.BeginInvoke(delegate()
                {
                    // Write message.
                    txtDebug.Text = "Camera initialized.";

                    // MessageBox.Show(Rho_StillCamera.Resolution.ToString());
                    Rho_StillCamera.FlashMode = Rho_FlashMode;
                    KeyValuePair<double, Size> CameraResolution = Rho_Supported_Resolutions.Aggregate((x, y) => Math.Abs(x.Value.Height - Rho_Paramenters["desired_height"]) < Math.Abs(y.Value.Height - Rho_Paramenters["desired_height"]) ? x : y);

                    Rho_StillCamera.Resolution = CameraResolution.Value;



                });
            }
            else
            {
                 m_Take_Picture_Output["status"] = "error";
                m_Take_Picture_Output["message"] = e.Exception.Message;
                 m_Take_Picture_Output["image_format"]=string.Empty;
                m_StoreTakePictureResult.set(m_Take_Picture_Output);
            }
        }
        void cam_CaptureImageAvailable(object sender, Microsoft.Devices.ContentReadyEventArgs e)
        {

            string fileName = Rho_StringParameters["imagename"] + "_" + DateTime.Now.ToLongDateString() + "_" + DateTime.Now.ToLongTimeString().Replace(':', '_');


            Deployment.Current.Dispatcher.BeginInvoke(delegate()
            {
            try
            {   // Write message to the UI thread.

               
                    BitmapImage bmp = new BitmapImage();
                    bmp.SetSource(e.ImageStream);

                    MemoryStream ms = new MemoryStream();
                    WriteableBitmap wb = new WriteableBitmap(bmp);
                    wb.SaveJpeg(ms, bmp.PixelWidth, bmp.PixelHeight, 0, 100);
                    byte[] imagebytes = ms.ToArray();
                    e.ImageStream.CopyTo(ms);
                    byte[] result = ms.ToArray();
                    strbase64 = System.Convert.ToBase64String(result);



                    e.ImageStream.Seek(0, SeekOrigin.Begin);

                    // Save photo to the media library camera roll.
                    Picture PictDetails = Rho_Store_Picturelibrary.SavePictureToCameraRoll(fileName, e.ImageStream);


                    // Write message to the UI thread.


                    // Set the position of the stream back to start
                    e.ImageStream.Seek(0, SeekOrigin.Begin);

                    // Save photo as JPEG to the local folder.
                  

                    // Write message to the UI thread.
                    Deployment.Current.Dispatcher.BeginInvoke(delegate()
                    {
                        txtDebug.Text = "Photo has been saved to the local folder.";
                        Return_To_Previous_Screen();

                    });

                    string returnablevalue = "";
                    try
                    {
                        IEnumerable<KeyValuePair<string, string>> OutPutFormat = Rho_OutPutFormat.Select(x => x).Where(k => k.Value == "image").ToList();

                        foreach (KeyValuePair<string, string> format in OutPutFormat)
                        {
                            returnablevalue = "C:\\Data\\Users\\Public\\Pictures\\Camera Roll\\" + fileName + ".jpg";
                        }

                    }
                    catch (Exception ex)
                    {


                    }

                    try
                    {
                        IEnumerable<KeyValuePair<string, string>> OutPutFormat = Rho_OutPutFormat.Select(x => x).Where(k => k.Value == "datauri").ToList();

                        foreach (KeyValuePair<string, string> format in OutPutFormat)
                        {

                            returnablevalue ="data:image/jpeg;base64,"+ strbase64;
                        }
                    }
                    catch (Exception ex)
                    {
                    }
                    m_Take_Picture_Output["status"] = "ok";
                    m_Take_Picture_Output["imageUri"] = returnablevalue;
                    m_Take_Picture_Output["imageHeight"] = PictDetails.Height.ToString();
                    m_Take_Picture_Output["imageWidth"] = PictDetails.Width.ToString();
                    m_Take_Picture_Output["image_uri"] = returnablevalue;
                    m_Take_Picture_Output["image_height"] = PictDetails.Height.ToString();
                    m_Take_Picture_Output["image_width"] = PictDetails.Width.ToString();

               


            }
            catch (Exception ex)
            {
                m_Take_Picture_Output["status"] = "error";
                m_Take_Picture_Output["message"] = ex.Message;
                 m_Take_Picture_Output["image_format"]=string.Empty;
            }
            finally
            {
                // Close image stream
                e.ImageStream.Close();
                m_StoreTakePictureResult.set(m_Take_Picture_Output);
            }
             });

        }

        private void SendData()
        {

        }

        public override void showPreview(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void hidePreview(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void capture(IMethodResult oResult)
        {
            // implement this method in C# here
        }


        private void Rho_Create_Camera_Layout()
        {

           // Rho_Control = new Control[LayoutGrid.Children.Count];
            //LayoutGrid.Children.CopyTo(Rho_Control, 0);

            //for (int counter = 0; counter < LayoutGrid.Children.Count; counter++)
            //{
              //  LayoutGrid.Children.RemoveAt(counter);
            //}
            //Create a Video Brush for painting in Canvas.    

            Rho_PhotoCameraBrush = new VideoBrush();

            Rho_PhotoCameraBrush.Stretch = Stretch.Fill;


            // Create a Canvas for painting the image.
            Rho_PhotoCameraCanvas = new Canvas();
            //Create Canvas for the whole screen.
            Rho_PhotoCameraCanvas.Height = Application.Current.Host.Content.ActualWidth;
            Rho_PhotoCameraCanvas.Width = Application.Current.Host.Content.ActualHeight;
            //Add Camera Brush as background to the Canvas.            
            Rho_PhotoCameraCanvas.Background = Rho_PhotoCameraBrush;

            //On Clicking the Camera Canvas Lets consider it as a camera click.
            Rho_PhotoCameraCanvas.Tap += Rho_PhotoCameraCanvas_Tap;

            //Set the Width of the Parent Frame to the Complete Device.

            ColumnDefinition Rho_Camera_Columncollection = new ColumnDefinition();
            GridLength Rho_Camera_Gridlength = new GridLength(Application.Current.Host.Content.ActualWidth);
            Rho_Camera_Columncollection.Width = Rho_Camera_Gridlength;

          
           // CRhoRuntime obj = CRhoRuntime.getInstance();


           // LayoutGrid.ColumnDefinitions.Add(Rho_Camera_Columncollection);
            LayoutGrid.Children.Add(Rho_PhotoCameraCanvas);
            CameraButtons.ShutterKeyPressed += CameraButtons_ShutterKeyPressed;

            //Open the Device in LandScape mode.
            //Uncomment it once you get the layout handle or open the property in main app.xaml.

           


        }

       void CameraButtons_ShutterKeyPressed(object sender, System.EventArgs e)
        {
            CaptureImage();
        }

       void Rho_PhotoCameraCanvas_Tap(object sender, System.Windows.Input.GestureEventArgs e)
       {
           CaptureImage();
       }

       void CaptureImage()
       {
           if (Rho_StillCamera != null)
           {
              
               try
               {
                   // Start image capture.

                   Rho_StillCamera.CaptureImage();

               }
               catch (Exception ex)
               {

                  // Dispatcher.BeginInvoke(delegate()
                  // {
                       // Cannot capture an image until the previous capture has completed.
                       // txtDebug.Text = ex.Message;
                 //  });
               }
           }
       }

    }

    public class CameraSingleton : CameraSingletonBase
    {
        object _rhoruntime;
        PhotoChooserTask photoChooserTask;
        Dictionary<string, int> Rho_CameraTypes = new Dictionary<string, int>();
        IMethodResult m_StorechoosePictureResult;
        Dictionary<string, string> m_choosePicture_output = new Dictionary<string, string>();
        string Store_CaptureImage_outputformat = "image";
        Dictionary<string, string> Data_Uri = new Dictionary<string, string>();
        Dictionary<string, string> Image = new Dictionary<string, string>();
        

        public CameraSingleton()
        {
            Data_Uri.Add("datauri","outputformat");
            Image.Add("image", "outputformat");
            
            // initialize singleton instance in C# here
        }
      
        public override void enumerate(IMethodResult oResult)
        {
            List<string> AvailabeCameras=new List<string>();


            try
            {
                PhotoCamera Primary = new PhotoCamera(CameraType.Primary);
                AvailabeCameras.Add(CameraType.Primary.ToString());               
            }
            catch (Exception ex)
            {
            }
            try
            {
                PhotoCamera FrontCamera = new PhotoCamera(CameraType.FrontFacing);
                AvailabeCameras.Add(CameraType.FrontFacing.ToString());
               
            }
            catch (Exception ex)
            {

            }        
            
            oResult.set(AvailabeCameras);
           
        }

        public override void getCameraByType(string cameraType, IMethodResult oResult)
        {
            Rho_CameraTypes.Clear();
           
            Rho_CameraTypes.Add("back", 0);
            Rho_CameraTypes.Add("front", 1);
            string CameraByTypeReturnType = null;
            List<string> AvailabeCameras = new List<string>();
           
           

            try
            {
                PhotoCamera CameraCheck = new PhotoCamera((CameraType)Rho_CameraTypes[cameraType.Trim().ToLower()]);
                CameraByTypeReturnType = cameraType;
            }
            catch (Exception ex)
            {
            }

            oResult.set(CameraByTypeReturnType);
        }
        
        public override void choosePicture(IReadOnlyDictionary<string,string> propertyMap, IMethodResult oResult)       
        {
            try
            {
                m_StorechoosePictureResult = oResult;
                Initialize_choosePicture();

                try
                {
                    Store_CaptureImage_outputformat = propertyMap["outputformat"].ToLower().Trim();
                }
                catch (Exception ex)
                {

                }
                photoChooserTask = new PhotoChooserTask();
                photoChooserTask.Completed += photoChooserTask_Completed;
                photoChooserTask.Show();
            }
            catch (Exception ex)
            {
                m_choosePicture_output["status"] = "error";
                m_choosePicture_output["message"] = ex.Message;
                m_choosePicture_output["image_format"] = string.Empty;
                m_StorechoosePictureResult.set(m_choosePicture_output);
            }
        }
        private void Initialize_choosePicture()
        {
            m_choosePicture_output.Clear();
            m_choosePicture_output.Add("status", "cancel");
            m_choosePicture_output.Add("imageUri", string.Empty);
            m_choosePicture_output.Add("imageHeight", string.Empty);
            m_choosePicture_output.Add("imageWidth", string.Empty);
            m_choosePicture_output.Add("imageFormat", "jpg");
            m_choosePicture_output.Add("message", string.Empty);
            m_choosePicture_output.Add("image_uri", string.Empty);
            m_choosePicture_output.Add("image_height", string.Empty);
            m_choosePicture_output.Add("image_width", string.Empty);
            m_choosePicture_output.Add("image_format", "jpg");
        }
        void photoChooserTask_Completed(object sender, PhotoResult e)
        {
            try
            {
                if (e.TaskResult == TaskResult.OK)
                {
                    string ReturnValue = e.OriginalFileName;
                    BitmapImage bmp = new BitmapImage();
                    bmp.SetSource(e.ChosenPhoto);
                    m_choosePicture_output["imageHeight"] = bmp.PixelHeight.ToString();
                    m_choosePicture_output["imageWidth"] = bmp.PixelWidth.ToString() ;
                    m_choosePicture_output["image_height"] = bmp.PixelHeight.ToString();
                    m_choosePicture_output["image_width"] = bmp.PixelWidth.ToString();
                    m_choosePicture_output["status"] = "ok";

                    try
                    {
                        string Data=Data_Uri[Store_CaptureImage_outputformat];

                        MemoryStream ms = new MemoryStream();
                        WriteableBitmap wb = new WriteableBitmap(bmp);
                        wb.SaveJpeg(ms, bmp.PixelWidth, bmp.PixelHeight, 0, 100);
                        byte[] imagebytes = ms.ToArray();
                        e.ChosenPhoto.CopyTo(ms);
                        byte[] result = ms.ToArray();
                        ReturnValue = System.Convert.ToBase64String(result);
                    }
                    catch (Exception ex)
                    {
                    }
                    m_choosePicture_output["imageUri"] = ReturnValue;
                }
                else
                {
                    m_choosePicture_output["status"] = "cancel";
                    m_choosePicture_output["image_format"] = string.Empty;
                    m_choosePicture_output["imageFormat"] = string.Empty;

                }
            }
            catch (Exception ex)
            {
                m_choosePicture_output["status"] = "error";
                m_choosePicture_output["message"] = ex.Message;
                m_choosePicture_output["image_format"] = string.Empty;
            }
            finally
            {
                m_StorechoosePictureResult.set(m_choosePicture_output);
            }
            
        }

       

        public override void copyImageToDeviceGallery(string pathToImage, IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }

    public class CameraFactory : CameraFactoryBase
    {
    }
}

}
