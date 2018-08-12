//
//  CameraImpl.cpp
#include <Qt>
#include <QHash>
#ifndef OS_SAILFISH
#include <QtGui>
#include <QtWidgets>
#include <QApplication>
#include <QWindow>
#endif
#include "CCameraData.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "generated/cpp/CameraBase.h"
#include "logging/RhoLog.h"
#include "camerarefresher.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;

    class CCameraSingletonImpl: public CCameraSingletonBase
    {
        QHash<QString, QString> defaultCameras;
    public:

        CCameraSingletonImpl(): CCameraSingletonBase() {
#ifndef OS_SAILFISH
            if (CCameraData::getValues().isEmpty()){
                CameraRefresher::refresh();
            }
            foreach (CCameraData * data, CCameraData::getValues()) {
                defaultCameras.insert(data->getCameraType(), data->getCameraID());
            }
#else
            QCameraInfo info;
            CCameraData::addNewCamera(info);
            defaultCameras.insert(ICamera::CAMERA_TYPE_BACK, ICamera::CAMERA_TYPE_BACK);

            QList<QObject *> objectList;
            foreach (CCameraData * obj, CCameraData::getValues()) {objectList.append(obj);}
            QtMainWindow::setContextProperty("camerasModel", objectList);
#endif
        }

        virtual ~CCameraSingletonImpl(){

        }
        
        //methods
        // enumerate Returns the cameras present on your device, allowing you to access your device's front or back camera. 
        virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) {
            rho::Vector<rho::String> arIDs = oResult.getStringArray();
            if(!CCameraData::isEmpty()){
                foreach (QString value, CCameraData::getKeys()) {
                    arIDs.addElement(value.toStdString());
                }
            }

            oResult.set(arIDs);
        } 

        // getCameraByType Returns the camera of requested type if that camera exist - else return nil. 
        virtual void getCameraByType( const rho::String& cameraType, rho::apiGenerator::CMethodResult& oResult) {
            oResult.set(defaultCameras.value(QString::fromStdString(cameraType)).toStdString());
        } 
        // choosePicture Choose a picture from the album. 

        static void getImageData(rho::Hashtable<String,String> & mapRes, QString fileNameToOpen){
#ifndef OS_SAILFISH
            CameraDialogView::getImageData(mapRes, fileNameToOpen);
#else
            QImage image(fileNameToOpen);
            if (image.isNull()){
                mapRes["status"] = "error";
                mapRes["message"] = "Image loading error";
                return;
            }
            mapRes["status"] = "ok";

            rho::String uri = QDir::current().relativeFilePath(fileNameToOpen).toStdString();
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

#endif
        }

        virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("choosePicture","Camera");

            CCameraData::choosePicture(oResult);
        } 
        // copyImageToDeviceGallery Save an image to the device gallery. 
        virtual void copyImageToDeviceGallery( const rho::String& pathToImage, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("copyImageToDeviceGallery","Camera");
            QString originalFileName(QString::fromStdString(pathToImage));
            QFileInfo info(originalFileName);
#ifndef OS_SAILFISH
            QString newFileName(CameraDialogView::getImageDir().absolutePath() + "/" + info.fileName());
#else
            QString newFileName;
#endif
            QFile::copy(originalFileName, newFileName);
            oResult.set(newFileName.toStdString());
        } 

        virtual rho::String getInitialDefaultID(){
#ifndef OS_SAILFISH
            QString defaultId = defaultCameras.value(ICamera::CAMERA_TYPE_FRONT);
            if (!defaultId.isEmpty()) return defaultId.toStdString();
            else return defaultCameras.value(ICamera::CAMERA_TYPE_BACK).toStdString();
#else
            return ICamera::CAMERA_TYPE_BACK;
#endif
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
            if (camera != nullptr){
                oResult.set(camera->getCameraType().toStdString());
            }
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
            camera->setTargetFileName(QString::fromStdString(fileName));
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
            capture(oResult);
        } 

        virtual void showPreview( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void hidePreview(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void capture(rho::apiGenerator::CMethodResult& oResult) {

            if (camera == nullptr) {
                rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
                mapRes["status"] = "error";
                mapRes["message"] = "Camera is not available";
                oResult.set(mapRes);
                return;
            }
            camera->showView(oResult);
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
            static CCameraSingletonImpl * impl = new CCameraSingletonImpl();
            return impl;
        }
        
        virtual ICamera* createModuleByID(const rho::String& strID){
            createModuleSingleton();
            return new CCameraImpl(strID);
        }
        
    };
    
}

extern "C" void Init_Camera_extension()
{
    rho::CCameraFactory::setInstance( new rho::CCameraFactory() );
    rho::Init_Camera_API();
    
}
