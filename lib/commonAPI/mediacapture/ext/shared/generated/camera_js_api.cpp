#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Camera"


rho::String js_Camera_getDesiredWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getDesiredWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setDesiredWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setDesiredWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getDesiredHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getDesiredHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setDesiredHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setDesiredHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getCompressionFormat(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getCompressionFormat(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setCompressionFormat(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setCompressionFormat(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getOutputFormat(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getOutputFormat(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setOutputFormat(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setOutputFormat(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getColorModel(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getColorModel(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setColorModel(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setColorModel(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getEnableEditing(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getEnableEditing(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setEnableEditing(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setEnableEditing(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getFlashMode(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getFlashMode(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setFlashMode(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setFlashMode(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getSaveToSharedGallery(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getSaveToSharedGallery(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setSaveToSharedGallery(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setSaveToSharedGallery(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getCaptureSound(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getCaptureSound(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setCaptureSound(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setCaptureSound(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getPreviewLeft(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getPreviewLeft(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setPreviewLeft(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setPreviewLeft(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getPreviewTop(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getPreviewTop(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setPreviewTop(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setPreviewTop(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getPreviewWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getPreviewWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setPreviewWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setPreviewWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getPreviewHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getPreviewHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setPreviewHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setPreviewHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_takePicture(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_takePicture(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getCameraInfo(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getCameraInfo(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_choosePicture(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_choosePicture(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_saveImageToDeviceGallery(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_saveImageToDeviceGallery(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Camera_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_def_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_Camera_enumerate(rho::json::CJSONArray& argv, const rho::String& strObjID);



rho::String js_s_Camera_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Camera_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);



extern "C" void Init_JSAPI_Camera(void)
{

    rho::apiGenerator::js_define_method("Rho:Camera:desiredWidth", js_Camera_getDesiredWidth);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_desiredWidth", js_s_Camera_def_getDesiredWidth);;

    rho::apiGenerator::js_define_method("Rho:Camera:desiredWidth=", js_Camera_setDesiredWidth);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_desiredWidth=", js_s_Camera_def_setDesiredWidth);;

    rho::apiGenerator::js_define_method("Rho:Camera:desiredHeight", js_Camera_getDesiredHeight);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_desiredHeight", js_s_Camera_def_getDesiredHeight);;

    rho::apiGenerator::js_define_method("Rho:Camera:desiredHeight=", js_Camera_setDesiredHeight);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_desiredHeight=", js_s_Camera_def_setDesiredHeight);;

    rho::apiGenerator::js_define_method("Rho:Camera:compressionFormat", js_Camera_getCompressionFormat);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_compressionFormat", js_s_Camera_def_getCompressionFormat);;

    rho::apiGenerator::js_define_method("Rho:Camera:compressionFormat=", js_Camera_setCompressionFormat);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_compressionFormat=", js_s_Camera_def_setCompressionFormat);;

    rho::apiGenerator::js_define_method("Rho:Camera:outputFormat", js_Camera_getOutputFormat);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_outputFormat", js_s_Camera_def_getOutputFormat);;

    rho::apiGenerator::js_define_method("Rho:Camera:outputFormat=", js_Camera_setOutputFormat);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_outputFormat=", js_s_Camera_def_setOutputFormat);;

    rho::apiGenerator::js_define_method("Rho:Camera:colorModel", js_Camera_getColorModel);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_colorModel", js_s_Camera_def_getColorModel);;

    rho::apiGenerator::js_define_method("Rho:Camera:colorModel=", js_Camera_setColorModel);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_colorModel=", js_s_Camera_def_setColorModel);;

    rho::apiGenerator::js_define_method("Rho:Camera:enableEditing", js_Camera_getEnableEditing);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_enableEditing", js_s_Camera_def_getEnableEditing);;

    rho::apiGenerator::js_define_method("Rho:Camera:enableEditing=", js_Camera_setEnableEditing);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_enableEditing=", js_s_Camera_def_setEnableEditing);;

    rho::apiGenerator::js_define_method("Rho:Camera:flashMode", js_Camera_getFlashMode);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_flashMode", js_s_Camera_def_getFlashMode);;

    rho::apiGenerator::js_define_method("Rho:Camera:flashMode=", js_Camera_setFlashMode);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_flashMode=", js_s_Camera_def_setFlashMode);;

    rho::apiGenerator::js_define_method("Rho:Camera:saveToSharedGallery", js_Camera_getSaveToSharedGallery);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_saveToSharedGallery", js_s_Camera_def_getSaveToSharedGallery);;

    rho::apiGenerator::js_define_method("Rho:Camera:saveToSharedGallery=", js_Camera_setSaveToSharedGallery);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_saveToSharedGallery=", js_s_Camera_def_setSaveToSharedGallery);;

    rho::apiGenerator::js_define_method("Rho:Camera:captureSound", js_Camera_getCaptureSound);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_captureSound", js_s_Camera_def_getCaptureSound);;

    rho::apiGenerator::js_define_method("Rho:Camera:captureSound=", js_Camera_setCaptureSound);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_captureSound=", js_s_Camera_def_setCaptureSound);;

    rho::apiGenerator::js_define_method("Rho:Camera:previewLeft", js_Camera_getPreviewLeft);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_previewLeft", js_s_Camera_def_getPreviewLeft);;

    rho::apiGenerator::js_define_method("Rho:Camera:previewLeft=", js_Camera_setPreviewLeft);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_previewLeft=", js_s_Camera_def_setPreviewLeft);;

    rho::apiGenerator::js_define_method("Rho:Camera:previewTop", js_Camera_getPreviewTop);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_previewTop", js_s_Camera_def_getPreviewTop);;

    rho::apiGenerator::js_define_method("Rho:Camera:previewTop=", js_Camera_setPreviewTop);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_previewTop=", js_s_Camera_def_setPreviewTop);;

    rho::apiGenerator::js_define_method("Rho:Camera:previewWidth", js_Camera_getPreviewWidth);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_previewWidth", js_s_Camera_def_getPreviewWidth);;

    rho::apiGenerator::js_define_method("Rho:Camera:previewWidth=", js_Camera_setPreviewWidth);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_previewWidth=", js_s_Camera_def_setPreviewWidth);;

    rho::apiGenerator::js_define_method("Rho:Camera:previewHeight", js_Camera_getPreviewHeight);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_previewHeight", js_s_Camera_def_getPreviewHeight);;

    rho::apiGenerator::js_define_method("Rho:Camera:previewHeight=", js_Camera_setPreviewHeight);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_previewHeight=", js_s_Camera_def_setPreviewHeight);;

    rho::apiGenerator::js_define_method("Rho:Camera:takePicture", js_Camera_takePicture);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_takePicture", js_s_Camera_def_takePicture);;

    rho::apiGenerator::js_define_method("Rho:Camera:getCameraInfo", js_Camera_getCameraInfo);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_getCameraInfo", js_s_Camera_def_getCameraInfo);;

    rho::apiGenerator::js_define_method("Rho:Camera:choosePicture", js_Camera_choosePicture);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_choosePicture", js_s_Camera_def_choosePicture);;

    rho::apiGenerator::js_define_method("Rho:Camera:saveImageToDeviceGallery", js_Camera_saveImageToDeviceGallery);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_saveImageToDeviceGallery", js_s_Camera_def_saveImageToDeviceGallery);;

    rho::apiGenerator::js_define_method("Rho:Camera:getProperty", js_Camera_getProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_getProperty", js_s_Camera_def_getProperty);;

    rho::apiGenerator::js_define_method("Rho:Camera:getProperties", js_Camera_getProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_getProperties", js_s_Camera_def_getProperties);;

    rho::apiGenerator::js_define_method("Rho:Camera:getAllProperties", js_Camera_getAllProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_getAllProperties", js_s_Camera_def_getAllProperties);;

    rho::apiGenerator::js_define_method("Rho:Camera:setProperty", js_Camera_setProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_setProperty", js_s_Camera_def_setProperty);;

    rho::apiGenerator::js_define_method("Rho:Camera:setProperties", js_Camera_setProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_setProperties", js_s_Camera_def_setProperties);;

    rho::apiGenerator::js_define_method("Rho:Camera:clearAllProperties", js_Camera_clearAllProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Camera:def_clearAllProperties", js_s_Camera_def_clearAllProperties);;

    rho::apiGenerator::js_define_method("Rho:Camera:enumerate", js_s_Camera_enumerate);


    rho::apiGenerator::js_define_method("Rho:Camera:getDefaultID", js_s_Camera_getDefaultID);
    rho::apiGenerator::js_define_method("Rho:Camera:setDefaultID", js_s_Camera_setDefaultID);

}

