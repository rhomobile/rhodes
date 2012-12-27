(function($) {

    function publicInterface() {
        return {
            getVersion: getVersion,
            getErrCodeFromXHR: getErrCodeFromXHR,
            getSession: getSession,
            setSession: setSession,
            deleteSession: deleteSession,
            getServerQueryUrl: getServerQueryUrl,
            login: login,
            clientCreate: clientCreate,
            clientReset: clientReset,
            serverQuery: serverQuery,
            postData: postData
        };
    }

    var rho = RhoSync.rho;

    var SESSION_COOKIE = 'rhosync_session';

    var respCodes = {
        HTTP_OK: 200,
        HTTP_PARTIAL_CONTENT: 206,
        HTTP_MOVED_TEMPORARILY: 302,
        HTTP_MOVED_PERMANENTLY: 301,
        HTTP_MOVED_PERM: 301,
        HTTP_BAD_REQUEST: 400,
        HTTP_NOT_FOUND: 404,
        HTTP_UNAUTHORIZED: 401,
        HTTP_RANGENOTSATISFY: 416,
        HTTP_INTERNAL_ERROR: 500,
        HTTP_NOTMODIFIED: 304
    };

    function getErrCodeFromXHR(xhr) {
        switch(xhr.status) {
            case respCodes.HTTP_UNAUTHORIZED: return rho.errors.ERR_UNATHORIZED;
            case respCodes.HTTP_OK: return rho.errors.ERR_NONE;
            case respCodes.HTTP_PARTIAL_CONTENT: return rho.errors.ERR_NONE;
            default: return rho.errors.ERR_REMOTESERVER;
        }
    }

    function getVersion() {
        return 3;
    }

    function getSession() {
        return _getCookie(SESSION_COOKIE);
    }

    function setSession(value) {
        return _setCookie(SESSION_COOKIE, value, 365, '/', window.location.hostname, false);
    }

    function deleteSession() {
        return _deleteCookie(SESSION_COOKIE, '/', window.location.hostname);
    }

    function getServerQueryUrl() {
        return rho.config.syncServer;
    }

    function _setCookie(name, value, days, path, domain, secure) {
        //TODO: to re-implement
//        if (days) {
//            var expDate = new Date();
//            expDate.setTime(expDate.getTime() + (days * 24 * 60 * 60 * 1000));
//        }
//        document.cookie = name + "=" + /*encodeURIComponent( */value /*)*/ +
//            ((days) ? "; expires=" + expDate.toGMTString() : "") +
//            ((path) ? "; path=" + path : "") +
//            ((domain) ? "; domain=" + domain : "") +
//            ((secure) ? "; secure" : "");
    }

    function _getCookie(name) {
        var cookies = document.cookie.split(';');
        for (var i = 0; i < cookies.length; i++) {
            var cookie = cookies[i].split('=');
            var cName = cookie[0].replace(/^\s+|\s+$/g, ''); // trim side spaces
            if (cName == name) {
                return (1 < cookie.length) ? cookie[1].replace(/^\s+|\s+$/g, '') : ''; // trim side spaces
            }
        }
        return null;
    }

    function _deleteCookie(name, path, domain) {
        if (_getCookie(name)) {
            document.cookie = name + "=" +
                ((path) ? "; path=" + path : "") +
                ((domain) ? "; domain=" + domain : "") +
                "; expires=Thu, 01-Jan-1970 00:00:01 GMT";
        }
    }

    function _net_call(url, data, method /*='post'*/, contentType /*='application/json'*/) {
        return $.Deferred(function(dfr){
            $.ajax({
                url: url,
                type: method || 'post',
                contentType: contentType || 'application/json',
                processData: false,
                data: $.toJSON(data),
                dataType: 'json'
            }).done(function(data, status, xhr){
                rho.notify.byEvent(rho.events.GENERIC_NOTIFICATION, status, data, xhr);
                dfr.resolve(status, data, xhr);
            }).fail(function(xhr, status, error){
                rho.notify.byEvent(rho.events.GENERIC_NOTIFICATION, status, error, xhr);
                dfr.reject(status, error, xhr);
            });
        }).promise();
    }

    function login(login, password) {
        return _net_call(rho.config.syncServer+'/clientlogin', {login:login, password:password, rememberme: 1});
    }

    /*
    function isLoggedIn() {
        return _getCookie(SESSION_COOKIE) ? true : false;
    }

    function logout() {
        _deleteCookie(SESSION_COOKIE);
    }
    */

    var clientCreate = function() {
        return _net_call(rho.config.syncServer+'/clientcreate', "", "get", "text/plain");
    };

    var clientReset = function(id) {
        // Request: GET /application/clientreset?client_id=7771137f497b4a8789e62da321117f50
        return _net_call(rho.config.syncServer+'/clientreset', {client_id: id}, "get", "text/plain");
    };

    function serverQuery(srcName, clientId, pageSize, token) {
/*
        var params = $.extend({
            version: 3,
            client_id: clientId,
            p_size: pageSize
        }, srcName ? {source_name: srcName}:{}, token ? {token: token}:{});
*/
        var url = "?version=3" +"&client_id=" +clientId +"&p_size=" +pageSize;
        url += srcName ? ("&source_name=" +srcName) : "";
        url += token ? ("&token=" +token) : "";
        return _net_call(rho.config.syncServer+url, '', 'get', 'text/plain');
    }

    function postData(data) {
        return _net_call(rho.config.syncServer+'', data);
    }

    $.extend(rho, {protocol: publicInterface()});

})(jQuery);
