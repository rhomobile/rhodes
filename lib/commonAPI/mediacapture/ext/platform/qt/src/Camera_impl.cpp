//
//  CameraImpl.cpp
#include <Qt>
#include <QString>
#include <QList>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImage>
#include <QHash>
#include "CCameraData.h"

#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "generated/cpp/CameraBase.h"
#include "logging/RhoLog.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;

    class CCameraSingletonImpl: public CCameraSingletonBase
    {
        QHash<QString, QString> defaultCameras;

    public:

        CCameraSingletonImpl(): CCameraSingletonBase()
        {
            foreach (QCameraInfo cameraInfo, QCameraInfo::availableCameras()) {
                const CCameraData * data = CCameraData::addNewCamera(cameraInfo);
                defaultCameras.insert(data->getCameraType(), data->getCameraID());
            }
        }

        virtual ~CCameraSingletonImpl(){
            CCameraData::cleanAll();
        }
        
        //methods
        // enumerate Returns the cameras present on your device, allowing you to access your device's front or back camera. 
        virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("enumerate","Camera");
            rho::Vector<rho::String> arIDs;
            if(!CCameraData::isEmpty()){
                foreach (QString value, CCameraData::getKeys()) {arIDs.addElement(value.toStdString());}
                oResult.set(arIDs);
            }else{
                oResult.set("");
            }
            
        } 
        // getCameraByType Returns the camera of requested type if that camera exist - else return nil. 
        virtual void getCameraByType( const rho::String& cameraType, rho::apiGenerator::CMethodResult& oResult) {
            oResult.set(defaultCameras.value(QString::fromStdString(cameraType)).toStdString());
        } 
        // choosePicture Choose a picture from the album. 

        static void getImageData(rho::Hashtable<String,String> & mapRes, QString fileNameToOpen){
            QImage image(fileNameToOpen);
            if (image.isNull()){
                mapRes["status"] = "error";
                mapRes["message"] = "Image loading error";
                return;
            }
            mapRes["status"] = "ok";

            rho::String uri = fileNameToOpen.toStdString();
            rho::String height = QString::number(image.height()).toStdString();
            rho::String width = QString::number(image.width()).toStdString();
            rho::String format = (QFileInfo(fileNameToOpen)).suffix().toStdString();

            mapRes["imageUri"] = uri;
            mapRes["imageHeight"] = height;
            mapRes["imageWidth"] = width;
            mapRes["imageFormat"] = format;

            mapRes["image_uri"] = uri;
            mapRes["image_height"] = height;
            mapRes["image_width"] = width;
            mapRes["image_format"] = format;
        }

        virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("choosePicture","Camera");
            QString fileNameToOpen = QFileDialog::getOpenFileName(0, "Open Image",
                                                                  QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last()
                                                                  , "Image Files (*.JPG *.JPE *.PNG *.BMP *.GIF)");

            rho::Hashtable<String,String>& mapRes = oResult.getStringHash();
            if (fileNameToOpen.isEmpty()){
                mapRes["status"] = "cancel";
                mapRes["message"] = "Open file dialog has been canceled";
                return;
            }

            getImageData(mapRes, fileNameToOpen);


        } 
        // copyImageToDeviceGallery Save an image to the device gallery. 
        virtual void copyImageToDeviceGallery( const rho::String& pathToImage, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("copyImageToDeviceGallery","Camera");
            QString originalFileName(QString::fromStdString(pathToImage));
            QFileInfo info(originalFileName);
            QString newFileName(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last() + "\\" + info.fileName());
            QFile::copy(originalFileName,newFileName);
            oResult.set(newFileName.toStdString());
        } 

        virtual rho::String getInitialDefaultID(){
            QString defaultId = defaultCameras.value("front");
            if (!defaultId.isEmpty()) return defaultId.toStdString();
            else return defaultCameras.value("back").toStdString();
        }

    };
    
    class CCameraImpl : public CCameraBase
    {
        CCameraImpl(){}
        CCameraData * camera;
    public:
        virtual ~CCameraImpl() {}

        //methods

        CCameraImpl(const rho::String& strID){
            camera = CCameraData::getCameraData(QString::fromStdString(strID));
        }

        virtual void getCameraType(rho::apiGenerator::CMethodResult& oResult) {
            if (camera != nullptr){oResult.set(camera->getCameraType().toStdString());}
        } 

        virtual void getMaxWidth(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getMaxHeight(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getSupportedSizeList(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getDesiredWidth(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setDesiredWidth( int desiredWidth, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getDesiredHeight(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setDesiredHeight( int desiredHeight, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getFileName(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setFileName( const rho::String& fileName, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getCompressionFormat(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setCompressionFormat( const rho::String& compressionFormat, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getOutputFormat(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setOutputFormat( const rho::String& outputFormat, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getColorModel(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setColorModel( const rho::String& colorModel, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getEnableEditing(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setEnableEditing( bool enableEditing, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getFlashMode(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setFlashMode( const rho::String& flashMode, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getSaveToDeviceGallery(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setSaveToDeviceGallery( bool saveToDeviceGallery, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getCaptureSound(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setCaptureSound( const rho::String& captureSound, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getPreviewLeft(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setPreviewLeft( int previewLeft, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getPreviewTop(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setPreviewTop( int previewTop, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getPreviewWidth(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setPreviewWidth( int previewWidth, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getPreviewHeight(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setPreviewHeight( int previewHeight, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getUseSystemViewfinder(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setUseSystemViewfinder( bool useSystemViewfinder, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getUseRealBitmapResize(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setUseRealBitmapResize( bool useRealBitmapResize, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getUseRotationBitmapByEXIF(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setUseRotationBitmapByEXIF( bool useRotationBitmapByEXIF, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getAimMode(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setAimMode( const rho::String& aimMode, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void takePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void showPreview( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {
            CCameraDialogWindow::showInstace(camera->getCameraObject());
        } 

        virtual void hidePreview(rho::apiGenerator::CMethodResult& oResult) {
            CCameraDialogWindow::hideInstace(camera->getCameraObject());
        } 

        virtual void capture(rho::apiGenerator::CMethodResult& oResult) {
            rho::Hashtable<String,String>& mapRes = oResult.getStringHash();

            const CCapturer::Result * result = camera->takeAPicture();
            if (result == nullptr){
                mapRes["status"] = "error";
                mapRes["message"] = "Camera ID fail";;
                return;
            }
            if (!result->errorFlag){

                CCameraSingletonImpl::getImageData( mapRes, result->savedFileName);
            }else{
                mapRes["status"] = "error";
                mapRes["message"] = result->errorMessage.toStdString();
                return;
            }



        } 

        virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {

        } 

    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class CCameraFactory: public CCameraFactoryBase    {
    public:
        CCameraFactory(){}
        
        ICameraSingleton* createModuleSingleton()
        { 
            return new CCameraSingletonImpl();
        }
        
        virtual ICamera* createModuleByID(const rho::String& strID){ return new CCameraImpl(strID); }
        
    };
    
}

extern "C" void Init_Camera_extension()
{
    rho::CCameraFactory::setInstance( new rho::CCameraFactory() );
    rho::Init_Camera_API();
    
}
