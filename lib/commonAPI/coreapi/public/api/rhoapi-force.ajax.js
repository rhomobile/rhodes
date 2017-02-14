/*
    Force Rho API to use AJAX bridge.
    Use in rhobuild.yml in a following way:

    ajax_api_bridge:
        wm: true
        rhosim: false

 */

(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';

    //window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] || rhoPlatform.id.AJAX;
    window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = rhoPlatform.id.AJAX;


})(Rho.jQuery, Rho, Rho.platform, Rho.util);
