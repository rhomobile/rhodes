/*
    ==============================================================================================================

    To return following object as result:
        { abc: 123 }

    You need to pass it from native to js this way:
        WebView.execute_js("__rhoNativeApiResult({result: { abc:123 } })");

    To return error as result:
        { code: 123, message: 'some error' }

    You need to pass it from native to js this way:
        WebView.execute_js("__rhoNativeApiResult({error: { code: 123, message: 'some error' } })");

    ==============================================================================================================
 */

(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';

    var apiResult = undefined;

    rhoPlatform.nativeApiCall = function (cmdText, async, resultHandler) {
        //window.alert(cmdText);
        window.external.notify(cmdText);
        //window.alert(rhoPlatform.nativeApiResult);

        resultHandler(JSON.parse(apiResult));
    };

    rhoPlatform.nativeApiResult = function(result) {
        apiResult = result;
    };

    window['__rhoNativeApiResult'] = rhoPlatform.nativeApiResult;

})(jQuery, Rho, Rho.platform, Rho.util);
