(function(){
    // =========================================================================================
    // This library is for performing ajax calls to sync engine from javascript, no jQuery used.
    // =========================================================================================

    var URL_dosync = "/system/syncengine/dosync";
    var URL_syncdb = "/system/syncdb";
    
    function buildQuery(params) {
        var query = "";
        for (var key in params) {
            if (params.hasOwnProperty(key)) {
                var type = typeof params[key];
                if ('string' == type || 'number' == type) {
                    if (0 < query.length) query = query + '&';
                    query = query + encodeURI(key) + '=' + encodeURI(params[key]);
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


    function dosync(/*bool*/ show_status_popup, /*const char **/ query_params) {
        request({
            url: URL_syncdb,
            query: {
                show_status_popup: 'undefined' != show_status_popup && show_status_popup ? 'true' : 'false',
                query_params: query_params
            }
        });
    }



/*
extern VALUE dosync_source(VALUE source_id, bool show_status_popup, const char * query_params);

// extern VALUE dosearch(VALUE ar_sources, const char *from, const char *params, bool sync_changes, int nProgressStep, const char* callback, const char* callback_params);
// extern VALUE login(const char *login, const char *password, const char* callback);

extern int logged_in();
extern void logout();
extern void stop_sync();
extern void set_notification(int source_id, const char *url, char* params);
extern void clear_notification(int source_id);

extern int set_pollinterval(int interval);
extern int get_pollinterval();

extern void set_syncserver(char* syncserver);

//extern VALUE get_src_attrs(const char* szPartition, int source_id);
//extern VALUE is_blob_attr(const char* szPartition, int source_id, const char* szAttrName);

extern void  set_objectnotify_url(const char* szUrl);
extern void  add_objectnotify(int nSrcID, const char* szObject);
extern void  clean_objectnotify();

//extern int   get_lastsync_objectcount(int nSrcID);
//extern int   get_pagesize();

extern void  set_pagesize(int pagesize);
extern void  set_threaded_mode(bool b);
extern void  enable_status_popup(bool b);
extern void  set_source_property(int nSrcID, const char* szPropName, const char* szPropValue);
extern void  set_ssl_verify_peer(bool b);
extern void  update_blob_attribs(const char* szPartition, int source_id);

extern VALUE is_syncing();

extern void  register_push();
*/

    window.Rho = window.Rho || {};
    window.Rho.dosync = dosync;

})();
