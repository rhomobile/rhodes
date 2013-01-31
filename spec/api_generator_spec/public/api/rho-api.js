var Rho = Rho || (function ($) {
    'use strict';

    // === API configuration ========================================================

    var thisFileName = 'rho-api.js';

    var apiParts = [
        "rho-api-barcode1.js"
    ];

    var RHO_ID_PARAM = '__rhoID';

    var API_CONTROLLER_URL = '/app/RhoJavascriptApi/command_handler';
    var API_CALLBACK_BASE_URL = '/app/RhoJavascriptApi/callback_handler';

    // === Private parts ============================================================

    var idCount = 0;
    var pendingCallbacks = {};

    function defaultEmptyCallback() {}

    function defaultId() { return 'DEFAULT_OBJECT_ID'; }

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

    function propsToHash(props) {
        var propHash = {};
        if ("string" == typeof props) {
            propHash[props] = null;
        } else if (props instanceof Array) {
            for (var i=0; i<props.length; i++) {
                propHash[props[i]] = null;
            }
        } else if (props instanceof Object) {
            propHash = props;
        }
    }

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

        var cmdText = $.toJSON(cmd);

        var result = null;
        var deferred =  new $.Deferred(function(dfr) {
            $.ajax({
                async: (null != valueCallback),
                type: 'get',
                url: API_CONTROLLER_URL,
                data:{data: cmdText},
                dataType: 'json',
                headers:{'Accept':'text/plain'}
            }).done(function (data) {
                result = jsValue(data);
                if (valueCallback) {
                    dfr.resolve(result);
                    valueCallback(result);
                }
            }).fail(function (xhr, status, message) {
                var errObj = null;
                try {
                    errObj = $.evalJSON(xhr.responseText);
                    if (errObj && "object" == typeof errObj && "string" == typeof errObj.error) {
                        message = errObj.error;
                    }
                } catch (ex) {};
                result = errObj;
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
            params.module = module;
            params.args = Array.prototype.slice.call(params.args);
            return commonReq(params);
        };
    }

    // === Factory handling =========================================================

    var objClasses = {};



    function objectForClass(className, id) {
        //var obj = null;
        //if ("function" == typeof objClasses[className]) {
        //    obj = new objClasses[className](id);
        //}
        return new Rho[className].constructor(id);
    }

    function defaultObjectForClass(className, id) {
        //var obj = null;
        //if ("function" == typeof objClasses[className]) {
        //    obj = new objClasses[className](id);
        //}
        return new Rho[className];
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


    function classTemplateFor(moduleName, typeName){
        var apiReq = apiReqFor(moduleName);

        objClasses[moduleName] = objClasses[moduleName] || {};

        var modClasses = objClasses[moduleName];

        if (modClasses[typeName]) {
            // return prototype if already defined;
            return modClasses[typeName];
        }

        // to don't force to use 'new'
        modClasses[typeName] = function(id){
            this.id = id;
        };

        modClasses[typeName].prototype.getId = function(){ return this.id; };

        modClasses[typeName].prototype.getProps = function(propHash, cb) {
            return apiReq(arguments, 'getProps', /* params number including optional callback */ 2, this.id);
        };

        modClasses[typeName].prototype.setProps = function(propHash, cb) {
            return apiReq(arguments, 'setProps', /* params number including optional callback */ 2, this.id);
        };

        return modClasses[typeName];
    }

    // === Common API implementation ================================================

    function loadCSS(url) {
        $('<link>').attr('rel', 'stylesheet').attr('href', url).appendTo('head');
    }

    function loadScript(url) {
        $('<script>').attr('type', 'text/javascript').attr('src', url).appendTo('head');
    }

    var thisFileURL = $("script[src$='" +thisFileName+ "']").attr('src');
    //
    function loadApiFile(fileName) {
        loadScript(thisFileURL.replace(thisFileName, fileName));
    }
    //
    function loadRestOfAPI() {
        $.each(apiParts, function (idx, fileName) {
            loadApiFile(fileName);
        });
    }

    // we should finish Rho definition first
    setTimeout(function() {
        loadRestOfAPI();
    }, 10);

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
        apiReqFor: apiReqFor,
        classTemplateFor: classTemplateFor,
        defaultId: defaultId,
        nextId: nextId
    };

    // === Public interface ==========================================================

    return {
        util: util,
        callback_handler: callbackHandler
    };

})(jQuery);
