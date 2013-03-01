(function ($, rho, rhoUtil) {
    'use strict';

    var module = null;

    var apiReq = rhoUtil.apiReqFor('Rho.Camera');

    // === Camera class definition ===

    function Camera(id) {
        if (!(this instanceof Camera)) return new Camera(id);

        this.getId = function () {
            return id;
        };
    };

    // === Camera instance members ===

    
        Camera.prototype.desiredWidth = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'desiredWidth',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setDesiredWidth = function(/* int */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setDesiredWidth',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.desiredHeight = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'desiredHeight',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setDesiredHeight = function(/* int */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setDesiredHeight',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.compressionFormat = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'compressionFormat',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setCompressionFormat = function(/* const rho::String& */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setCompressionFormat',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.outputFormat = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'outputFormat',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setOutputFormat = function(/* const rho::String& */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setOutputFormat',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.colorModel = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'colorModel',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setColorModel = function(/* const rho::String& */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setColorModel',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.enableEditing = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'enableEditing',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setEnableEditing = function(/* bool */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setEnableEditing',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.flashMode = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'flashMode',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setFlashMode = function(/* const rho::String& */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setFlashMode',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.saveToSharedGallery = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'saveToSharedGallery',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setSaveToSharedGallery = function(/* bool */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setSaveToSharedGallery',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.captureSound = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'captureSound',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setCaptureSound = function(/* const rho::String& */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setCaptureSound',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.previewLeft = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'previewLeft',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setPreviewLeft = function(/* int */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setPreviewLeft',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.previewTop = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'previewTop',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setPreviewTop = function(/* int */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setPreviewTop',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.previewWidth = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'previewWidth',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setPreviewWidth = function(/* int */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setPreviewWidth',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.previewHeight = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'previewHeight',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setPreviewHeight = function(/* int */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setPreviewHeight',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.takePicture = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'takePicture',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.getCameraInfo = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getCameraInfo',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.choosePicture = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'choosePicture',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.saveImageToDeviceGallery = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'saveImageToDeviceGallery',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.getProperty = function(/* const rho::String& */ propertyName, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getProperty',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.getProperties = function(/* const rho::Vector<rho::String>& */ arrayofNames, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getProperties',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.getAllProperties = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getAllProperties',
                valueCallbackIndex: 0
            });
        };

    
        Camera.prototype.setProperty = function(/* const rho::String& */ propertyName, /* const rho::String& */ propertyValue, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setProperty',
                valueCallbackIndex: 2
            });
        };

    
        Camera.prototype.setProperties = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setProperties',
                valueCallbackIndex: 1
            });
        };

    
        Camera.prototype.clearAllProperties = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'clearAllProperties',
                valueCallbackIndex: 0
            });
        };

    

    // === Camera static members ===

    
            Camera.COMPRESSION_FORMAT_JPG = 'jpg';
        
    
            Camera.COMPRESSION_FORMAT_PNG = 'png';
        
    
            Camera.OUTPUT_FORMAT_IMAGE = 'image';
        
    
            Camera.OUTPUT_FORMAT_DATAURI = 'dataUri';
        
    
            Camera.FLASH_ON = 'on';
        
    
            Camera.FLASH_OFF = 'off';
        
    
            Camera.FLASH_AUTO = 'auto';
        
    
            Camera.FLASH_RED_EYE = 'redEye';
        
    
            Camera.FLASH_TORCH = 'torch';
        
    

    
        Camera['enumerate'] = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method: 'enumerate',
                valueCallbackIndex: 0
            });
        };
    

    // === Camera default instance support ===

    

        Camera['default'] = function () {
            return new Camera(
                apiReq({
                    instanceId: '0',
                    args: [],
                    method:'getDefaultID'
                })
            );
        };

        Camera['getDefaultID'] = function (valueCallback) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method:'getDefaultID',
                valueCallbackIndex: 0
            });
        };

        Camera['setDefaultID'] = function (id) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method:'setDefaultID'
            });
        };

    

    rhoUtil.namespace('Rho.Camera', Camera);

})(jQuery, Rho, Rho.util);
