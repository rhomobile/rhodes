(function(){
    // =========================================================================================
    // This library is for performing ajax calls to sync engine from javascript, no jQuery used.
    // =========================================================================================

    var BASE_URL = "/system/syncengine/";

    function buildQuery(params) {
        var query = "";
        for (var key in params) {
            if (params.hasOwnProperty(key)) {
                var type = typeof params[key];
                if ('string' == type || 'number' == type || 'boolean' == type) {
                    var value = '' + params[key];
                    if (0 < query.length) query = query + '&';
                    query = query + encodeURI(key) + '=' + encodeURI(value);
                }
            }
        }
        return query;
    }

    var xhr=false;
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

        // attempt to create XHR
        if (!xhr) {
            try {
                if ('undefined' != typeof XMLHttpRequest)
                    xhr = new XMLHttpRequest();
                else if (window.createRequest)
                    xhr = window.createRequest();
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
                opts.success = function(){};
            if ('function' != typeof opts.error)
                opts.error = function(){};

            var q = opts.query;
            if (0 < q.length)
                q = '?' + q;
            
            xhr.open(opts.type, opts.url + q, true /*it is async request*/);
            xhr.onreadystatechange = function () {
                if (xhr.readyState == 4) {
                    opts.success.apply('undefined' == typeof that ? this : that, [
                        xhr.response, xhr.status, xhr
                    ]);
                }
            };
            xhr.onerror = function () {
                opts.error.apply('undefined' == typeof that ? this : that, [
                    xhr.response, "error", xhr
                ]);
            }
            xhr.ontimeout = function () {
                opts.error.apply('undefined' == typeof that ? this : that, [
                    "", "timeout", xhr
                ]);
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

    function set_objectnotify_url(/*string*/ url, /*function*/ success, /*function*/ error) {
        request({
            relUrl: "set_objectnotify_url",
            query: {
                url: url
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

    function set_notification(/*string*/ srcName, /*string*/ url, /*string*/ params,
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

    function login(/*string*/ login, /*string*/ password, /*string*/ callback,
            /*function*/ success, /*function*/ error) {
        request({
            relUrl: "login",
            query: {
                login: login,
                password: password,
                callback: callback
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

    window.Rho = window.Rho || {};

    window.Rho.set_threaded_mode = set_threaded_mode;
    window.Rho.enable_status_popup = enable_status_popup;
    window.Rho.set_ssl_verify_peer = set_ssl_verify_peer;
    window.Rho.register_push = register_push;

    window.Rho.set_syncserver = set_syncserver;
    window.Rho.set_pollinterval = set_pollinterval;
    window.Rho.get_pollinterval = get_pollinterval;
    window.Rho.set_pagesize = set_pagesize;
    window.Rho.get_pagesize = get_pagesize;

    window.Rho.login = login;
    window.Rho.logged_in = logged_in;
    window.Rho.logout = logout;

    window.Rho.dosync = dosync;
    window.Rho.dosync_source = dosync_source;
    window.Rho.is_syncing = is_syncing;
    window.Rho.stop_sync = stop_sync;

    window.Rho.set_objectnotify_url = set_objectnotify_url;
    window.Rho.clean_objectnotify = clean_objectnotify;

    //window.Rho.get_lastsync_objectcount = get_lastsync_objectcount;
    //window.Rho.clear_notification = clear_notification;
    //window.Rho.set_source_property = set_source_property;
    //window.Rho.update_blob_attribs = update_blob_attribs;
    //window.Rho.add_objectnotify = add_objectnotify;
    //window.Rho.set_notification = set_notification;
    //window.Rho.dosearch = dosearch;
    //window.Rho.get_src_attrs = get_src_attrs;
    //window.Rho.is_blob_attr = is_blob_attr;

})();
