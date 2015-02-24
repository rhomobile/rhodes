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


namespace rho
{

    namespace CameraImpl
    {
        public class Camera : CameraBase
        {
            #region UIElements Required for creating Camera region

            PhotoCamera Rho_StillCamera;
            MediaLibrary Rho_Store_Picturelibrary = new MediaLibrary();

            VideoBrush Rho_PhotoCameraBrush;
            Canvas Rho_PhotoCameraCanvas;
            TextBox txtDebug = new TextBox();
            Grid Store_PreviousGridElements;
            // Holds current flash mode.

            PhotoChooserTask Rho_photoChooserTask;
            PhoneApplicationFrame rootFrame;

            #endregion

            #region Variable Declartion region

            int Rho_Camera_selected = 0;
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
            IReadOnlyDictionary<string, string> Store_TakePicture_Arguments;
            CompositeTransform Rho_Camera_Rotation;
            Dictionary<PageOrientation, Dictionary<string, double>> CameraRotation;

            string Rho_FilePath = "C:\\Data\\Users\\Public\\Pictures\\Camera Roll\\";
            #endregion


            public Camera()
            {
                CRhoRuntime.getInstance().logEvent("Camera class -->Constructor");

            }

            #region CameraBase abstract class implimentation
            /// <summary>
            /// This is an overloaded method,We set the type of camera type.
            /// </summary>
            /// <param name="strID"></param>
            /// <param name="native"></param>
            public override void setNativeImpl(string strID, long native)
            {
                CRhoRuntime.getInstance().logEvent(" Camera class--> setNativeImpl" + strID);
                base.setNativeImpl(strID, native);
                InitializeCameraProperties(_strID);

            }
            /// <summary>
            /// Set Basic Camera style like type of camera.
            /// </summary>
            /// <param name="strId"> Front or Back camera</param>
            public void InitializeCameraProperties(string strId)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->Entered InitializeCameraProperties ");
                try
                {
                    // initialize class instance in C# here

                    try
                    {
                        m_CameraTypes.Clear();
                        m_CameraTypes.Add("back", 0);
                        m_CameraTypes.Add("front", 1);


                        // if strid is blank select the primary //Simha
                        Rho_Camera_selected = m_CameraTypes[strId];
                    }
                    catch (Exception ex)
                    {
                        CRhoRuntime.getInstance().logEvent("Camera class->Invalid Camera type Specified, So setting to default camera(back)");
                        Rho_Camera_selected = m_CameraTypes["back"];
                    }
                    using (PhotoCamera Temp_camera = new PhotoCamera((CameraType)Rho_Camera_selected))
                    {
                        Rho_Paramenters.Add("desired_height", Temp_camera.Resolution.Height);
                        Rho_Paramenters.Add("desired_width", Temp_camera.Resolution.Width);


                        Rho_AliasParametersName.Add("desiredheight", "desired_height");
                        Rho_AliasParametersName.Add("desiredwidth", "desired_width");
                        Rho_AliasParametersName.Add("flashmode", "flash_mode");



                        Rho_Paramenters.Add("camera_type", (int)Temp_camera.CameraType);

                        Rho_StringParameters.Add("filename", "Img");
                        Rho_StringParameters.Add("imageformat", "jpg");
                        Rho_StringParameters.Add("captureSound", string.Empty);


                        Rho_OutputType.Add("image", "outputformat");
                        Rho_OutputType.Add("datauri", "outputformat");
                        Rho_OutPutFormat.Add("outputformat", "image");


                        //CameraResolutionhsize  what is hsize meaning in this //Simha
                        IEnumerable<Size> CameraResolutionsize = Temp_camera.AvailableResolutions;
                        // change the variable name x,y to meaning ful name
                        Rho_Paramenters.Add("getmaxwidth", CameraResolutionsize.Max(x => x.Width));
                        Rho_Paramenters.Add("getmaxheight", CameraResolutionsize.Max(Y => Y.Height));
                        int Counter = 0;
                        foreach (Size ResolutionStyle in CameraResolutionsize)
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
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-> InitializeCameraProperties, Setting up Camera Related Variables-->Exception" + ex.Message);

                }
                CameraRotation = new Dictionary<PageOrientation, Dictionary<string, double>>();

                Dictionary<string, double> LandscapeLeft = new Dictionary<string, double>();

                LandscapeLeft.Add("rotation", -90);
                LandscapeLeft.Add("Height", Application.Current.Host.Content.ActualWidth);
                LandscapeLeft.Add("Width", Application.Current.Host.Content.ActualHeight);
                LandscapeLeft.Add("CameraType", 0);

                CameraRotation.Add(PageOrientation.LandscapeLeft, LandscapeLeft);



                Dictionary<string, double> LandscapeRight = new Dictionary<string, double>();
                LandscapeRight.Add("rotation", 90);
                LandscapeRight.Add("Height", Application.Current.Host.Content.ActualWidth);
                LandscapeRight.Add("Width", Application.Current.Host.Content.ActualHeight);

                CameraRotation.Add(PageOrientation.LandscapeRight, LandscapeRight);



                Dictionary<string, double> PortraitDown = new Dictionary<string, double>();
                PortraitDown.Add("rotation", 0);
                PortraitDown.Add("Height", Application.Current.Host.Content.ActualHeight);
                PortraitDown.Add("Width", Application.Current.Host.Content.ActualWidth);
                PortraitDown.Add("front", -180);

                CameraRotation.Add(PageOrientation.PortraitDown, PortraitDown);

                Dictionary<string, double> PortraitUp = new Dictionary<string, double>();
                PortraitUp.Add("rotation", 0);
                PortraitUp.Add("Height", Application.Current.Host.Content.ActualHeight);
                PortraitUp.Add("Width", Application.Current.Host.Content.ActualWidth);
                PortraitUp.Add("front", 180);

                CameraRotation.Add(PageOrientation.PortraitUp, PortraitUp);
                CRhoRuntime.getInstance().logEvent("Camera class-->End InitializeCameraProperties");


            }



            /// <summary>
            /// Get the kind of Camera (front or Back) facing.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getCameraType(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-> getCameraType");
                using (PhotoCamera TempCamera = new PhotoCamera((CameraType)Rho_Paramenters["camera_type"]))
                {
                    oResult.set(TempCamera.CameraType.ToString());
                }
            }

            /// <summary>
            /// Get Maximum Width of the Resolution.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getMaxWidth(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> getMaxWidth");
                try
                {
                    oResult.set(Rho_Paramenters["getmaxwidth"]);
                    return;
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-> getMaxWidth" + ex.Message);
                }


            }
            /// <summary>
            /// Get Maximum Height of the Resolution
            /// </summary>
            /// <param name="oResult"></param>

            public override void getMaxHeight(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> getMaxHeight");
                try
                {
                    oResult.set(Rho_Paramenters["getmaxheight"]);
                    return;
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class->getMaxHeight" + ex.Message);
                }


            }

            /// <summary>
            /// Get all the Supported Resolution of the specified Camera type.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getSupportedSizeList(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> Entered getSupportedSizeList");
                using (PhotoCamera TempCamera = new PhotoCamera((CameraType)Rho_Paramenters["camera_type"]))
                {

                    List<Dictionary<string, string>> RTypes = new List<Dictionary<string, string>>();

                    try
                    {
                        //Simha //Unused then remove it

                        IEnumerable<Size> objsize = TempCamera.AvailableResolutions;
                        foreach (Size size in objsize)
                        {
                            Dictionary<string, string> Store_Resolution = new Dictionary<string, string>();
                            Store_Resolution.Add("width", size.Width.ToString());
                            Store_Resolution.Add("height", size.Height.ToString());
                            RTypes.Add(Store_Resolution);

                        }
                    }
                    catch (Exception ex)
                    {
                        CRhoRuntime.getInstance().logEvent("Camera class->" + ex.Message);
                    }
                    oResult.set(RTypes);

                }
                CRhoRuntime.getInstance().logEvent("Camera class--> End getSupportedSizeList");
            }

            /// <summary>
            /// Get the setted Resolution Width of the Camera Type (Back/Front).
            /// </summary>
            /// <param name="oResult"></param>

            public override void getDesiredWidth(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> getDesiredWidth");
                oResult.set(Rho_Paramenters["desired_width"].ToString());
            }

            /// <summary>
            /// Sets the  Width from the avaialble Resolution,if not available then sets to the nearest available Resolution.
            /// </summary>
            /// <param name="desiredWidth">Width to be setted </param>
            /// <param name="oResult"></param>
            public override void setDesiredWidth(int desiredWidth, IMethodResult oResult)
            {
                try
                {
                    CRhoRuntime.getInstance().logEvent("Camera class--> setDesiredWidth " + desiredWidth);
                    double closestWidth = Rho_Screen_Resolution_Width.Aggregate((x, y) => Math.Abs(x - desiredWidth) < Math.Abs(y - desiredWidth) ? x : y);
                    Rho_Paramenters["desired_width"] = closestWidth;
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->setDesiredWidth--> Exception" + ex.ToString());
                }
            }
            /// <summary>
            /// Get the setted Resolution Height of the Camera Type (Back/Front).
            /// </summary>
            /// <param name="oResult"></param>
            public override void getDesiredHeight(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> getDesiredHeight");
                oResult.set(Rho_Paramenters["desired_height"].ToString());
            }
            /// <summary>
            /// Sets the  Height from the avaialble Resolution,if not available then sets to the nearest available Resolution.
            /// </summary>
            /// <param name="desiredWidth">Height from the Resolution </param>
            /// <param name="oResult"></param>
            public override void setDesiredHeight(int desiredHeight, IMethodResult oResult)
            {
                try
                {
                    CRhoRuntime.getInstance().logEvent("Camera class--> setDesiredHeight" + desiredHeight);
                    double closestHeight = Rho_Screen_Resolution_Height.Aggregate((x, y) => Math.Abs(x - desiredHeight) < Math.Abs(y - desiredHeight) ? x : y);
                    Rho_Paramenters["desired_height"] = closestHeight;
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->setDesiredHeight--> Exception" + ex.ToString());
                }
            }

            /// <summary>
            /// Gets the File Name to be used when picture to be saved under CameraRoll.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getFileName(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> getFileName" + Rho_StringParameters["filename"]);
                oResult.set(Rho_StringParameters["filename"]);
            }

            /// <summary>
            /// Sets the File Name to be used when picture to be saved under CameraRoll.
            /// </summary>
            /// <param name="fileName"></param>
            /// <param name="oResult"></param>
            public override void setFileName(string fileName, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> setFileName " + fileName);
                Rho_StringParameters["filename"] = fileName;
            }

            /// <summary>
            /// Gets the Compression Format in WP8 we support only Jpeg
            /// </summary>
            /// <param name="oResult"></param>
            public override void getCompressionFormat(IMethodResult oResult)
            {
                oResult.set(Rho_StringParameters["imageformat"]);
            }

            public override void setCompressionFormat(string compressionFormat, IMethodResult oResult)
            {
                //AS WP8 does not support any other format apart from jpeg, need to check in 8.1 
                // Rho_StringParameters["ImageFormat"] = compressionFormat;
            }
            /// <summary>
            /// get either dataURI(Base64 shall be sent) or image(path of the captured jpeg file).
            /// </summary>
            /// <param name="oResult"></param>
            public override void getOutputFormat(IMethodResult oResult)
            {
                oResult.set(Rho_OutPutFormat["outputformat"]);
            }
            /// <summary>
            /// get either dataURI or image.
            /// </summary>
            /// <param name="outputFormat">dataURI(Base64 shall be sent) or image(path of the captured jpeg file) </param>
            /// <param name="oResult"></param>
            public override void setOutputFormat(string outputFormat, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class->setOutputFormat type");
                Rho_OutPutFormat.Clear();
                try
                {
                    string storevalue = Rho_OutPutFormat[outputFormat.ToLower().Trim()];
                    Rho_OutPutFormat.Add("outputformat", outputFormat.ToLower().Trim());

                }

                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class->invalid setOutputFormat " + outputFormat + " Exception " + ex.ToString());
                    Rho_OutPutFormat.Clear();
                    Rho_OutPutFormat.Add("outputformat", "image");

                }




            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getColorModel(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="colorModel"></param>
            /// <param name="oResult"></param>
            public override void setColorModel(string colorModel, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getEnableEditing(IMethodResult oResult)
            {

            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="enableEditing"></param>
            /// <param name="oResult"></param>
            public override void setEnableEditing(bool enableEditing, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Get the kind of flash mode setted (On,Off, Auto)
            /// </summary>
            /// <param name="oResult"></param>
            public override void getFlashMode(IMethodResult oResult)
            {
                string FlashModeType = FlashMode.Auto.ToString();
                try
                {
                    CRhoRuntime.getInstance().logEvent("Camera class--> getFlashMode");
                    IEnumerable<KeyValuePair<string, int>> Flashmode = Rho_Flash_Types.Select(x => x).Where(k => k.Value == (int)Rho_FlashMode).ToList();
                    FlashModeType = string.Empty; //give proper varible name for returnablevalue //Simha
                    foreach (KeyValuePair<string, int> obj in Flashmode)
                    {
                        FlashModeType = obj.Key;
                    }
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class->invalid getFlashMode " + ex.ToString());
                }
                oResult.set(FlashModeType);
            }

            /// <summary>
            /// Set the kind of flash mode (On,Off,Auto).
            /// </summary>
            /// <param name="flashMode"></param>
            /// <param name="oResult"></param>
            public override void setFlashMode(string flashMode, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> setFlashMode");
                try
                {
                    Rho_FlashMode = (FlashMode)(Rho_Flash_Types[flashMode.ToLower().Trim()]);
                    Rho_Flashmodes["flash_mode"] = flashMode.ToLower().Trim();

                }
                catch (Exception ex)
                {
                    Rho_FlashMode = (FlashMode)(Rho_Flash_Types[FLASH_AUTO]);
                    CRhoRuntime.getInstance().logEvent("Camera class->invalid setFlashMode " + ex.ToString());
                }
            }

            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getSaveToDeviceGallery(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="saveToDeviceGallery"></param>
            /// <param name="oResult"></param>
            public override void setSaveToDeviceGallery(bool saveToDeviceGallery, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getCaptureSound(IMethodResult oResult)
            {
                oResult.set(Rho_StringParameters["captureSound"]);
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="captureSound"></param>
            /// <param name="oResult"></param>
            public override void setCaptureSound(string captureSound, IMethodResult oResult)
            {
                Rho_StringParameters["captureSound"] = captureSound;
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getPreviewLeft(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="previewLeft"></param>
            /// <param name="oResult"></param>
            public override void setPreviewLeft(int previewLeft, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getPreviewTop(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="previewTop"></param>
            /// <param name="oResult"></param>
            public override void setPreviewTop(int previewTop, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getPreviewWidth(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="previewWidth"></param>
            /// <param name="oResult"></param>
            public override void setPreviewWidth(int previewWidth, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getPreviewHeight(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="previewHeight"></param>
            /// <param name="oResult"></param>
            public override void setPreviewHeight(int previewHeight, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getUseSystemViewfinder(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="useSystemViewfinder"></param>
            /// <param name="oResult"></param>
            public override void setUseSystemViewfinder(bool useSystemViewfinder, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getAimMode(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="aimMode"></param>
            /// <param name="oResult"></param>
            public override void setAimMode(string aimMode, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="propertyMap"></param>
            /// <param name="oResult"></param>
            public override void showPreview(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void hidePreview(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in WP8.
            /// </summary>
            /// <param name="oResult"></param>
            public override void capture(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Set the Configuration as per parameters of camera of take Image
            /// </summary>
            /// <param name="propertyMap">Contains the details of paramenters sent by user to takepicture</param>
            private void SetCameraConfiguration(IReadOnlyDictionary<string, string> propertyMap)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> SetCameraConfiguration");
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
                        Rho_Flashmodes[ParametersKeyName] = Parameters.Value.ToString().Trim();
                        Rho_FlashMode = (FlashMode)(Rho_Flash_Types[Rho_Flashmodes[ParametersKeyName]]);
                        
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
            /// <summary>
            /// To show Still Camera, on taking the picture we shall move to previous screen.
            /// </summary>
            /// <param name="propertyMap">Contains the arguments set by user.</param>
            /// <param name="oResult"></param>
            public override void takePicture(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> takePicture");
                try
                {
                    Store_TakePicture_Arguments = propertyMap;
                    SetCameraConfiguration(propertyMap);
                    Initialize_TakePictureCallBack();
                    Store_PreviousGridElements = LayoutGrid;       
                    Rho_Create_Camera_Layout();                                

                    InitializeEventsRelatedtoCamera();

                    m_StoreTakePictureResult = oResult;
                  

                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class--> takePicture exception" + ex.ToString());
                    m_Take_Picture_Output["status"] = "error";
                    m_Take_Picture_Output["message"] = ex.Message;
                    m_Take_Picture_Output["image_format"] = string.Empty;
                    oResult.set(m_Take_Picture_Output);
                }
                CRhoRuntime.getInstance().logEvent("Camera class--> End takePicture");
            }





            #endregion

            # region takeImage call the below methods.

            /// <summary>
            /// On Unlock of Phone this event is fired.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            void rootFrame_Unobscured(object sender, EventArgs e)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> rootFrame_Unobscured");
                UninitializeRegisteredEvents();
                takePicture(Store_TakePicture_Arguments, m_StoreTakePictureResult);
            }
            /// <summary>
            /// Initialize Camera Related Events
            /// </summary>
            private void InitializeEventsRelatedtoCamera()
            {
                rootFrame.Unobscured += rootFrame_Unobscured;

                //On Clicking the Camera Canvas Lets consider it as a camera click.
                Rho_PhotoCameraCanvas.Tap += Rho_PhotoCameraCanvas_Tap;
                CameraButtons.ShutterKeyPressed += CameraButtons_ShutterKeyPressed;
                Rho_MainPage.OrientationChanged += Rho_MainPage_OrientationChanged;
                Rho_StillCamera.Initialized += new EventHandler<Microsoft.Devices.CameraOperationCompletedEventArgs>(cam_Initialized);

                // Event is fired when the capture sequence is complete and an image is available.
                Rho_StillCamera.CaptureImageAvailable += new EventHandler<Microsoft.Devices.ContentReadyEventArgs>(cam_CaptureImageAvailable);
                Rho_MainPage.BackKeyPress += Rho_MainPage_BackKeyPress;
            }

            /// <summary>
            /// Uninitialize all events which are registered in TakePicture
            /// </summary>
            private void UninitializeRegisteredEvents()
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> UninitializeRegisteredEvents");
                Rho_MainPage.OrientationChanged -= Rho_MainPage_OrientationChanged;
                rootFrame.Unobscured -= rootFrame_Unobscured;
                Rho_MainPage.BackKeyPress -= Rho_MainPage_BackKeyPress;
                Rho_StillCamera.CaptureImageAvailable -= new EventHandler<Microsoft.Devices.ContentReadyEventArgs>(cam_CaptureImageAvailable);

                Rho_StillCamera.Initialized -= new EventHandler<Microsoft.Devices.CameraOperationCompletedEventArgs>(cam_Initialized);

                Rho_StillCamera.CaptureImageAvailable -= new EventHandler<Microsoft.Devices.ContentReadyEventArgs>(cam_CaptureImageAvailable);
                CameraButtons.ShutterKeyPressed -= CameraButtons_ShutterKeyPressed;
            }
            /// <summary>
            /// Return to the previous screen.
            /// </summary>
            void Return_To_Previous_Screen()
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> Return_To_Previous_Screen");
                try
                {
                    UninitializeRegisteredEvents();

                    LayoutGrid.Children.Remove(Rho_PhotoCameraCanvas);
                    LayoutGrid = Store_PreviousGridElements;

                    Rho_PhotoCameraCanvas = null;
                   Rho_PhotoCameraBrush = null;
                   GC.Collect();


                    Rho_MainPage.SupportedOrientations = SupportedPageOrientation.PortraitOrLandscape;
                    Rho_MainPage.Orientation = PageOrientation.Portrait;
                    

                }
                catch (AccessViolationException et)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->Return_To_Previous_Screen--> AccessViolationException" + et.ToString());
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->Return_To_Previous_Screen-->Exception" + ex.ToString());
                }
            }
            /// <summary>
            /// Set the default value for the call back parameters
            /// </summary>
            private void Initialize_TakePictureCallBack()
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->Initialize_TakePictureCallBack");
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

            /// <summary>
            /// on clicking of BackPress on Mobile.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            void Rho_MainPage_BackKeyPress(object sender, System.ComponentModel.CancelEventArgs e)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->Rho_MainPage_BackKeyPress");
                try
                {
                    Return_To_Previous_Screen();
                    e.Cancel = true;
                    m_Take_Picture_Output["image_format"] = string.Empty;
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->Rho_MainPage_BackKeyPress -->Error" + ex.ToString());
                    m_Take_Picture_Output["status"] = "error";
                    m_Take_Picture_Output["message"] = ex.Message;
                    m_Take_Picture_Output["image_format"] = string.Empty;
                }
                m_StoreTakePictureResult.set(m_Take_Picture_Output);
            }

            protected AudioVideoCaptureDevice Device { get; set; }


            /// <summary>
            /// When camera is up.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>

            void cam_Initialized(object sender, Microsoft.Devices.CameraOperationCompletedEventArgs e)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->cam_Initialized");
                if (e.Succeeded)
                {
                    Rho_PhotoCameraCanvas.Dispatcher.BeginInvoke(delegate()
                    {
                        // Write message.
                        txtDebug.Text = "Camera initialized.";
                        Rho_StillCamera.FlashMode = Rho_FlashMode;
                        try
                        {
                            KeyValuePair<double, Size> CameraResolution = Rho_Supported_Resolutions.Aggregate((x, y) => Math.Abs(x.Value.Height - Rho_Paramenters["desired_height"]) < Math.Abs(y.Value.Height - Rho_Paramenters["desired_height"]) ? x : y);
                            Rho_StillCamera.Resolution = CameraResolution.Value;
                        }
                        catch (Exception ex)
                        {
                            CRhoRuntime.getInstance().logEvent("Camera class-->cam_Initialized-->Exception"+ex.ToString());
                        }                     



                    });
                }
                else
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->cam_Initialized-->Exception" + e.Exception.ToString());
                    m_Take_Picture_Output["status"] = "error";
                    m_Take_Picture_Output["message"] = e.Exception.Message;
                    m_Take_Picture_Output["image_format"] = string.Empty;
                    m_StoreTakePictureResult.set(m_Take_Picture_Output);
                }
            }

            /// <summary>
            /// After clicking image this event is fired.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            void cam_CaptureImageAvailable(object sender, Microsoft.Devices.ContentReadyEventArgs e)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->cam_CaptureImageAvailable");
                string fileName = Rho_StringParameters["filename"] + "_" + DateTime.Now.ToLongDateString() + "_" + DateTime.Now.ToLongTimeString().Replace(':', '_');


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
                                returnablevalue = Rho_FilePath + fileName + ".jpg";
                                // Check filename contains .jpg if exists don't add the extension otherwise add it // Simha
                            }

                        }
                        catch (Exception ex)
                        {

                            CRhoRuntime.getInstance().logEvent("Camera class-->cam_CaptureImageAvailable-->Dispatcher-->exception" + ex.ToString());
                        }

                        try
                        {
                            IEnumerable<KeyValuePair<string, string>> OutPutFormat = Rho_OutPutFormat.Select(x => x).Where(k => k.Value == "datauri").ToList();

                            foreach (KeyValuePair<string, string> format in OutPutFormat)
                            {

                                returnablevalue = "data:image/jpeg;base64," + strbase64;
                            }
                        }
                        catch (Exception ex)
                        {
                            CRhoRuntime.getInstance().logEvent("Camera class-->cam_CaptureImageAvailable-->OutPutFormat-->exception" + ex.ToString());
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
                        CRhoRuntime.getInstance().logEvent("Camera class-->cam_CaptureImageAvailable-->exception" + ex.ToString());
                        m_Take_Picture_Output["status"] = "error";
                        m_Take_Picture_Output["message"] = ex.Message;
                        m_Take_Picture_Output["image_format"] = string.Empty;
                    }
                    finally
                    {
                        // Close image stream
                        e.ImageStream.Close();
                        m_StoreTakePictureResult.set(m_Take_Picture_Output);
                    }
                });

            }


            /// <summary>
            /// On pressing Camera Hardware button
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            void CameraButtons_ShutterKeyPressed(object sender, System.EventArgs e)
            {
                CaptureImage();
            }

            /// <summary>
            /// on clicking on Screen.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            void Rho_PhotoCameraCanvas_Tap(object sender, System.Windows.Input.GestureEventArgs e)
            {
                CaptureImage();
            }

            /// <summary>
            /// Capture the Image.
            /// </summary>
            void CaptureImage()
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->CaptureImage");
                if (Rho_StillCamera != null)
                {

                    try
                    {
                        // Start image capture.
                        rootFrame.Unobscured -= rootFrame_Unobscured;
                        Rho_StillCamera.CaptureImage();

                    }
                    catch (Exception ex)
                    {
                        CRhoRuntime.getInstance().logEvent("Camera class-->CaptureImage-->exception" + ex.ToString());

                    }
                }
            }



            #endregion
            /// <summary>
            /// On changing the screen Orientation.
            /// </summary>
            /// <param name="sender">MainPage</param>
            /// <param name="e">Type of Orienation</param>

            void Rho_MainPage_OrientationChanged(object sender, OrientationChangedEventArgs e)
            {

                CRhoRuntime.getInstance().logEvent("Camera class-->Rho_MainPage_OrientationChange");
                Rho_PhotoCameraCanvas.Dispatcher.BeginInvoke(() =>
                {
                    SetCameraRotation(e.Orientation);
                });

            }
            /// <summary>
            /// Initialize/Set the camera as per the Screen Rotation
            /// </summary>
            /// <param name="OrientationStyle"> Type of Orientation</param>

            void SetCameraRotation(PageOrientation OrientationStyle)
            {
                try
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->SetCameraRotation");
                    double rotation = Rho_StillCamera.Orientation;



                    // double strvalue = CameraRotation[OrientationStyle]["rotation"];
                    rotation = Rho_StillCamera.Orientation + CameraRotation[OrientationStyle]["rotation"];
                    Rho_PhotoCameraCanvas.Height = CameraRotation[OrientationStyle]["Height"];
                    Rho_PhotoCameraCanvas.Width = CameraRotation[OrientationStyle]["Width"];
                    try
                    {
                        rotation = Rho_StillCamera.Orientation + CameraRotation[OrientationStyle][_strID];
                    }
                    catch (Exception ex)
                    {
                        CRhoRuntime.getInstance().logEvent("Camera class-->SetCameraRotation-->LandScapeMode" + ex.ToString());
                    }



                    Rho_Camera_Rotation.Rotation = rotation;
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->SetCameraRotation-->Exception" + ex.ToString());
                }
            }
            # region Set the camera layout
            /// <summary>
            /// Create a camera Layout.
            /// </summary>
            private void Rho_Create_Camera_Layout()
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->Rho_Create_Camera_Layout");
                try
                {
                    rootFrame = Rho_MainPage.RootVisual() as PhoneApplicationFrame;  

                      //Create a desired Camera Object.
                    Rho_StillCamera = new Microsoft.Devices.PhotoCamera((CameraType)Rho_Camera_selected);
                    //Create a Video Brush for painting in Canvas.  
                    Rho_PhotoCameraBrush = new VideoBrush();
                    //Create a Camera Rotaion Object.
                    Rho_Camera_Rotation = new CompositeTransform();
                    //Initialize CenterX and CenterY for Rho Camera Roation
                    Rho_Camera_Rotation.CenterX = 0.5;
                    Rho_Camera_Rotation.CenterY = 0.5;
                    //Add the Screen rotation to Composite Transformation object as this object takes care of rotating the camera.
                    Rho_Camera_Rotation.Rotation = Rho_StillCamera.Orientation;

                    Rho_PhotoCameraBrush.Stretch = Stretch.Fill;
                    //Set Composite transform object to PhotoBrush.
                    Rho_PhotoCameraBrush.RelativeTransform = Rho_Camera_Rotation;
                    // Create a Canvas for painting the image.
                    Rho_PhotoCameraCanvas = new Canvas();
                    //Create Canvas for the whole screen.

                    ColumnDefinition Rho_Camera_Columncollection = new ColumnDefinition();

                    //check if the set orienation is required, if it is front camera then we may have to do some extra calculation. else it appears upside down.
                    SetCameraRotation(Rho_MainPage.Orientation);
                    //Set Desired camera as a child for CAmera Brush.
                    Rho_PhotoCameraBrush.SetSource(Rho_StillCamera);
                    //Add Camera Brush as background to the Canvas.            
                    Rho_PhotoCameraCanvas.Background = Rho_PhotoCameraBrush;
                                       

                    //Set the Width of the Parent Frame to the Complete Device.


                    GridLength Rho_Camera_Gridlength = new GridLength(Application.Current.Host.Content.ActualWidth);
                    Rho_Camera_Columncollection.Width = Rho_Camera_Gridlength;
                    //Add canvas to the Main screen object.
                   LayoutGrid.Children.Add(Rho_PhotoCameraCanvas);                   
                   


                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->Rho_Create_Camera_Layout-->Exception " + ex.ToString());
                }



            }

            #endregion


        }
        #region Singleton class
        /// <summary>
        /// Singleton class
        /// </summary>
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

            /// <summary>
            /// Initialize DataURI and Image types.
            /// </summary>
            public CameraSingleton()
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->CameraSingleton");
                Data_Uri.Add("datauri", "outputformat");
                Image.Add("image", "outputformat");

                // initialize singleton instance in C# here
            }
            /// <summary>
            /// Get the list of supported Camera
            /// </summary>
            /// <param name="oResult"></param>
            public override void enumerate(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->enumerate");
                List<string> AvailabeCameras = new List<string>();


                try
                {
                    PhotoCamera Primary = new PhotoCamera(CameraType.Primary);
                    AvailabeCameras.Add("back");
                }
                catch (Exception ex)
                {
                }
                try
                {
                    PhotoCamera FrontCamera = new PhotoCamera(CameraType.FrontFacing);
                    AvailabeCameras.Add("front");

                }
                catch (Exception ex)
                {
                    //Throw the Exception //Simha
                }

                oResult.set(AvailabeCameras);

            }

            /// <summary>
            /// Get whether the supplied camera type is supported or not.
            /// </summary>
            /// <param name="cameraType">type of camera supported (back or front)</param>
            /// <param name="oResult"></param>
            public override void getCameraByType(string cameraType, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->enumerate");
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
                    CRhoRuntime.getInstance().logEvent("Camera class-->enumerate-->Exception" + ex.ToString());
                }

                oResult.set(CameraByTypeReturnType);
            }
            /// <summary>
            /// Choose the Pictures from the shown list.
            /// </summary>
            /// <param name="propertyMap">supports only outputformat.</param>
            /// <param name="oResult"></param>
            public override void choosePicture(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->choosePicture");
                try
                {
                    m_StorechoosePictureResult = oResult;
                    Initialize_choosePicture();

                    try
                    {
                        Store_CaptureImage_outputformat = propertyMap["outputFormat"].ToLower().Trim();
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
                    CRhoRuntime.getInstance().logEvent("Camera class-->choosePicture-->Exception" + ex.ToString());
                    m_choosePicture_output["status"] = "error";
                    m_choosePicture_output["message"] = ex.Message;
                    m_choosePicture_output["image_format"] = string.Empty;
                    m_StorechoosePictureResult.set(m_choosePicture_output);
                }
            }

            /// <summary>
            /// Initialize the callback with the default values.
            /// </summary>
            private void Initialize_choosePicture()
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->Initialize_choosePicture");
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
            /// <summary>
            /// After selecting the Picture this event is raised.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            void photoChooserTask_Completed(object sender, PhotoResult e)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->photoChooserTask_Completed");
                try
                {
                    if (e.TaskResult == TaskResult.OK)
                    {
                        string ReturnValue = e.OriginalFileName;
                        BitmapImage bmp = new BitmapImage();
                        bmp.SetSource(e.ChosenPhoto);
                        m_choosePicture_output["imageHeight"] = bmp.PixelHeight.ToString();
                        m_choosePicture_output["imageWidth"] = bmp.PixelWidth.ToString();
                        m_choosePicture_output["image_height"] = bmp.PixelHeight.ToString();
                        m_choosePicture_output["image_width"] = bmp.PixelWidth.ToString();
                        m_choosePicture_output["status"] = "ok";

                        try
                        {
                            string Data = Data_Uri[Store_CaptureImage_outputformat];

                            MemoryStream ms = new MemoryStream();
                            WriteableBitmap wb = new WriteableBitmap(bmp);
                            wb.SaveJpeg(ms, bmp.PixelWidth, bmp.PixelHeight, 0, 100);
                            byte[] imagebytes = ms.ToArray();
                            e.ChosenPhoto.CopyTo(ms);
                            byte[] result = ms.ToArray();
                            ReturnValue = "data:image/jpeg;base64," + System.Convert.ToBase64String(result);
                        }
                        catch (Exception ex)
                        {
                            CRhoRuntime.getInstance().logEvent("Camera class-->Not an Data URI");
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
                    CRhoRuntime.getInstance().logEvent("Camera class-->photoChooserTask_Completed--> Exception" + ex.ToString());
                    m_choosePicture_output["status"] = "error";
                    m_choosePicture_output["message"] = ex.Message;
                    m_choosePicture_output["image_format"] = string.Empty;
                }
                finally
                {
                    m_StorechoosePictureResult.set(m_choosePicture_output);
                }

            }


            /// <summary>
            /// Not supported by WP8.
            /// </summary>
            /// <param name="pathToImage"></param>
            /// <param name="oResult"></param>
            public override void copyImageToDeviceGallery(string pathToImage, IMethodResult oResult)
            {
                // implement this method in C# here
            }
        }

        public class CameraFactory : CameraFactoryBase
        {
        }
        #endregion
    }

}
