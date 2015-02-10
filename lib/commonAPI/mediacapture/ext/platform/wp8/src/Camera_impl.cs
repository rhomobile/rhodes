using System;
using System.Collections.Generic;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Tasks;
using System.Windows.Media.Imaging;
using System.IO.IsolatedStorage;
using rhoruntime;
using rho.common;
namespace rho
{

    namespace CameraImpl
    {
        public class CameraResult
        {
            public IMethodResult _oResult = null;

            public void cameraTask_Completed(object sender, PhotoResult e)
            {
                rho.common.Hashtable<String, String> mapRes = new rho.common.Hashtable<String, String>();

                switch (e.TaskResult)
                {
                    case TaskResult.OK:
                        WriteableBitmap writeableBitmap = new WriteableBitmap(1600, 1200);
                        writeableBitmap.LoadJpeg(e.ChosenPhoto);

                        string imageFolder = "rho/apps";//CFilePath.join( rho_native_rhopath(), RHO_APPS_DIR);//"rho";
                        string datetime = DateTime.Now.ToString().Replace("/", "");
                        datetime = datetime.Replace(":", "");
                        string imageFileName = "Foto_" + datetime.Replace(" ", String.Empty) + ".jpg";
                        string filePath = "";
                        using (var isoFile = IsolatedStorageFile.GetUserStoreForApplication())
                        {

                            if (!isoFile.DirectoryExists(imageFolder))
                            {
                                isoFile.CreateDirectory(imageFolder);
                            }

                            filePath = System.IO.Path.Combine(imageFolder, imageFileName);
                            using (var stream = isoFile.CreateFile(filePath))
                            {
                                writeableBitmap.SaveJpeg(stream, writeableBitmap.PixelWidth, writeableBitmap.PixelHeight, 0, 100);
                            }
                        }

                        mapRes["imageUri"] = "/"+imageFileName;//e.OriginalFileName;
                        mapRes["image_uri"] = "/"+imageFileName;//e.OriginalFileName;
                        mapRes["status"] = "ok";

                        break;
                    case TaskResult.None:
                        mapRes["message"] = "Error";
                        mapRes["status"] = "error";
                        break;
                    case TaskResult.Cancel:
                        mapRes["message"] = "User cancelled operation";
                        mapRes["status"] = "cancel";
                        break;
                    default:
                        break;
                }

                _oResult.set(mapRes);

                //Code to display the photo on the page in an image control named myImage.
                //System.Windows.Media.Imaging.BitmapImage bmp = new System.Windows.Media.Imaging.BitmapImage();
                //bmp.SetSource(e.ChosenPhoto);
                //myImage.Source = bmp;
            }
        };

        public class Camera : CameraBase
        {
            public override void getCameraType(IMethodResult oResult)
            {
                oResult.set("back");
                // implement this method in C# here
            }

            public override void getMaxWidth(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getMaxHeight(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getSupportedSizeList(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getDesiredWidth(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void setDesiredWidth(int desiredWidth, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getDesiredHeight(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void setDesiredHeight(int desiredHeight, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getFileName(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void setFileName(string fileName, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getCompressionFormat(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void setCompressionFormat(string compressionFormat, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getOutputFormat(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void setOutputFormat(string outputFormat, IMethodResult oResult)
            {
                // implement this method in C# here
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
                // implement this method in C# here
            }

            public override void setEnableEditing(bool enableEditing, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void getFlashMode(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void setFlashMode(string flashMode, IMethodResult oResult)
            {
                // implement this method in C# here
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
                // implement this method in C# here
            }

            public override void setCaptureSound(string captureSound, IMethodResult oResult)
            {
                // implement this method in C# here
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

            public override void takePicture(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                CameraResult result = new CameraResult();
                result._oResult = oResult;
                CameraCaptureTask cameraCaptureTask;
                cameraCaptureTask = new CameraCaptureTask();
                cameraCaptureTask.Completed += new EventHandler<PhotoResult>(result.cameraTask_Completed);
                cameraCaptureTask.Show();
                //CameraPage page = new CameraPage();
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
        }

        public class CameraSingleton : CameraSingletonBase
        {
            public override void enumerate(IMethodResult oResult)
            {
                rho.common.Vector<String> arIDs = new rho.common.Vector<String>();
                arIDs.addElement("back");

                oResult.set(arIDs);
            }

            public override void getCameraByType(string cameraType, IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void choosePicture(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
            {
                CameraResult result = new CameraResult();
                result._oResult = oResult;
                PhotoChooserTask photoChooserTask;
                photoChooserTask = new PhotoChooserTask();
                photoChooserTask.Completed += new EventHandler<PhotoResult>(result.cameraTask_Completed);
                photoChooserTask.Show();
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
