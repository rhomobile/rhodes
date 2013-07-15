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

    function normalizeAjaxData(data) {
        // if it is an object then return as it is
        if ('object' == typeof data) return data;
        // if it is a query string then parse it and return as an object
        if ('string' == typeof data) {
            data = $.trim(data);
            data = (0 <= data.indexOf('?')) ? data.substring(data.indexOf('?')) : data;
            var items = {};
            $.each(data.split('&'), function(idx, item){
                var keyVal = item.split('=');
                if (keyVal && keyVal[0]) {
                    items[keyVal[0]] = keyVal[1] || null;
                }
            });
            return items;
        }
        // if it is neither string nor object,
        // then so far we have no idea how to handle it
        return {};
    }

    var _ajax = $.ajax;

    function isLocalUrl(url) {
        //console.log('isLocalUrl: ' +url);
        if (url.match(/^https?:\/\//) &&
            !url.match(/^https?:\/\/(127\.0\.0\.1|localhost)/)) return false;
        return true;
    }

    function wp7ajax() {
        //console.log('in wp7ajax');
        //console.log('window.location: ' +window.location);
        // there may be ajax(options) or ajax(url, options) form of call
        var options = normalizeAjaxOptions.apply(this, arguments);
        //console.log('options has been normalized, options.url == "' +options.url +'"');

        // if we have URL in request options and it is a local URL
        if (isLocalUrl(options.url)) {
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

        // set next call id value
        var cbIdValue = (_rho_callbackId_valuePrefix + callbackCount++);

        // ensure data is an object, not string
        var data = normalizeAjaxData(options.data);

        // set callback id param value
        data[_rho_callbackId_paramName] = encodeURIComponent(cbIdValue);

        // set deferred object to resolve/reject late
        options[_rho_deferred_paramName] = $.Deferred();

        // store options for pending callback
        pendingCallbacks[cbIdValue] = options;

        // compose GET request formatted URI
        var urlQueryParams = "";
        $.each(data, function(name, value){
            urlQueryParams += ((0 < urlQueryParams.length ? "&" : "?")
                +encodeURIComponent(name) +'=' +encodeURIComponent(value));
        });

        var request = $.extend({}, {
            url: options.url,
            type: options.type || 'GET',
            contentType: options.contentType || 'application/x-www-form-urlencoded',
            headers: options.headers || {},
            username: options.username || null,
            password: options.password || null,
            data: data
        });

        if (options.accepts) {
            request.headers['Accept'] = options.accepts;
        }

        //console.log('wp7notifyProxy: urlQueryParams: ' +urlQueryParams);
        var reqObj = JSON.stringify(request);
        window.external.notify('request:' +reqObj);
        //window.external.notify('request:' +options.url +urlQueryParams);
        return options[_rho_deferred_paramName];
    }

    function fireHandlers(options, result, headers, status, errCode) {
        // TODO: fake jqXHR needs to be provided
        var jqXHR = {
            getResponseHeader: function(name) {
                return ("object" == typeof headers) ? headers[name] : undefined;
            },
            getAllResponseHeaders: function() {
                return ("object" == typeof headers) ? headers : {};
            }
        };
        if ("error" == status) {
            if ('function' == typeof options.error) {
                // start handler asynchronously
                setTimeout(function(){
                    options.error.apply(options.context, [jqXHR, status, result]);
                }, 1);
            }
            if ('object' == typeof options[_rho_deferred_paramName]) {
                options[_rho_deferred_paramName].resolve([jqXHR, status, result]);
            }
        } else {
            if ('function' == typeof options.success) {
                // start handler asynchronously
                setTimeout(function(){
                    options.success.apply(options.context, [result, status, jqXHR]);
                }, 1);
            }
            if ('object' == typeof options[_rho_deferred_paramName]) {
                options[_rho_deferred_paramName].resolve([result, status, jqXHR]);
            }
        }
    }

    window._rho_ajaxProxyCallback = function(callbackId, result, headers, status, errCode) {
        var cbId = decodeURIComponent(callbackId);
        //console.log('_rho_ajaxProxyCallback: callback for: ' +cbId);
        //console.log('_rho_ajaxProxyCallback: result: ' +result);
        if (pendingCallbacks[cbId]) {
            //console.log('_rho_ajaxProxyCallback: callback found!');
            fireHandlers(pendingCallbacks[cbId], result,
                ("string" == typeof headers) ? JSON.parse(headers) : headers,
                status, errCode);
            delete pendingCallbacks[cbId];
        }
    };

    window._rho_execJsWrapper = function(expr) {
        eval(expr);
    };

})(jQuery);
