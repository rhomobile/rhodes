#include "../generated/lib/Camera_impl.h"
#include "../generated/lib/CameraFactory.h"
#include "api_generator/uwp/MethodResultImpl.h"

using namespace rho::apiGenerator;
using namespace rhoruntime;

namespace rho {


void CCameraImpl::getCameraType(CMethodResult& oResult)
{
    try {
        _runtime->getCameraType(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getMaxWidth(CMethodResult& oResult)
{
    try {
        _runtime->getMaxWidth(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getMaxHeight(CMethodResult& oResult)
{
    try {
        _runtime->getMaxHeight(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getSupportedSizeList(CMethodResult& oResult)
{
    try {
        _runtime->getSupportedSizeList(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getDesiredWidth(CMethodResult& oResult)
{
    try {
        _runtime->getDesiredWidth(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setDesiredWidth(int desiredWidth, CMethodResult& oResult)
{
    try {
        _runtime->setDesiredWidth(desiredWidth, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getDesiredHeight(CMethodResult& oResult)
{
    try {
        _runtime->getDesiredHeight(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setDesiredHeight(int desiredHeight, CMethodResult& oResult)
{
    try {
        _runtime->setDesiredHeight(desiredHeight, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getFileName(CMethodResult& oResult)
{
    try {
        _runtime->getFileName(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setFileName(const rho::String& fileName, CMethodResult& oResult)
{
        Platform::String^ _fileName = rho::common::convertStringToWP8(fileName);
    try {
        _runtime->setFileName(_fileName, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getCompressionFormat(CMethodResult& oResult)
{
    try {
        _runtime->getCompressionFormat(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setCompressionFormat(const rho::String& compressionFormat, CMethodResult& oResult)
{
        Platform::String^ _compressionFormat = rho::common::convertStringToWP8(compressionFormat);
    try {
        _runtime->setCompressionFormat(_compressionFormat, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getOutputFormat(CMethodResult& oResult)
{
    try {
        _runtime->getOutputFormat(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setOutputFormat(const rho::String& outputFormat, CMethodResult& oResult)
{
        Platform::String^ _outputFormat = rho::common::convertStringToWP8(outputFormat);
    try {
        _runtime->setOutputFormat(_outputFormat, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getColorModel(CMethodResult& oResult)
{
    try {
        _runtime->getColorModel(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setColorModel(const rho::String& colorModel, CMethodResult& oResult)
{
        Platform::String^ _colorModel = rho::common::convertStringToWP8(colorModel);
    try {
        _runtime->setColorModel(_colorModel, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getEnableEditing(CMethodResult& oResult)
{
    try {
        _runtime->getEnableEditing(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setEnableEditing(bool enableEditing, CMethodResult& oResult)
{
    try {
        _runtime->setEnableEditing(enableEditing, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getFlashMode(CMethodResult& oResult)
{
    try {
        _runtime->getFlashMode(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setFlashMode(const rho::String& flashMode, CMethodResult& oResult)
{
        Platform::String^ _flashMode = rho::common::convertStringToWP8(flashMode);
    try {
        _runtime->setFlashMode(_flashMode, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getSaveToDeviceGallery(CMethodResult& oResult)
{
    try {
        _runtime->getSaveToDeviceGallery(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setSaveToDeviceGallery(bool saveToDeviceGallery, CMethodResult& oResult)
{
    try {
        _runtime->setSaveToDeviceGallery(saveToDeviceGallery, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getCaptureSound(CMethodResult& oResult)
{
    try {
        _runtime->getCaptureSound(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setCaptureSound(const rho::String& captureSound, CMethodResult& oResult)
{
        Platform::String^ _captureSound = rho::common::convertStringToWP8(captureSound);
    try {
        _runtime->setCaptureSound(_captureSound, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getPreviewLeft(CMethodResult& oResult)
{
    try {
        _runtime->getPreviewLeft(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setPreviewLeft(int previewLeft, CMethodResult& oResult)
{
    try {
        _runtime->setPreviewLeft(previewLeft, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getPreviewTop(CMethodResult& oResult)
{
    try {
        _runtime->getPreviewTop(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setPreviewTop(int previewTop, CMethodResult& oResult)
{
    try {
        _runtime->setPreviewTop(previewTop, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getPreviewWidth(CMethodResult& oResult)
{
    try {
        _runtime->getPreviewWidth(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setPreviewWidth(int previewWidth, CMethodResult& oResult)
{
    try {
        _runtime->setPreviewWidth(previewWidth, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getPreviewHeight(CMethodResult& oResult)
{
    try {
        _runtime->getPreviewHeight(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setPreviewHeight(int previewHeight, CMethodResult& oResult)
{
    try {
        _runtime->setPreviewHeight(previewHeight, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getUseSystemViewfinder(CMethodResult& oResult)
{
    try {
        _runtime->getUseSystemViewfinder(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setUseSystemViewfinder(bool useSystemViewfinder, CMethodResult& oResult)
{
    try {
        _runtime->setUseSystemViewfinder(useSystemViewfinder, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getUseRealBitmapResize(CMethodResult& oResult)
{
    try {
        _runtime->getUseRealBitmapResize(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setUseRealBitmapResize(bool useRealBitmapResize, CMethodResult& oResult)
{
    try {
        _runtime->setUseRealBitmapResize(useRealBitmapResize, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getUseRotationBitmapByEXIF(CMethodResult& oResult)
{
    try {
        _runtime->getUseRotationBitmapByEXIF(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setUseRotationBitmapByEXIF(bool useRotationBitmapByEXIF, CMethodResult& oResult)
{
    try {
        _runtime->setUseRotationBitmapByEXIF(useRotationBitmapByEXIF, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::getAimMode(CMethodResult& oResult)
{
    try {
        _runtime->getAimMode(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::setAimMode(const rho::String& aimMode, CMethodResult& oResult)
{
        Platform::String^ _aimMode = rho::common::convertStringToWP8(aimMode);
    try {
        _runtime->setAimMode(_aimMode, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::takePicture(const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
{
        Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ _propertyMap = rho::common::convertHashToWP8(propertyMap);
    try {
        _runtime->takePicture(_propertyMap, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::showPreview(const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
{
        Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ _propertyMap = rho::common::convertHashToWP8(propertyMap);
    try {
        _runtime->showPreview(_propertyMap, ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::hidePreview(CMethodResult& oResult)
{
    try {
        _runtime->hidePreview(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}

void CCameraImpl::capture(CMethodResult& oResult)
{
    try {
        _runtime->capture(ref new CMethodResultImpl((int64)&oResult));
    } catch (Platform::Exception^ e) {
        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
    }
}


class CCameraSingleton: public CCameraSingletonBase
{
private:
    ICameraSingletonImpl^ _runtime;
public:
    CCameraSingleton(ICameraSingletonImpl^ runtime): CCameraSingletonBase(), _runtime(runtime) { defaultID = "back"; }
    ~CCameraSingleton(){}

    virtual void enumerate(CMethodResult& oResult)
    {
        try {
            _runtime->enumerate(ref new CMethodResultImpl((int64)&oResult));
        } catch (Platform::Exception^ e) {
            LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
        }
    }

    virtual void getCameraByType(const rho::String& cameraType, CMethodResult& oResult)
    {
        Platform::String^ _cameraType = rho::common::convertStringToWP8(cameraType);
        try {
            _runtime->getCameraByType(_cameraType, ref new CMethodResultImpl((int64)&oResult));
        } catch (Platform::Exception^ e) {
            LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
        }
    }

    virtual void choosePicture(const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
    {
        Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ _propertyMap = rho::common::convertHashToWP8(propertyMap);
        try {
            _runtime->choosePicture(_propertyMap, ref new CMethodResultImpl((int64)&oResult));
        } catch (Platform::Exception^ e) {
            LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
        }
    }

    virtual void copyImageToDeviceGallery(const rho::String& pathToImage, CMethodResult& oResult)
    {
        Platform::String^ _pathToImage = rho::common::convertStringToWP8(pathToImage);
        try {
            _runtime->copyImageToDeviceGallery(_pathToImage, ref new CMethodResultImpl((int64)&oResult));
        } catch (Platform::Exception^ e) {
            LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());
        }
    }

	rho::String defaultID;

    virtual rho::String getDefaultID(){return defaultID;}
    virtual rho::String getInitialDefaultID(){return defaultID;}
	virtual void setDefaultID(const rho::String& strID) { defaultID = strID; }
    
    //virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor){} // TODO: implement addCommandToQueue
    //virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor){} // TODO: implement callCommandInThread
};

ICameraFactoryImpl^ CCameraFactory::_impl;

ICamera* CCameraFactory::createModuleByID(const rho::String& strID)
{
    return new CCameraImpl(strID, _impl->getImpl(rho::common::convertStringToWP8(strID)));
}

ICameraSingleton* CCameraFactory::createModuleSingleton()
{
    return new CCameraSingleton(_impl->getSingletonImpl());
}

}

extern "C" void Init_Camera_extension()
{
    rho::CCameraFactory::setInstance( new rho::CCameraFactory() );
    rho::Init_Camera_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoCameraApi");
}
