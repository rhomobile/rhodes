//console.log('@@@@@@@@@@@@@@@@@@   start eval Rho');



global.RhomobileEngine = {}

function initializeRhoAPI() {

    //'use strict';

    // === API configuration ========================================================

    var thisFileName = 'rhoapi.js';

    var RHO_ID_PARAM = '__rhoID';
    var RHO_CLASS_PARAM = '__rhoClass';
    var RHO_CALLBACK_PARAM = '__rhoCallback';

    var API_CONTROLLER_URL = '/system/js_api_entrypoint';
    //var API_CALLBACK_BASE_URL = '/system/js_api_entrypoint';
    var API_CALLBACK_BASE_URL = '';

    var NOT_INSTANCE_ID = '0';


    ////console.log("***************************");

    // === simplified jQuery 1.9.1 parts =============================================

    var __$$ = {
        isFunction: function (obj) {
            return 'function' === typeof obj;
        },

        isPlainObject: function (obj) {
            return obj && 'object' === typeof obj;
        },

        isArray: function (obj) {
            return (obj && 'Array' == (obj).constructor.name);
        },

        extend: function () {
            var src, copyIsArray, copy, name, options, clone,
                target = arguments[0] || {},
                i = 1,
                length = arguments.length,
                deep = false;

            // Handle a deep copy situation
            if (typeof target === "boolean") {
                deep = target;

                // skip the boolean and the target
                target = arguments[ i ] || {};
                i++;
            }

            // Handle case when target is a string or something (possible in deep copy)
            if (typeof target !== "object" && !__$$.isFunction(target)) {
                target = {};
            }

            // extend jQuery itself if only one argument is passed
            if (i === length) {
                target = this;
                i--;
            }

            for (; i < length; i++) {
                // Only deal with non-null/undefined values
                if ((options = arguments[ i ]) != null) {
                    // Extend the base object
                    for (name in options) {
                        src = target[ name ];
                        copy = options[ name ];

                        // Prevent never-ending loop
                        if (target === copy) {
                            continue;
                        }

                        // Recurse if we're merging plain objects or arrays
                        if (deep && copy && ( __$$.isPlainObject(copy) || (copyIsArray = __$$.isArray(copy)) )) {
                            if (copyIsArray) {
                                copyIsArray = false;
                                clone = src && __$$.isArray(src) ? src : [];

                            } else {
                                clone = src && __$$.isPlainObject(src) ? src : {};
                            }

                            // Never move original objects, clone them
                            target[ name ] = __$$.extend(deep, clone, copy);

                            // Don't bring in undefined values
                        } else if (copy !== undefined) {
                            target[ name ] = copy;
                        }
                    }
                }
            }

            // Return the modified object
            return target;
        },

        ajax: function(opts) {

        },
        each:function (anObjectOrArray, callback) {
            if (this.isArray(anObjectOrArray)) {
                anObjectOrArray.forEach(function (each, idx) {
                    callback(idx, each);
                });
            }
            else {
                for (var property in anObjectOrArray) {
                    if (anObjectOrArray.hasOwnProperty(property)) {
                        callback(property, anObjectOrArray[property]);
                    }
                }
            }
        }
    };


    // === Core parts ============================================================
    ////console.log("*************************** 2");
    var baseCallbackId = new Date().valueOf();
    var idCount = 0;
    var pendingCallbacks = {};


    ////console.log("*************************** 3");

    function getVmID() {
        return 'jxcore';
    }

    function defaultEmptyCallback() {
    }

    function nextId(tag) {
        if ('undefined' == typeof tag || !tag)
            tag = RHO_ID_PARAM;

        return (tag + '#' + baseCallbackId + idCount++);
    }

    function toBool(value) {
        if (value && 'string' == typeof value) {
            return (value.toLowerCase() == "true")
        }
        return false;
    }
    ////console.log("*************************** 31");

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
    ////console.log("*************************** 32");

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
        if ((value != null) && ('object' == typeof value)) {
            if (value[RHO_ID_PARAM] && value[RHO_CLASS_PARAM]) {
                return objectForClass(value[RHO_CLASS_PARAM], value[RHO_ID_PARAM]);
            } else {
                return scanForInstances(value);
            }
        }
        return value;
    }
    ////console.log("*************************** 33");

    function jsValue(result) {

        ////console.log("&&&&&& 1");
        if ('undefined' == typeof result)
            throw 'Invalid API JSON response';
        ////console.log("&&&&&& 2");

        if (null == result || 'object' != typeof result) {
            ////console.log("&&&&&& 3");
            return result;
        }
        ////console.log("&&&&&& 4");

        var value = __$$.extend(result instanceof Array ? [] : {}, result);
        ////console.log("&&&&&& 5");

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

    ////console.log("*************************** 34");


    var reqIdCount = 0;

    function wrapFunctions(key, value) {
        if ('function' != typeof value) return value;

        return RHO_CALLBACK_PARAM + ":" + prepareCallback(value, true);
    }


    ////console.log("*************************** 35");


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
        //////console.log(cmdText);

        var result = null;

        function handleResult(rawResult) {
            ////console.log("===== A");
            result = jsValue(rawResult);
            ////console.log("===== B");
            if (valueCallback) {
                ////console.log("===== C");
                valueCallback(result);
            }
            ////console.log("===== D");
        }

        function handleError(errObject) {
            throw errObject.message;
        }
        ////console.log("*************************** 5 a");

        ////console.log("*************************** Rho.platform.nativeApiCall = "+ typeof Rho.platform.nativeApiCall);


        Rho.platform.nativeApiCall(cmdText, null != valueCallback, function (result) {
            ////console.log("===== 1");
            if (result['error']) {
                ////console.log("===== 2");
                handleError(result['error']);
                ////console.log("===== 3");
            }
            else {
                ////console.log("===== 4");
                handleResult(result['result']);
                ////console.log("===== 5");
            }
        });
        ////console.log("*************************** 5 b");

        return (null != valueCallback) ? null : result;
    }

    ////console.log("*************************** 6");


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


    ////console.log("*************************** 7");


    function namespace(nsPathStr, membersObj, override) {

        ////console.log("------ 1");
        membersObj = membersObj || {};
        //console.log("------ 2");

        var ns = global;
        var parts = nsPathStr.split(/[\:\.]/);
        var nsLog = '';
        //console.log("------ 3");

        for (var i = 0; i < parts.length; i++) {
            var nsProp = parts[i];
            nsLog = nsLog + (i == 0 ? '' : '.') + nsProp;
            //console.log("------  4");

            var subNs = ns[nsProp];
            if (!(subNs instanceof Object || 'undefined' == typeof subNs)) {
                throw "Namespace " + nsLog + " is already defined and it isn't an object!";
            }
            //console.log("------  5");

            if (i == parts.length - 1) {
                if (ns[nsProp])
                    ns[nsProp] = extendSafely(ns[nsProp], membersObj, override);
                else
                    ns[nsProp] = membersObj;
            }
            //console.log("------  6");
            ns[nsProp] = ns[nsProp] || {};
            ns = ns[nsProp];
            //console.log("------  7");
        }
        //console.log("------  8");
        return ns;
    }


    //console.log("*************************** 8");

    // === Property proxy support ====================================================

    var propsSupport = {
        ffHackKeywords: false,
        ffHackMethod: false,
        js185: false
    };

    (function propertySupportCheck() {
/*        propsSupport.ffHackKeywords = (function supported_firefoxHack_keywords() {
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
        })();*/

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

    // Here is default (fallback option) implementation of property using explicit accessors.
    // It will be used in case we have no any support for natural props syntax in a browser.
    // Usage sample: obj.setSomething(123), var abc = obj.getSomething()
    // ====================================================================================
    var createPropProxy_fallback = function (obj, propDescr, getter, setter) {
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

        if (null != getter && 'function' == typeof getter) {
            obj[accessorName('get', propDescr)] = getter;
        }
        if (null != setter && 'function' == typeof setter) {
            obj[accessorName('set', propDescr)] = setter;
        }
    };

    //console.log("*************************** 9");


    var createPropProxy = createPropProxy_fallback;

    if (propsSupport.js185) {
        // the best case, js185 props are supported
        createPropProxy = function (obj, propDescr, getter, setter) {
            var propName = propDescr.split(':')[0];

            // TODO: implement enumeration of propProxy to extend target namespace with them.
            var js185PropDef = {configurable: true, enumerable: false /* true */};
            if (null != getter && 'function' == typeof getter) {
                js185PropDef['get'] = getter;
            }
            if (null != setter && 'function' == typeof setter) {
                js185PropDef['set'] = setter;
            }
            Object.defineProperty(obj, propName, js185PropDef);
        };
    } else if (propsSupport.ffHackMethod) {
        // backup option, props are supported with firefox hack
        createPropProxy = function (obj, propDescr, getter, setter) {
            var propName = propDescr.split(':')[0];

            obj.__defineGetter__(propName, getter);
            obj.__defineSetter__(propName, setter);
        };
    } else {
        // Sorry, no luck. We able provide just a default implementation with explicit accessors.
        // It is the best thing we can do in this case.
    }

    // ====================================================================================
    //console.log("*************************** 10");

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
            var propAccess = propDef['propAccess'];

            var getter = (0 <= propAccess.indexOf('r')) ? propAccessReqFunc(apiReq, propDef['propName'], 'r', idFunc, propDef['customGet']) : null;
            var setter = (0 <= propAccess.indexOf('w')) ? propAccessReqFunc(apiReq, propDef['propName'], 'w', idFunc, propDef['customSet']) : null;

            try {
                createPropProxy(obj, propDef['propName'], getter, setter);
            } catch (ex) {
                // we unable to create property with this name, so log it
                incompatibleProps.push(name);
            }
            // create explicit accessors
            createPropProxy_fallback(obj, propDef['propName'], getter, setter);
        }
    }

    //console.log("*************************** 11");

    // entity property support =======================================================

    function createRawPropsProxy(obj, propDefs, force_fallback ) {
        if (!(propDefs instanceof Array))
            throw 'Property definitions list should be Array instance';

        if (createPropProxy != createPropProxy_fallback || true == force_fallback) {
            for (var i=0; i<propDefs.length; i++) {
                var propDef = propDefs[i];

                try {
                    createPropProxy(obj, propDef['propName'], propDef['propGetter'], propDef['propSetter']);
                } catch (ex) {
                    // we unable to create property with this name, so log it
                    incompatibleProps.push(name);
                }
            }
        }
    }

    // === Method calls =========================================================

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

    //console.log("*************************** 12");

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

    function appendDomElement(target, tagName, attributes) {
        var elm = document.createElement(tagName);
        for (var name in attributes) {
            if (!attributes.hasOwnProperty(name)) continue;
            elm.setAttribute(name, attributes[name]);
        }
        target.appendChild(elm);
    }



    function loadApiModules(moduleNames) {
    }

    // === Callback handler ==========================================================

    function callbackHandler(callbackId, resultObj) {
        var cbId = decodeURIComponent(callbackId);
        ////console.log('Rho.callback_handler: callback for: ' +cbId);
        ////console.log('Rho.callback_handler: resultObj: ' +resultObj);

        var opts = pendingCallbacks[cbId];

        if ('object' == typeof opts && opts) {
            ////console.log('Rho.callback_handler: callback found!');

            if ('function' == typeof opts.callback) {
                var result = null;
                var err = null;
                if (resultObj) {
                    err = resultObj['error'];
                    if (!err)
                        result = jsValue(resultObj['result']);
                }
                opts.callback(result, err);
            }

            if (!opts.isPersistent)
                delete pendingCallbacks[cbId];
        }
    };

    // === Utility internal methods ==================================================
    //console.log("*************************** 12");

    var rhoUtil = {
        flag: {
            API_AJAX_URL: '__rho_apiAjaxURL',
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
        createRawPropsProxy: createRawPropsProxy,
        createMethodsProxy: createMethodsProxy,
        methodAccessReqFunc: methodAccessReqFunc,
        incompatibleProps: incompatibleProps,
        rhoIdParam: function () { return RHO_ID_PARAM },
        rhoClassParam: function () { return RHO_CLASS_PARAM },
        nextId: nextId
    };

    var rhoPlatform = {
        id: {
            AJAX: 'ajax',
            AUTO: 'auto',
            RHOSIMULATOR: 'rhosimulator',
            ANDROID: 'android',
            IPHONE:  'iphone',
            WP8: 'wp8',
            WM: 'wm',
            WIN32: 'win32',
            JXCORE: 'jxcore'
        },
        nativeApiCall: "",
        nativeApiResult: function(){/* intentionally empty stub function */}
    }

    // === Public interface ==========================================================
    //console.log("*************************** 13");

    var rho = {
        jQuery: __$$,
        util: rhoUtil,
        platform: rhoPlatform,
        callbackHandler: callbackHandler
    };

    // === js-to-native bridges ======================================================

    /* ========================================================================
     You can force bridge type will be used in a following ways:

     window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = rhoPlatform.id.ANDROID;
     window[rhoUtil.flag.NATIVE_BRIDGE_TYPE] = 'android';
     window['__rho_nativeBridgeType'] = 'android';

     Feel free to define it at any moment.
     ======================================================================== */

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
    addBridge(rhoPlatform.id.JXCORE, function() {
        return {
            apiCall: function (cmdText, async, resultHandler) {
                //console.log("+++ 1");
                var nativeApiResult = {};
                //console.log("+++ 2");
                global.RhomobileEngine.tau_js_entry_point_result = null;
                //console.log("+++ 3");
                process.natives.js_entry_point(cmdText);
                //console.log("+++ 4");
                if (global.RhomobileEngine.tau_js_entry_point_result != null) {
                    //console.log("+++ 5");
                    nativeApiResult = global.RhomobileEngine.tau_js_entry_point_result;
                    //console.log("+++ 6");
                    global.RhomobileEngine.tau_js_entry_point_result = null;
                    //console.log("+++ 7");
                }
                //console.log("+++ 8");
                //console.log("nativeApiResult type = "+ typeof nativeApiResult );
                //console.log("nativeApiResult = "+nativeApiResult );
                //console.log("resultHandler type = "+ typeof resultHandler );
                //var parsed = JSON.parse(nativeApiResult);
                //console.log("+++ 9");
                resultHandler(nativeApiResult);
                //console.log("+++ 10");
            }
        };
    });

    rhoPlatform.nativeApiCall = function() {
        // use forced bridge type value in case it defined,
        // otherwise use auto-detected bridge type
        var platformBridge = bridges[rhoPlatform.id.JXCORE];

        platformBridge().apiCall.apply(this, arguments);
    }

    //return rho;

    global.Rho = rho;
    //console.log("*************************** 14");

}


initializeRhoAPI();
//var EB = Rho;



//console.log("*************************** 15");
