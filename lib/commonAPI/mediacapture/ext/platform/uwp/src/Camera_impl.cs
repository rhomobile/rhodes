using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using rhodes;
using rhoruntime;
using Windows.Graphics.Display;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.Storage.Pickers;
using Windows.Media.Capture;
using Windows.Storage.Streams;
using Windows.Graphics.Imaging;
using Windows.UI.Xaml.Media.Imaging;
using Windows.Media.Editing;
using Windows.Media.Core;
using Windows.Foundation;
using Windows.Devices.Enumeration;
using Windows.System.Display;



namespace rho
{
    namespace CameraImpl
    {
        public class Camera : CameraBase
        {
            DeviceInformation cameraInformation = null;
            string cameraType = null;
            
            Dictionary<string, string> rhoParameters = new Dictionary<string, string>();
            Size resolution;
            Size maxResolution;
            bool isFormatJPG = true;
            List<Size> imageResolutions = new List<Size>();

            static Dictionary<string, DeviceInformation> availableCameras = new Dictionary<string, DeviceInformation>();

            public static async Task<DeviceInformationCollection> getDevices()
            {
                DeviceInformationCollection result = await DeviceInformation.FindAllAsync(DeviceClass.VideoCapture);
                return result;
            }

            static public Dictionary<string, DeviceInformation> initCameraIDs()
            {
                deb("Finds all video capture devices");
                if (availableCameras.Count == 0){
                    try{
                        var task = getDevices();
                        
                        try{task.Start();}
                        catch(Exception e){deb("Task not started: " + e.Message);}
                        
                        try{
                            if (task.Status == TaskStatus.Running){task.Wait();}
                        } catch (Exception e){deb("Task not waited: " + e.Message);}    

                        DeviceInformationCollection devices = task.Result;
                        
                        deb("Trying to find camera from device list, size = " + devices.Count);
                        foreach (var device in devices){
                            deb("Checking device");
                            try{
                                //if (MediaCapture.IsVideoProfileSupported(device.Id)){
                                    if (device.EnclosureLocation.Panel.Equals(Windows.Devices.Enumeration.Panel.Front)){
                                        availableCameras.Add(CAMERA_TYPE_FRONT, device);
                                        deb("Camera found: " + CAMERA_TYPE_FRONT);
                                    }else if (device.EnclosureLocation.Panel.Equals(Windows.Devices.Enumeration.Panel.Back)){
                                        availableCameras.Add(CAMERA_TYPE_BACK, device);
                                        deb("Camera found: " + CAMERA_TYPE_BACK);
                                    }else{
                                        deb("Found camera in strange place");
                                    }
                                //}
                            }
                            catch (Exception e){deb(e.Message);}
                        }
                    }catch(Exception e){deb(e.Message);}
                }
                return availableCameras;
            }

            public Camera(string id) : base(id)
            {
                initCameraIDs();
                cameraType = id;
                deb("Creating camera with ID: " + cameraType);
                try{
                    cameraInformation = initCameraIDs()[cameraType];
                }
                catch (Exception e) {
                    try{
                        cameraInformation = initCameraIDs()["back"];
                    }catch(Exception exception){cameraInformation = null;}
                }

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
                try{
                    CRhoRuntime.getInstance().logEvent(" Camera class--> setNativeImpl" + strID);
                    base.setNativeImpl(strID, native);
                    cameraType = strID;
                    cameraInformation = initCameraIDs()[cameraType];
                }
                catch (Exception ex){}

                try
                {
                    var mediaInitSettings = new MediaCaptureInitializationSettings { VideoDeviceId = cameraInformation.Id };
                    IReadOnlyList<MediaCaptureVideoProfile> profiles = MediaCapture.FindAllVideoProfiles(cameraInformation.Id);

                    foreach (MediaCaptureVideoProfile element in profiles){
                        foreach (MediaCaptureVideoProfileMediaDescription description in element.SupportedRecordMediaDescription){
                            try{
                                imageResolutions.Add(new Size(description.Width, description.Height));
                            } catch (Exception ex){
                                CRhoRuntime.getInstance().logEvent("Camera class->" + ex.Message);
                            }
                        }
                    }
                }
                catch (Exception e) { }

            }
            
            /// <summary>
            /// Get the kind of Camera (front or Back) facing.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getCameraType(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-> getCameraType");
                try{
                    oResult.set(cameraType);
                }catch (Exception ex){
                    CRhoRuntime.getInstance().logEvent("Camera class-> getMaxWidth" + ex.Message);
                }
            }

            /// <summary>
            /// Get Maximum Width of the Resolution.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getMaxWidth(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> getMaxWidth");
                try{
                    oResult.set(maxResolution.Width);
                }
                catch (Exception ex){
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
                try{
                    oResult.set(maxResolution.Height);
                }
                catch (Exception ex){
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
                List<Dictionary<string, string>> RTypes = new List<Dictionary<string, string>>();

                foreach (Size size in imageResolutions){
                        Dictionary<string, string> Store_Resolution = new Dictionary<string, string>();
                        Store_Resolution.Add("width", size.Width.ToString());
                        Store_Resolution.Add("height", size.Height.ToString());
                        RTypes.Add(Store_Resolution);
                }
                oResult.set(RTypes);
                CRhoRuntime.getInstance().logEvent("Camera class--> End getSupportedSizeList");

            }

            /// <summary>
            /// Get the setted Resolution Width of the Camera Type (Back/Front).
            /// </summary>
            /// <param name="oResult"></param>

            public override void getDesiredWidth(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> getDesiredWidth");
                oResult.set(resolution.Width);
            }

            /// <summary>
            /// Sets the  Width from the avaialble Resolution,if not available then sets to the nearest available Resolution.
            /// </summary>
            /// <param name="desiredWidth">Width to be setted </param>
            /// <param name="oResult"></param>
            public override void setDesiredWidth(int desiredWidth, IMethodResult oResult)
            {
                try{
                    CRhoRuntime.getInstance().logEvent("Camera class--> setDesiredWidth " + desiredWidth);
                    resolution.Width = desiredWidth;
                }
                catch (Exception ex){
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
                oResult.set(resolution.Height);
            }
            /// <summary>
            /// Sets the  Height from the avaialble Resolution,if not available then sets to the nearest available Resolution.
            /// </summary>
            /// <param name="desiredWidth">Height from the Resolution </param>
            /// <param name="oResult"></param>
            public override void setDesiredHeight(int desiredHeight, IMethodResult oResult)
            {
                try{
                    CRhoRuntime.getInstance().logEvent("Camera class--> setDesiredHeight" + desiredHeight);
                    resolution.Height = desiredHeight;
                }catch (Exception ex){
                    CRhoRuntime.getInstance().logEvent("Camera class-->setDesiredHeight--> Exception" + ex.ToString());
                }

            }

            /// <summary>
            /// Gets the File Name to be used when picture to be saved under CameraRoll.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getFileName(IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> getFileName" + rhoParameters["filename"]);
                oResult.set(rhoParameters["filename"]);
            }

            /// <summary>
            /// Sets the File Name to be used when picture to be saved under CameraRoll.
            /// </summary>
            /// <param name="fileName"></param>
            /// <param name="oResult"></param>
            public override void setFileName(string fileName, IMethodResult oResult)
            {
                Dictionary<bool, bool> stringnullorempty = new Dictionary<bool, bool>();
                stringnullorempty.Add(false, false);
                try{
                    bool filenameemptyornull = stringnullorempty[String.IsNullOrEmpty(fileName)];
                    CRhoRuntime.getInstance().logEvent("Camera class--> setFileName " + fileName);
                    rhoParameters["filename"] = fileName;
                }catch (Exception ex){
                    rhoParameters["filename"] = "Img";
                }
            }

            /// <summary>
            /// Gets the Compression Format in UWP we support only Jpeg
            /// </summary>
            /// <param name="oResult"></param>
            public override void getCompressionFormat(IMethodResult oResult)
            {
                oResult.set(rhoParameters["imageformat"]);
            }

            /// <summary>
            ///     Not Supported in UWP
            /// </summary>
            /// <param name="compressionFormat"></param>
            /// <param name="oResult"></param>
            public override void setCompressionFormat(string compressionFormat, IMethodResult oResult)
            {
                //AS UWP does not support any other format apart from jpeg
                rhoParameters["ImageFormat"] = compressionFormat;
            }
            /// <summary>
            /// get either dataURI(Base64 shall be sent) or image(path of the captured jpeg file).
            /// </summary>
            /// <param name="oResult"></param>
            public override void getOutputFormat(IMethodResult oResult)
            {
                // AS UWP does not support any other format apart from jpg
                oResult.set(isFormatJPG?"jpg":"png");
            }
            /// <summary>
            /// get either dataURI or image.
            /// </summary>
            /// <param name="outputFormat">dataURI(Base64 shall be sent) or image(path of the captured jpeg file) </param>
            /// <param name="oResult"></param>
            public override void setOutputFormat(string outputFormat, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class->setOutputFormat type");
                try
                {
                    /*string DataURI = Rho_OutputType[outputFormat.ToLower().Trim()];
                    Rho_OutPutFormat.Clear();
                    Rho_OutPutFormat.Add("outputformat", outputFormat.ToLower().Trim());*/
                    if (outputFormat.ToLower().Trim() == "png") { isFormatJPG = false; }else
                    {
                        isFormatJPG = true;
                    }
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class->invalid setOutputFormat " + outputFormat + " Exception " + ex.ToString());
                }
                       

            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getColorModel(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="colorModel"></param>
            /// <param name="oResult"></param>
            public override void setColorModel(string colorModel, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getEnableEditing(IMethodResult oResult)
            {

            }
            /// <summary>
            /// Not supported in UWP.
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

                string FlashModeType = FLASH_AUTO;
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
                var Rho_FlashMode = flashMode;
                try
                {
                    Rho_FlashMode = flashMode.ToLower().Trim();
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class->invalid setFlashMode " + ex.ToString());
                }

            }

            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getSaveToDeviceGallery(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="saveToDeviceGallery"></param>
            /// <param name="oResult"></param>
            public override void setSaveToDeviceGallery(bool saveToDeviceGallery, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getCaptureSound(IMethodResult oResult)
            {
                // implement this method in C# here
                //oResult.set(Rho_StringParameters["captureSound"]);
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="captureSound"></param>
            /// <param name="oResult"></param>
            public override void setCaptureSound(string captureSound, IMethodResult oResult)
            {
                // implement this method in C# here
                //Rho_StringParameters["captureSound"] = captureSound;
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getPreviewLeft(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="previewLeft"></param>
            /// <param name="oResult"></param>
            public override void setPreviewLeft(int previewLeft, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getPreviewTop(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="previewTop"></param>
            /// <param name="oResult"></param>
            public override void setPreviewTop(int previewTop, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getPreviewWidth(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="previewWidth"></param>
            /// <param name="oResult"></param>
            public override void setPreviewWidth(int previewWidth, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getPreviewHeight(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="previewHeight"></param>
            /// <param name="oResult"></param>
            public override void setPreviewHeight(int previewHeight, IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getUseSystemViewfinder(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="useSystemViewfinder"></param>
            /// <param name="oResult"></param>
            public override void setUseSystemViewfinder(bool useSystemViewfinder, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getUseRotationBitmapByEXIF(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void setUseRotationBitmapByEXIF(bool useRotationBitmapByEXIF, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getUseRealBitmapResize(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="useSystemViewfinder"></param>
            /// <param name="oResult"></param>
            public override void setUseRealBitmapResize(bool useSystemViewfinder, IMethodResult oResult)
            {
                // implement this method in C# here
            }


            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void getAimMode(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="aimMode"></param>
            /// <param name="oResult"></param>
            public override void setAimMode(string aimMode, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="propertyMap"></param>
            /// <param name="oResult"></param>
            public override void showPreview(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void hidePreview(IMethodResult oResult)
            {
                // implement this method in C# here
            }
            /// <summary>
            /// Not supported in UWP.
            /// </summary>
            /// <param name="oResult"></param>
            public override void capture(IMethodResult oResult)
            {
                // implement this method in C# here
            }

                       

            private async Task takePicturAsync(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                Dictionary<string, string> takePictureOutput = new Dictionary<string, string>();
                try {
                    CameraCaptureUI captureUI = new CameraCaptureUI();

                    captureUI.PhotoSettings.Format = isFormatJPG ? CameraCaptureUIPhotoFormat.Jpeg : CameraCaptureUIPhotoFormat.Png;
                    captureUI.PhotoSettings.MaxResolution = CameraCaptureUIMaxPhotoResolution.HighestAvailable;
                    captureUI.PhotoSettings.CroppedAspectRatio = resolution;

                    StorageFile file = await captureUI.CaptureFileAsync(CameraCaptureUIMode.Photo);
                  
                    if (file == null) {
                        takePictureOutput["status"] = STATUS_CANCELLED;
                        Task.Run(() => oResult.set(takePictureOutput));
                        return;
                    }

                    StorageFolder dbFolder = await StorageFolder.GetFolderFromPathAsync(MainPage.getDBDir().FullName);
                    string fileName = null;
                    try{
                        var n = DateTime.Now;
                        fileName = "IMG_" + n.Year.ToString() + n.Month.ToString() + n.Day.ToString() + "_" 
                                        + n.Hour.ToString() + n.Minute.ToString() + n.Second.ToString() + ".jpg";
                        deb("FileName is: " + fileName);

                        file = await file.CopyAsync(dbFolder, fileName);
                        deb("Image has been taken to file path: " + file.Path);
                    }catch(Exception e){
                        throw new Exception("Can't create file in db-files");
                    }
  
                    Uri imageUri = new Uri(file.Path);
                    deb("Uri has been extracted: " + imageUri);

                    if (propertyMap.ContainsKey("outputFormat"))
                    {
                        if (propertyMap["outputFormat"] == "image"){
                            deb("outputFormat is image");
                            takePictureOutput["image_uri"] = imageUri.AbsoluteUri;
                            takePictureOutput["imageUri"] = imageUri.AbsoluteUri;
                        }

                        if (propertyMap["outputFormat"] == "imagePath"){
                            deb("outputFormat is imagePath");
                            takePictureOutput["image_uri"] = file.Path;
                            takePictureOutput["imageUri"] = file.Path;
                        }

                        if (propertyMap["outputFormat"] == "dataUri"){
                            deb("outputFormat is dataUri");
                            MemoryStream ms1 = new MemoryStream();
                            (await file.GetThumbnailAsync(Windows.Storage.FileProperties.ThumbnailMode.PicturesView)).AsStreamForRead().CopyTo(ms1);
                            string strbase64 = System.Convert.ToBase64String(ms1.ToArray());
                            takePictureOutput["image_uri"] = "data:image/jpeg;base64," + strbase64;
                            takePictureOutput["imageUri"] = "data:image/jpeg;base64," + strbase64;
                        }
                        deb("propertyMap[\"outputFormat\"] complited");
                    }else
                    {
                        takePictureOutput["image_uri"] = file.Path;
                        takePictureOutput["imageUri"] = file.Path;
                    }


                    deb("Creating image");

                    var imageFile = await dbFolder.GetFileAsync(fileName);
                    using (IRandomAccessStream stream = await imageFile.OpenAsync(FileAccessMode.Read))
                    {
                        BitmapDecoder decoder = await BitmapDecoder.CreateAsync(stream);
                        SoftwareBitmap image = await decoder.GetSoftwareBitmapAsync();
                        deb("Image created");
                        
                        takePictureOutput["status"] = STATUS_OK;
                        takePictureOutput["imageHeight"] = image.PixelHeight.ToString();
                        takePictureOutput["imageWidth"] = image.PixelWidth.ToString();

                        takePictureOutput["image_height"] = image.PixelHeight.ToString();
                        takePictureOutput["image_width"] = image.PixelWidth.ToString();
                        deb("All properties wrote successfully");
                    }

                }
                catch (Exception ex)
                {
                    deb("Exception: " + ex.Message);
                    CRhoRuntime.getInstance().logEvent("Camera class--> takePicture exception" + ex.ToString());
                    
                    takePictureOutput["status"] = STATUS_ERROR;
                    takePictureOutput["message"] = ex.Message;
                    takePictureOutput["image_format"] = string.Empty;
                    takePictureOutput["imageFormat"] = string.Empty;
                }
                finally
                {
                    deb("Setting result");
                    Task.Run(()=>oResult.set(takePictureOutput));
                    
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

                dispatchInvoke(delegate () {
                    Task task = takePicturAsync(propertyMap, oResult);
                });

                CRhoRuntime.getInstance().logEvent("Camera class--> End takePicture");
                        
            }
            
            /// <summary>
            /// Set the default value for the call back parameters
            /// </summary>
            private void initializeTakePictureCallBack()
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->Initialize_TakePictureCallBack");
                /*m_Take_Picture_Output.Clear();
                m_Take_Picture_Output.Add("status", "cancel");
                m_Take_Picture_Output.Add("imageUri", string.Empty);
                m_Take_Picture_Output.Add("imageHeight", string.Empty);
                m_Take_Picture_Output.Add("imageWidth", string.Empty);
                m_Take_Picture_Output.Add("imageFormat", "jpg");
                m_Take_Picture_Output.Add("message", string.Empty);
                m_Take_Picture_Output.Add("image_uri", string.Empty);
                m_Take_Picture_Output.Add("image_height", string.Empty);
                m_Take_Picture_Output.Add("image_width", string.Empty);
                m_Take_Picture_Output.Add("image_format", "jpg");*/
            }

            /// <summary>
            /// Stores file in its own virtual drive rho\apps\app
            /// </summary>
            /// <param name="file">Stream of file that needs to be saved in the Location.</param>
            /// <param name="fileName">Name i n which the file needs to be saved</param>
            /// <param name="StorechoosePictureResult">Callback event</param>
            /// <param name="choosePicture_output">The path of the image needs to be stored</param>
            /// <returns>Successll or error</returns>
            public async Task SaveToLocalFolderAsync(Stream file, string fileName, IMethodResult StoreTakePictureResult, Dictionary<string, string> TakePicture_output)
            {
                string FileNameSuffix = "__DTF__";
                StorageFolder localFolder = ApplicationData.Current.LocalFolder;

                string strLocalFolder = CRhoRuntime.getInstance().getRootPath(ApplicationData.Current.LocalFolder.Path.ToString());

                string[] strFolders = strLocalFolder.Split(new string[] { ApplicationData.Current.LocalFolder.Path.ToString() }, StringSplitOptions.None);
                Dictionary<bool, bool> Rho_SubFolder_Pass = new Dictionary<bool, bool>();
                Rho_SubFolder_Pass.Add(true, true);
                try
                {
                    //bool FIleExists= Rho_SubFolder_Pass[strFolders[1].Contains(localFolder.Path.ToString())];
                    string[] StrSubFolders = strFolders[0].Split('/');

                    foreach (string Path in StrSubFolders)
                    {
                        try
                        {
                            bool BlankFolder = Rho_SubFolder_Pass[!string.IsNullOrEmpty(Path)];
                            var subfolders = await localFolder.GetFoldersAsync();
                            foreach (StorageFolder appFolder in subfolders)
                            {
                                try
                                {
                                    bool status = Rho_SubFolder_Pass[appFolder.Name.Contains(Path)];
                                    localFolder = appFolder;
                                }
                                catch (Exception ex)
                                {

                                }

                            }
                        }
                        catch (Exception ex)
                        {

                        }

                    }
                }
                catch (Exception ex)
                {
                }

                string[] picList = Directory.GetFiles(localFolder.Path);
                try
                {
                    bool image = Rho_SubFolder_Pass[fileName.Contains(".jpg")];
                    fileName = fileName.Replace(".jpg", FileNameSuffix + ".jpg");
                }
                catch (Exception ex)
                {
                    fileName = fileName + FileNameSuffix;
                }

                foreach (string DeleteFile in picList)
                {
                    try
                    {
                        bool fileexist = Rho_SubFolder_Pass[DeleteFile.Contains(FileNameSuffix)];

                        File.Delete(DeleteFile);
                    }
                    catch (Exception ex)
                    {

                    }
                }


                StorageFile storageFile = await localFolder.CreateFileAsync(fileName, CreationCollisionOption.ReplaceExisting);
                Task<Stream> outputStreamTask = storageFile.OpenStreamForWriteAsync();
                Stream outputStream = outputStreamTask.Result;
                var bitmap = new BitmapImage();
                /*bitmap.SetSource(file);
                var wb = new WriteableBitmap(bitmap);
                wb.SaveJpeg(outputStream, wb.PixelWidth, wb.PixelHeight, 0, 100);
                outputStream.Close();*/

                TakePicture_output["imageUri"] = "\\" + storageFile.Name;
                TakePicture_output["image_uri"] = "\\" + storageFile.Name;

                StoreTakePictureResult.set(TakePicture_output);


            }

            
            #endregion
          


        }
        #region Singleton class
        /// <summary>
        /// Singleton class
        /// </summary>
        public class CameraSingleton : CameraSingletonBase
        {
            
            /// <summary>
            /// Initialize DataURI and Image types.
            /// </summary>
            public CameraSingleton() : base()
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->CameraSingleton");
            }
            /// <summary>
            /// Get the list of supported Camera
            /// </summary>
            /// <param name="oResult"></param>
            public override void enumerate(IMethodResult oResult)
            {
                Camera.deb("Singleton enumeration");
                CRhoRuntime.getInstance().logEvent("Camera class-->enumerate");
                List<string> availabeCameras = null;
                try
                {
                    availabeCameras = Camera.initCameraIDs().Keys.ToList();
                }catch(Exception e)
                {
                    availabeCameras = new List<string>();
                }
                oResult.set(availabeCameras);
                Camera.deb("Singleton enumeration end");
            }

            /// <summary>
            /// Get whether the supplied camera type is supported or not.
            /// </summary>
            /// <param name="cameraType">type of camera supported (back or front)</param>
            /// <param name="oResult"></param>
            public override void getCameraByType(string cameraType, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->getCameraByType");
                oResult.set(Camera.initCameraIDs().ContainsKey(cameraType));
            }





            private static async Task choosePictureAsync(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class-->choosePicture");
                Dictionary<string, string> m_choosePicture_output = new Dictionary<string, string>();
                try
                {
                    var picker = new FileOpenPicker();
                    picker.ViewMode = PickerViewMode.Thumbnail;
                    picker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
                    
                    picker.FileTypeFilter.Add(".jpg");
                    picker.FileTypeFilter.Add(".jpeg");
                    picker.FileTypeFilter.Add(".png");

                    StorageFile file = await picker.PickSingleFileAsync();
                    if (file != null)
                    {
                        using (IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.Read))
                        {
                            BitmapDecoder decoder = await BitmapDecoder.CreateAsync(stream);
                            SoftwareBitmap image = await decoder.GetSoftwareBitmapAsync();
                            string format = file.FileType;
                            m_choosePicture_output["status"] = CameraBase.STATUS_OK;

                            m_choosePicture_output["imageHeight"] = image.PixelHeight.ToString();
                            m_choosePicture_output["imageWidth"] = image.PixelWidth.ToString();
                            m_choosePicture_output["imageFormat"] = format;
                            m_choosePicture_output["imagePath"] = file.Path;

                            m_choosePicture_output["image_height"] = image.PixelHeight.ToString();
                            m_choosePicture_output["image_width"] = image.PixelWidth.ToString();
                            m_choosePicture_output["image_format"] = format;
                            m_choosePicture_output["image_path"] = file.Path;

                            Uri imageUri = new Uri(file.Path);
                            m_choosePicture_output["image_uri"] = imageUri.AbsoluteUri;
                            m_choosePicture_output["imageUri"] = imageUri.AbsoluteUri;
                        }

                    }
                    else
                    {
                        m_choosePicture_output.Add("status", CameraBase.STATUS_CANCELLED);
                    }  

                }
                catch (Exception ex)
                {
                    m_choosePicture_output = new Dictionary<string, string>();
                    CRhoRuntime.getInstance().logEvent("Camera class-->choosePicture-->Exception" + ex.ToString());
                    m_choosePicture_output["status"] = "error";
                    m_choosePicture_output["message"] = ex.Message;
                    oResult.set(m_choosePicture_output);
                }
                finally
                {
                    Task.Run(() => oResult.set(m_choosePicture_output));
                }
            }



            /// <summary>
            /// Choose the Pictures from the shown list.
            /// </summary>
            /// <param name="propertyMap">supports only outputformat.</param>
            /// <param name="oResult"></param>
            public override void choosePicture(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                CRhoRuntime.getInstance().logEvent("Camera class--> choosePicture");

                dispatchInvoke(delegate () {
                    Task task = choosePictureAsync(propertyMap, oResult);
                });

                CRhoRuntime.getInstance().logEvent("Camera class--> End choosePicture");
            }

            public async Task<string> copyImageToDeviceGalleryAsync(string pathToImage, IMethodResult oResult)
            {
                StorageFile file = await StorageFile.GetFileFromPathAsync(pathToImage);
                var copied = await file.CopyAsync(ApplicationData.Current.LocalFolder);
                deb("Image has been taken to file path: " + copied.Path);
                return copied.Path;
            }

            public override void copyImageToDeviceGallery(string pathToImage, IMethodResult oResult)
            {
                Dictionary<string, string> copyPictureOutput = new Dictionary<string, string>();
                try {
                    Task<string> task = copyImageToDeviceGalleryAsync(pathToImage, oResult);
                    try { task.Start(); } catch (Exception ex) { }
                    try { if (task.Status == TaskStatus.Running) {task.Wait(); }} catch (Exception ex) {}
                    string result = task.Result;

                    if (result != null)
                    {
                        copyPictureOutput["status"] = CameraBase.STATUS_OK;
                        copyPictureOutput["pathToImage"] = result;
                    }else{
                        throw new Exception("Can't copy picture for some reasons");
                    }
                }
                catch (Exception ex)
                {
                    CRhoRuntime.getInstance().logEvent("Camera class-->copyImageToDeviceGallery-->Exception" + ex.ToString());
                    copyPictureOutput = new Dictionary<string, string>();
                    copyPictureOutput["status"] = "error";
                    copyPictureOutput["message"] = ex.Message;
                    oResult.set(copyPictureOutput);
                }
                finally
                {
                    Task.Run(() => oResult.set(copyPictureOutput));
                }


            }
            
        }

        public class CameraFactory : CameraFactoryBase
        {
            
        }
        #endregion
    }

}
