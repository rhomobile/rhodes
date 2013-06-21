/* Rho API WinMob native bridge */

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
/*
(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';


    var bridge    = new WebkitBridge();
    var apiResult = undefined;

    rhoPlatform.nativeApiCall = function (cmdText, async, resultHandler) {
        //window.alert(cmdText);

        var host = "http://localhost:25125";

        var apiResult = bridge.framework(host, API_CONTROLLER_URL, cmdText);
        alert (response); 

        resultHandler(JSON.parse(apiResult));
    };

    rhoPlatform.nativeApiResult = function(result) {
        apiResult = result;
    };

    window['__rhoNativeApiResult'] = rhoPlatform.nativeApiResult;

})(jQuery, Rho, Rho.platform, Rho.util);
*/