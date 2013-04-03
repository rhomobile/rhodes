var Rho = Rho || (function ($) {
    'use strict';

    // === API configuration ========================================================

    var thisFileName = 'rhoapi.js';

    var RHO_ID_PARAM = '__rhoID';
    var RHO_CLASS_PARAM = '__rhoClass';
    var RHO_CALLBACK_PARAM = '__rhoCallback';

    var API_CONTROLLER_URL = '/system/js_api_entrypoint';
    //var API_CALLBACK_BASE_URL = '/system/js_api_entrypoint';
    var API_CALLBACK_BASE_URL = '';

    // === Private parts ============================================================

    var idCount = 0;
    var pendingCallbacks = {};

    function getVmID() {
        return window['__rhoJsVmID'] || null;
    }

    function defaultEmptyCallback() {}

    function nextId(tag) {
        if ('undefined' == typeof tag || !tag)
            tag = RHO_ID_PARAM;

        return (tag +'#'+ idCount++);
    }

    function prepareCallback(callback, /*opt*/ isPersistent, /*opt*/ id) {
        if ('string' == typeof callback)
            return callback;

        if ('function' != typeof callback)
            callback = defaultEmptyCallback;

        if ('undefined' == typeof id || !id)
            id = nextId();

        pendingCallbacks[id] = {
            id: id,
            callback: callback,
            isPersistent: ('undefined' != typeof isPersistent) && isPersistent
        };
        // store options for pending callback
        return API_CALLBACK_BASE_URL + id;
    }

    function scanForInstances(value) {
        for (var prop in value) {
            if (!value.hasOwnProperty(prop)) continue;
            if ('object' == typeof value[prop]) {
                value[prop] = createInstance(value[prop]);
            }
        }
        return value;
    }

    function createInstance(value) {
        if ('object' == typeof value) {
            if (value[RHO_ID_PARAM] && value[RHO_CLASS_PARAM]) {
                return objectForClass(value[RHO_CLASS_PARAM], value[RHO_ID_PARAM]);
            } else {
                return scanForInstances(value);
            }
        }
        return value;
    }

    function jsValue(result) {

        if ('undefined' == typeof result)
            throw 'Invalid API JSON response';

        if (null == result || 'object' != typeof result)
            return result;

        var value = $.extend(result instanceof Array ? [] : {}, result);

        return scanForInstances(value);
    }

    function namesToProps(names) {
        var propHash = {};
        if ("string" == typeof names) {
            names = names.split(/[\s\,]/);
        }
        if (names instanceof Array) {
            for (var i=0; i<names.length; i++) {
                propHash[names[i]] = null;
            }
        } else if (names instanceof Object) {
            propHash = names;
        }
    }

    var reqIdCount = 0;
    function commonReq(params) {

        var valueCallback = null;

        if ("number" == typeof params.valueCallbackIndex) {
            if (params.valueCallbackIndex < params.args.length-1)
                throw 'Generated API method error: wrong position for value callback argument!';

            if (params.valueCallbackIndex == params.args.length-1)
                valueCallback = params.args.pop();

            if (valueCallback && "function" != typeof valueCallback)
                throw 'Value callback should be a function!';
        }

        var persistentCallback = null;
        var persistentCallbackOptParams = null;

        if ("number" == typeof params.persistentCallbackIndex) {
            if (params.persistentCallbackIndex < params.args.length-2
                || params.persistentCallbackIndex >= params.args.length)
                throw 'Generated API method error: wrong position for persistent callback argument!';

            if (params.persistentCallbackIndex == params.args.length-2) {
                persistentCallbackOptParams = params.args.pop();
                persistentCallback = params.args.pop();
            } else {
                persistentCallback = params.args.pop();
            }

            if (persistentCallback && 'function' != typeof persistentCallback)
                throw 'Persistent callback should be a function!';

            if (persistentCallbackOptParams && 'string' != typeof persistentCallbackOptParams)
                throw 'Persistent callback optional parameters should be a string!';
            
            var persistentCallbackOptParams = persistentCallbackOptParams || null;

            persistentCallback = prepareCallback(persistentCallback, true);
        }

        var cmd = { 'method': params.method, 'params': params.args };

        cmd[RHO_CLASS_PARAM] = params.module;
        cmd[RHO_ID_PARAM] = params.instanceId || null;
        cmd['jsonrpc'] = '2.0';
        cmd['id'] = reqIdCount++;

        if (persistentCallback) {
            cmd[RHO_CALLBACK_PARAM] = {
                id: persistentCallback,
                vmID: getVmID(),
                optParams: persistentCallbackOptParams
            };
        }

        var cmdText = $.toJSON(cmd);
        console.log(cmdText);

        var result = null;
        var deferred =  new $.Deferred(function(dfr) {
            function handleError(errObject) {
                dfr.reject(errObject.message, errObject.code);
                throw errObject.message;
            }

            $.ajax({
                async: (null != valueCallback),
                type: 'post',
                url: API_CONTROLLER_URL,
                data: cmdText,
                dataType: 'json',
                headers:{'Accept':'text/plain'}
            }).done(function (data) {
                if (data['error']) {
                    handleError(data['error']);
                } else {
                    result = jsValue(data['result']);
                    dfr.resolve(result);
                    if (valueCallback) {
                        valueCallback(result);
                    }
                }
            }).fail(function (xhr, status, message) {
                handleError({message: message, code: xhr.statusCode()});
            });
        }).promise();

        return (null != valueCallback) ? deferred : result;
    }

    function apiReqFor(module) {
        return function(params) {
            params.args = Array.prototype.slice.call(params.args);
            if ('getProperties' == params.method && 0 < params.args.length) {
                params.args[0] = namesToProps(params.args[0]);
            }
            params.module = module;
            params.method = params.method;
            return commonReq(params);
        };
    }

    function extendSafely(destination, source) {
        var src = source;
        var dst = destination;
        if ('function' == typeof src) {
            if ('function' == typeof dst)
                throw "Namespace definition conflict!";

            src = destination;
            dst = source;
        }
        for (var prop in src) {
            if (dst.hasOwnProperty(prop))
                continue;
            if (src.hasOwnProperty(prop))
                dst[prop] = src[prop];
        }
        return dst;
    }

    function namespace(nsPathStr, membersObj) {
        membersObj = membersObj || {};

        var ns = window;
        var parts = nsPathStr.split(/[\:\.]/);
        var nsLog = '';

        for (var i=0; i<parts.length; i++) {
            var nsProp = parts[i]; 
            nsLog = nsLog +(i==0 ? '' : '.')+ nsProp;

            var subNs = ns[nsProp];
            if (!(subNs instanceof Object || 'undefined' == typeof subNs)) {
                throw "Namespace "+ nsLog +" is already defined and it isn't an object!";
            }

            if(i == parts.length-1) {
                if (ns[nsProp])
                    ns[nsProp] = extendSafely(ns[nsProp], membersObj);
                else
                    ns[nsProp] = membersObj;
            }
            ns[nsProp] = ns[nsProp] || {};
            ns = ns[nsProp];
        }
        return ns;
    }

    // === Factory handling =========================================================

    function objectForClass(className, id) {
        var instObject = {};
        instObject[RHO_CLASS_PARAM] = className;
        instObject[RHO_ID_PARAM] = id;
        return new (namespace(className))(instObject);
    }

    // === Modules loading implementation ============================================

    function loadCSS(url) {
        $('<link>').attr('rel', 'stylesheet').attr('href', url).appendTo('head');
    }

    function loadScript(url) {
        $('<script>').attr('type', 'text/javascript').attr('src', url).appendTo('head');
    }

    var thisFileURL = $("script[src$='" +thisFileName+ "']").attr('src');
    //
    function loadApiModule(moduleName) {
        loadScript(thisFileURL.replace(thisFileName, moduleName+'.js'));
    }
    //
    function loadApiModules(parts) {
        for(var i=0; i<parts.length; i++) {
            loadApiModule(parts[i]);
        }
    }

    // === Callback handler ==========================================================

    function callbackHandler(callbackId, resultObj) {
        var cbId = decodeURIComponent(callbackId);
        //console.log('Rho.callback_handler: callback for: ' +cbId);
        //console.log('Rho.callback_handler: resultObj: ' +resultObj);

        var opts = pendingCallbacks[cbId];

        if ('object' == typeof opts && opts) {
            //console.log('Rho.callback_handler: callback found!');

            if ('function' == typeof opts.callback) {
                var result = {};
                $.each(resultObj, function(name, value) {
                    try {
                        if (value.match(/^(true|false)$/i))
                        value = Boolean(value);
                        // TODO: at this moment only integer numbers w/o leading zeros
                        // are converted automatically
                        else if (value.match(/^([1-9][0-9]*|0)$/))
                            value =  Number(value);
                        result[name] = value;
                    } catch(ex) {}
                });
                //console.log(resultObj);
                //console.dir(result);
                opts.callback(result);
            }

            if (!opts.isPersistent)
                delete pendingCallbacks[cbId];
        }
    };

    // === Utility internal methods ==================================================

    var util = {
        namespace: namespace,
        apiReqFor: apiReqFor,
        namesToProps: namesToProps,
        rhoIdParam: function(){return RHO_ID_PARAM},
        rhoClassParam: function(){return RHO_CLASS_PARAM},
        nextId: nextId
    };

    // === Public interface ==========================================================

    return {
        loadApiModules: loadApiModules,
        util: util,
        callbackHandler: callbackHandler
    };

})(jQuery);
