var Rho = Rho || (function ($) {
    'use strict';

    // === API configuration ========================================================

    var thisFileName = 'rhoapi.js';

    var RHO_ID_PARAM = '__rhoID';

    var API_CONTROLLER_URL = '/app/RhoJavascriptApi/command_handler';
    var API_CALLBACK_BASE_URL = '/app/RhoJavascriptApi/callback_handler';

    // === Private parts ============================================================

    var idCount = 0;
    var pendingCallbacks = {};

    function defaultEmptyCallback() {}

    function nextId(tag) {
        if ('undefined' == typeof tag || !tag)
            tag = RHO_ID_PARAM;

        return (tag + idCount++);
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
        return API_CALLBACK_BASE_URL+id;
    }

    function jsValue(result) {
        var value = null;

        switch(result['type']) {
            case 'instance':
                if (result['class']) {
                    var ids = result.value.split(' ');
                    if (1 == ids.length) {
                        value = objectForClass(result['class'], ids[0]);
                    } else {
                        value = [];
                        for (var i=0; i<ids.length; i++)
                            value.push(objectForClass(result['class'], ids[i]));
                    }
                }
                break;
            case 'default':
                if (result['class']) {
                    value = defaultObjectForClass(result['class'], result.value);
                }
                break;
            default:
                value = result.value;
        }
        return value;
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
                throw 'Value callback should be a last passed argument!';

            valueCallback = params.args.pop();

            if (valueCallback && "function" != typeof valueCallback)
                throw 'Value callback should be a function!';
        }

        var cmd = { module:params.module, method:params.method, args:params.args };

        if ("number" == typeof params.callbackIndex) {
            cmd['callback_index'] = params.callbackIndex;
            cmd['args'][params.callbackIndex] = prepareCallback(params.args[params.callbackIndex]);
        }
        cmd['jsonrpc'] = '2.0';
        cmd['id'] = reqIdCount++;

        var cmdText = $.toJSON(cmd);
        console.log(cmdText);

        var result = null;
        var deferred =  new $.Deferred(function(dfr) {
            $.ajax({
                async: (null != valueCallback),
                type: 'post',
                url: API_CONTROLLER_URL,
                data:{data: cmdText},
                dataType: 'json',
                headers:{'Accept':'text/plain'}
            }).done(function (data) {
                result = jsValue(data['result']);
                if (valueCallback) {
                    dfr.resolve(result);
                    valueCallback(result);
                }
            }).fail(function (xhr, status, message) {
                var errObj = null;
                try {
                    errObj = $.evalJSON(xhr.responseText);
                    message = errObj.error.message;
                } catch (ex) {};
                result = errObj.error;
                if (valueCallback) {
                    dfr.reject(message, result);
                    valueCallback(result);
                }
            });
        }).promise();

        return (null != valueCallback) ? deferred : result;
    }

    function apiReqFor(module) {
        return function(params) {
            params.args = Array.prototype.slice.call(params.args);
            if ('getProps' == params.method && 0 < params.args.length) {
                params.args[0] = namesToProps(params.args[0]);
            }
            params.method = module +'.'+ params.method;
            return commonReq(params);
        };
    }

    function extendSafely(dst, src) {
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
                    extendSafely(ns[nsProp], membersObj);
                else
                    ns[nsProp] = membersObj;
            }
            ns[nsProp] = ns[nsProp] || {};
            ns = ns[nsProp];
        }
        return ns;
    }

    // === Factory handling =========================================================

    var objClasses = {};

    function objectForClass(className, id) {
        //var obj = null;
        //if ("function" == typeof objClasses[className]) {
        //    obj = new objClasses[className](id);
        //}
        return namespace(className)(id);
    }

    function off_objectForClass(className, id) {
        var obj = null;
        if ("function" == typeof objClasses[className]) {
            obj = new objClasses[className](id);
        }
        return obj;
    }

    function off_defaultObjectForClass(className, id) {
        var obj = null;
        if ("function" == typeof objClasses[className]) {
            obj = new objClasses[className](id);
        }
        return obj;
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
        setTimeout(function() {
            $.each(parts, function (idx, part) {
                loadApiModule(part);
            });
        }, 10);
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
        nextId: nextId
    };

    // === Public interface ==========================================================

    return {
        loadApiModules: loadApiModules,
        util: util,
        callback_handler: callbackHandler
    };

})(jQuery);
