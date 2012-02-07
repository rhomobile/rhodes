(function($){

    var _rho_deferred_paramName = "_rho_deferred";
    var _rho_callbackId_paramName = "_rho_callbackId";
    var _rho_callbackId_valuePrefix = "_rhoId#";

    // console function proxies ============================================
    if ("object" != typeof console) console = {};

    if ("function" != typeof console.log) console.log = function(msg) {
        window.external.notify('console.log: ' +msg);
    };

    if ("function" != typeof console.info) console.info = console.log;

    if ("function" != typeof console.warn) console.warn = function(msg) {
        window.external.notify('console.warn: ' +msg);
    };

    if ("function" != typeof console.error) console.error = function(msg) {
        window.external.notify('console.error: ' +msg);
    };

    if ("function" != typeof window.alert && "object" != typeof window.alert) window.alert = function(msg) {
        window.external.notify('alert: ' +msg);
    };
    // =====================================================================

    // do nothing if jQuery is undefined
    if (!$) {
        console.error('jQuery is undefined');
        return;
    }

    var callbackCount = 0;
    var pendingCallbacks = {};

    function normalizeAjaxOptions(urlOrOptions, options) {
        if ('string' == typeof urlOrOptions && 'object' == typeof options) {
            return $.extend({}, {url: urlOrOptions}, options);
        } else if ('object' == typeof urlOrOptions) {
            return urlOrOptions;
        }
        return {};
    }

    var _ajax = $.ajax;

    function wp7ajax() {
        //console.log('in wp7ajax');
        //console.log('window.location: ' +window.location);
        // there may be ajax(options) or ajax(url, options) form of call
        var options = normalizeAjaxOptions.apply(this, arguments);
        //console.log('options has been normalized, options.url == "' +options.url +'"');

        // if we have URL in request options and it is a local URL
        if (options.url.match(/^https?:\/\/(127\.0\.0\.1|localhost)/) ||
            options.url.match(/^x-wmapp1:/)) {
            //console.log('it is a local app URL');
            // then call proxy
            return wp7notifyProxy.apply(this, [options]);
        }
        // else, let default AJAX function to handle it
        return _ajax.apply(this, arguments);
    }

    $.ajax = wp7ajax;

    function wp7notifyProxy(options) {
        //console.log('in wp7notifyProxy');
        /*
         * IMPORTANT NOTE! ===========================================
         * At the moment this implementation doesn't support anything
         * besides GET request. No headers, cookies, response status
         * codes, etc. Just request parameters and response content.
         * ===========================================================
         */

        //console.log('options.url: ' + options.url);
        //console.log('typeof options.data: ' + typeof options.data);
        //if ("string" == typeof options.data) console.log('options.data: ' + options.data);

        var urlQueryParams = "";
        // set next call id value
        var cbIdValue = encodeURIComponent(_rho_callbackId_valuePrefix + callbackCount++);

        if ("string" == typeof options.data) {
            urlQueryParams = options.data;
        }
        if ("object" != typeof options.data) {
            options.data = {};
        }

        // set callback id param value
        options.data[_rho_callbackId_paramName] = cbIdValue;
        // set deferred object to resolve/reject late
        options[_rho_deferred_paramName] = $.Deferred();
        // store options for pending callback
        pendingCallbacks[options.data[_rho_callbackId_paramName]] = options;

        // compose GET request formatted URI
        $.each(options.data, function(name, value){
            urlQueryParams += ((0 < urlQueryParams.length ? "&" : "")
                +encodeURIComponent(name) +'=' +encodeURIComponent(value));
        });

        //console.log('wp7notifyProxy: urlQueryParams: ' +urlQueryParams);
        window.external.notify('request:' +options.url +"?" +urlQueryParams);
        return options[_rho_deferred_paramName];
    }

    function fireHandlers(options, result, status, errCode) {
        // TODO: fake jqXHR needs to be provided
        var jqXHR = null;

        if ("error" == status) {
            if ('function' == typeof options.error) {
                // start handler asynchronously
                setTimeout(function(){
                    options.error.apply(this, [jqXHR, status, result]);
                }, 1);
            }
            if ('object' == typeof options[_rho_deferred_paramName]) {
                options[_rho_deferred_paramName].resolve([jqXHR, status, result]);
            }
        } else {
            if ('function' == typeof options.success) {
                // start handler asynchronously
                setTimeout(function(){
                    options.success.apply(this, [result, status, jqXHR]);
                }, 1);
            }
            if ('object' == typeof options[_rho_deferred_paramName]) {
                options[_rho_deferred_paramName].resolve([result, status, jqXHR]);
            }
        }
    }

    window._rho_ajaxProxyCallback = function(callbackId, result, status, errCode) {
        var cbId = decodeURIComponent(callbackId);
        //console.log('_rho_ajaxProxyCallback: callback for: ' +cbId);
        if (pendingCallbacks[cbId]) {
            fireHandlers(pendingCallbacks[cbId], result, status, errCode);
            delete pendingCallbacks[cbId];
        }
    };


})(jQuery);
