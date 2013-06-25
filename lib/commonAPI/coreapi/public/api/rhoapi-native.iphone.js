/* Rho API iPhone native bridge */

(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';

    var RHO_API_CALL_TAG = '__rhoNativeApiCall';
    var RHO_API_TAG = '__rhoNativeApi';

    var xhr = null;

    var nativeApi = window[RHO_API_TAG] || {

        apiCall: function (cmdText, async, resultHandler) {
            //window.alert(cmdText);

            if (xhr && xhr.readyState != 4) {
                xhr = null;
            }

            xhr = xhr || new XMLHttpRequest();
            xhr.open('HEAD', '/!'+RHO_API_TAG+'?' + (+new Date()), async);

            xhr.onload = function () {
                if (xhr.readyState === 4) {
                    //if (xhr.status === 200) {
                        //window.alert(nativeApiResult);
                        //alert("responce["+xhr.responseText+"]");
                        resultHandler(JSON.parse(xhr.responseText));
                    //} else {
                    //    resultHandler({error: {code: -2, message: 'Native API error code returned'}});
                    //}
                }
            };

            xhr.onerror = function () {
                resultHandler({error: {code: -1, message: 'Native API call error'}});
            };

            xhr.setRequestHeader(RHO_API_CALL_TAG, cmdText);
            xhr.send(null);
        }
    };

    // TODO: uncomment as native handler will be implemented
    rhoPlatform.nativeApiCall = nativeApi.apiCall;

})(jQuery, Rho, Rho.platform, Rho.util);
