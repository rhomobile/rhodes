(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';

    /* ========================================================================
    You can force bridge type here, you can use it as:
        window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = rhoUtil.platform.id.ANDROID;

    Or this way:
        window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = 'android';

    Also, you can configure it right from your HTML file:
        window['__rho_nativeBridgeType'] = 'android';

    Just uncomment the line below and set appropriate value.
    ======================================================================== */
    //window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = rhoUtil.platform.id.AJAX;

    if (window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] == rhoUtil.platform.id.AJAX) return;

    var RHO_API_CALL_TAG = '__rhoNativeApiCall';
    var RHO_API_TAG = '__rhoNativeApi';

    var bridges = {};

    bridges[rhoUtil.platform.id.ANDROID] = function () {
        return {
            apiCall: function (cmdText, async, resultHandler) {
                var nativeApiResult = {};
                if (window[RHO_API_TAG] && 'function' == typeof window[RHO_API_TAG]['apiCall']) {
                    nativeApiResult = window[RHO_API_TAG].apiCall(cmdText, async);
                } else {
                    nativeApiResult = prompt(cmdText, RHO_API_CALL_TAG + ':prompt');
                }
                resultHandler(JSON.parse(nativeApiResult));
            }
        };
    }

    bridges[rhoUtil.platform.id.IPHONE] = function() {
        return window[RHO_API_TAG] || {
            apiCall: function (cmdText, async, resultHandler) {
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
    }

    bridges[rhoUtil.platform.id.WP8] = function() {
        var apiResult = undefined;
        rhoPlatform.nativeApiResult = function(result) { apiResult = result; };
        window['__rhoNativeApiResult'] = rhoPlatform.nativeApiResult;

        return {
            apiCall: function (cmdText, async, resultHandler) {
                window.external.notify(cmdText);
                resultHandler(JSON.parse(apiResult));
            }
        }
    }

    bridges[rhoUtil.platform.id.WM] = function() {
        var bridge = new WebkitBridge();
        return {
            apiCall: function (cmdText, async, resultHandler) {
                var apiResult = bridge.framework(cmdText);
                resultHandler(JSON.parse(apiResult));
            }
        }
    }

    bridges[rhoUtil.platform.id.RHOSIMLATOR] = function() {
        return {
            apiCall: function (cmdText, async, resultHandler) {
                var nativeApiResult = {};

                if (window[RHO_API_TAG] && 'function' == typeof window[RHO_API_TAG]['apiCall']) {
                    nativeApiResult = window[RHO_API_TAG].apiCall(cmdText, async);
                }
                resultHandler(JSON.parse(nativeApiResult));
            }
        };
    }

    bridges[rhoUtil.platform.id.WIN32] = function() {
        return {
            apiCall: function (cmdText, async, resultHandler) {
                var nativeApiResult = {};

                if (window[RHO_API_TAG] && 'function' == typeof window[RHO_API_TAG]['apiCall']) {
                    nativeApiResult = window[RHO_API_TAG].apiCall(cmdText, async);
                }
                resultHandler(JSON.parse(nativeApiResult));
            }
        };
    }

    // the order is important
    var bridgeMapping = [
        [/RhoSimulator/                    , bridges[rhoUtil.platform.id.RHOSIMLATOR] ],
        [/Android/                         , bridges[rhoUtil.platform.id.ANDROID]     ],
        [/iPhone|iPod|iPad/                , bridges[rhoUtil.platform.id.IPHONE]      ],
        [/Windows\s+Phone/                 , bridges[rhoUtil.platform.id.WP8]         ],
        [/Windows\s+(?:Mobile|CE)|WM [0-9]/, bridges[rhoUtil.platform.id.WM]          ],
        [/Windows/                         , bridges[rhoUtil.platform.id.WIN32]       ]
    ];

    // use forced bridge type value
    var platformBridge = bridges[window[rhoUtil.flag.NATIVE_BRIDGE_TYPE]];


    // autodetect bridge type
    if (!platformBridge) {
        for (var i = 0; bridgeMapping.length; ++i) {
            if (bridgeMapping[i][0].test(navigator.userAgent)) {
                platformBridge = bridgeMapping[i][1];
                break;
            }
        }
    }

    if (platformBridge) {
        rhoPlatform.nativeApiCall = platformBridge().apiCall;
    } else {
        throw 'User-Agent [' + navigator.userAgent + '] does not match any known platform. Going to use AJAX bridge.';
    }

})(Rho.jQuery, Rho, Rho.platform, Rho.util);
