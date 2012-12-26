var RhoSync = (function($) {

    function publicInterface() {
        return {
            errors: errors,
            init: init,
            login: login,
            logout: logout,
            isLoggedIn: isLoggedIn,
            getLoggedInUsername: getLoggedInUsername,
            syncAllSources: syncAllSources
        };
    }

    var defaults = {
        syncServer: '',
        pollInterval: 20,
        database: {
            namePrefix: 'rhoSyncDb_',
            name: 'UNINITIALIZED',
            version: '1.0',
            comment: 'RhoSync database',
            size: (2*1024*1024)
        }
    };

    var errors = {
        ERR_NONE: 'No error',
        ERR_NETWORK: 'Network error',
        ERR_REMOTESERVER: 'Remote server access error',
        ERR_RUNTIME: 'Runtime error',
        ERR_UNEXPECTEDSERVERRESPONSE: 'Unexpected server response',
        ERR_DIFFDOMAINSINSYNCSRC: 'Different synchronization domain error',
        ERR_NOSERVERRESPONSE: 'No server response',
        ERR_CLIENTISNOTLOGGEDIN: 'Client not logged in',
        ERR_CUSTOMSYNCSERVER: 'Custom sync server',
        ERR_UNATHORIZED: 'Unauthorized access',
        ERR_CANCELBYUSER: 'Canceled by user',
        ERR_SYNCVERSION: 'Synchronization version error',
        ERR_GEOLOCATION: 'Geolocation error'
    };

    var events = {
        GENERIC_NOTIFICATION: 'rhoSyncGenericNotification',
        ERROR: 'rhoSyncError',
        CLIENT_CREATED: 'rhoSyncClientCreated',
        STATUS_CHANGED: 'rhoSyncStatusChanged',
        SYNCHRONIZING: 'rhoSyncSourceSynchronizing',
        SYNC_SOURCE_END: 'rhoSyncSourceSynchronizationEnd'
    };

    function init(modelDefs, storageType, syncProgressCb, doReset) {
        return $.Deferred(function(dfr){
            rho.storage.init(doReset).done(function(){
                rho.engine.restoreSession().done(function(){
                    _resetModels();
                    _loadModels(storageType, modelDefs, syncProgressCb).done(function(){
                        dfr.resolve();
                    }).fail(function(obj, error){
                        dfr.reject("models load error: " +error);
                    });
                }).fail(function(errCode, error){
                    dfr.reject("session restoring error: " +error);
                });
            }).fail(function(error){
                dfr.reject("storage initialization error: " +error);
            });
        }).promise();
    }

    function login(login, password, oNotify) {
        return $.Deferred(function(dfr){
            rho.engine.login(login, password, oNotify).done(function(){
                dfr.resolve();
            }).fail(function(errCode, errMsg){
                dfr.reject(errCode, errMsg);
            });
        }).promise();
    }

    function logout() {
        return rho.engine.logout();
    }

    function isLoggedIn() {
        return rho.engine.isSessionExist();
    }

    function getLoggedInUsername() {
        if (!isLoggedIn()) return null;
        var md64str = rho.engine.getSession();
        md64str = md64str.replace(/--.*$/, '').replace(/%0A/, '');
        md64str = md64str.replace(/\s/, '');
        md64str = md64str.replace(/^.*?login"/, '');
        return md64str;
    }

    function syncAllSources() {
        return rho.engine.doSyncAllSources();
    }

    function _initDbSources(tx, configSources) {
        return $.Deferred(function(dfr){
            rho.storage.loadAllSources(tx).done(function (tx, dbSources) {

                var startId = rho.engine.getStartSourceId(dbSources);

                var dbSourceMap = {};
                $.each(dbSources, function(idx, src){
                    dbSourceMap[src.name] = src;
                });

                var dfrMap = {}; // to resolve/reject each exact item
                var dfrs = []; // to watch on all of them
                $.each(configSources, function(name, cfgSource){
                    var dfr = new $.Deferred();
                    dfrMap[name] = dfr;
                    dfrs.push(dfr.promise());
                });

                $.each(configSources, function(name, cfgSource){
                    // if source from config is already present in db
                    var dbSource = dbSourceMap[cfgSource.name];
                    if (dbSource) { // then update it if needed
                        var updateNeeded = false;

                        if (dbSource.sync_priority != cfgSource.sync_priority) {
                            dbSource.sync_priority = cfgSource.sync_priority;
                            updateNeeded = true;
                        }
                        if (dbSource.sync_type != cfgSource.sync_type) {
                            dbSource.sync_type = cfgSource.sync_type;
                            updateNeeded = true;
                        }
                        if (dbSource.associations != cfgSource.associations) {
                            dbSource.associations = cfgSource.associations;
                            updateNeeded = true;
                        }
                        if (!cfgSource.id) {
                            cfgSource.id = dbSource.id;
                        }
                        if (updateNeeded) {
                            rho.storage.storeSource(dbSource, tx).done(function(tx, source){
                                dfrMap[name].resolve(source);
                            }).fail(function(obj, err){
                                dfrMap[name].reject(obj, err);
                            });
                        }
                    } else { // if configured source not in db yet
                        if (!cfgSource.id) {
                            cfgSource.id = startId;
                            startId =+ 1;
                        }
                        rho.storage.insertSource(cfgSource, tx).done(function(tx, source){
                            dfrMap[name].resolve(source);
                        }).fail(function(obj, err){
                            dfrMap[name].reject(obj, err);
                        });
                    }
                });
                $.when(dfrs).done(function(resolvedDfrs){
                    dfr.resolve();
                }).fail(function(obj, err){
                    dfr.reject(obj, err);
                });
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function _initSources(sources) {
        return $.Deferred(function(dfr){
            $.each(sources, function(name, source){
                source.associations = '';
            });
            $.each(sources, function(name, source){
                if (!source || !source.model || !source.model.belongsTo) return;
                $.each(source.model.belongsTo, function(keyAttr, ownerName){
                    var ownerSrc = sources[ownerName];
                    if (!ownerSrc) {
                        //TODO: report the error
                        //puts ( "Error: belongs_to '#{source['name']}' : source name '#{src_name}' does not exist."  )
                        return;
                    }
                    var str = ownerSrc.associations || '';
                    str += (0 < str.length) ? ', ' : '';
                    str += (source.name +', ' +keyAttr);
                    ownerSrc.associations = str;
                });
            });

            rho.storage.open().done(function(db){
                rho.storage.rwTx(db).ready(function(db, tx){
                    _initDbSources(tx, sources).done(function(){
                        dfr.resolve();
                    }).fail(function(obj, err) {
                        dfr.reject(obj, err);
                    });
                    //initSyncSourceProperties(sources, tx);
                }).fail(function(obj, err){
                    //TODO: report the error
                    dfr.reject(obj, err);
                });
            });
        }).promise();
    }


    var models = {}; // name->model map

    var allModelsLoaded = false;

    function _resetModels() {
        models = {};
        allModelsLoaded = false;
    }
    
    function _loadModels(storageType, modelDefs, syncProgressCb) {
        if (allModelsLoaded) return $.Deferred().resolve().promise();

        function _addLoadedModel(defn) {
            var model = new rho.domain.Model(defn);
            model.source.sync_priority = parseInt(defn['sync_priority'] || 1000);
            model.source.sync_type = 'incremental';
            model.source.partition = 'user';
            var sourceId = defn['source_id'] ? parseInt(defn['source_id']) : null;
            model.source.id = sourceId;
            if (sourceId && rho.engine.maxConfigSrcId < sourceId) {
                rho.engine.maxConfigSrcId = sourceId;
            }
            models[defn.name] = model;
            rho.engine.sources[defn.name] = model.source;
        }

        function _loadModel(defn) {
            if (!defn || defn.isLoaded) return;
            defn.isLoaded = true;
            if ('string' == typeof defn.name) {
                _addLoadedModel(defn)
            }
        }

        if (modelDefs && 'object' == typeof modelDefs) {
            if ($.isArray(modelDefs)) {
                $.each(modelDefs, function(idx, defn){_loadModel(defn);});
            } else {
                _loadModel(modelDefs);
            }
        }
        allModelsLoaded = true;

        return _initSources(rho.engine.sources).done(function(){
            $.each(rho.engine.sources, function(name, src){
                rho.engine.getNotify().setNotification(src, new rho.notify.SyncNotification(function(){
                    if ("function" == typeof syncProgressCb) {
                        syncProgressCb(name);
                        return false;
                    }
                }, false));
            });
        });
    }

    // rhosync internal parts we _have_to_ make a public
    var rho = {
        config: $.extend({}, defaults, RhoConfig),
        events: events,
        models: models,

        domain: null,
        protocol: null,
        engine: null,
        notify: null,
        storage: null
    };

    return $.extend(publicInterface(), {rho: rho});

})(jQuery);
