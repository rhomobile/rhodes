(function($) {

    function publicInterface() {
        return {
            Logger: Logger,
            errors: RhoSync.errors,
            deferredMapOn: deferredMapOn,
            passRejectTo: passRejectTo
        };
    }

    var rho = RhoSync.rho;

    function Logger(name) {

        var levels = {
            trace: 0,
            info: 1,
            warning: 2,
            error: 3,
            fatal: 4
        };
        var levelTag = ['Trace', 'Info', 'Warning', 'Error', 'Fatal'];

        var level = parseLogLevel(rho.config.logLevel);

        this.trace = function(message) {
            var l = levels.trace;
            if (level > l) return;
            withConsole(function(c){
                c.info(buildMsg(l, message));
            });
        };

        this.info = function(message) {
            var l = levels.info;
            if (level > l) return;
            withConsole(function(c){
                c.info(buildMsg(l, message));
            });
        };

        this.warning = function(message, exception) {
            var l = levels.warning;
            if (level > l) return;
            withConsole(function(c){
                c.warn(buildMsg(l, message));
                if (exception) {
                    c.warn('EXCEPTION: ' +exception);
                }
            });
        };

        this.error = function(message, exception) {
            var l = levels.error;
            if (level > l) return;
            withConsole(function(c){
                c.error(buildMsg(l, message));
                if (exception) {
                    c.error('EXCEPTION: ' +exception);
                }
            });
        };

        this.fatal = function(message, exception) {
            var l = levels.fatal;
            if (level > l) return;
            withConsole(function(c){
                c.error(buildMsg(l, message));
                if (exception) {
                    c.error('EXCEPTION: ' +exception);
                }
            });
        };

        function parseLogLevel(name) {
            var isValid = ("string" == typeof name && name.toLowerCase() in levels);
            return isValid ? levels[name.toLowerCase()] : levels.warning;
        }

        function withConsole(callback) {
          if (window.console) {
            callback(window.console)
          }
        }

        function buildMsg(severity, text) {
            var date = Date().replace(/\S+\s(.*?)\sGMT\+.*$/, '$1');
            return date +' [' +levelTag[severity] +']' +' (' +name +') ' +text;
        }
    }

    function passRejectTo(dfr, doReport) {
        return function() {
            if (doReport) {
                //TODO: some log output
            }
            dfr.reject(arguments);
        };
    }

    function deferredMapOn(obj) {
        var dfrMap = {}; // to resolve/reject each exact item
        var dfrs = []; // to watch on all of them

        $.each(obj, function(key, value){
            var dfr = new $.Deferred();
            dfrMap[key] = dfr;
            dfrs.push(dfr.promise());
        });

        return {
            resolve: function(key, args) {
                if (dfrMap[key]) dfrMap[key].resolve.apply(dfrMap[key], args);
            },
            reject: function(key, args) {
                if (dfrMap[key]) dfrMap[key].reject.apply(dfrMap[key], args);
            },
            when: function() {
                return $.when.apply(this, dfrs);
            }
        };
    }

    $.extend(rho, publicInterface());
    $.extend(RhoSync, {Logger: Logger});

})(jQuery);
