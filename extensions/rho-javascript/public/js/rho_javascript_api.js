/*
    In case you need to use pre-4.0 JS API together with new Common JS API you can
    involve compatibility layer this way:

        <!-- required for pre-4.0 JS API -->
        <script type="text/javascript" src="/public/jquery/jquery-1.6.4.min.js"></script>
        <script type="text/javascript" src="/public/jquery/jquery.json-2.3.min.js"></script>

        <!-- pre-4.0 JS API -->
        <script type="text/javascript" src="/public/js/rho_javascript_api.js"></script>

        <!-- compatibility layer -->
        <script type="text/javascript" src="/public/js/rho_common_api_noconflict.js"></script>

        <!-- new Common JS API -->
        <script type="text/javascript" charset="utf-8" src="/public/api/rhoapi-modules.js"></script>

    NOTE: load order is important!
*/

var Rho = (function ($) {

    // Config ============================================================

    var RHO_ID_PARAM = '__rhoID';

    var API_CONTROLLER_URL = '/app/RhoJavascriptApi/command_handler';
    var API_CALLBACK_BASE_URL = '/app/RhoJavascriptApi/callback_handler';

    // Utility Methods ===================================================

    var _idCount = 0;
    var _pendingCallbacks = {};

    function _defaultCallback() {}

    function nextId(tag) {
        if ('undefined' == typeof tag || !tag)
            tag = RHO_ID_PARAM;

        return (tag + _idCount++);
    }

    function prepareCallback(callback, /*opt*/ isPersistent, /*opt*/ id) {
        if ('string' == typeof callback)
            return callback;

        if ('function' != typeof callback)
            callback = _defaultCallback;

        if ('undefined' == typeof id || !id)
            id = nextId();

        _pendingCallbacks[id] = {
            id: id,
            callback: callback,
            isPersistent: ('undefined' != typeof isPersistent) && isPersistent
        };
        // store options for pending callback
        return API_CALLBACK_BASE_URL+id;
    }

    function json_encode(val) {
        return val;
    }

    function _jsValue(result) {
        var value = result;

        switch(result['type']) {
            case 'symbol':
            case 'string':
                value = result.value;
                break;
            case 'fixnum':
            case 'integer':
            case 'bignum':
            case 'float':
            case 'rational':
                value = Number(result.value);
                break;
            case 'trueclass':
            case 'falseclass':
                value = ('true' == result.value);
                break;
            case 'time':
                value = new Date(Number(result.time.sec_epoch)*1000);
                break;
            default:
                if ('undefined' != typeof(result['value'])) {
                    value = result.value;
                }
        }
        return value;
    }

    function _api_req(module, method, args, callbackIndex, callback) {
        var cmd = {module:module, method:method, args:args};

        if ("number" == typeof callbackIndex) {
            cmd['callback_index'] = callbackIndex;
            cmd['args'][callbackIndex] = prepareCallback(callback);
        }

        var cmdText = $.toJSON(cmd);
        return new $.Deferred(function(dfr) {
            $.ajax({
                type: 'get',
                url: API_CONTROLLER_URL,
                data:{data: cmdText},
                dataType: 'json',
                headers:{'Accept':'text/plain'}
            }).done(function (result) {
                var res = result;
                res = _jsValue(result);
                dfr.resolve(res);
            }).fail(function (xhr, status, message) {
                var errObj = $.evalJSON(xhr.responseText);
                if (errObj && "object" == typeof errObj && "string" == typeof errObj.error) {
                    message = errObj.error;
                }
                dfr.reject(message, errObj);
            });
        }).promise();
    }

    function _req_for(module) {
        return function(args, method, callbackIndex, callback) {
            return _api_req(module, method, Array.prototype.slice.call(args), callbackIndex, callback);
        };
    }

    // SyncEngine ======================================================

    var syncEngineAPI = function() {

        var _req = _req_for('SyncEngine');

        var BASE_URL = "/system/syncengine/";

        function setSyncCallback(callback, id, isPersistent) {
            if ('undefined' == typeof callback || !callback)
                callback = _defaultCallback;

            if ('undefined' == typeof id || !id)
                id = nextId();

            _pendingCallbacks[id] = {
                id: id,
                callback: callback,
                isPersistent: ('undefined' != typeof isPersistent) && isPersistent
            };
            // store options for pending callback
            return 'javascript:window._rho_syncEngineCallback("' +id +'")';
        }

        function buildQuery(params) {
            var query = "";
            for (var key in params) {
                if (params.hasOwnProperty(key)) {
                    var type = typeof params[key];
                    if ('string' == type || 'number' == type || 'boolean' == type) {
                        var value = '' + params[key];
                        if (0 < query.length) query = query + '&';
                        query = query + encodeURIComponent(key) + '=' + encodeURIComponent(value);
                    }
                }
            }
            return query;
        }

        /*
            Option names are:
                type    - request type (optional, 'GET' by default)
                url     - request URL
                query   - request query string, may be defined as string or an object with properties (optional)
                body    - request body (optional)
                success - request success handler (optional)
                error   - request error handler (optional)
         */
        function request(opts, that) {
            var xhr = false;

            // attempt to create XHR
            if (!xhr) {
                try {
                    if ('undefined' != typeof XMLHttpRequest) {
                        xhr = new XMLHttpRequest();
                    } else if (window.createRequest) {
                        xhr = window.createRequest();
                    } else if (window.ActiveXObject) {
                        try {
                            xhr = new ActiveXObject("MSXML2.XMLHTTP");
                        } catch(ex) {
                            xhr = new ActiveXObject("Microsoft.XMLHTTP");
                        }
                    } else {
                        //alert('no xhr!');
                    }
                } catch(ex) {
                    xhr = false;
                }
            }

            if (xhr) {
                // ensure required opts are defined
                if ('object' != typeof opts)
                    opts = {};
                if ('string' != typeof opts.type)
                    opts.type = 'GET';
                if ('string' != typeof opts.baseUrl)
                    opts.baseUrl = BASE_URL;
                if ('string' != typeof opts.relUrl)
                    opts.relUrl = '';
                if ('string' != typeof opts.url)
                    opts.url = opts.baseUrl + opts.relUrl;
                if ('undefined' == typeof opts.query)
                    opts.query = '';
                if ('object' == typeof opts.query)
                    opts.query = buildQuery(opts.query);
                if ('undefined' == typeof opts.body)
                    opts.body = null;
                if ('function' != typeof opts.success)
                    opts.success = function() {};
                if ('function' != typeof opts.error)
                    opts.error = function() {};

                var q = opts.query;
                if (0 < q.length)
                    q = '?' + q;

                xhr.open(opts.type, opts.url + q, true /*it is async request*/);
                xhr.onreadystatechange = function () {
                    if (xhr.readyState == 4) {
                        opts.success.apply('undefined' == typeof that ? this : that, [
                            xhr.response || xhr.responseText, xhr.status || xhr.statusText, xhr
                        ]);
                    }
                };
                if (xhr.onerror) {
                    xhr.onerror = function () {
                        opts.error.apply('undefined' == typeof that ? this : that, [
                            xhr.response || xhr.responseText, "error", xhr
                        ]);
                    };
                }
                if (xhr.ontimeout) {
                    xhr.ontimeout = function () {
                        opts.error.apply('undefined' == typeof that ? this : that, [
                            "", "timeout", xhr
                        ]);
                    };
                }
                xhr.send(opts.body);
            }
        }


        function logged_in(/*function*/ success, /*function*/ error) {
            request({
                relUrl: "logged_in",
                success: success,
                error: error
            });
        }

        function logout(/*function*/ success, /*function*/ error) {
            request({
                relUrl: "logout",
                success: success,
                error: error
            });
        }

        function dosync(/*bool*/ show_status_popup, /*string*/ query_params,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "dosync",
                success: success,
                error: error
            });
        }

        function dosync_source(/*string*/ srcName, /*bool*/ show_status_popup, /*string*/ query_params,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "dosync_source",
                query: {
                    srcName: srcName
                },
                success: success,
                error: error
            });
        }

        function stop_sync(/*function*/ success, /*function*/ error) {
            request({
                relUrl: "stop_sync",
                success: success,
                error: error
            });
        }

        function set_pollinterval(/*number*/ interval, /*function*/ success, /*function*/ error) {
            request({
                relUrl: "set_pollinterval",
                query: {
                    interval: interval
                },
                success: success,
                error: error
            });
        }

        function get_pollinterval(/*function*/ success, /*function*/ error) {
            request({
                relUrl: "get_pollinterval",
                success: success,
                error: error
            });
        }

        function set_syncserver(/*string*/ syncserver, /*function*/ success, /*function*/ error) {
            request({
                relUrl: "set_syncserver",
                query: {
                    syncserver: syncserver
                },
                success: success,
                error: error
            });
        }

        function set_pagesize(/*number*/ size, /*function*/ success, /*function*/ error) {
            request({
                relUrl: "set_pagesize",
                query: {
                    pagesize: size
                },
                success: success,
                error: error
            });
        }

        function get_pagesize(/*function*/ success, /*function*/ error) {
            request({
                relUrl: "get_pagesize",
                success: success,
                error: error
            });
        }

        function get_lastsync_objectcount(/*string*/ srcName, /*function*/ success, /*function*/ error) {
            request({
                relUrl: "get_lastsync_objectcount",
                query: {
                    srcName: srcName
                },
                success: success,
                error: error
            });
        }

        function is_syncing(/*function*/ success, /*function*/ error) {
            request({
                relUrl: "is_syncing",
                success: success,
                error: error
            });
        }

        function enable_status_popup(/*boolean*/ enable, /*function*/ success, /*function*/ error) {
            request({
                relUrl: "enable_status_popup",
                query: {
                    enable: enable
                },
                success: success,
                error: error
            });
        }

        function set_threaded_mode(/*boolean*/ threaded, /*function*/ success, /*function*/ error) {
            request({
                relUrl: "set_threaded_mode",
                query: {
                    threaded: threaded
                },
                success: success,
                error: error
            });
        }

        function register_push(/*function*/ success, /*function*/ error) {
            request({
                relUrl: "register_push",
                success: success,
                error: error
            });
        }

        function set_source_property(/*string*/ srcName, /*string*/ propName, /*string*/ propValue,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "set_source_property",
                query: {
                    srcName: srcName,
                    propName: propName,
                    propValue: propValue
                },
                success: success,
                error: error
            });
        }

        function set_ssl_verify_peer(/*boolean*/ verify, /*function*/ success, /*function*/ error) {
            request({
                relUrl: "set_ssl_verify_peer",
                query: {
                    verify: verify
                },
                success: success,
                error: error
            });
        }

        function update_blob_attribs(/*string*/ partition, /*string*/ srcName,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "update_blob_attribs",
                query: {
                    partition: partition,
                    srcName: srcName
                },
                success: success,
                error: error
            });
        }

        function set_objectnotify(/*function*/ notify, /*function*/ success, /*function*/ error) {
            var uri = setSyncCallback(notify, nextId('notify'), true /*it is persistent callback*/);
            request({
                relUrl: "set_objectnotify_url",
                query: {
                    url: uri
                },
                success: success,
                error: error
            });
        }

        function add_objectnotify(/*string*/ srcName, /*string*/ objectId,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "add_objectnotify",
                query: {
                    srcName: srcName,
                    objectId: objectId
                },
                success: success,
                error: error
            });
        }

        function clean_objectnotify(/*function*/ success, /*function*/ error) {
            request({
                relUrl: "clean_objectnotify",
                success: success,
                error: error
            });
        }

        function off_set_notification(/*string*/ srcName, /*string*/ url, /*string*/ params,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "set_notification",
                query: {
                    srcName: srcName,
                    url: url,
                    params: params
                },
                success: success,
                error: error
            });
        }

        function clear_notification(/*string*/ srcName, /*function*/ success, /*function*/ error) {
            request({
                relUrl: "clear_notification",
                query: {
                    srcName: srcName
                },
                success: success,
                error: error
            });
        }

        function login(/*string*/ login, /*string*/ password, /*function*/ callback,
                /*function*/ success, /*function*/ error) {
            var uri = setSyncCallback(callback, nextId('callback'));
            request({
                relUrl: "login",
                query: {
                    login: login,
                    password: password,
                    callback: uri
                },
                success: success,
                error: error
            });
        }

        function dosearch(/*string*/ srcNames, /*string*/ from, /*string*/ params, /*boolean*/ syncChanges,
                /*number*/ progressStep, /*string*/ callback, /*string*/ callbackParams,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "dosearch",
                query: {
                    srcNames: srcNames,
                    from: from,
                    params: params,
                    syncChanges: syncChanges,
                    progressStep: progressStep,
                    callback: callback,
                    callbackParams: callbackParams
                },
                success: success,
                error: error
            });
        }

        function get_src_attrs(/*string*/ partition, /*string*/ srcName,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "get_src_attrs",
                query: {
                    partition: partition,
                    srcName: srcName
                },
                success: success,
                error: error
            });
        }

        function is_blob_attr(/*string*/ partition, /*string*/ srcName, /*string*/ attrName,
                /*function*/ success, /*function*/ error) {
            request({
                relUrl: "is_blob_attr",
                query: {
                    partition: partition,
                    srcName: srcName,
                    attrName: attrName
                },
                success: success,
                error: error
            });
        }

        window._rho_syncEngineCallback = function(callbackId) {
            var cbId = decodeURIComponent(callbackId);
            //console.log('_rho_syncEngineCallback: callback for: ' +cbId);
            //console.log('_rho_syncEngineCallback: result: ' +result);

            var opts = _pendingCallbacks[cbId];
            var callback = null;

            if ('undefined' != typeof opts && opts) {
                //console.log('_rho_syncEngineCallback: callback found!');
                callback = opts.callback;
                if (!opts.isPersistent)
                    delete _pendingCallbacks[cbId];
            }
            return callback;
        };

        /*
       int  rho_sync_get_lastsync_objectcount(int nSrcID);
       void rho_sync_clear_notification(int source_id);
       void  set_source_property(int nSrcID, const char* szPropName, const char* szPropValue);
       void  update_blob_attribs(const char* szPartition, int source_id);
       void  add_objectnotify(int nSrcID, const char* szObject);
       void set_notification(int source_id, const char *url, char* params);
       VALUE dosearch(VALUE ar_sources, const char *from, const char *params, bool sync_changes, int nProgressStep, const char* callback, const char* callback_params);
       VALUE get_src_attrs(const char* szPartition, int source_id);
       VALUE is_blob_attr(const char* szPartition, int source_id, const char* szAttrName);
        */

        function setNotification(source_id, callback, params) {
            return _req([source_id, 'callback_placeholder', params], 'set_notification', 1, callback);
        }

        return {
            set_notification: setNotification,

            set_threaded_mode: set_threaded_mode,
            enable_status_popup: enable_status_popup,
            set_ssl_verify_peer: set_ssl_verify_peer,
            register_push: register_push,

            set_syncserver: set_syncserver,
            set_pollinterval: set_pollinterval,
            get_pollinterval: get_pollinterval,
            set_pagesize: set_pagesize,
            get_pagesize: get_pagesize,

            login: login,
            logged_in: logged_in,
            logout: logout,

            dosync: dosync,
            dosync_source: dosync_source,
            is_syncing: is_syncing,
            stop_sync: stop_sync,

            set_objectnotify: set_objectnotify,
            clean_objectnotify: clean_objectnotify

            //get_lastsync_objectcount: get_lastsync_objectcount,
            //clear_notification: clear_notification,
            //set_source_property: set_source_property,
            //update_blob_attribs: update_blob_attribs,
            //add_objectnotify: add_objectnotify,
            //set_notification: set_notification,
            //dosearch: dosearch,
            //get_src_attrs: get_src_attrs,
            //is_blob_attr: is_blob_attr,
        };
    };

    // RhoApplication API ======================================================

    var rhoApplicationAPI = function () {

        var _req = _req_for('Rho::RhoApplication');

        function getBaseAppPath()                 { return _req(arguments, 'get_base_app_path'); }
        function getAppPath(relativePath)         { return _req(arguments, 'get_app_path'); }
        function getUserPath()                    { return _req(arguments, 'get_user_path'); }
        function getBlobPath(relativePath)        { return _req(arguments, 'get_blob_path'); }
        function getBlobFolder()                  { return _req(arguments, 'get_blob_folder'); }
        function getModelPath(appDir, modelName)  { return _req(arguments, 'get_model_path'); }

        return {
            get_base_app_path: getBaseAppPath,
            get_app_path: getAppPath,
            get_user_path: getUserPath,
            get_blob_path: getBlobPath,
            get_blob_folder: getBlobFolder,
            get_model_path: getModelPath
        };
    };

    // File API =================================================================

    var fileAPI = function () {

        var _req = _req_for('File');

        function File(id) {
            function _instance(args)  { return [id].concat(Array.prototype.slice.call(args)); }
            this.getId  = function () { return id; }

            this.getc   = function () { return _req(_instance(arguments), 'getc'); }
            this.ungetc = function (char /*string with an only char*/) { return _req(_instance(arguments), 'ungetc'); }
            this.gets   = function (separatorOrLimit, /*opt*/ limit) { return _req(_instance(arguments), 'gets'); }
            this.putc   = function (code)  { return _req(_instance(arguments), 'putc'); }
            this.puts   = function (string /*, anotherString, etc - as many as you wish*/)  { return _req(_instance(arguments), 'puts'); }
            this.flush  = function ()  { return _req(_instance(arguments), 'flush'); }
            this.close  = function ()  { return _req(_instance(arguments), 'close'); }
        };

        function absolute_path(fileName, /*opt*/ dirString) { return _req(arguments, 'absolute_path'); }

        function open(fileName, /*opt*/ mode, /*opt*/ perm) {
            var args = arguments;
            return new $.Deferred(function(dfr) {
                _req(args, 'open').done(function(result) {
                    //console.dir(result);
                    dfr.resolve(new File(result[RHO_ID_PARAM]));
                }).fail(function(msg) {
                    dfr.reject(msg);
                });
            }).promise();
        }

        function atime(fileName)                    { return _req(arguments, 'atime'); }
        function basename(fileName, /*opt*/ suffix) { return _req(arguments, 'basename'); }
        function is_blockdev(fileName)              { return _req(arguments, 'blockdev?'); }
        function is_chardev(fileName)               { return _req(arguments, 'chardev?'); }

        function chmod(mode, fileName /*, anotherFileName - as many as you need*/) { return _req(arguments, 'chmod'); }
        function chown(uid, gid, fileName /*, anotherFileName - as many as you need*/) { return _req(arguments, 'chown'); }

        function ctime(fileName)        { return _req(arguments, 'ctime'); }
        function do_delete(fileName)    { return _req(arguments, 'delete'); }
        function is_directory(fileName) { return _req(arguments, 'directory?'); }
        function dirname(fileName)      { return _req(arguments, 'dirname'); }

        function is_executable(fileName)      { return _req(arguments, 'executable?'); }
        function is_executable_real(fileName) { return _req(arguments, 'executable_real?'); }
        function is_exist(fileName)           { return _req(arguments, 'exist?'); }

        function expand_path(fileName, /*opt*/ dirStrig) { return _req(arguments, 'expand_path'); }

        function extname(fileName) { return _req(arguments, 'extname'); }
        function is_file(fileName) { return _req(arguments, 'file?'); }

        function is_fnmatch(pattern, path) { return _req(arguments, 'fnmatch?'); }
        function ftype(fileName) { return _req(arguments, 'ftype'); }

        function is_grpowned(fileName) { return _req(arguments, 'grpowned?'); }

        function is_identical(fileName1, fileName2) { return _req(arguments, 'identical?'); }

        function join(part, /*opt*/ nextPart /*put as many args as you need*/) { return _req(arguments, 'join'); }

        function lchmod(mode, fileName, oneMoreFileName /*as many as you need*/) { return _req(arguments, 'lchmod'); }
        function lchown(uid, gid, fileName, oneMoreFileName /*as many as you need*/) { return _req(arguments, 'lchown'); }

        function link(oldName, newName) { return _req(arguments, 'link'); }
        function lstat(fileName) { return _req(arguments, 'lstat'); }

        function mtime(fileName) { return _req(arguments, 'mtime'); }

        function is_owned(fileName) { return _req(arguments, 'owned?'); }
        function path(fileName) { return _req(arguments, 'path'); }

        function is_pipe(fileName) { return _req(arguments, 'pipe?'); }

        function is_readable(fileName) { return _req(arguments, 'readable?'); }
        function is_readable_real(fileName) { return _req(arguments, 'readable_real?'); }

        function readlink(fileName) { return _req(arguments, 'readlink'); }
        function realdirpath(pathName, /*opt*/ dirStrig) { return _req(arguments, 'realdirpath'); }
        function realpath(pathName, /*opt*/ dirStrig) { return _req(arguments, 'realpath'); }
        function rename(oldName, newName) { return _req(arguments, 'rename'); }

        function is_setgid(fileName) { return _req(arguments, 'setgid?'); }
        function is_setuid(fileName) { return _req(arguments, 'setuid?'); }

        function size(fileName) { return _req(arguments, 'size'); }
        function is_size(fileName) { return _req(arguments, 'size?'); }

        function is_socket(fileName) { return _req(arguments, 'socket?'); }

        function split(fileName) { return _req(arguments, 'split'); }
        function stat(fileName) { return _req(arguments, 'stat'); }

        function is_sticky(fileName) { return _req(arguments, 'sticky?'); }

        function symlink(oldName, newName) { return _req(arguments, 'symlink'); }

        function is_symlink(fileName) { return _req(arguments, 'symlink?'); }

        function truncate(fileName, /*int*/length) { return _req(arguments, 'truncate'); }
        function umask(/*opt*/mask /*integer value*/) { return _req(arguments, 'umask'); }

        function utime(/*Date*/ atime, /*Date*/ mtime, fileName) {
            var at = atime.getTime()/1000; // seconds since epoch start (00:00AM 01-Jan-1970)
            var mt = mtime.getTime()/1000; // seconds since epoch start (00:00AM 01-Jan-1970)
            return _req([at, mt, fileName], 'utime');
        }

        function is_world_readable(fileName) { return _req(arguments, 'world_readable?'); }

        function is_world_writable(fileName) { return _req(arguments, 'world_writable?'); }

        function is_writable(fileName) { return _req(arguments, 'writable?'); }
        function is_writable_real(fileName) { return _req(arguments, 'writable_real?'); }

        function is_zero(fileName) { return _req(arguments, 'zero?'); }

        return {
            absolute_path: absolute_path,
            atime: atime,
            basename: basename,
            is_blockdev: is_blockdev,
            is_chardev: is_chardev,
            chmod: chmod,
            chown: chown,
            ctime: ctime,
            do_delete: do_delete,
            is_directory: is_directory,
            dirname: dirname,
            is_executable: is_executable,
            is_executable_real: is_executable_real,
            is_exist: is_exist,
            expand_path: expand_path,
            extname: extname,
            is_file: is_file,
            is_fnmatch: is_fnmatch,
            ftype: ftype,
            is_grpowned: is_grpowned,
            is_identical: is_identical,
            join: join,
            lchmod: lchmod,
            lchown: lchown,
            link: link,
            lstat: lstat,
            mtime: mtime,
            //new_file: new_file,
            open: open,
            is_owned: is_owned,
            path: path,
            is_pipe: is_pipe,
            is_readable: is_readable,
            is_readable_real: is_readable_real,
            readlink: readlink,
            realdirpath: realdirpath,
            realpath: realpath,
            rename: rename,
            is_setgid: is_setgid,
            is_setuid: is_setuid,
            size: size,
            is_size: is_size,
            is_socket: is_socket,
            split: split,
            stat: stat,
            is_sticky: is_sticky,
            symlink: symlink,
            is_symlink: is_symlink,
            truncate: truncate,
            umask: umask,
            unlink: do_delete,
            utime: utime,
            is_world_readable: is_world_readable,
            is_world_writable: is_world_writable,
            is_writable: is_writable,
            is_writable_real: is_writable_real,
            is_zero: is_zero
        };
    };

    // Synstem API ======================================================

    var systemAPI = function () {

        var _req = _req_for('System');

        function getProperty(paramName) { return _req(arguments, 'get_property'); }
        function openUrl(url) { return _req(arguments, 'open_url'); }

        function zipFile(zipPathName, filePathName, password) { return _req(arguments, 'zip_file'); }
        function zipFiles(zipPathName, filesPath, /*string array*/fileNames, password) { return _req(arguments, 'zip_files'); }
        function unzipFile(zipPathName) { return _req(arguments, 'unzip_file'); }

        return {
            get_property:getProperty,
            open_url: openUrl,
            zip_file: zipFile,
            zip_files: zipFiles,
            unzip_file: unzipFile
        };
    };

    // Camera API ======================================================

    var cameraAPI = function () {

        var _req = _req_for('Camera');

        function takePicture(callback, params) {
            return _req(['callback_placeholder', params || {}], 'take_picture', 0, callback);
        }

        function choosePicture(callback, params) {
            return _req(['callback_placeholder', params || {}], 'choose_picture', 0, callback);
        }

        function getCameraInfo(cameraType) { return _req(arguments, 'get_camera_info'); }

        return {
            take_picture:takePicture,
            choose_picture: choosePicture,
            get_camera_info: getCameraInfo
        };
    };

    // Geolocation API ======================================================

    var geolocationAPI = function () {

        var _req = _req_for('GeoLocation');

        function latitude()         { return _req(arguments, 'latitude'); }
        function longitude()        { return _req(arguments, 'longitude'); }
        function isKnownPosition()  { return _req(arguments, 'known_position?'); }
        function turnOff()          { return _req(arguments, 'known_position?'); }

        function setNotification(callback, paramStr, ping_interval) {
            return _req(['callback_placeholder', paramStr || '', ping_interval], 'set_notification', 0, callback);
        }

        function haversine_distance(latitude1, longitude1, latitude2, longitude2) {
            return _req(arguments, 'haversine_distance');
        }

        return {
            latitude: latitude,
            longitude: longitude,
            is_known_position: isKnownPosition,
            turnoff: turnOff,
            set_notification: setNotification,
            haversine_distance: haversine_distance
        };
    };

    // SignatureCapture API ======================================================

    var signatureCaptureAPI = function () {

        var _req = _req_for('Rho::SignatureCapture');

        function visible(visibility, params)  { return _req(arguments, 'visible'); }
        function clear()                      { return _req(arguments, 'clear'); }


        function take(callback, params) {
            return _req(['callback_placeholder', params || {}], 'take', 0, callback);
        }

        function capture(callback) {
            return _req(['callback_placeholder'], 'capture', 0, callback);
        }

        return {
            visible: visible,
            clear: clear,
            take: take,
            capture: capture
        };
    };

    // Barcode API ======================================================

    var barcodeAPI = function () {

        var _req = _req_for('Barcode');

        function barcodeRecognize(fileFullPathName)  { return _req(arguments, 'barcode_recognize'); }

        function takeBarcode(callback, params) {
            return _req(['callback_placeholder', params || {}], 'take_barcode', 0, callback);
        }

        return {
            barcode_recognize: barcodeRecognize,
            take_barcode: takeBarcode
        };
    };

    // RhoLog API ======================================================

    var logAPI = function () {

        var _req = _req_for('RhoLog');

        function error(tag, message)  { return _req(arguments, 'error'); }
        function info(tag, message)   { return _req(arguments, 'info'); }

        return {
            error: error,
            info: info
        };
    };

    // MapView API ======================================================

    var mapViewAPI = function () {

        var _req = _req_for('MapView');

        function create(options)              { return _req(arguments, 'create'); }
        function setFileCachingEnable(enable) { return _req(arguments, 'set_file_caching_enable'); }

        function preloadMapTiles(options, callback) {
            return _req([options, 'callback_placeholder'], 'preload_map_tiles', 1, callback);
        }

        return {
            create: create,
            set_file_caching_enable: setFileCachingEnable,
            preload_map_tiles: preloadMapTiles
        };
    };

    // AsyncHttp API ======================================================

    var asyncHttpAPI = function () {

        var _req = _req_for('Rho::AsyncHttp');

        function cancel() { return _req(arguments, 'cancel'); }

        function get(url, headers, callback, params) {
            return _req([url, headers, 'callback_placeholder', params], 'get', 2, callback);
        }

        function post(url, headers, body, callback, params) {
            return _req([url, headers, body, 'callback_placeholder', params], 'post', 3, callback);
        }

        function downloadFile(url, headers, fileName, callback, params) {
            return _req([url, headers, fileName, 'callback_placeholder', params], 'download_file', 3, callback);
        }

        function uploadFile(url, headers, fileName, body, callback, params) {
            return _req([url, headers, fileName, body, 'callback_placeholder', params], 'upload_file', 4, callback);
        }

        return {
            cancel: cancel,
            get: get,
            post: post,
            download_file: downloadFile,
            upload_file: uploadFile
        };
    };

    // AudioCapture API ======================================================

    var audioCaptureAPI = function () {

        var _req = _req_for('AudioCapture');

        function start()  { return _req(arguments, 'start'); }
        function stop()   { return _req(arguments, 'stop'); }
        function cancel() { return _req(arguments, 'cancel'); }

        function setDuration(duration)    { return _req(arguments, 'set_duration'); }
        function setDestination(fullPath) { return _req(arguments, 'set_destination'); }
        function setName(fileName)        { return _req(arguments, 'set_name'); }

        function setAudioSaveEvent(callback) {
            return _req(['callback_placeholder'], 'set_audio_save_event', 0, callback);
        }

        return {
            start: start,
            stop: stop,
            cancel: cancel,
            set_duration: setDuration,
            set_destination: setDestination,
            set_name: setName,
            set_audio_save_event: setAudioSaveEvent,
            set_notification: setAudioSaveEvent
        };
    };

    // Database API ======================================================

    var databaseAPI = function () {

        var _req = _req_for('Rho::WebSql');

        function Transaction(id) {
            function _instance(args)  { return [id].concat(Array.prototype.slice.call(args)); }
            this.getId  = function () { return id; }

            this.executeSql = function(sql, vars, callback) {
                _req(_instance([sql, vars]), 'execute_sql').done(function(result) {
                    callback(result);
                }).fail(function(msg) {
                    throw msg;
                });
            }

            this.commit = function()  { return _req(_instance(arguments), 'commit'); }
            this.rollback = function()  { return _req(_instance(arguments), 'rollback'); }
        }

        function Database(id) {
            function _instance(args)  { return [id].concat(Array.prototype.slice.call(args)); }
            this.getId  = function () { return id; }

            this.transaction = function(callback) {
                _req(_instance([]), 'transaction').done(function(result) {
                    var tx = new Transaction(result[RHO_ID_PARAM]);
                    try {
                        callback(tx);
                        tx.commit();
                    } catch(ex) {
                        tx.rollback();
                    }
                }).fail(function(msg) {
                    throw msg;
                });
            }
        };

        function openDatabase(name, version, descr, size) {
            var args = arguments;
            return new $.Deferred(function(dfr) {
                _req(args, 'open_database').done(function(result) {
                    //console.dir(result);
                    dfr.resolve(new Database(result[RHO_ID_PARAM]));
                }).fail(function(msg) {
                    dfr.reject(msg);
                });
            }).promise();
        }

        return {
            openDatabase: openDatabase
        };
    };

    // ==========================================================================

    function callbackHandler(callbackId, resultObj) {
        var cbId = decodeURIComponent(callbackId);
        //console.log('Rho.callback_handler: callback for: ' +cbId);
        //console.log('Rho.callback_handler: resultObj: ' +resultObj);

        var opts = _pendingCallbacks[cbId];

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
                delete _pendingCallbacks[cbId];
        }
    };

    // ==========================================================================
    return {
        System: systemAPI(),
        RhoApplication: rhoApplicationAPI(),
        File: fileAPI(),
        Camera: cameraAPI(),
        GeoLocation: geolocationAPI(),
        SignatureCapture: signatureCaptureAPI(),
        Barcode: barcodeAPI(),
        MapView: mapViewAPI(),
        Log: logAPI(),
        AsyncHttp: asyncHttpAPI(),
        AudioCapture: audioCaptureAPI(),
        Database: databaseAPI(),
        pre4_callback_handler: callbackHandler
    };

})(jQuery);
