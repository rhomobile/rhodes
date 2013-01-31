(function ($, rho, rhoUtil) {
    'use strict';

    var module = null;

    var apiReq = rhoUtil.apiReqFor('Barcode1');

    // === Barcode1 class definition ===

    function Barcode1(id) { this.id = id; };
    Barcode1.prototype.getId = function(){ return this.id; };

    Barcode1.prototype.getProps = function(props, cb) {
        return apiReq({
            instanceId: this.getId(),
            args: arguments,
            method: 'getProps',
            valueCallbackIndex: 1
        });
    };

    Barcode1.prototype.setProps = function(props) {
        return apiReq({
            instanceId: this.getId(),
            args: arguments,
            method: 'setProps'
        });
    };

    Barcode1.prototype.enumerate = function(cb) {
        return apiReq({
            instanceId: this.getId(),
            args: arguments,
            method:'enumerate',
            valueCallbackIndex: 0
        });
    };

    // === Default instance ===

    rho['Barcode1'] = rho['Barcode1'] || new Barcode1(rhoUtil.defaultId());

})(jQuery, Rho, Rho.util);
