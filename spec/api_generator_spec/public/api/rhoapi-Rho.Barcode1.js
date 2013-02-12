(function ($, rho, rhoUtil) {
    'use strict';

    var module = null;

    var apiReq = rhoUtil.apiReqFor('Rho.Barcode1');

    // === Barcode1 class definition ===

    function Barcode1(id) {
        if (!(this instanceof Barcode1)) return new Barcode1(id);

        this.getId = function () {
            return id;
        };
    };

    // === Barcode1 instance members ===

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

    // === Barcode1 static members ===

    Barcode1.CAMERA_READER = 'CAMERA_READER';
    Barcode1.LASER_READER = 'LASER_READER';

    Barcode1['default'] = function () {
        return new Barcode1(
            apiReq({
                instanceId: '0',
                args: [],
                method:'getDefaultID'
            })
        );
    };

    Barcode1['getDefaultID'] = function (cd) {
        return apiReq({
            instanceId: '0',
            args: arguments,
            method:'getDefaultID',
            valueCallbackIndex: 0
        });
    };

    Barcode1['setDefaultID'] = function () {
        return apiReq({
            instanceId: '0',
            args: arguments,
            method:'setDefaultID'
        });
    };

    Barcode1['enumerate'] = function(cb) {
        return apiReq({
            instanceId: '0',
            args: arguments,
            method:'enumerate',
            valueCallbackIndex: 0
        });
    };

    rhoUtil.namespace('Rho.Barcode1', Barcode1);

})(jQuery, Rho, Rho.util);
