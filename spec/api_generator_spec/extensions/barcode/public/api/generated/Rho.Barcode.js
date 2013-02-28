(function ($, rho, rhoUtil) {
    'use strict';

    var module = null;

    var apiReq = rhoUtil.apiReqFor('Rho.Barcode');

    // === Barcode class definition ===

    function Barcode(id) {
        if (!(this instanceof Barcode)) return new Barcode(id);

        this.getId = function () {
            return id;
        };
    };

    // === Barcode instance members ===

    
        Barcode.prototype.illuminationMode = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'illuminationMode',
                valueCallbackIndex: 0
            });
        };

    
        Barcode.prototype.setIlluminationMode = function(/* const rho::String& */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setIlluminationMode',
                valueCallbackIndex: 1
            });
        };

    
        Barcode.prototype.test = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'test',
                valueCallbackIndex: 0
            });
        };

    
        Barcode.prototype.setTest = function(/* const rho::String& */ value, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setTest',
                valueCallbackIndex: 1
            });
        };

    
        Barcode.prototype.enable = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'enable',
                valueCallbackIndex: 1
            });
        };

    
        Barcode.prototype.start = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'start',
                valueCallbackIndex: 0
            });
        };

    
        Barcode.prototype.stop = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'stop',
                valueCallbackIndex: 0
            });
        };

    
        Barcode.prototype.disable = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'disable',
                valueCallbackIndex: 0
            });
        };

    
        Barcode.prototype.take = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'take',
                valueCallbackIndex: 1
            });
        };

    
        Barcode.prototype.getProperty = function(/* const rho::String& */ propertyName, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getProperty',
                valueCallbackIndex: 1
            });
        };

    
        Barcode.prototype.getProperties = function(/* const rho::Vector<rho::String>& */ arrayofNames, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getProperties',
                valueCallbackIndex: 1
            });
        };

    
        Barcode.prototype.getAllProperties = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getAllProperties',
                valueCallbackIndex: 0
            });
        };

    
        Barcode.prototype.setProperty = function(/* const rho::String& */ propertyName, /* const rho::String& */ propertyValue, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setProperty',
                valueCallbackIndex: 2
            });
        };

    
        Barcode.prototype.setProperties = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setProperties',
                valueCallbackIndex: 1
            });
        };

    
        Barcode.prototype.clearAllProperties = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'clearAllProperties',
                valueCallbackIndex: 0
            });
        };

    

    // === Barcode static members ===

    
            Barcode.MODE_AUTO = 'auto';
        
    
            Barcode.MODE_ALWAYSE_ON = 'alwaysOn';
        
    
            Barcode.MODE_ALWAYSE_OFF = 'alwaysOff';
        
    

    
        Barcode['enumerate'] = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method: 'enumerate',
                valueCallbackIndex: 0
            });
        };
    

    // === Barcode default instance support ===

    

        Barcode['default'] = function () {
            return new Barcode(
                apiReq({
                    instanceId: '0',
                    args: [],
                    method:'getDefaultID'
                })
            );
        };

        Barcode['getDefaultID'] = function (valueCallback) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method:'getDefaultID',
                valueCallbackIndex: 0
            });
        };

        Barcode['setDefaultID'] = function (id) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method:'setDefaultID'
            });
        };

    

    rhoUtil.namespace('Rho.Barcode', Barcode);

})(jQuery, Rho, Rho.util);
