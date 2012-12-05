(function($) {

    function publicInterface() {
        return {
            // classes
            Client: Client,
            Source: Source,
            // fields
            states: states,
            getSession: function() {return session},
            restoreSession: restoreSession,
            sources: sources,
            sourcesArray: sourcesArray,
            maxConfigSrcId: 1,
            // methods
            login: login,
            logout: logout,
            getState: getState,
            setState: setState,
            isSearch: isInSearch,
            doSyncAllSources: doSyncAllSources,
            stopSync: stopSync,
            getNotify: getNotify,
            getSyncPageSize:  function() {return syncPageSize},
            getClientId:  function() {return clientId},
            getStartSourceId: getStartSourceId,
            findSourceBy: findSourceBy,
            getSourceOptions: getSourceOptions,
            isNoThreadedMode: isNoThreadedMode,
            isSessionExist: isSessionExist,
            isContinueSync: isContinueSync,
            setSchemaChanged: function(value) {schemaChanged = value},
            isSchemaChanged: isSchemaChanged,
            isStoppedByUser: function() {return isStoppedByUser}
        };
    }

    var rho = RhoSync.rho;

    var states = {
        none: 0,
        syncAllSources: 1,
        syncSource: 2,
        search: 3,
        stop: 4,
        exit: 5
    };

    var sources = {}; // name->source map
    // to be ordered by priority and associations after checkSourceAssociations() call
    var sourcesArray = [];

    var sourceOptions = new SourceOptions();

    function SourceOptions() {
        var srcOptions = {};

        this.setProperty = function(srcId, name, value) {
            var hashOptions = srcOptions[srcId];
            if (!hashOptions) {
                hashOptions = {};
                srcOptions[srcId] = hashOptions;
            }
            hashOptions[name] = value || "";
        };
        
        this.getProperty = function(srcId, name) {
            var hashOptions = srcOptions[srcId];
            if (hashOptions) {
                return hashOptions[name] || "";
            }
            return "";
        };

        this.getBoolProperty = function(nSrcID, szPropName)
        {
            var strValue = this.getProperty(nSrcID, szPropName);
            return (strValue == "1" || strValue == "true");
        }
    }

    var notify = null;
    function getNotify() {
        notify = notify || new rho.notify.SyncNotify(rho.engine);
        return notify;
    }

    var syncState = states.none;
    var isSearch = false;
    var errCode = rho.errors.ERR_NONE;
    var error = "";
    var serverError = "";
    var schemaChanged = false;
    var session = null;
    var clientId = null;
    var syncPageSize = 2000;


    var LOG = new rho.Logger('SyncEngine');

    function isInSearch() {
        return isSearch;
    }

    function isSchemaChanged() {
        return schemaChanged;
    }

    function restoreSession() {
        return $.Deferred(function(dfr){
            session = "";
            rho.storage.executeSql("SELECT session FROM client_info").done(function(tx, rs){
                for (var i=0; i<rs.rows.length; i++) {
                    var s = rs.rows.item(i)['session'];
                    if (s) {
                        session = s;
                        break;
                    }
                }
                rho.protocol.setSession(session);
                dfr.resolve();
            }).fail(_rejectOnDbAccessEror(dfr));
        }).promise();
    }

    function logout() {
        return $.Deferred(function(dfr){
            _cancelRequests();
            rho.storage.executeSql("UPDATE client_info SET session = NULL").done(function(){
                session = "";
                dfr.resolve();
            }).fail(_rejectOnDbAccessEror(dfr));
            //loadAllSources();
        }).promise();
    }

    function login(login, password, oNotify) {
        return $.Deferred(function(dfr){
            isStoppedByUser = false;
            
            rho.protocol.login(login, password).done(function(){
                session = rho.protocol.getSession();

                rho.config.database.name = rho.config.database.namePrefix + login;

                rho.storage.init(/*false - don't reset any data by default*/).done(function(){
                    if(!session) {
                        LOG.error("Return empty session.");
                        var errCode = rho.errors.ERR_UNEXPECTEDSERVERRESPONSE;
                        getNotify().callLoginCallback(oNotify, errCode, "" );
                        dfr.reject(errCode, "");
                        return;
                    }

                    if (isStoppedByUser) {
                        dfr.reject(rho.errors.ERR_CANCELBYUSER, "Stopped by user");
                        return;
                    }

                    _updateClientSession(rho.protocol.getSession()).done(function(client){
                        if (rho.config["rho_sync_user"]) {
                            var strOldUser = rho.config["rho_sync_user"];
                            if (name != strOldUser) {
                                //if (isNoThreadedMode()) {
                                //    // RhoAppAdapter.resetDBOnSyncUserChanged();
                                //} else {
                                //    // NetResponse resp1 = getNet().pushData( getNet().resolveUrl("/system/resetDBOnSyncUserChanged"), "", null );
                                //}
                            }
                        }

                        rho.config["rho_sync_user"] = name;
                        getNotify().callLoginCallback(oNotify, rho.errors.ERR_NONE, "" );

                        dfr.resolve();
                    }).fail(_rejectPassThrough(dfr));
                }).fail(_rejectPassThrough(dfr));

            }).fail(function(status, error, xhr){
                var errCode = rho.protocol.getErrCodeFromXHR(xhr);
                if (_isTimeout(error)) {
                    errCode = rho.errors.ERR_NOSERVERRESPONSE;
                }
                if (errCode != rho.errors.ERR_NONE) {
                    getNotify().callLoginCallback(oNotify, errCode, xhr.responseText);
                }
                dfr.reject(errCode, error);
            });
        }).promise();
    }

    function _updateClientSession(session) {
        return $.Deferred(function(dfr){
            // obtain client id from the server
            rho.storage.loadAllClients().done(function(clients){
                if (0 < clients.length) {
                    rho.storage.executeSql("UPDATE client_info SET session=?", [session]).done(function(tx, rs){
                        dfr.resolve(clients[0]);
                    }).fail(_rejectOnDbAccessEror(dfr));
                } else {
                    var client = new Client(null);
                    client.session = session;
                    rho.storage.insertClient(client).done(function(tx, client){
                        dfr.resolve(client);
                    }).fail(_rejectOnDbAccessEror(dfr));
                }
            }).fail(_rejectOnDbAccessEror(dfr));
        }).promise();
    }

    function _updateClientId(id) {
        return $.Deferred(function(dfr){
            // obtain client id from the server
            rho.storage.loadAllClients().done(function(tx, clients){
                if (0 < clients.length) {
                    rho.storage.executeSql("UPDATE client_info SET client_id=?", [id]).done(function(tx, rs){
                        dfr.resolve(id);
                    }).fail(_rejectOnDbAccessEror(dfr));
                } else {
                    var client = new Client(null);
                    client.id = id;
                    rho.storage.insertClient(client).done(function(tx, client){
                        dfr.resolve(id);
                    }).fail(_rejectOnDbAccessEror(dfr));
                }
            }).fail(_rejectOnDbAccessEror(dfr));
        }).promise();
    }

    function _createClient() {
        return $.Deferred(function(dfr){
            // obtain client id from the server
            rho.protocol.clientCreate().done(function(status, data){
                if (data && data.client && data.client.client_id){
                    // persist new client
                    _updateClientId(data.client.client_id).done(function(id){
                        dfr.resolve(id);
                    }).fail(_rejectPassThrough(dfr));
                } else {
                    dfr.reject(rho.errors.ERR_UNEXPECTEDSERVERRESPONSE, data);
                }
            }).fail(function(status, error){
                var errCode = _isTimeout(error) ? rho.errors.ERR_NOSERVERRESPONSE : rho.errors.ERR_NETWORK;
                dfr.reject(errCode, error);
            });
        }).promise();
    }

    function _resetClient(clientId) {
        return $.Deferred(function(dfr){
            rho.protocol.clientReset(clientId).done(function(status, data){
                if (data && data.sources){
                    dfr.resolve();
                } else {
                    dfr.reject(rho.errors.ERR_UNEXPECTEDSERVERRESPONSE, data);
                }
            }).fail(function(status, error){
                var errCode = _isTimeout(error) ? rho.errors.ERR_NOSERVERRESPONSE : rho.errors.ERR_NETWORK;
                dfr.reject(errCode, error);
            });

        }).promise();
    }

    function _isTimeout(msg) {
        return (msg && msg.match(/time(d)?\s+out/i));
    }

    function doSyncAllSources() {
        return $.Deferred(function(dfr){

            prepareSync(states.syncAllSources, null).done(function(){
                if (isContinueSync()) {
                    syncAllSources().done(function(){
                        _finally();
                        _localAfterIsContinueSync();
                    }).fail(function(errCode, errMsg){
                        _finally();
                        rho.notify.byEvent(rho.events.ERROR, "Sync failed", errMsg);
                        dfr.reject(errCode, errMsg);
                    });
                } else {_localAfterIsContinueSync();}

                function _finally(){
                    if (getState() != states.exit) {
                        setState(states.none);
                    }
                }

                function _localAfterIsContinueSync() {
                    getNotify().cleanCreateObjectErrors();
                    dfr.resolve();
                }
            }).fail(function(errCode, errMsg){
                dfr.reject(errCode, errMsg);
            });
        }).promise();
    }

    function prepareSync(eState, oSrcID) {
        return $.Deferred(function(dfr){
            setState(eState);
            isSearch =  (eState == states.search);
            isStoppedByUser = false;
            errCode = rho.errors.ERR_NONE;
            error = "";
            serverError = "";
            schemaChanged = false;

            loadAllSources().done(function(){
                loadSession().done(function(s){
                    session = s;
                    if (isSessionExist()) {
                        loadClientID().done(function(clnId){
                            clientId = clnId;
                            if (errCode == rho.errors.ERR_NONE) {
                                getNotify().cleanLastSyncObjectCount();
                                //doBulkSync();
                                dfr.resolve();
                                return;
                            }
                            _localFireErrorNotification();
                            stopSync();
                            dfr.reject(errCode, error);
                        }).fail(_rejectPassThrough(dfr));
                    }else {
                        errCode = rho.errors.ERR_CLIENTISNOTLOGGEDIN;
                        _localFireErrorNotification();
                        stopSync();
                        dfr.reject(errCode, error);
                    }

                    function _localFireErrorNotification() {
                        var src = null;
                        if (oSrcID) {
                            src = findSourceBy('id', oSrcID);
                        }
                        if (src) {
                            src.errCode = errCode;
                            src.error = error;
                            getNotify().fireSyncNotification(src, true, src.errCode, "");
                        } else {
                            getNotify().fireAllSyncNotifications(true, errCode, error, "");
                        }
                    }

                }).fail(_rejectPassThrough(dfr));
            }).fail(_rejectPassThrough(dfr));
        }).promise();
    }

    function findSourceBy(key, value) {
        for(var i = 0; i < sourcesArray.length; i++) {
            if ((sourcesArray[i])[key] == value)
                return sourcesArray[i];
        }
        return null;
    }

    function loadSession() {
        return $.Deferred(function(dfr){
            rho.storage.loadAllClients().done(function(tx, clients){
                var s = null;
                for (var i=0; i<clients.length; i++) {
                    if (clients[i].session) {
                        s = clients[i].session;
                        break;
                    }
                }
                dfr.resolve(s);
            }).fail(_rejectOnDbAccessEror(dfr));

        }).promise();
    }

    function loadClientID() {
        return $.Deferred(function(dfr){
            var clnId = '';
            var resetClient = false;
            
            rho.storage.loadAllClients().done(function(tx, clients){
                var client = null;

                if (0 < clients.length) {
                    client = clients[0];
                    clnId = client.id;
                    resetClient = client.reset;
                }

                if (!clnId) {
                    _createClient().done(function(id){
                        dfr.resolve(id);
                    }).fail(_rejectPassThrough(dfr));
                } else if (resetClient) {
                    _resetClient(clnId).done(function(clientId){
                        client.reset = 0;
                        rho.storage.storeClient(client).done(function(){
                            dfr.resolve(clientId);
                        }).fail(function(obj, err){
                            stopSync();
                            _rejectOnDbAccessEror(dfr)(obj, err);
                        });
                    }).fail(function(errCode, error){
                        stopSync();
                        dfr.reject(errCode, error);
                    });
                } else {
                    dfr.resolve(clnId);
                }
            }).fail(_rejectOnDbAccessEror(dfr));
        }).promise();
    }

    function loadAllSources() {
        return $.Deferred(function(dfr){
            //if (isNoThreadedMode()) {
            //    // RhoAppAdapter.loadAllSyncSources();
            //} else {
            //    // getNet().pushData( getNet().resolveUrl("/system/loadallsyncsources"), "", null );
            //}
            sources = {};

            rho.storage.loadAllSources().done(function(tx, srcs){
                $.each(srcs, function(idx, src){
                    if (src.sync_type == 'none') return;
                    src.storage = rho.storage;
                    src.engine = rho.engine;
                    sources[src.name] = src;
                });
                checkSourceAssociations();
                dfr.resolve();
            }).fail(_rejectOnDbAccessEror(dfr));
        }).promise();
    }


    function checkSourceAssociations() {
        var hashPassed = {};

        function _insertIntoArray(array, index, value) {
            if (index >= array.length) return array.concat(value);  
            if (index < 0) index = 0;
            return array.slice(0, index).concat(value, array.slice(index));
        }

        function _findSrcIndex(srcArray, strSrcName) {
            for (var i = 0; i < srcArray.length; i++) {
                if (strSrcName == srcArray[i].name) return i;
            }
            return -1;
        }

        // map to array
        var srcArray = [];
        $.each(sources, function(name, src){
            srcArray.push(src);
        });
        // sorted by priority
        srcArray.sort(function(srcA, srcB){
            if(srcA.sync_priority < srcB.sync_priority) return -1;
            if(srcA.sync_priority > srcB.sync_priority) return +1;
            return 0;
        });

        
        for(var nCurSrc = srcArray.length-1; nCurSrc > 0;) {
            var oCurSrc = srcArray[nCurSrc];
            if (oCurSrc.getAssociations().length == 0 || oCurSrc.name in hashPassed ) {
                nCurSrc--;
            } else {
                var nSrc = nCurSrc;
                for(var i = 0; i < oCurSrc.getAssociations().length; i++) {
                    var oAssoc = oCurSrc.getAssociations()[i];
                    var nAssocSrcIndex = _findSrcIndex(srcArray, oAssoc.m_strSrcName);
                    if (nAssocSrcIndex >=0 && nAssocSrcIndex < nSrc )
                    {
                        srcArray.splice(nSrc, 1);
                        _insertIntoArray(srcArray, nAssocSrcIndex, oCurSrc);
                        nSrc = nAssocSrcIndex;
                    }
                }
            }
            hashPassed[oCurSrc.name] = true;
        }

        // back to map
        sources = {};
        $.each(srcArray, function(idx, src){
            sources[src.name] = src;
        });
        // and instance field
        sourcesArray = srcArray;
    }

    function syncAllSources() {
        return $.Deferred(function(dfr){
            var isError = false;

            // The sources field may be inconsistent with sourceArray
            // field after checkSourceAssociations(), so we cannot
            // rely on it here. Going to build new map for deferred objects handling.
            var srcMap = {};
            $.each(sourcesArray, function(idx, src){
                srcMap[src.name] = src;
            });

            var dfrMap = rho.deferredMapOn($.extend({}, srcMap, {'rhoStartSyncSource': 'noMatterValue_itUseJustKeys'}));

            var syncErrors = [];

            var startSrcIndex = getStartSourceIndex();
            var startSrc = (0 <= startSrcIndex ? sourcesArray[startSrcIndex] : null);
            if (0 <= startSrcIndex) {
                syncOneSource(startSrcIndex).done(function(){
                    dfrMap.resolve('rhoStartSyncSource', ["ok"]);
                }).fail(function(errCode, error){
                    isError = true;
                    syncErrors.push({source: startSrc.name, errCode: errCode, error: error});
                    // We shouldn't stop the whole sync process on current source error,
                    // so resolve it instead of reject. Error is handled later.
                    dfrMap.resolve('rhoStartSyncSource', ["error", errCode, error]);
                });
            } else {
                dfrMap.resolve('rhoStartSyncSource', ["ok"]);
            }

            $.each(sourcesArray, function(i, src){
                syncOneSource(i).done(function(){
                    dfrMap.resolve(src.name, ["ok"]);
                }).fail(function(errCode, error){
                    isError = true;
                    syncErrors.push({source: src.name, errCode: errCode, error: error});
                    // We shouldn't stop the whole sync process on current source error,
                    // so resolve it instead of reject. Error is handled later.
                    dfrMap.resolve(src.name, ["error", errCode, error]);
                });
            });

            dfrMap.when().done(function(){
                if (!isError && !isSchemaChanged()) {
                    // TODO: to implement RhoAppAdapter.getMessageText("sync_completed")
                    getNotify().fireSyncNotification(null, true, rho.errors.ERR_NONE, "sync_completed");
                    dfr.resolve(rho.errors.NONE, "Sync completed");
                } else {
                    dfr.reject('Error for source"' +syncErrors[0].source +'": ' +(syncErrors[0].error||syncErrors[0].errCode));
                }
            }).fail(function(){
                // it shouldn't happen, because we resolving on errors
                LOG.error('Implementation error in SyncEngine.syncAllSources: some source has been rejected!');
                dfr.reject(syncErrors);
            });
        }).promise();
    }

    function getStartSourceIndex() {
        for(var i=0; i<sourcesArray.length; i++) {
            if (!sourcesArray[i].isEmptyToken()) return i;
        }
        return -1;
    }

    function syncOneSource(index) {
        return $.Deferred(function(dfr){
            var source = sourcesArray[index];
            
            if ( source.sync_type == "bulk_sync_only") {
                dfr.resolve(null); //TODO: do resolve it as a source?
            } else if (isSessionExist() && getState() != states.stop ) {
                source.sync().done(function(){
                    dfr.resolve(source);
                }).fail(function(obj, error){
                    if (source.errCode == rho.errors.ERR_NONE) {
                        source.errCode = rho.errors.ERR_RUNTIME;
                    }
                    setState(states.stop);
                    dfr.reject(rho.errors.ERR_RUNTIME, "sync is stopped: " +error);
                }).then(_finally, _finally);
                function _finally() {
                    getNotify().onSyncSourceEnd(index, sourcesArray);
                }
            } else {
                dfr.reject(rho.errors.ERR_RUNTIME, "sync is stopped");
            }
        }).promise();
    }

    function getState() { return syncState; }
    function setState(state) {
        syncState = state;
    }
    
    function isContinueSync() {
        var st = getState();
        return st != states.exit && st != states.stop;
    }

    function isSyncing() {
        var st = getState();
        return st == states.syncAllSources || st == states.syncSource;
    }

    function stopSync() {
        if (isContinueSync()) {
            setState(states.stop);
            _cancelRequests();
        }
    }

    function _cancelRequests() {
        //TODO: to implement
        /*
        if (m_NetRequest!=null)
            m_NetRequest.cancel();

        if (m_NetRequestClientID!=null)
            m_NetRequestClientID.cancel();
        */
    }

    function getStartSourceId(dbSources) {
        var startId = 0;
        $.each(dbSources, function(name, dbSource){
            startId = (dbSource.id > startId) ? dbSource.id : startId;
        });
        if (startId < rho.engine.maxConfigSrcId) {
            startId =  rho.engine.maxConfigSrcId + 2;
        } else {
            startId += 1;
        }
        return startId;
    }

    function getSourceOptions() {
        return sourceOptions;
    }

    function isNoThreadedMode() {
        return false;
    }

    function isSessionExist() {
        return session ? true : false;
    }

    var isStoppedByUser = false;

    function _stopSyncByUser() {
        isStoppedByUser = true;
        stopSync();
    }

    function _exitSync() {
        if (isContinueSync()) {
            setState(states.exit);
            _cancelRequests();
        }
    }

    function Source(id, name, syncType, storage, engine) {
        var LOG = new rho.Logger('SyncSource');

        this.storage = storage;
        this.engine = engine;

        this.id = id;
        this.name = name;
        this.token = null;
        this.sync_priority = null;  // bigint, no default
        this.partition = null;      // varchar, no default
        this.sync_type = syncType;  // varchar, no default
        this.metadata = null;
        this.last_updated = 0;
        this.last_inserted_size = 0;
        this.last_deleted_size = 0;
        this.last_sync_duration = 0;
        this.last_sync_success = 0;
        this.backend_refresh_time = 0;
        this.source_attribs = null;
        this.schema = null;
        this.schema_version = null;
        this.associations = null;
        this.blob_attribs = null;

        this.arAssociations = [];
        this.getAssociations = function() {
            return this.arAssociations;
        };

        this.isTokenFromDb = true;
        this.errCode = rho.errors.ERR_NONE;
        this.error = '';
        this.serverError = '';

        this.totalCount = 0;
        this.curPageCount = 0;
        this.serverObjectsCount = 0;

        this.insertedCount = 0;
        this.deletedCount = 0;

        this.getAtLeastOnePage = false;
        this.refreshTime = 0;

        this.multipartItems = [];
        this.blobAttrs = [];
        this.schemaSource = false;

        this.progressStep = -1;

        function SourceAssociation(strSrcName, strAttrib) {
            this.m_strSrcName = strSrcName;
            this.m_strAttrib = strAttrib;
        }

        this.isEmptyToken = function() {
            return this.token == 0;
        };

        this.setToken = function(token) {
            this.token = token;
            this.isTokenFromDb = false;
        };

        this.processToken = function(token) {
            var that = this;
            return $.Deferred(function(dfr){
                if ( token > 1 && that.token == token ){
                    //Delete non-confirmed records

                    that.setToken(token); //For m_bTokenFromDB = false;
                    //getDB().executeSQL("DELETE FROM object_values where source_id=? and token=?", getID(), token );
                    //TODO: add special table for id,token
                    dfr.resolve();
                }else
                {
                    that.setToken(token);
                    rho.storage.executeSql("UPDATE sources SET token=? where source_id=?", [+that.token, that.id]).done(function(){
                        dfr.resolve();
                    }).fail(_rejectOnDbAccessEror(dfr));
                }
            }).promise();
        };

        this.parseAssociations = function(strAssociations) {
            var that = this;
            if (!strAssociations) return;

            var srcName = "";
            $.each(strAssociations.split(','), function(idx, attrName){
                if (srcName) {
                    that.arAssociations.push(new SourceAssociation(srcName, attrName) );
                    srcName = "";
                } else {
                    srcName = attrName;
                }
            });
        };

        this.syncServerChanges = function() {
            var that = this;
            return $.Deferred(function(dfr){
                LOG.info("Sync server changes source ID :" + that.id);

                _localAsyncWhile();
                function _localAsyncWhile() {
                    that.curPageCount =0;

                    var strUrl = rho.protocol.getServerQueryUrl("");
                    var clnId = that.engine.getClientId();
                    var pgSize = that.engine.getSyncPageSize();
                    var tkn = (!that.isTokenFromDb && that.token>1) ? that.token:null;
                    LOG.info( "Pull changes from server. Url: " + (strUrl+_localGetQuery(that.name, clnId, pgSize, tkn)));

                    rho.protocol.serverQuery(that.name, clnId, pgSize, tkn
                            /*, that.engine*/).done(function(status, data, xhr){

                        //var testResp = that.engine.getSourceOptions().getProperty(that.id, "rho_server_response");
                        //data = testResp ? $.parseJSON(testResp) : data;

                        that.processServerResponse_ver3(data).done(function(){

                            if (that.engine.getSourceOptions().getBoolProperty(that.id, "pass_through")) {
                                that.processToken(0).done(function(){
                                    _localNextIfContinued();
                                }).fail(function(errCode, err) {
                                    _rejectPassThrough(dfr)(errCode, err);
                                });
                            } else {_localNextIfContinued();}

                            function _localNextIfContinued() {
                                if (that.token && that.engine.isContinueSync()) {
                                    // go next in async while loop
                                    _localAsyncWhile()
                                } else {
                                    _localWhileExit();
                                }
                            }

                        }).fail(function(errCode, err) {
                            _rejectPassThrough(dfr)(errCode, err);
                        });
                    }).fail(function(status, error, xhr){
                        that.engine.stopSync();
                        that.errCode = rho.protocol.getErrCodeFromXHR(xhr);
                        that.error = error;
                        dfr.reject(errCode, error);
                        //_localWhileExit(); //TODO: am I sure?
                    });
                }
                var _whileEnded = false;
                function _localWhileExit() {
                    if (!_whileEnded) {
                        _whileEnded = true;
                        if (that.engine.isSchemaChanged()) {
                            that.engine.stopSync();
                        }
                        dfr.resolve();
                    }
                }

                function _localGetQuery(srcName, clnId, pgSize, token) {
                    var strQuery = "?client_id=" + clnId +
                        "&p_size=" + pgSize + "&version=3";
                    strQuery += srcName ? ("&source_name=" + srcName) : '';
                    return strQuery += token ? ("&token=" + token) : '';
                }
            }).promise();
        };

        this.processServerResponse_ver3 = function(data) {
            var that = this;
            return $.Deferred(function(dfr){
                var itemIndex = 0;
                var item = null;
                
                item = data[itemIndex];
                if (undefined != item.version){
                    itemIndex++;
                    if (item.version != rho.protocol.getVersion()) {
                        LOG.error("Sync server send data with incompatible version. Client version: " +rho.protocol.getVersion()
                            +"; Server response version: " +item.version +". Source name: " +that.name);
                        that.engine.stopSync();
                        that.errrCode = rho.errors.ERR_UNEXPECTEDSERVERRESPONSE;
                        dfr.reject(that.errCode, "Sync server send data with incompatible version.");
                        return;
                    }
                }

                item = data[itemIndex];
                if (undefined != item.token){
                    itemIndex++;
                    that.processToken(+item.token).done(function(){
                        _localAfterProcessToken();
                    }).fail(function(errCode, error){
                        dfr.reject(that.errCode, error);
                    });
                } else {_localAfterProcessToken();}

                function _localAfterProcessToken() {
                    item = data[itemIndex];
                    if (undefined != item.source) {
                        itemIndex++;
                        //skip it. it uses in search only
                    }
                    item = data[itemIndex];
                    if (undefined != item.count) {
                        itemIndex++;
                        that.curPageCount = (+item.count);
                    }
                    item = data[itemIndex];
                    if (undefined != item['refresh_time']) {
                        itemIndex++;
                        that.refreshTime = (+item['refresh_time']);
                    }
                    item = data[itemIndex];
                    if (undefined != item['progress_count']) {
                        itemIndex++;
                        //TODO: progress_count
                        //setTotalCount(oJsonArr.getCurItem().getInt("progress_count"));
                    }
                    item = data[itemIndex];
                    if (undefined != item['total_count']) {
                        itemIndex++;
                        that.totalCount = (+item['total_count']);
                    }
                    //if ( getServerObjectsCount() == 0 )
                    //    that.getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");

                    if (that.token == 0) {
                        //oo conflicts
                        rho.storage.executeSql("DELETE FROM changed_values where source_id=? and sent>=3", [that.id]).done(function(){
                            _localAfterTokenIsZero();
                        }).fail(_rejectOnDbAccessEror(dfr));
                        //
                    } else {_localAfterTokenIsZero();}

                    function _localAfterTokenIsZero(){
                        LOG.info("Got " + that.curPageCount + "(Processed: " +  that.serverObjectsCount
                                + ") records of " + that.totalCount + " from server. Source: " + that.name
                                + ". Version: " + item.version );

                        if (that.engine.isContinueSync()) {
                            item = data[itemIndex];
                            itemIndex++;

                            var oCmds = item;

                            if (undefined != oCmds['schema-changed']) {
                                that.engine.setSchemaChanged(true);
                                _localAfterProcessServerErrors();
                            } else if (!that.processServerErrors(oCmds)) {
                                rho.storage.rwTx().ready(function(db, tx){
                                    if (that.engine.getSourceOptions().getBoolProperty(that.id, "pass_through")) {
                                        if (that.schemaSource) {
                                            //rho.storage.executeSql( "DELETE FROM " + that.name );
                                        } else {
                                            rho.storage.executeSql( "DELETE FROM object_values WHERE source_id=?", [that.id], tx).done(function(tx, rs){
                                                _localAfterDeleteObjectValues();
                                            }).fail(_rejectOnDbAccessEror(dfr));
                                        }
                                    } else {_localAfterDeleteObjectValues();}

                                    function _localAfterDeleteObjectValues() {
                                        if (undefined != oCmds["metadata"] && that.engine.isContinueSync() ) {
                                            var strMetadata = oCmds["metadata"];
                                            rho.storage.executeSql("UPDATE sources SET metadata=? WHERE source_id=?", [strMetadata, that.id], tx).done(function(){
                                                _localAfterSourcesUpdate();
                                            }).fail(_rejectOnDbAccessEror(dfr));
                                        } else {_localAfterSourcesUpdate();}

                                        function _localAfterSourcesUpdate(){
                                            if (undefined != oCmds["links"] && that.engine.isContinueSync() ) {
                                                that.processSyncCommand("links", oCmds["links"], true, tx);
                                            }
                                            if (undefined != oCmds["delete"] && that.engine.isContinueSync() ) {
                                                that.processSyncCommand("delete", oCmds["delete"], true, tx);
                                            }
                                            if (undefined != oCmds["insert"] && that.engine.isContinueSync() ) {
                                                that.processSyncCommand("insert", oCmds["insert"], true, tx);
                                            }

                                            that.getNotify().fireObjectsNotification();
                                            _localAfterProcessServerErrors();
                                        }
                                    }
                                }).done(function(db, status){
                                    dfr.resolve();
                                }).fail(function(obj,err){
                                    _rejectOnDbAccessEror(dfr)(obj,err);
                                });
                            } else {_localAfterProcessServerErrors();}

                            function _localAfterProcessServerErrors() {
                                _localAfterIfContinueSync();
                            }
                        } else {_localAfterIfContinueSync();}

                        function _localAfterIfContinueSync(){
                            if (that.curPageCount > 0) {
                                that.getNotify().fireSyncNotification(this, false, rho.errors.ERR_NONE, "");
                            }
                            dfr.resolve(); //TODO: do we need dfr.reject() on errors happen? reporting at least?
                        }
                    }
                }
            }).promise();
        };

        this.processSyncCommand = function(strCmd, oCmdEntry, bCheckUIRequest, tx) {
            var that = this;
            return $.Deferred(function(dfr){

                var dfrMap = rho.deferredMapOn(oCmdEntry);
                $.each(oCmdEntry, function(strObject, attrs){
                    if (!that.engine.isContinueSync()) return;

                    if (that.schemaSource) {
                        //that.processServerCmd_Ver3_Schema(strCmd,strObject,attrIter);
                    } else {
                        $.each(attrs, function(strAttrib, strValue){
                            if (!that.engine.isContinueSync()) return;

                            that.processServerCmd_Ver3(strCmd,strObject,strAttrib,strValue, tx).done(function(){
                                _localAfterIfSchemaSource();
                            }).fail(function(errCode, error){
                                LOG.error("Sync of server changes failed for " + that.name + "; \object: " + strObject, error);
                                dfrMap.reject(strObject, [errCode, error]);
                            });

                        });

                    } /* else {_localAfterIfSchemaSource()}*/

                    function _localAfterIfSchemaSource() {
                        dfrMap.resolve(strObject, [tx]);

                        if (that.sync_type == "none") {
                            return;
                        }

                        if (bCheckUIRequest) {
                            var nSyncObjectCount  = that.getNotify().incLastSyncObjectCount(that.id);
                            if ( that.progressStep > 0 && (nSyncObjectCount % that.progressStep == 0) ) {
                                that.getNotify().fireSyncNotification(this, false, rho.errors.ERR_NONE, "");
                            }

                            //TODO: to discuss with Evgeny
                            //if (getDB().isUIWaitDB()) {
                            //    LOG.INFO("Commit transaction because of UI request.");
                            //    getDB().endTransaction();
                            //    SyncThread.getInstance().sleep(1000);
                            //    getDB().startTransaction();
                            //}
                        }
                    }
                });
                dfrMap.when().done(function(){
                    dfr.resolve(tx);
                }).fail(_rejectPassThrough(dfr));

            }).promise();
        };

        function CAttrValue(strAttrib, strValue) {
            this.m_strAttrib = strAttrib;
            this.m_strValue = strValue;
            this.m_strBlobSuffix = "";

            if ("string" == typeof this.m_strAttrib && this.m_strAttrib.match(/\-rhoblob$/)) {
                this.m_strBlobSuffix = "-rhoblob";
                this.m_strAttrib = this.m_strAttrib.substring(0, this.m_strAttrib.length-this.m_strBlobSuffix.length);
            }
        }

        this.processServerCmd_Ver3 = function(strCmd, strObject, strAttrib, strValue, tx) {
            var that = this;
            return $.Deferred(function(dfr){

                var oAttrValue = new CAttrValue(strAttrib,strValue);

                if (strCmd == "insert") {

                    //if ( !processBlob(strCmd,strObject,oAttrValue) )
                    //    return;

                    rho.storage.executeSql("SELECT source_id FROM object_values "+
                            "WHERE object=? and attrib=? and source_id=? LIMIT 1 OFFSET 0",
                            [strObject, oAttrValue.m_strAttrib, that.id], tx).done(function(tx, rs){
                        if (0 == rs.rows.length) {
                            rho.storage.executeSql("INSERT INTO object_values "+
                                    "(attrib, source_id, object, value) VALUES(?,?,?,?)",
                                    [oAttrValue.m_strAttrib, that.id, strObject, oAttrValue.m_strValue], tx).done(function(tx, rs){

                                _localAfterInserOrUpdate();
                            }).fail(_rejectOnDbAccessEror(dfr));

                        } else {
                            
                            rho.storage.executeSql("UPDATE object_values " +
                                "SET value=? WHERE object=? and attrib=? and source_id=?",
                                 [oAttrValue.m_strValue, strObject, oAttrValue.m_strAttrib, that.id], tx).done(function(tx, rs){

                                if (that.sync_type != "none") {
                                    // oo conflicts
                                    rho.storage.executeSql("UPDATE changed_values SET sent=4 where object=? "+
                                            "and attrib=? and source_id=? and sent>1",
                                            [strObject, oAttrValue.m_strAttrib, that.id], tx).done(function(tx, rs){
                                        _localAfterSyncTypeNone();
                                    }).fail(_rejectOnDbAccessEror(dfr));
                                    //
                                } else {_localAfterSyncTypeNone();}

                                function _localAfterSyncTypeNone() {
                                    _localAfterInserOrUpdate();
                                }
                            }).fail(_rejectOnDbAccessEror(dfr));

                        }
                    }).fail(_rejectOnDbAccessEror(dfr));

                    function _localAfterInserOrUpdate() {
                        if (that.sync_type != "none") {
                            that.getNotify().onObjectChanged(that.id, strObject, rho.notify.actions.update);
                        }
                        that.insertedCount++;
                        dfr.resolve(tx);
                    }

                } else if (strCmd == "delete") {

                    rho.storage.executeSql("DELETE FROM object_values where object=? and attrib=? and source_id=?",
                            [strObject, oAttrValue.m_strAttrib, that.id], tx).done(function(tx, rs){

                        if (that.sync_type != "none") {
                            that.getNotify().onObjectChanged(that.id, strObject, rho.notify.actions['delete']);
                            // oo conflicts
                            rho.storage.executeSql("UPDATE changed_values SET sent=3 where object=? "+
                                    "and attrib=? and source_id=?",
                                    [strObject, oAttrValue.m_strAttrib, that.id], tx).done(function(tx, rs){
                                _localAfterSyncTypeNone();
                            }).fail(_rejectOnDbAccessEror(dfr));
                            //
                        } else {_localAfterSyncTypeNone();}

                        function _localAfterSyncTypeNone() {
                            that.deletedCount++;
                            dfr.resolve(tx);
                        }
                    }).fail(_rejectOnDbAccessEror(dfr));

                } else if (strCmd == "links") {

                    that.processAssociations(strObject, oAttrValue.m_strValue, tx).done(function(tx){
                        rho.storage.executeSql("UPDATE object_values SET object=? where object=? and source_id=?",
                                [oAttrValue.m_strValue, strObject, that.id], tx).done(function(){
                            rho.storage.executeSql("UPDATE changed_values SET object=?,sent=3 where object=? "+
                                    "and source_id=?",
                                    [oAttrValue.m_strValue, strObject, that.id], tx).done(function(){
                                that.getNotify().onObjectChanged(that.id, strObject, rho.notify.actions.create);
                                dfr.resolve(tx);
                            }).fail(_rejectOnDbAccessEror(dfr));
                        }).fail(_rejectOnDbAccessEror(dfr));
                    }).fail(_rejectPassThrough(dfr));
                }
            }).promise();
        };

        this.processAssociations = function(strOldObject, strNewObject, tx) {
            var that = this;
            return $.Deferred(function(dfr){
                if (that.associations.length == 0) {
                    dfr.resolve();
                    return;
                }

                var dfrMap = rho.deferredMapOn(that.associations);
                //TODO: do we need recursion (via .done()) here?
                for (var i=0; i < that.associations.length; i++) {
                    var pSrc = engine.findSourceBy('name', (/*(SourceAssociation)*/that.associations[i]).m_strSrcName);
                    if (pSrc) {
                        pSrc.updateAssociation(strOldObject, strNewObject,
                                (/*(SourceAssociation)*/that.associations[i]).m_strAttrib, tx).done(function(){
                            dfrMap.resolve(i, []);
                        }).fail(function(errCode, err){
                            dfrMap.reject(i, [errCode, err]);
                        });
                    }
                }
                dfrMap.when().done(function(){
                    dfr.resolve(tx);
                }).fail(_rejectPassThrough(dfr));
            }).promise();
        };

        this.updateAssociation = function (strOldObject, strNewObject, strAttrib, tx) {
            var that = this;
            return $.Deferred(function(dfr){
                if (that.schemaSource) {
                    //var strSqlUpdate = "UPDATE ";
                    //strSqlUpdate += that.name + " SET " + strAttrib + "=? where " + strAttrib + "=?";
                    //
                    //rho.storage.executeSql(strSqlUpdate, [strNewObject, strOldObject], tx).done(function(){
                    //    _localAfterIfSchemaSource();
                    //}).fail(_rejectOnDbAccessEror(dfr));

                    _localAfterIfSchemaSource(); // because real logic is commented out above
                } else {
                    rho.storage.executeSql("UPDATE object_values SET value=? where attrib=? and source_id=? and value=?",
                        [strNewObject, strAttrib, that.id, strOldObject], tx).done(function(){
                        _localAfterIfSchemaSource();
                    }).fail(_rejectOnDbAccessEror(dfr));
                } /* else {_localAfterIfSchemaSource();}*/

                function _localAfterIfSchemaSource() {
                    rho.storage.executeSql("UPDATE changed_values SET value=? "+
                            "where attrib=? and source_id=? and value=?",
                            [strNewObject, strAttrib, that.id, strOldObject], tx).done(function(){
                        dfr.resolve(tx);
                    }).fail(_rejectOnDbAccessEror(dfr));
                }
            }).promise();
        };

        this.processServerErrors = function(oCmds) {
            var that = this;
            var errorsFound = false;

            $.each(oCmds, function(errType, errObj){
                if (errType.match(/^(source|search)-error$/i)) {
                    _localSetSourceErrors(errType);
                } else if (errType.match(/-error$/i)) {
                    _localSetObjectErrors(errType);
                }
            });

            function _localSetSourceErrors(errType) {
                errorsFound = true;
                that.ErrCode =rho.errors.ERR_CUSTOMSYNCSERVER;

                $.each(oCmds[errType], function(errSubtype, errObj){
                    that.serverError += that.serverError ? '&' : '';
                    that.serverError += "server_errors[" + encodeURI(errSubtype) + "][message]=" + encodeURI(errObj["message"]);
                });
            }

            function _localSetObjectErrors(errType) {
                errorsFound = true;
                that.ErrCode =rho.errors.ERR_CUSTOMSYNCSERVER;

                $.each(oCmds[errType], function(objId, err){
                    if (objId.match(/-error$/i)) {
                        // it is object error message
                        objId = objId.substring(0, objId.length-'-error'.length);
                        that.serverError += that.serverError ? '&' : '';
                        that.serverError += "server_errors[" + encodeURI(errType) + "][" + encodeURI(objId) + "][message]=" + encodeURI(err["message"]);
                    } else {
                        // it is object error attribs
                        $.each(err, function(attrName, attrValue){
                            that.serverError += that.serverError ? '&' : '';
                            that.serverError += "server_errors[" + encodeURI(errType) + "][" + encodeURI(objId) + "][attributes][" + encodeURI(attrName) + "]=" + encodeURI(attrValue);
                        });
                    }
                });
            }
            return errorsFound;
        };

        this.syncClientChanges = function() {
            var that = this;
            return $.Deferred(function(dfr){

                var bSyncedServer = false;

                that.isPendingClientChanges().done(function(found){
                    if (found) {
                        LOG.info( "Client has unconfirmed created items. Call server to update them." );
                        that.syncServerChanges().done(function(){
                            bSyncedServer = true;
                            _localAfterIfClientHaveUnconfirmedItems();
                        }).fail(_rejectPassThrough(dfr));
                    } else {_localAfterIfClientHaveUnconfirmedItems();}
                }).fail(_rejectOnDbAccessEror(dfr));

                function _localAfterIfClientHaveUnconfirmedItems() {

                    that.isPendingClientChanges().done(function(found){
                        if (bSyncedServer && found) {
                            LOG.info( "Server does not sent created items. Stop sync." );
                            that.engine.setState(states.stop);
                            _localAfterIfServerSentCreatedItems();
                        } else {
                            rho.storage.executeSql("SELECT object FROM changed_values "+
                                    "WHERE source_id=? LIMIT 1 OFFSET 0", [that.id]).done(function(tx, rs){
                                var bSyncClient = false;
                                // TODO: to investigate later
                                // some interference between webkit debugger and rs.rows happens here,
                                // so extra checks were added to eliminate the problem.
                                bSyncClient = (rs.rows && rs.rows.length && 0 < rs.rows.length);
                                //bSyncClient = (0 < rs.rows.length);

                                if (bSyncClient) {
                                    that.doSyncClientChanges().done(function(){

                                        bSyncedServer = false;
                                        _localAfterIfSyncClient();
                                    }).fail(_rejectPassThrough(dfr));
                                } else {_localAfterIfSyncClient();}

                                function _localAfterIfSyncClient() {
                                    _localAfterIfServerSentCreatedItems();
                                }
                            }).fail(_rejectOnDbAccessEror(dfr));
                        } /* else {_localAfterIfServerSentCreatedItems();}*/
                    }).fail(_rejectOnDbAccessEror(dfr));

                    function _localAfterIfServerSentCreatedItems() {
                        // just a stub
                        //dfr.resolve(false /*it means: no, server changes hasn't been synchronized*/);
                        dfr.resolve(bSyncedServer);
                    }
                }
            }).promise();
        };

        this.isPendingClientChanges = function() {
            var that = this;
            return $.Deferred(function(dfr){
                rho.storage.executeSql("SELECT object FROM changed_values "+
                        "WHERE source_id=? and update_type='create' and sent>1  LIMIT 1 OFFSET 0",
                        [that.id]).done(function(tx, rs){
                    // TODO: to investigate later
                    // some interference between webkit debugger and rs.rows happens here,
                    // so extra checks were added to eliminate the problem.
                    dfr.resolve(rs.rows && rs.rows.length && 0 < rs.rows.length);
                    //dfr.resolve(0 < rs.rows.length);
                }).fail(function(obj, err){
                    dfr.reject(obj, err);
                });
            }).promise();
        };

        this.doSyncClientChanges = function() {
            var that = this;
            return $.Deferred(function(dfr){

                var arUpdateTypes = ["create", "update", "delete"];
                var arUpdateSent = {};

                that.multipartItems = [];
                that.blobAttrs = [];

                var bSend = false;

                var body = {
                    source_name: that.name,
                    client_id: that.engine.getClientId()
                };

                var dfrMap = rho.deferredMapOn(arUpdateTypes);
                $.each(arUpdateTypes, function(idx, updateType){
                    if (that.engine.isContinueSync()) {
                        arUpdateSent[updateType] = true;
                        bSend = true;
                        that.makePushBody_Ver3(updateType, true).done(function(part){
                            body[updateType] = part;
                            dfrMap.resolve(idx, []);
                        }).fail(function(obj, err){
                            dfrMap.reject(idx, [obj, err]);
                        });
                    } else {
                        dfrMap.resolve(idx, []);
                    }
                });
                dfrMap.when().done(function(){
                    _localAfterBodyUpdatePartsPrepared();
                }).fail(_rejectPassThrough(dfr));


                function _localAfterBodyUpdatePartsPrepared() {
                    /*
                    var blobPart = {blob_fields: []};
                    $.each(that.blobAttrs, function(idx, id){
                        blobPart.blob_fields.push(id);
                    });
                    body = $.extend(body, blobPart);
                    */

                    if (bSend) {
                        LOG.info( "Push client changes to server. Source: " + that.name);
                        LOG.trace("Push body: " + $.toJSON(body));

                        if (that.multipartItems.length > 0) {
                            /*
                            MultipartItem oItem = new MultipartItem();
                            oItem.m_strBody = strBody;
                            //oItem.m_strContentType = getProtocol().getContentType();
                            oItem.m_strName = "cud";
                            m_arMultipartItems.addElement(oItem);

                            NetResponse resp = getNet().pushMultipartData( getProtocol().getClientChangesUrl(), m_arMultipartItems, getSync(), null );
                            if ( !resp.isOK() )
                            {
                                getSync().setState(SyncEngine.esStop);
                                m_nErrCode = RhoAppAdapter.ERR_REMOTESERVER;
                                m_strError = resp.getCharData();
                            }
                            */
                            _localAfterIfMultipartItems();
                        } else {
                            rho.protocol.postData(body).done(function(status, data, xhr){
                                _localAfterIfMultipartItems();
                            }).fail(function(status, error, xhr){
                                that.engine.setState(rho.states.stop);
                                that.errCode = rho.protocol.getErrCodeFromXHR(xhr);
                                that.errCode = _isTimeout(error) ? rho.errors.ERR_NOSERVERRESPONSE : that.errCode;
                                that.error = error;
                                dfr.reject(that.errCode, that.error);
                            });
                        } /* else {_localAfterIfMultipartItems();}*/
                    } else {_localAfterIfMultipartItems();}

                    function _localAfterIfMultipartItems() {
                        var dfrMap = rho.deferredMapOn(arUpdateSent);

                        $.each(arUpdateSent, function(updateType, isDone){
                            if (that.engine.isContinueSync() && isDone /*isDone is always true, no false values there*/) {
                                //oo conflicts
                                if (updateType == 'create') {
                                    that.storage.executeSql("UPDATE changed_values SET sent=2 "+
                                            "WHERE source_id=? and update_type=? and sent=1",
                                            [that.id, updateType]).done(function(){
                                        dfrMap.resolve(updateType, []);
                                    }).fail(function(obj, err){
                                        dfrMap.reject(updateType, [rho.errors.ERR_RUNTIME, "db access error: " +err]);
                                    });
                                } else {
                                //
                                    that.storage.executeSql("DELETE FROM changed_values "+
                                            "WHERE source_id=? and update_type=? and sent=1",
                                            [that.id, updateType]).done(function(){
                                        dfrMap.resolve(updateType, []);
                                    }).fail(function(obj, err){
                                        dfrMap.reject(updateType, [rho.errors.ERR_RUNTIME, "db access error: " +err]);
                                    });
                                }
                            } else {
                                dfrMap.resolve(updateType, []);
                            }
                        });

                        dfrMap.when().done(function(){
                            that.multipartItems = [];
                            that.blobAttrs = [];
                            dfr.resolve();
                        }).fail(_rejectPassThrough(dfr));
                    }
                }

            }).promise();
        };

        this.makePushBody_Ver3 = function(strUpdateType, isSync) {
            var that = this;
            return $.Deferred(function(dfr){
                var bodyPart = {};

                //getDB().Lock(); //TODO: ?!
                if (isSync) {
                    _updateAllAttribChanges().done(function(){
                        _localAfterChangesUpdated();
                    }).fail(_rejectOnDbAccessEror(dfr));
                } else {_localAfterChangesUpdated();}

                function _localAfterChangesUpdated() {
                    rho.storage.executeSql("SELECT attrib, object, value, attrib_type "+
                        "FROM changed_values where source_id=? and update_type =? and sent<=1 ORDER BY object",
                            [that.id, strUpdateType]).done(function(tx, rs){
                        _localSelectedChangedValues(tx, rs);
                    }).fail(_rejectOnDbAccessEror(dfr));

                    function _localSelectedChangedValues(tx, rs) {
                        // TODO: to investigate later
                        // some interference between webkit debugger and rs.rows happens here,
                        // so extra checks were added to eliminate the problem.
                        //if (0 == rs.rows.length) {
                        if (rs.rows && rs.rows.length && 0 == rs.rows.length) {
                            //getDB().Unlock(); //TODO: ?!
                            dfr.resolve(bodyPart);
                            return;
                        }
                        for(var i=0; i<rs.rows.length; i++) {
                            var strAttrib = rs.rows.item(i)['attrib'];
                            var strObject = rs.rows.item(i)['object'];
                            var value = rs.rows.item(i)['value'];
                            var attribType = rs.rows.item(i)['update_type'];

                            if (attribType == "blob.file") {
                                //MultipartItem oItem = new MultipartItem();
                                //oItem.m_strFilePath = RhodesApp.getInstance().resolveDBFilesPath(value);
                                //oItem.m_strContentType = "application/octet-stream";
                                //oItem.m_strName = strAttrib + "-" + strObject;
                                //
                                //m_arBlobAttrs.addElement(strAttrib);
                                //m_arMultipartItems.addElement(oItem);
                            }
                            if (!bodyPart[strObject]) bodyPart[strObject] = {};
                            bodyPart[strObject][strAttrib] = value;
                        }
                        if (isSync) {
                            rho.storage.executeSql("UPDATE changed_values SET sent=1 "+
                                    "WHERE source_id=? and update_type=? and sent=0",
                                    [that.id, strUpdateType]).done(function(){
                                dfr.resolve(bodyPart);
                            }).fail(_rejectOnDbAccessEror(dfr));
                        }
                        //getDB().Unlock(); //TOOD: ?!
                    }
                }
            }).promise();
        };

        function _updateAllAttribChanges() {
            var that = this;
            return $.Deferred(function(dfr){
                //Check for attrib = object
                rho.storage.rwTx().ready(function(db, tx){
                    rho.storage.executeSql("SELECT object, source_id, update_type " +
                        "FROM changed_values where attrib = 'object' and sent=0", [], tx).done(function(tx, rsChanges){

                        if (0 == rsChanges.rows.length)  return; //TODO: dfr.resolve() ?!
                        _localChangedValuesSelectedInTx(tx, rsChanges);

                    })/*.fail(_rejectOnDbAccessEror(dfr))*/;
                }).done(function(){
                    dfr.resolve();
                }).fail(function(obj,err){
                    _rejectOnDbAccessEror(dfr)(obj,err);
                });

                function _localChangedValuesSelectedInTx(tx, rsChanges) {
                    var arObj = [];
                    var arUpdateType = [];
                    var arSrcID = [];

                    for (var i=0; i<rsChanges.rows.length; i++) {
                        arObj.push(rsChanges.rows.item(i)['object']);
                        arSrcID.push(rsChanges.rows.item(i)['source_id']);
                        arUpdateType.push(rsChanges.rows.item(i)['update_type']);
                    }

                    var dfrMap = rho.deferredMapOn(arObj);
                    $.each(arObj, function(objIdx, obj) {
                        rho.storage.executeSql("SELECT name, schema FROM sources " +
                                "WHERE source_id=?", [arSrcID[objIdx]], tx).done(function(tx, resSrc){

                            var isSchemaSrc = false;
                            var strTableName = "object_values";
                            if (resSrc.rows.length > 0) {
                                isSchemaSrc = (resSrc.rows.item(0)['schema']);
                                if (isSchemaSrc)
                                    strTableName = resSrc.rows.item(0)['name'];
                            }

                            if (isSchemaSrc) {
                                /*
                                IDBResult res2 = executeSQL( "SELECT * FROM " + strTableName + " where object=?", arObj.elementAt(i) );
                                for( int j = 0; j < res2.getColCount(); j ++)
                                {
                                    String strAttrib = res2.getColName(j);
                                    String value = res2.getStringByIdx(j);
                                    String attribType = getAttrMgr().isBlobAttr((Integer)arSrcID.elementAt(i), strAttrib) ? "blob.file" : "";

                                    executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)",
                                            arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, new Integer(0) );
                                }
                                */
                                dfrMap.resolve(objIdx, []);
                            } else {
                                rho.storage.executeSql("SELECT attrib, value FROM " + strTableName +
                                        " where object=? and source_id=?",
                                         [obj, arSrcID[objIdx]], tx).done(function(tx, rsAttribs){
                                    _localSelectedAttribs(tx, rsAttribs);
                                }).fail(_rejectOnDbAccessEror(dfr));

                                function _localSelectedAttribs(tx, rsAttribs) {
                                    for (var attrIdx=0; attrIdx<rsAttribs.rows.length; attrIdx++) {
                                        var strAttrib = rsAttribs.rows.item(attrIdx)['attrib'];
                                        var value = rsAttribs.rows.item(attrIdx)['value'];

                                        var attribType = rho.storage.attrManager.isBlobAttr(arSrcID[objIdx], strAttrib) ? "blob.file" : "";

                                        rho.storage.executeSql("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)",
                                            [arSrcID[objIdx], obj, strAttrib, value, arUpdateType[objIdx], attribType, 0], tx).done(function(){
                                        }).fail(_rejectOnDbAccessEror(dfr));
                                    }
                                    dfrMap.resolve(objIdx, []);
                                }
                            }
                        }).fail(function(obj, err){
                            dfrMap.reject(objIdx, [obj, err]);
                        });
                    });

                    dfrMap.when().done(function(){
                        rho.storage.executeSql("DELETE FROM changed_values WHERE attrib=?", ['object'], tx);
                    }).fail(_rejectOnDbAccessEror(dfr));
                }
            }).promise();
        }

        this.sync = function(){
            var that = this;
            return $.Deferred(function(dfr){
                //TODO: to implement RhoAppAdapter.getMessageText("syncronizing")
                that.getNotify().reportSyncStatus("syncronizing" + that.name + "...", that.errCode, that.error);

                var startTime = Date.now();

                if (that.isTokenFromDb && that.token > 1) {
                    that.syncServerChanges().done(function(){
                        _finally();
                        dfr.resolve();
                    }).fail(_catch);
                } else {
                    if (that.isEmptyToken()) {
                        that.processToken(1).done(function(){
                            _localSyncClient();
                        }).fail(_catch);
                    } else {_localSyncClient();}

                    function _localSyncClient() {
                        that.syncClientChanges().done(function(serverSyncDone){
                            if (!serverSyncDone) that.syncServerChanges().done(function(){
                                _finally();
                                dfr.resolve();
                            }).fail(_catch);
                        }).fail(_catch);
                    }
                }
                function _catch(errCode, error) {
                    that.engine.stopSync();
                    _finally();
                    dfr.reject(errCode, error);
                }
                function _finally() {
                    var endTime = Date.now();

                    rho.storage.executeSql(
                            "UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, "
                            +"last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?",
                            [(endTime/1000), that.getInsertedCount(), that.getDeletedCount(),
                              endTime - startTime,
                              (that.getAtLeastOnePage ? 1 : 0), that.refreshTime, that.id] );
                }
            }).promise();
        };

        this.getNotify = function() {
            return this.engine.getNotify();
        };

        this.getInsertedCount = function() {
            return this.insertedCount;
        };

        this.getDeletedCount = function() {
            return this.deletedCount;
        }

    }

    function _rejectOnDbAccessEror(deferred) {
        return function(obj, error){
            var err = error;
            //if ('object' == typeof error && undefined != error['message']) {
            //    err = error.message;
            //}
            deferred.reject(rho.errors.ERR_RUNTIME, "db access error: " +err);
        };
    }

    function _rejectPassThrough(deferred){
        return function(errCode, err){
            deferred.reject(errCode, err);
        }
    }

    function Client(id) {
        this.id = id;
        this.session = null;
        this.token = null;
        this.token_sent = 0;
        this.reset = 0;
        this.port = null;
        this.last_sync_success = null;
    }


    $.extend(rho, {engine: publicInterface()});

})(jQuery);
