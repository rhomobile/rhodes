(function ($, rho) {
    'use strict';

    var moduleName = 'Barcode1';
    var module = {};

    var apiReq = rho.commonAPI.apiReqFor(moduleName);

    // === Scanner class definition ===

    module.Scanner = function (id) { this.id = id; };
    module.Scanner.prototype.getId = function(){ return this.id; };

    module.Scanner.prototype.getProps = function(props, cb) {
        return apiReq({
            instanceId: this.id,
            args: arguments,
            method: 'getProps',
            valueCallbackIndex: 1
        });
    };

    module.Scanner.prototype.setProps = function(props) {
        return apiReq({
            instanceId: this.id,
            args: arguments,
            method: 'setProps'
        });
    };

    // === Module methods definition ===

    module.enumerate = function(cb) {
        return apiReq({
            instanceId: 'DEFAULT_OBJECT_ID',
            args: arguments,
            method:'enumerate',
            valueCallbackIndex: 0
        });
    };

    module.getProps = function(props, cb) {
        return apiReq({
            instanceId: 'DEFAULT_OBJECT_ID',
            args: arguments,
            method: 'getProps',
            valueCallbackIndex: 1
        });
    };

    // === make it public ===

    rho[moduleName] = rho[moduleName] || module;
})(jQuery, Rho);
