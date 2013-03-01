
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_mCamera;

VALUE rb_Camera_getDesiredWidth(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getDesiredWidth(int argc, VALUE *argv);
VALUE rb_Camera_setDesiredWidth(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setDesiredWidth(int argc, VALUE *argv);
VALUE rb_Camera_getDesiredHeight(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getDesiredHeight(int argc, VALUE *argv);
VALUE rb_Camera_setDesiredHeight(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setDesiredHeight(int argc, VALUE *argv);
VALUE rb_Camera_getCompressionFormat(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getCompressionFormat(int argc, VALUE *argv);
VALUE rb_Camera_setCompressionFormat(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setCompressionFormat(int argc, VALUE *argv);
VALUE rb_Camera_getOutputFormat(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getOutputFormat(int argc, VALUE *argv);
VALUE rb_Camera_setOutputFormat(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setOutputFormat(int argc, VALUE *argv);
VALUE rb_Camera_getColorModel(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getColorModel(int argc, VALUE *argv);
VALUE rb_Camera_setColorModel(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setColorModel(int argc, VALUE *argv);
VALUE rb_Camera_getEnableEditing(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getEnableEditing(int argc, VALUE *argv);
VALUE rb_Camera_setEnableEditing(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setEnableEditing(int argc, VALUE *argv);
VALUE rb_Camera_getFlashMode(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getFlashMode(int argc, VALUE *argv);
VALUE rb_Camera_setFlashMode(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setFlashMode(int argc, VALUE *argv);
VALUE rb_Camera_getSaveToSharedGallery(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getSaveToSharedGallery(int argc, VALUE *argv);
VALUE rb_Camera_setSaveToSharedGallery(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setSaveToSharedGallery(int argc, VALUE *argv);
VALUE rb_Camera_getCaptureSound(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getCaptureSound(int argc, VALUE *argv);
VALUE rb_Camera_setCaptureSound(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setCaptureSound(int argc, VALUE *argv);
VALUE rb_Camera_getPreviewLeft(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getPreviewLeft(int argc, VALUE *argv);
VALUE rb_Camera_setPreviewLeft(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setPreviewLeft(int argc, VALUE *argv);
VALUE rb_Camera_getPreviewTop(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getPreviewTop(int argc, VALUE *argv);
VALUE rb_Camera_setPreviewTop(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setPreviewTop(int argc, VALUE *argv);
VALUE rb_Camera_getPreviewWidth(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getPreviewWidth(int argc, VALUE *argv);
VALUE rb_Camera_setPreviewWidth(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setPreviewWidth(int argc, VALUE *argv);
VALUE rb_Camera_getPreviewHeight(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getPreviewHeight(int argc, VALUE *argv);
VALUE rb_Camera_setPreviewHeight(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setPreviewHeight(int argc, VALUE *argv);
VALUE rb_Camera_takePicture(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_takePicture(int argc, VALUE *argv);
VALUE rb_Camera_getCameraInfo(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getCameraInfo(int argc, VALUE *argv);
VALUE rb_Camera_choosePicture(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_choosePicture(int argc, VALUE *argv);
VALUE rb_Camera_saveImageToDeviceGallery(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_saveImageToDeviceGallery(int argc, VALUE *argv);
VALUE rb_Camera_getProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getProperty(int argc, VALUE *argv);
VALUE rb_Camera_getProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getProperties(int argc, VALUE *argv);
VALUE rb_Camera_getAllProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_getAllProperties(int argc, VALUE *argv);
VALUE rb_Camera_setProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setProperty(int argc, VALUE *argv);
VALUE rb_Camera_setProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_setProperties(int argc, VALUE *argv);
VALUE rb_Camera_clearAllProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_Camera_def_clearAllProperties(int argc, VALUE *argv);
VALUE rb_s_Camera_enumerate(int argc, VALUE *argv);



VALUE rb_Camera_s_default(VALUE klass);
VALUE rb_Camera_s_setDefault(VALUE klass, VALUE obj);


VALUE getRuby_Camera_Module(){ return rb_mCamera; }



static void _free_class_object(void *p)
{
    ruby_xfree(p);
}

static VALUE _allocate_class_object(VALUE klass)
{
    VALUE valObj = 0;
    char ** ppString = NULL;
    void* pData = ALLOC(void*);
    memset( pData, 0, sizeof(pData) );
    
	valObj = Data_Wrap_Struct(klass, 0, _free_class_object, pData);

    Data_Get_Struct(valObj, char *, ppString);
    *ppString = xmalloc(10);
    sprintf(*ppString, "%X", valObj);

    return valObj;
}

void Init_RubyAPI_Camera(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    

	rb_mCamera = rb_define_class_under(rb_mParent, "Camera", rb_cObject);

	rb_define_alloc_func(rb_mCamera, _allocate_class_object);
    //Constructor should be not available in case of static members
    //rb_undef_alloc_func(rb_mCamera);

    rb_define_method(rb_mCamera, "desiredWidth", rb_Camera_getDesiredWidth, -1);
    rb_define_singleton_method(rb_mCamera, "desiredWidth", rb_s_Camera_def_getDesiredWidth, -1);
    rb_define_method(rb_mCamera, "desiredWidth=", rb_Camera_setDesiredWidth, -1);
    rb_define_singleton_method(rb_mCamera, "desiredWidth=", rb_s_Camera_def_setDesiredWidth, -1);
    rb_define_method(rb_mCamera, "desiredHeight", rb_Camera_getDesiredHeight, -1);
    rb_define_singleton_method(rb_mCamera, "desiredHeight", rb_s_Camera_def_getDesiredHeight, -1);
    rb_define_method(rb_mCamera, "desiredHeight=", rb_Camera_setDesiredHeight, -1);
    rb_define_singleton_method(rb_mCamera, "desiredHeight=", rb_s_Camera_def_setDesiredHeight, -1);
    rb_define_method(rb_mCamera, "compressionFormat", rb_Camera_getCompressionFormat, -1);
    rb_define_singleton_method(rb_mCamera, "compressionFormat", rb_s_Camera_def_getCompressionFormat, -1);
    rb_define_method(rb_mCamera, "compressionFormat=", rb_Camera_setCompressionFormat, -1);
    rb_define_singleton_method(rb_mCamera, "compressionFormat=", rb_s_Camera_def_setCompressionFormat, -1);
    rb_define_method(rb_mCamera, "outputFormat", rb_Camera_getOutputFormat, -1);
    rb_define_singleton_method(rb_mCamera, "outputFormat", rb_s_Camera_def_getOutputFormat, -1);
    rb_define_method(rb_mCamera, "outputFormat=", rb_Camera_setOutputFormat, -1);
    rb_define_singleton_method(rb_mCamera, "outputFormat=", rb_s_Camera_def_setOutputFormat, -1);
    rb_define_method(rb_mCamera, "colorModel", rb_Camera_getColorModel, -1);
    rb_define_singleton_method(rb_mCamera, "colorModel", rb_s_Camera_def_getColorModel, -1);
    rb_define_method(rb_mCamera, "colorModel=", rb_Camera_setColorModel, -1);
    rb_define_singleton_method(rb_mCamera, "colorModel=", rb_s_Camera_def_setColorModel, -1);
    rb_define_method(rb_mCamera, "enableEditing", rb_Camera_getEnableEditing, -1);
    rb_define_singleton_method(rb_mCamera, "enableEditing", rb_s_Camera_def_getEnableEditing, -1);
    rb_define_method(rb_mCamera, "enableEditing=", rb_Camera_setEnableEditing, -1);
    rb_define_singleton_method(rb_mCamera, "enableEditing=", rb_s_Camera_def_setEnableEditing, -1);
    rb_define_method(rb_mCamera, "flashMode", rb_Camera_getFlashMode, -1);
    rb_define_singleton_method(rb_mCamera, "flashMode", rb_s_Camera_def_getFlashMode, -1);
    rb_define_method(rb_mCamera, "flashMode=", rb_Camera_setFlashMode, -1);
    rb_define_singleton_method(rb_mCamera, "flashMode=", rb_s_Camera_def_setFlashMode, -1);
    rb_define_method(rb_mCamera, "saveToSharedGallery", rb_Camera_getSaveToSharedGallery, -1);
    rb_define_singleton_method(rb_mCamera, "saveToSharedGallery", rb_s_Camera_def_getSaveToSharedGallery, -1);
    rb_define_method(rb_mCamera, "saveToSharedGallery=", rb_Camera_setSaveToSharedGallery, -1);
    rb_define_singleton_method(rb_mCamera, "saveToSharedGallery=", rb_s_Camera_def_setSaveToSharedGallery, -1);
    rb_define_method(rb_mCamera, "captureSound", rb_Camera_getCaptureSound, -1);
    rb_define_singleton_method(rb_mCamera, "captureSound", rb_s_Camera_def_getCaptureSound, -1);
    rb_define_method(rb_mCamera, "captureSound=", rb_Camera_setCaptureSound, -1);
    rb_define_singleton_method(rb_mCamera, "captureSound=", rb_s_Camera_def_setCaptureSound, -1);
    rb_define_method(rb_mCamera, "previewLeft", rb_Camera_getPreviewLeft, -1);
    rb_define_singleton_method(rb_mCamera, "previewLeft", rb_s_Camera_def_getPreviewLeft, -1);
    rb_define_method(rb_mCamera, "previewLeft=", rb_Camera_setPreviewLeft, -1);
    rb_define_singleton_method(rb_mCamera, "previewLeft=", rb_s_Camera_def_setPreviewLeft, -1);
    rb_define_method(rb_mCamera, "previewTop", rb_Camera_getPreviewTop, -1);
    rb_define_singleton_method(rb_mCamera, "previewTop", rb_s_Camera_def_getPreviewTop, -1);
    rb_define_method(rb_mCamera, "previewTop=", rb_Camera_setPreviewTop, -1);
    rb_define_singleton_method(rb_mCamera, "previewTop=", rb_s_Camera_def_setPreviewTop, -1);
    rb_define_method(rb_mCamera, "previewWidth", rb_Camera_getPreviewWidth, -1);
    rb_define_singleton_method(rb_mCamera, "previewWidth", rb_s_Camera_def_getPreviewWidth, -1);
    rb_define_method(rb_mCamera, "previewWidth=", rb_Camera_setPreviewWidth, -1);
    rb_define_singleton_method(rb_mCamera, "previewWidth=", rb_s_Camera_def_setPreviewWidth, -1);
    rb_define_method(rb_mCamera, "previewHeight", rb_Camera_getPreviewHeight, -1);
    rb_define_singleton_method(rb_mCamera, "previewHeight", rb_s_Camera_def_getPreviewHeight, -1);
    rb_define_method(rb_mCamera, "previewHeight=", rb_Camera_setPreviewHeight, -1);
    rb_define_singleton_method(rb_mCamera, "previewHeight=", rb_s_Camera_def_setPreviewHeight, -1);
    rb_define_method(rb_mCamera, "takePicture", rb_Camera_takePicture, -1);
    rb_define_singleton_method(rb_mCamera, "takePicture", rb_s_Camera_def_takePicture, -1);
    rb_define_method(rb_mCamera, "getCameraInfo", rb_Camera_getCameraInfo, -1);
    rb_define_singleton_method(rb_mCamera, "getCameraInfo", rb_s_Camera_def_getCameraInfo, -1);
    rb_define_method(rb_mCamera, "choosePicture", rb_Camera_choosePicture, -1);
    rb_define_singleton_method(rb_mCamera, "choosePicture", rb_s_Camera_def_choosePicture, -1);
    rb_define_method(rb_mCamera, "saveImageToDeviceGallery", rb_Camera_saveImageToDeviceGallery, -1);
    rb_define_singleton_method(rb_mCamera, "saveImageToDeviceGallery", rb_s_Camera_def_saveImageToDeviceGallery, -1);
    rb_define_method(rb_mCamera, "getProperty", rb_Camera_getProperty, -1);
    rb_define_singleton_method(rb_mCamera, "getProperty", rb_s_Camera_def_getProperty, -1);
    rb_define_method(rb_mCamera, "getProperties", rb_Camera_getProperties, -1);
    rb_define_singleton_method(rb_mCamera, "getProperties", rb_s_Camera_def_getProperties, -1);
    rb_define_method(rb_mCamera, "getAllProperties", rb_Camera_getAllProperties, -1);
    rb_define_singleton_method(rb_mCamera, "getAllProperties", rb_s_Camera_def_getAllProperties, -1);
    rb_define_method(rb_mCamera, "setProperty", rb_Camera_setProperty, -1);
    rb_define_singleton_method(rb_mCamera, "setProperty", rb_s_Camera_def_setProperty, -1);
    rb_define_method(rb_mCamera, "setProperties", rb_Camera_setProperties, -1);
    rb_define_singleton_method(rb_mCamera, "setProperties", rb_s_Camera_def_setProperties, -1);
    rb_define_method(rb_mCamera, "clearAllProperties", rb_Camera_clearAllProperties, -1);
    rb_define_singleton_method(rb_mCamera, "clearAllProperties", rb_s_Camera_def_clearAllProperties, -1);
    rb_define_singleton_method(rb_mCamera, "enumerate", rb_s_Camera_enumerate, -1);



    rb_define_singleton_method(rb_mCamera, "default", rb_Camera_s_default, 0);
    rb_define_singleton_method(rb_mCamera, "setDefault", rb_Camera_s_setDefault, 1);
    rb_define_singleton_method(rb_mCamera, "default=", rb_Camera_s_setDefault, 1);



    rb_const_set(rb_mCamera, rb_intern("COMPRESSION_FORMAT_JPG"), rb_str_new2("jpg") );
    rb_const_set(rb_mCamera, rb_intern("COMPRESSION_FORMAT_PNG"), rb_str_new2("png") );
    rb_const_set(rb_mCamera, rb_intern("OUTPUT_FORMAT_IMAGE"), rb_str_new2("image") );
    rb_const_set(rb_mCamera, rb_intern("OUTPUT_FORMAT_DATAURI"), rb_str_new2("dataUri") );
    rb_const_set(rb_mCamera, rb_intern("FLASH_ON"), rb_str_new2("on") );
    rb_const_set(rb_mCamera, rb_intern("FLASH_OFF"), rb_str_new2("off") );
    rb_const_set(rb_mCamera, rb_intern("FLASH_AUTO"), rb_str_new2("auto") );
    rb_const_set(rb_mCamera, rb_intern("FLASH_RED_EYE"), rb_str_new2("redEye") );
    rb_const_set(rb_mCamera, rb_intern("FLASH_TORCH"), rb_str_new2("torch") );



}

