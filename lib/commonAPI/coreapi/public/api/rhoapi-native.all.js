(function ($, rho, rhoPlatform, rhoUtil) {
    'use strict';

    /* ========================================================================
    You can force bridge type here, you can use it as:
        window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = rhoPlatform.id.ANDROID;

    Or this way:
        window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = 'android';

    Also, you can configure it right from your HTML file:
        window['__rho_nativeBridgeType'] = 'android';

    Just uncomment the line below and set appropriate value.
    ======================================================================== */
    //window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = rhoPlatform.id.AJAX;

    if (window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] == rhoPlatform.id.AJAX) return;

    var RHO_API_CALL_TAG = '__rhoNativeApiCall';
    var RHO_API_TAG = '__rhoNativeApi';

    var bridges = {};
    var addBridge = function(platformId, bridgeFactory) {
        bridges[platformId] = function() {
            var bridge = bridgeFactory();
            bridge.apiCall.platformId = platformId;
            return bridge;
        };
    };

    addBridge(rhoPlatform.id.ANDROID, function() {
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
    });

    addBridge(rhoPlatform.id.IPHONE, function() {
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
    });

    addBridge(rhoPlatform.id.WP8, function() {
        var apiResult = undefined;
        rhoPlatform.nativeApiResult = function(result) { apiResult = result; };
        window['__rhoNativeApiResult'] = rhoPlatform.nativeApiResult;

        return {
            apiCall: function (cmdText, async, resultHandler) {
                window.external.notify(cmdText);
                resultHandler(JSON.parse(apiResult));
            }
        }
    });

    addBridge(rhoPlatform.id.WM, function() {
        var bridge = new WebkitBridge();
        return {
            apiCall: function (cmdText, async, resultHandler) {
                var apiResult = bridge.framework(cmdText);
                resultHandler(JSON.parse(apiResult));
            }
        }
    });

    addBridge(rhoPlatform.id.RHOSIMLATOR, function() {
        return {
            apiCall: function (cmdText, async, resultHandler) {
                var nativeApiResult = {};

                if (window[RHO_API_TAG] && 'function' == typeof window[RHO_API_TAG]['apiCall']) {
                    nativeApiResult = window[RHO_API_TAG].apiCall(cmdText, async);
                }
                resultHandler(JSON.parse(nativeApiResult));
            }
        };
    });

    addBridge(rhoPlatform.id.WIN32, function() {
        return {
            apiCall: function (cmdText, async, resultHandler) {
                var nativeApiResult = {};

                if (window[RHO_API_TAG] && 'function' == typeof window[RHO_API_TAG]['apiCall']) {
                    nativeApiResult = window[RHO_API_TAG].apiCall(cmdText, async);
                }
                resultHandler(JSON.parse(nativeApiResult));
            }
        };
    });

    // the order is important
    var bridgeMapping = [
        [/RhoSimulator/                    , bridges[rhoPlatform.id.RHOSIMLATOR] ],
        [/Android/                         , bridges[rhoPlatform.id.ANDROID]     ],
        [/iPhone|iPod|iPad/                , bridges[rhoPlatform.id.IPHONE]      ],
        [/Windows\s+Phone/                 , bridges[rhoPlatform.id.WP8]         ],
        [/Windows\s+(?:Mobile|CE)|WM [0-9]/, bridges[rhoPlatform.id.WM]          ],
        [/Windows/                         , bridges[rhoPlatform.id.WIN32]       ]
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
