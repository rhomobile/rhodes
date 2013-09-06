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

    var NOT_INSTANCE_ID = '0';

    // === Core parts ============================================================

    var idCount = 0;
    var pendingCallbacks = {};

    function getVmID() {
        return window['__rhoJsVmID'] || null;
    }

    function defaultEmptyCallback() {
    }

    function nextId(tag) {
        if ('undefined' == typeof tag || !tag)
            tag = RHO_ID_PARAM;

        return (tag + '#' + idCount++);
    }

    function prepareCallback(callback, /*opt*/ isPersistent, /*opt*/ id) {
        /*
        Rho.Log.info('prepareCallback.callback: type: ' + typeof callback,"JSC");
        Rho.Log.info('prepareCallback.callback: isPersistent: ' + isPersistent,"JSC");
        Rho.Log.info('prepareCallback.callback: id: ' + id,"JSC");
        */

        if ('string' == typeof callback)
            return callback;

        if ('function' != typeof callback)
            callback = defaultEmptyCallback;

        if ('undefined' == typeof id || !id)
            id = nextId();

        var data = {
            id: id,
            callback: callback,
            isPersistent: ('undefined' != typeof isPersistent) && isPersistent
        };

        pendingCallbacks[id] = data;

        /*
        var arr = [], p, i = 0;
        for (p in data) {
            arr[i++] = " " + p + " : " + data[p]+" ";
        }
        var str = arr.join(', ');

        Rho.Log.info('prepareCallback.callback: pendingCallbacks: {' + str + "}","JSC");
        */
        // store options for pending callback
        return API_CALLBACK_BASE_URL + id;
    }

    function scanForInstances(value) {
        for (var prop in value) {
            if (!value.hasOwnProperty(prop)) continue;
            if ('object' == typeof value[prop]) {
                value[prop] = createInstances(value[prop]);
            }
        }
        return value;
    }

    function createInstances(value) {
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

        return createInstances(value);
    }

    function namesToProps(names) {
        var namesObj = {};
        if ("string" == typeof names) {
            names = names.split(/[\s\,]/);
        }
        if (names instanceof Array) {
            for (var i = 0; i < names.length; i++) {
                if (0 < names[i].length)
                    namesObj[names[i]] = null;
            }
        } else if (names instanceof Object) {
            namesObj = names;
        }
        return namesObj;
    }

    function namesToArray(names) {
        var namesArray = [];
        if ("string" == typeof names) {
            names = names.split(/[\s\,]/);
        }
        if (names instanceof Array) {
            for (var i = 0; i < names.length; i++) {
                if (0 < names[i].length)
                    namesArray.push(names[i]);
            }
        } else if (names instanceof Object) {
            for (var name in names) {
                if (names.hasOwnProperty(name) && 0 < name.length)
                    namesArray.push(name);
            }
        }
        return namesArray;
    }

    var reqIdCount = 0;

    function wrapFunctions(key, value) {
        if ('function' != typeof value) return value;

        return RHO_CALLBACK_PARAM + ":" + prepareCallback(value, true);
    }

    function commonReq(params) {

        var valueCallback = null;

        if ("number" == typeof params.valueCallbackIndex) {
            if (params.valueCallbackIndex < params.args.length - 1)
                throw 'Generated API method error: wrong position for value callback argument!';

            if (params.valueCallbackIndex == params.args.length - 1)
                valueCallback = params.args.pop();

            if (valueCallback && "function" != typeof valueCallback)
                throw 'Value callback should be a function!';
        }

        var persistentCallback = null;
        var persistentCallbackOptParams = null;

        if ("number" == typeof params.persistentCallbackIndex) {
            if (params.persistentCallbackIndex < params.args.length - 2)
                throw 'Generated API method error: wrong position for persistent callback argument!';

            if (params.persistentCallbackIndex == params.args.length - 2) {
                persistentCallbackOptParams = params.args.pop();
                persistentCallback = params.args.pop();
            } else if (params.persistentCallbackIndex == params.args.length - 1) {
                persistentCallback = params.args.pop();
            }

            if (persistentCallback && 'function' != typeof persistentCallback)
                throw 'Persistent callback should be a function!';

            if (persistentCallbackOptParams && 'string' != typeof persistentCallbackOptParams)
                throw 'Persistent callback optional parameters should be a string!';

            var persistentCallbackOptParams = persistentCallbackOptParams || null;

            if (persistentCallback)
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

        var cmdText = JSON.stringify(cmd, wrapFunctions);
        //console.log(cmdText);

        var result = null;
        var deferred = new $.Deferred(function (dfr) {

            function handleResult(rawResult) {
                result = jsValue(rawResult);
                dfr.resolve(result);
                if (valueCallback) {
                    valueCallback(result);
                }
            }

            function handleError(errObject) {
                dfr.reject(errObject.message, errObject.code);
                throw errObject.message;
            }

            Rho.platform.nativeApiCall(cmdText, null != valueCallback, function (result) {
                if (result['error'])
                    handleError(result['error']);
                else
                    handleResult(result['result']);
            });

        }).promise();

        return (null != valueCallback) ? deferred : result;
    }

    function apiReqFor(module) {
        return function (params) {
            params.args = Array.prototype.slice.call(params.args);
            if ('getProperties' == params.method && 0 < params.args.length) {
                params.args[0] = namesToArray(params.args[0]);
            }
            params.module = module;
            params.method = params.method;
            return commonReq(params);
        };
    }

    function extendSafely(destination, source, override) {
        var src = source;
        var dst = destination;
        var sourceIsFunc = false;

        if ('function' == typeof src) {
            if ('function' == typeof dst && !override)
                throw "Namespace definition conflict!";

            sourceIsFunc = true;
            src = destination;
            dst = source;
        }

        for (var prop in src) {
            if (!src.hasOwnProperty(prop)) continue;
            if (dst.hasOwnProperty(prop) && !override) continue;
            if ('prototype' == prop) {
                if(sourceIsFunc) continue;

                if('object' != typeof dst[prop])
                    dst[prop] = {};
                for (var subProp in src[prop]) {
                    if (!src[prop].hasOwnProperty(subProp)) continue;
                    dst[prop][subProp] = src[prop][subProp];
                }
                continue;
            }
            dst[prop] = src[prop];
        }
        return dst;
    }

    function namespace(nsPathStr, membersObj, override) {
        membersObj = membersObj || {};

        var ns = window;
        var parts = nsPathStr.split(/[\:\.]/);
        var nsLog = '';

        for (var i = 0; i < parts.length; i++) {
            var nsProp = parts[i];
            nsLog = nsLog + (i == 0 ? '' : '.') + nsProp;

            var subNs = ns[nsProp];
            if (!(subNs instanceof Object || 'undefined' == typeof subNs)) {
                throw "Namespace " + nsLog + " is already defined and it isn't an object!";
            }

            if (i == parts.length - 1) {
                if (ns[nsProp])
                    ns[nsProp] = extendSafely(ns[nsProp], membersObj, override);
                else
                    ns[nsProp] = membersObj;
            }
            ns[nsProp] = ns[nsProp] || {};
            ns = ns[nsProp];
        }
        return ns;
    }

    // === Property proxy support ====================================================

    var propsSupport = {
        ffHackKeywords: false,
        ffHackMethod: false,
        js185: false
    };

    (function propertySupportCheck() {
        propsSupport.ffHackKeywords = (function supported_firefoxHack_keywords() {
            var testObj = {};
            var okGet = false;
            var okSet = false;
            try {
                testObj = {
                    get propGet() {
                        okGet = true;
                        return okGet;
                    },
                    set propSet(val) {
                        okSet = val;
                    }
                };
                testObj.propSet = testObj.propGet;
            } catch (ex) {};
            return okGet && okSet;
        })();

        propsSupport.ffHackMethod = (function supported_firefoxHack_method() {
            var testObj = {};
            var okGet = false;
            var okSet = false;
            try {
                testObj.__defineGetter__('propGet', function () {
                    okGet = true;
                    return okGet;
                });
                testObj.__defineSetter__('propSet', function (val) {
                    okSet = val;
                });

                testObj.propSet = testObj.propGet;
            } catch (ex) {};
            return okGet && okSet;
        })();

        propsSupport.js185 = (function supported_js185_standard() {
            var testObj = {};
            var okGet = false;
            var okSet = false;
            try {
                Object.defineProperty(testObj, 'propGet', {
                    get: function () {
                        okGet = true;
                        return okGet;
                    }
                });
                Object.defineProperty(testObj, 'propSet', {
                    set: function (val) {
                        okSet = val;
                    }
                });
                testObj.propSet = testObj.propGet;
            } catch (ex) {};
            return okGet && okSet;
        })();
    })();
    // at this point we have property support level already detected


    function propAccessReqFunc(apiReqFunc, propName, rw, idFunc, customFunc) {
        var isSet = ('w' == rw);

        var propNameParts = propName.split(':');

        propName = propNameParts[0];
        var methodGet = propName;
        var methodSet = propName + '=';

        if (2 < propNameParts.length)
            methodSet = propNameParts[2];

        if (1 < propNameParts.length)
            methodGet = propNameParts[1];

        return function () {
            try {
                if ('function' == typeof customFunc)
                    return customFunc.apply(this, arguments);
            } catch(ex) {
                throw "Custom accessor function exception: " + ex;
            }

            return apiReqFunc({
                instanceId: ('function' == typeof idFunc) ? idFunc.apply(this, []) : NOT_INSTANCE_ID,
                args: arguments,
                method: isSet ? methodSet : methodGet,
                valueCallbackIndex: (isSet ? 1 : 0)
            });
        };
    }

    // Here is default (fallback option) implementation of property using explicit accessors.
    // It will be used in case we have no any support for natural props syntax in a browser.
    // Usage sample: obj.setSomething(123), var abc = obj.getSomething()
    // ====================================================================================
    var createPropProxy_fallback = function (obj, propDescr, propAccess, apiReqFunc, idFunc, customGet, customSet) {
        var propName = propDescr.split(':')[0];

        function accessorName(accessor, propDescr) {
            var names = propDescr.split(':');
            var propName = names[0];
            var getterName = names[1];
            var setterName = names[2];

            if (('get' == accessor) && getterName)
                return getterName;

            if (('set' == accessor) && setterName)
                return setterName;

            return accessor + propName.charAt(0).toUpperCase() + propName.slice(1);
        }

        if (0 <= propAccess.indexOf('w')) {
            obj[accessorName('set', propDescr)] = propAccessReqFunc(apiReqFunc, propDescr, 'w', idFunc, customSet);
        }
        if (0 <= propAccess.indexOf('r')) {
            obj[accessorName('get', propDescr)] = propAccessReqFunc(apiReqFunc, propDescr, 'r', idFunc, customGet);
        }
    };

    var createPropProxy = createPropProxy_fallback;

    if (propsSupport.js185) {
        // the best case, js185 props are supported
        createPropProxy = function (obj, propDescr, propAccess, apiReqFunc, idFunc, customGet, customSet) {
            var propName = propDescr.split(':')[0];

            // TODO: implement enumeration of propProxy to extend target namespace with them.
            var js185PropDef = {configurable: true, enumerable: false /* true */};
            if (0 <= propAccess.indexOf('r')) {
                js185PropDef['get'] = propAccessReqFunc(apiReqFunc, propDescr, 'r', idFunc, customGet);
            }
            if (0 <= propAccess.indexOf('w')) {
                js185PropDef['set'] = propAccessReqFunc(apiReqFunc, propDescr, 'w', idFunc, customSet);
            }
            Object.defineProperty(obj, propName, js185PropDef);
        };
    } else if (propsSupport.ffHackMethod) {
        // backup option, props are supported with firefox hack
        createPropProxy = function (obj, propDescr, propAccess, apiReqFunc, idFunc, customGet, customSet) {
            var propName = propDescr.split(':')[0];

            obj.__defineGetter__(propName, propAccessReqFunc(apiReqFunc, propDescr, 'r', idFunc, customGet));
            obj.__defineSetter__(propName, propAccessReqFunc(apiReqFunc, propDescr, 'w', idFunc, customSet));
        };
    } else {
        // Sorry, no luck. We able provide just a default implementation with explicit accessors.
        // It is the best thing we can do in this case.
    }

    var incompatibleProps = [];

    // Properties bulk definition.
    // Sample:
    //
    //    Rho.util.createPropsProxy(Application, {
    //        'publicFolder': 'r',
    //        'startURI': 'rw',
    //        'version': 'r',
    //        'title': 'rw'
    //    }, apiReq);
    //
    function createPropsProxy(obj, propDefs, apiReq, idFunc) {
        if (!(propDefs instanceof Array))
            throw 'Property definitions list should be Array instance';

        for (var i=0; i<propDefs.length; i++) {
            var propDef = propDefs[i];

            try {
                createPropProxy(obj, propDef['propName'], propDef['propAccess'], apiReq, idFunc, propDef['customGet'], propDef['customSet']);
            } catch (ex) {
                // we unable to create property with this name, so log it
                incompatibleProps.push(name);
            }
            // create explicit accessors
            createPropProxy_fallback(obj, propDef['propName'], propDef['propAccess'], apiReq, idFunc, propDef['customGet'], propDef['customSet']);
        }
    }

    function methodAccessReqFunc(nativeName, persistentCallbackIndex, valueCallbackIndex, apiReq, idFunc) {
        return function() {
            return apiReq({
                instanceId: ('function' == typeof idFunc) ? idFunc.apply(this, []) : NOT_INSTANCE_ID,
                args: arguments,
                method: nativeName,
                persistentCallbackIndex: persistentCallbackIndex,
                valueCallbackIndex: valueCallbackIndex
            });
        }
    }

    function createMethodsProxy(obj, methodDefs, apiReq, idFunc) {
        if (!(methodDefs instanceof Array))
            throw 'Property definitions list should be Array instance';
        
        for (var i=0; i<methodDefs.length; i++) {
            var methodDef = methodDefs[i];
            try {
                obj[methodDef['methodName']] = methodAccessReqFunc(
                    methodDef['nativeName'],
                    methodDef['persistentCallbackIndex'],
                    methodDef['valueCallbackIndex'],
                    apiReq, idFunc
                );
            } catch (ex) {
                // we unable to create property with this name, so log it..
                incompatibleProps.push(methodDef['methodName']);
            }
        }
    }

    function namespaceAlias(ns, parent, alias) {
        if (!parent) throw 'No parent namespace for alias!';
        if (parent[alias]) throw 'Alias definition conflict!';

        parent[alias] = ns;
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

    var thisFileURL = $("script[src$='" + thisFileName + "']").attr('src');
    //
    function loadApiModule(moduleName) {
        loadScript(thisFileURL.replace(thisFileName, moduleName + '.js'));
    }

    //
    function loadApiModules(parts) {
        for (var i = 0; i < parts.length; i++) {
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
                var result = null;
                var err = null;
                if (resultObj) {
                    result = jsValue(resultObj['result']);
                    err = resultObj['error'];
                }
                opts.callback(result, err);
            }

            if (!opts.isPersistent)
                delete pendingCallbacks[cbId];
        }
    };

    // === Native API call ===========================================================

    function nativeApiCall_ajax(cmdText, async, resultHandler) {
        $.ajax({
            async: async,
            type: 'post',
            url: API_CONTROLLER_URL,
            data: cmdText,
            dataType: 'json',
            headers: {'Accept': 'text/plain'}
        }).done(function (data) {
            resultHandler(data);
        }).fail(function (xhr, status, message) {
            resultHandler({error: {message: message, code: xhr.statusCode()}});
        });
    }

    // === Old API support ===========================================================

    function importOldApiTo(namespace) {
        // move non-conflicting modules from old js api to this one
        if ('undefined' != typeof window.RhoOld) {
            if ('object' == typeof window.RhoOld) {
                for (var prop in window.RhoOld) {
                    if (window.RhoOld.hasOwnProperty(prop)
                        && 'undefined' != typeof window.RhoOld[prop]
                        && 'undefined' == typeof namespace[prop]) {

                        namespace[prop] = window.RhoOld[prop];
                    }
                }
            }
            //delete window.RhoOld;
            window.RhoOld = undefined;
        }
        return namespace;
    }

    // === Utility internal methods ==================================================

    var util = {
        flag: {
            USE_AJAX_BRIDGE: '__rho_useAjaxBridge',
            NATIVE_BRIDGE_TYPE: '__rho_nativeBridgeType'
        },
        loadApiModules: loadApiModules,
        namespace: namespace,
        namespaceAlias: namespaceAlias,
        apiReqFor: apiReqFor,
        namesToProps: namesToProps,
        namesToArray: namesToArray,
        createPropsProxy: createPropsProxy,
        createMethodsProxy: createMethodsProxy,
        incompatibleProps: incompatibleProps,
        rhoIdParam: function () { return RHO_ID_PARAM },
        rhoClassParam: function () { return RHO_CLASS_PARAM },
        nextId: nextId
    };

    var platform = {
        id: {
            AJAX: 'ajax',
            AUTO: 'auto',
            RHOSIMLATOR: 'rhosimulator',
            ANDROID: 'android',
            IPHONE:  'iphone',
            WP8: 'wp8',
            WM: 'wm',
            WIN32: 'win32'
        },
        nativeApiCall: nativeApiCall_ajax,
        nativeApiResult: function(){/* intentionally empty stub function */}
    }

    // === Public interface ==========================================================

    return importOldApiTo({
            jQuery: $,
            util: util,
            platform: platform,
            callbackHandler: callbackHandler
    });

})(jQuery.noConflict(true));
