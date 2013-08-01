/* Rho API iPhone native bridge */

(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';

    if (window[rhoUtil.flag.USE_AJAX_BRIDGE]) return;

    var RHO_API_CALL_TAG = '__rhoNativeApiCall';
    var RHO_API_TAG = '__rhoNativeApi';

    var nativeApi = window[RHO_API_TAG] || {

        apiCall: function (cmdText, async, resultHandler) {
            //window.alert(cmdText);
            var headers = {};
            headers[RHO_API_CALL_TAG] = cmdText;
            $.ajax({
                async: async,
                type: 'head',
                url: '/!'+RHO_API_TAG+'?' + (+new Date()),
                dataType: 'json',
                headers: headers
            }).done(function (data, status, jqXHR) {
                resultHandler(JSON.parse(jqXHR.responseText));
            }).fail(function (xhr, status, message) {
                resultHandler({error: {message: message, code: xhr.statusCode()}});
            });
        }
    };

    // TODO: uncomment as native handler will be implemented
    rhoPlatform.nativeApiCall = nativeApi.apiCall;

})(Rho.jQuery, Rho, Rho.platform, Rho.util);
