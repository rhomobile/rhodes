(function ($, rho, rhoUtil) {
    'use strict';

    var module = null;

    var apiReq = rhoUtil.apiReqFor('Rho.Mediacapture');

    // === Mediacapture class definition ===

    function Mediacapture(id) {
        if (!(this instanceof Mediacapture)) return new Mediacapture(id);

        this.getId = function () {
            return id;
        };
    };

    // === Mediacapture instance members ===

    
        Mediacapture.prototype.enable = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'enable',
                valueCallbackIndex: 1
            });
        };

    
        Mediacapture.prototype.start = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'start',
                valueCallbackIndex: 0
            });
        };

    
        Mediacapture.prototype.stop = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'stop',
                valueCallbackIndex: 0
            });
        };

    
        Mediacapture.prototype.disable = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'disable',
                valueCallbackIndex: 0
            });
        };

    
        Mediacapture.prototype.take = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'take',
                valueCallbackIndex: 1
            });
        };

    
        Mediacapture.prototype.getProperty = function(/* const rho::String& */ propertyName, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getProperty',
                valueCallbackIndex: 1
            });
        };

    
        Mediacapture.prototype.getProperties = function(/* const rho::Vector<rho::String>& */ arrayofNames, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getProperties',
                valueCallbackIndex: 1
            });
        };

    
        Mediacapture.prototype.getAllProperties = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'getAllProperties',
                valueCallbackIndex: 0
            });
        };

    
        Mediacapture.prototype.setProperty = function(/* const rho::String& */ propertyName, /* const rho::String& */ propertyValue, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setProperty',
                valueCallbackIndex: 2
            });
        };

    
        Mediacapture.prototype.setProperties = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'setProperties',
                valueCallbackIndex: 1
            });
        };

    
        Mediacapture.prototype.clearAllProperties = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: 'clearAllProperties',
                valueCallbackIndex: 0
            });
        };

    

    // === Mediacapture static members ===

    
            Mediacapture.MODE_AUTO = 'auto';
        
    
            Mediacapture.MODE_ALWAYSE_ON = 'alwaysOn';
        
    
            Mediacapture.MODE_ALWAYSE_OFF = 'alwaysOff';
        
    

    
        Mediacapture['enumerate'] = function(/* optional function */ oResult) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method: 'enumerate',
                valueCallbackIndex: 0
            });
        };
    

    // === Mediacapture default instance support ===

    

        Mediacapture['default'] = function () {
            return new Mediacapture(
                apiReq({
                    instanceId: '0',
                    args: [],
                    method:'getDefaultID'
                })
            );
        };

        Mediacapture['getDefaultID'] = function (valueCallback) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method:'getDefaultID',
                valueCallbackIndex: 0
            });
        };

        Mediacapture['setDefaultID'] = function (id) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method:'setDefaultID'
            });
        };

    

    rhoUtil.namespace('Rho.Mediacapture', Mediacapture);

})(jQuery, Rho, Rho.util);
