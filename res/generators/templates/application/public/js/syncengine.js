(function(){
    // =========================================================================================
    // This library is for performing ajax calls to sync engine from javascript, no jQuery used.
    // =========================================================================================

    var URL_logged_in = "/system/syncengine/logged_in";
    var URL_logout = "/system/syncengine/logout";
    var URL_dosync = "/system/syncengine/dosync";
    var URL_dosync_source = "/system/syncengine/dosync_source";
    var URL_stop_sync = "/system/syncengine/stop_sync";
    var URL_set_pollinterval = "/system/syncengine/set_pollinterval";
    var URL_get_pollinterval = "/system/syncengine/get_pollinterval";
    var URL_set_syncserver = "/system/syncengine/set_syncserver";
    var URL_set_pagesize = "/system/syncengine/set_pagesize";
    var URL_get_pagesize = "/system/syncengine/get_pagesize";
    var URL_get_lastsync_objectcount = "/system/syncengine/get_lastsync_objectcount";
    var URL_is_syncing = "/system/syncengine/is_syncing";
    var URL_enable_status_popup = "/system/syncengine/enable_status_popup";
    var URL_set_threaded_mode = "/system/syncengine/set_threaded_mode";
    var URL_register_push = "/system/syncengine/register_push";

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
            if ('string' != typeof opts.url)
                opts.url = '';
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
            url: URL_logged_in,
            success: success,
            error: error
        });
    }

    function logout(/*function*/ success, /*function*/ error) {
        request({
            url: URL_logout,
            success: success,
            error: error
        });
    }

    function dosync(/*bool*/ show_status_popup, /*string*/ query_params,
            /*function*/ success, /*function*/ error) {
        request({
            url: URL_dosync,
            success: success,
            error: error
        });
    }

    function dosync_source(/*string*/ srcName, /*bool*/ show_status_popup, /*string*/ query_params,
            /*function*/ success, /*function*/ error) {
        request({
            url: URL_dosync_source,
            query: {
                srcName: srcName
            },
            success: success,
            error: error
        });
    }

    function stop_sync(/*function*/ success, /*function*/ error) {
        request({
            url: URL_stop_sync,
            success: success,
            error: error
        });
    }

    function set_pollinterval(/*number*/ interval, /*function*/ success, /*function*/ error) {
        request({
            url: URL_set_pollinterval,
            query: {
                interval: interval
            },
            success: success,
            error: error
        });
    }

    function get_pollinterval(/*function*/ success, /*function*/ error) {
        request({
            url: URL_get_pollinterval,
            success: success,
            error: error
        });
    }

    function set_syncserver(/*string*/ syncserver, /*function*/ success, /*function*/ error) {
        request({
            url: URL_set_syncserver,
            query: {
                syncserver: syncserver
            },
            success: success,
            error: error
        });
    }

    function set_pagesize(/*number*/ size, /*function*/ success, /*function*/ error) {
        request({
            url: URL_set_pagesize,
            query: {
                pagesize: size
            },
            success: success,
            error: error
        });
    }

    function get_pagesize(/*function*/ success, /*function*/ error) {
        request({
            url: URL_get_pagesize,
            success: success,
            error: error
        });
    }

    function get_lastsync_objectcount(/*string*/ srcName, /*function*/ success, /*function*/ error) {
        request({
            url: URL_get_lastsync_objectcount,
            query: {
                srcName: srcName
            },
            success: success,
            error: error
        });
    }

    function is_syncing(/*function*/ success, /*function*/ error) {
        request({
            url: URL_is_syncing,
            success: success,
            error: error
        });
    }

    function enable_status_popup(/*boolean*/ enable, /*function*/ success, /*function*/ error) {
        request({
            url: URL_enable_status_popup,
            query: {
                enable: enable
            },
            success: success,
            error: error
        });
    }

    function set_threaded_mode(/*boolean*/ threaded, /*function*/ success, /*function*/ error) {
        request({
            url: URL_set_threaded_mode,
            query: {
                threaded: threaded
            },
            success: success,
            error: error
        });
    }

    function register_push(/*function*/ success, /*function*/ error) {
        request({
            url: URL_register_push,
            success: success,
            error: error
        });
    }



/*
// extern VALUE dosearch(VALUE ar_sources, const char *from, const char *params, bool sync_changes, int nProgressStep, const char* callback, const char* callback_params);
// extern VALUE login(const char *login, const char *password, const char* callback);

extern void set_notification(int source_id, const char *url, char* params);
extern void clear_notification(int source_id);

//extern VALUE get_src_attrs(const char* szPartition, int source_id);
//extern VALUE is_blob_attr(const char* szPartition, int source_id, const char* szAttrName);

extern void  set_objectnotify_url(const char* szUrl);
extern void  add_objectnotify(int nSrcID, const char* szObject);
extern void  clean_objectnotify();

extern void  set_source_property(int nSrcID, const char* szPropName, const char* szPropValue);
extern void  set_ssl_verify_peer(bool b);
extern void  update_blob_attribs(const char* szPartition, int source_id);


*/

    window.Rho = window.Rho || {};
    window.Rho.logged_in = logged_in;
    window.Rho.logout = logout;
    window.Rho.dosync = dosync;
    window.Rho.stop_sync = stop_sync;
    window.Rho.set_pollinterval = set_pollinterval;
    window.Rho.get_pollinterval = get_pollinterval;
    window.Rho.set_syncserver = set_syncserver;
    window.Rho.set_pagesize = set_pagesize;
    window.Rho.get_pagesize = get_pagesize;
    window.Rho.get_lastsync_objectcount = get_lastsync_objectcount;
    window.Rho.is_syncing = is_syncing;

    window.Rho.dosync_source = dosync_source;
    window.Rho.enable_status_popup = enable_status_popup;
    window.Rho.set_threaded_mode = set_threaded_mode;
    window.Rho.register_push = register_push;

})();
