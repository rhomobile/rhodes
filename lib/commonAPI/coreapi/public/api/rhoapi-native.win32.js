/* Rho API RhoSimulator native bridge */

(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';

    if ((window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] != undefined) && (window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] == rhoUtil.platform.id.AJAX)) return;

    var RHO_API_CALL_TAG = '__rhoNativeApiCall';
    var RHO_API_TAG = '__rhoNativeApi';

    var nativeApi = {

        apiCall: function (cmdText, async, resultHandler) {
            //window.alert(cmdText);

            var nativeApiResult = {};

            if (window[RHO_API_TAG] && 'function' == typeof window[RHO_API_TAG]['apiCall']) {
                nativeApiResult = window[RHO_API_TAG].apiCall(cmdText, async);
            }

            //window.alert(nativeApiResult);
            resultHandler(JSON.parse(nativeApiResult));
        }
    };

    // TODO: uncomment as native handler will be implemented
    rhoPlatform.nativeApiCall = nativeApi.apiCall;

})(Rho.jQuery, Rho, Rho.platform, Rho.util);
