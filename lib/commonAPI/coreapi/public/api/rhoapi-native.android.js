/* */

(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';

    var RHO_API_CALL_TAG = '__rhoNativeApiCall:';
    var RHO_API_TAG = '__rhoNativeApi:';

    var nativeApi = window[RHO_API_TAG] || {

        apiCall: function (cmdText, async, resultHandler) {
            //window.alert(cmdText);

            var nativeApiResult =  prompt(cmdText, RHO_API_CALL_TAG + 'prompt');

            //window.alert(nativeApiResult);
            resultHandler($.evalJSON(nativeApiResult));
        }
    };

    // TODO: uncomment as native handler will be implemented
    // rhoPlatform.nativeApiCall = nativeApi.apiCall;

})(jQuery, Rho, Rho.platform, Rho.util);
