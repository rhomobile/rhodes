(function($) {

    function publicInterface() {
        return {
        // AttrManager
        attrManager: attrManager,
        // Client
        listClientsId: listClientsId,
        loadClient: loadClient,
        loadAllClients: loadAllClients,
        storeClient: storeClient,
        insertClient: insertClient,
        deleteClient: deleteClient,
        // Client
        listSourcesId: listSourcesId,
        loadSource: loadSource,
        loadAllSources: loadAllSources,
        storeSource: storeSource,
        insertSource: insertSource,
        deleteSource: deleteSource,
        // low-level
        init: _init,
        open: _open,
        tx: _tx,
        roTx: _roTx,
        rwTx: _rwTx,
        executeSql: _executeSql,
        executeBatchSql: _executeBatchSql,
        initSchema: _initSchema,
        getAllTableNames: _getAllTableNames
        };
    }

    var rho = RhoSync.rho;

    var initDbSchemaSQL = ''
        +'DROP TABLE IF EXISTS client_info;'
        +'CREATE TABLE client_info ('
                +' "client_id" VARCHAR(255) default NULL,'
                +' "session" VARCHAR(255) default NULL,'
                +' "token" VARCHAR(255) default NULL,'
                +' "token_sent" BIGINT default 0,'
                +' "reset" BIGINT default 0,'
                +' "port" VARCHAR(10) default NULL,'
                +' "last_sync_success" VARCHAR(100) default NULL);'
        +'DROP TABLE IF EXISTS object_values;'
        +'CREATE TABLE object_values ('
                +' "source_id" BIGINT default NULL,'
                +' "attrib" varchar(255) default NULL,'
                +' "object" varchar(255) default NULL,'
                +' "value" varchar default NULL);'
        +'DROP TABLE IF EXISTS changed_values;'
        +'CREATE TABLE changed_values ('
                +' "source_id" BIGINT default NULL,'
                +' "attrib" varchar(255) default NULL,'
                +' "object" varchar(255) default NULL,'
                +' "value" varchar default NULL,'
                +' "attrib_type" varchar(255) default NULL,'
                +' "update_type" varchar(255) default NULL,'
                +' "sent" BIGINT default 0);'
        +'DROP TABLE IF EXISTS sources;'
        +'CREATE TABLE sources ('
                +' "source_id" BIGINT PRIMARY KEY,'
                +' "name" VARCHAR(255) default NULL,'
                +' "token" BIGINT default NULL,'
                +' "sync_priority" BIGINT,'
                +' "partition" VARCHAR(255),'
                +' "sync_type" VARCHAR(255),'
                +' "metadata" varchar default NULL,'
                +' "last_updated" BIGINT default 0,'
                +' "last_inserted_size" BIGINT default 0,'
                +' "last_deleted_size" BIGINT default 0,'
                +' "last_sync_duration" BIGINT default 0,'
                +' "last_sync_success" BIGINT default 0,'
                +' "backend_refresh_time" BIGINT default 0,'
                +' "source_attribs" varchar default NULL,'
                +' "schema" varchar default NULL,'
                +' "schema_version" varchar default NULL,'
                +' "associations" varchar default NULL,'
                +' "blob_attribs" varchar default NULL);'
        +'DROP INDEX IF EXISTS by_src_id;'
        +'CREATE INDEX by_src_id on object_values ("source_id");'
        +'DROP INDEX IF EXISTS by_src_object;'
        +'CREATE UNIQUE INDEX by_src_object ON object_values ("object", "attrib", "source_id");'
        +'DROP INDEX IF EXISTS by_src_value;'
        +'CREATE INDEX by_src_value ON object_values ("attrib", "source_id", "value");'
        ;

    // low-level functions ========================

    var _dbMap = {};

    function _open(name, version, comment, size) {
        function _getDb(name, version) {
            if (!_dbMap[name]) {
                _dbMap[name] = {};
            }
            if (!_dbMap[name][version]) {
                _dbMap[name][version] = null;
            }
            return _dbMap[name][version];
        }
        function _setDb(name, version, db) {
            if (!_dbMap[name]) {
                _dbMap[name] = {};
            }
            _dbMap[name][version] = db;
            return db;
        }

        return $.Deferred(function(dfr){
            var nm = name || rho.config.database.name;
            var vn = version || rho.config.database.version;
            var ct = comment || rho.config.database.comment;
            var sz = size || rho.config.database.size;

            var db = _getDb(nm, vn);

            if (db) {
                dfr.resolve(db);
                return;
            }
            try {
                db = _setDb(nm, vn, openDatabase(nm, vn, ct, sz));
                dfr.resolve(db);
            } catch(ex) {
                dfr.reject(ex);
            }
        }).promise();
    }

    function _tx(readWrite, optionalDb) {
        var readyDfr = $._Deferred();
        var dfr = $.Deferred();
        function resolveTx(db) {
            if (readWrite && readWrite != "read-only") {
                db.transaction(function(tx){
                    readyDfr.resolve(db, tx);
                }, function (err) {
                    dfr.reject(db, err);
                }, function(){
                    dfr.resolve(db, "ok");
                });
            } else {
                db.readTransaction(function(tx){
                    readyDfr.resolve(db, tx);
                }, function (err) {
                    dfr.reject(db, err);
                }, function(){
                    dfr.resolve(db, "ok");
                });
            }
        }
        if (optionalDb) {
             resolveTx(optionalDb);
        } else {
            _open().done(function(db){
               resolveTx(db);
            }).fail(function(ex){
                dfr.reject(null, ex);
            });
        }
        dfr.promise();
        dfr.ready = readyDfr.done;
        return dfr;
    }

    function _roTx(optionalDb) {
        return _tx(false, optionalDb);
    }

    function _rwTx(optionalDb) {
        return _tx("read-write", optionalDb);
    }

    function _executeSql(sql, values, optionalTx) {
        return $.Deferred(function(dfr){
            function execInTx(tx, sql, values) {
                tx.executeSql(sql, values, function(tx, rs){
                    dfr.resolve(tx, rs);
                }, function(tx, err){
                    dfr.reject(tx, err);
                });
            }
            if (optionalTx) {
                execInTx(optionalTx, sql, values);
            } else {
                // ok, going to use new tx from default database
                var readWrite = !sql.match(/^\s*select\s+/i);
                _tx(readWrite).ready(function(db, tx){
                    execInTx(tx, sql, values);
                }).done(function(obj, status){
                    dfr.resolve(obj, status);
                }).fail(function(obj, err){
                    dfr.reject(obj, err);
                });
            }
        }).promise();
    }

    function _executeBatchSql(sql, optionalTx)
    {
        var statements = sql.replace(/^\s+|;\s*$/, '').split(";");

        var dfrs = [];
        var dfrIdx = 0;

        // Deferred object for wrapping db/tx
        var dfr = $.Deferred();
        dfrs.push(dfr);
        dfrIdx++;

        // Accumulate deferred objects for aggregate
        // resolving, one per each statement
        $.each(statements, function(idx, val){
            dfrs.push($.Deferred());
        });

        function execBatchInTx(tx, sqlArray) {
            var dfr = dfrs[dfrIdx];
            if (0 < sqlArray.length) {
                var sql = sqlArray.shift();
                // execute current statement
                _executeSql(sql, null, tx).done(function(tx, rs){
                    // so far, so good
                    dfr.resolve(tx, rs, sql);
                    // execute next statement recursively
                    dfrIdx++;
                    execBatchInTx(tx, sqlArray);
                }).fail(function(tx, err){
                    dfr.reject(tx, err);
                });
            }
        }

        if(optionalTx) {
            execBatchInTx(optionalTx, statements);
        } else {
            _tx("read-write" /*anything evaluated as true*/).ready(function(db, tx){
                execBatchInTx(tx, statements);
            }).done(function(obj, status){
                dfr.resolve(obj, status);
            }).fail(function(obj, err){
                dfr.reject(obj, err);
            });
        }

        var promises = [];
        $.each(dfrs, function(idx, dfr){
            promises.push(dfr.promise());
        });

        return $.when.apply(this, promises);
    }

    function _initSchema()
    {
        return _executeBatchSql(initDbSchemaSQL);
    }

    function _getAllTableNames(optionalTx)
    {
        return $.Deferred(function(dfr){
            _executeSql("SELECT name FROM sqlite_master WHERE type='table'",
                    null, optionalTx).done(function(tx, rs){
                var tableNames = [];
                for(var i=0; i<rs.rows.length; i++) {
                    tableNames.push(rs.rows.item(i)['name']);
                }
                dfr.resolve(tx, tableNames);
            }).fail(function(obj, err){
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function _init(doReset) {
        return $.Deferred(function(dfr){
            _getAllTableNames().done(function(tx, names){
                if ((4/*explicitly defined tables*/ +1/*implicit system table*/) != names.length
                        || doReset) {
                    _initSchema().done(function(){
                        dfr.resolve(null, "db schema initialized");
                    }).fail(function(obj, err){
                        dfr.reject(obj, "db schema initialization error: " +err);
                    });
                }
                dfr.resolve(null, "db schema is ok");
            }).fail(function(obj, err){
                dfr.reject(obj, "db tables read error: " +err);
            });
        }).promise();
    }

    // Client-related ========================

    function listClientsId(optionalTx) {
        return $.Deferred(function(dfr){
            _executeSql('SELECT client_id FROM client_info', null, optionalTx).done(function(tx, rs) {
                var ids = [];
                for(var i=0; i<rs.rows.length; i++) {
                    ids.push(rs.rows.item(i)['client_id']);
                }
                dfr.resolve(tx, ids);
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function loadClient(id, optionalTx) {
        return $.Deferred(function(dfr){
            _executeSql('SELECT * FROM client_info WHERE client_id = ?', [id],
                    optionalTx).done(function(tx, rs) {
                if (0 == rs.rows.length) {
                    dfr.reject(id, 'Not found');
                } else {
                    var client = new rho.engine.Client(id);
                    client.session           = rs.rows.item(0)['session'];
                    client.token             = rs.rows.item(0)['token'];
                    client.token_sent        = rs.rows.item(0)['token_sent'];
                    client.reset             = rs.rows.item(0)['reset'];
                    client.port              = rs.rows.item(0)['port'];
                    client.last_sync_success = rs.rows.item(0)['last_sync_success'];
                    dfr.resolve(tx, client);
                }
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function loadAllClients(optionalTx) {
        return $.Deferred(function(dfr){
            _executeSql('SELECT * FROM client_info', null, optionalTx).done(function(tx, rs) {
                var clients = [];
                for(var i=0; i<rs.rows.length; i++) {
                    var client = new rho.engine.Client(rs.rows.item(i)['client_id']);
                    client.session           = rs.rows.item(i)['session'];
                    client.token             = rs.rows.item(i)['token'];
                    client.token_sent        = rs.rows.item(i)['token_sent'];
                    client.reset             = rs.rows.item(i)['reset'];
                    client.port              = rs.rows.item(i)['port'];
                    client.last_sync_success = rs.rows.item(i)['last_sync_success'];
                    clients.push(client);
                }
                dfr.resolve(tx, clients);
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function storeClient(client, optionalTx, isNew) {
        var updateQuery = 'UPDATE client_info SET'
            +' session = ?,'
            +' token = ?,'
            +' token_sent = ?,'
            +' reset = ?,'
            +' port = ?,'
            +' last_sync_success = ?'
            +' WHERE client_id = ?';
        var insertQuery = 'INSERT INTO client_info ('
            +' session,'
            +' token,'
            +' token_sent,'
            +' reset,'
            +' port,'
            +' last_sync_success,'
            +' client_id'
            +' ) VALUES (?, ?, ?, ?, ?, ?, ?)';
        return $.Deferred(function(dfr){
            _executeSql(isNew ? insertQuery : updateQuery, [
                client.session,
                client.token,
                client.token_sent,
                client.reset,
                client.port,
                client.last_sync_success,
                client.id], optionalTx).done(function(tx, rs) {
                dfr.resolve(tx, client);
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function insertClient(client, optionalTx) {
        return storeClient(client, optionalTx, true);
    }

    function deleteClient(clientOrId, optionalTx) {
        var id = ("object" == typeof clientOrId) ? clientOrId.id : clientOrId;
        return $.Deferred(function(dfr){
            _executeSql('DELETE FROM client_info WHERE client_id = ?', [id], optionalTx).done(function(tx, rs) {
                    dfr.resolve(tx, null);
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    // Source-related ========================

    function listSourcesId(optionalTx) {
        return $.Deferred(function(dfr){
            _executeSql('SELECT source_id FROM sources', null, optionalTx).done(function(tx, rs) {
                var ids = [];
                for(var i=0; i<rs.rows.length; i++) {
                    ids.push(rs.rows.item(i)['source_id']);
                }
                dfr.resolve(tx, ids);
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function loadSource(id, optionalTx) {
        return $.Deferred(function(dfr){
            _executeSql('SELECT * FROM sources WHERE source_id = ?', [id],
                    optionalTx).done(function(tx, rs) {
                if (0 == rs.rows.length) {
                    dfr.reject(id, 'Not found');
                } else {
                    var source = new rho.engine.Source(
                            rs.rows.item(0)['source_id'],
                            rs.rows.item(0)['name'],
                            rs.rows.item(0)['sync_type'],
                            rho.storage,
                            rho.engine
                            );
                    source.token                = rs.rows.item(0)['token'];
                    source.sync_priority        = rs.rows.item(0)['sync_priority'];
                    source.partition            = rs.rows.item(0)['partition'];
                    source.sync_type            = rs.rows.item(0)['sync_type'];
                    source.metadata             = rs.rows.item(0)['metadata'];
                    source.last_updated         = rs.rows.item(0)['last_updated'];
                    source.last_inserted_size   = rs.rows.item(0)['last_inserted_size'];
                    source.last_deleted_size    = rs.rows.item(0)['last_deleted_size'];
                    source.last_sync_duration   = rs.rows.item(0)['last_sync_duration'];
                    source.last_sync_success    = rs.rows.item(0)['last_sync_success'];
                    source.backend_refresh_time = rs.rows.item(0)['backend_refresh_time'];
                    source.source_attribs       = rs.rows.item(0)['source_attribs'];
                    source.schema               = rs.rows.item(0)['schema'];
                    source.schema_version       = rs.rows.item(0)['schema_version'];
                    source.associations         = rs.rows.item(0)['associations'];
                    source.blob_attribs         = rs.rows.item(0)['blob_attribs'];
                    source.parseAssociations();
                    dfr.resolve(tx, source);
                }
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function loadAllSources(optionalTx) {
        return $.Deferred(function(dfr){
            _executeSql('SELECT * FROM sources ORDER BY sync_priority', null, optionalTx).done(function(tx, rs) {
                var sources = [];
                for(var i=0; i<rs.rows.length; i++) {
                    var source = new rho.engine.Source(
                            rs.rows.item(i)['source_id'],
                            rs.rows.item(i)['name'],
                            rs.rows.item(i)['sync_type'],
                            rho.storage,
                            rho.engine
                            );
                    source.token                = rs.rows.item(i)['token'];
                    source.sync_priority        = rs.rows.item(i)['sync_priority'];
                    source.partition            = rs.rows.item(i)['partition'];
                    source.sync_type            = rs.rows.item(i)['sync_type'];
                    source.metadata             = rs.rows.item(i)['metadata'];
                    source.last_updated         = rs.rows.item(i)['last_updated'];
                    source.last_inserted_size   = rs.rows.item(i)['last_inserted_size'];
                    source.last_deleted_size    = rs.rows.item(i)['last_deleted_size'];
                    source.last_sync_duration   = rs.rows.item(i)['last_sync_duration'];
                    source.last_sync_success    = rs.rows.item(i)['last_sync_success'];
                    source.backend_refresh_time = rs.rows.item(i)['backend_refresh_time'];
                    source.source_attribs       = rs.rows.item(i)['source_attribs'];
                    source.schema               = rs.rows.item(i)['schema'];
                    source.schema_version       = rs.rows.item(i)['schema_version'];
                    source.associations         = rs.rows.item(i)['associations'];
                    source.blob_attribs         = rs.rows.item(i)['blob_attribs'];
                    source.parseAssociations();
                    sources.push(source);
                }
                dfr.resolve(tx, sources);
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function storeSource(source, optionalTx, isNew) {
        var updateQuery = 'UPDATE sources SET'
            +' name = ?,'
            +' token = ?,'
            +' sync_priority = ?,'
            +' partition = ?,'
            +' sync_type = ?,'
            +' metadata = ?,'
            +' last_updated = ?,'
            +' last_inserted_size = ?,'
            +' last_deleted_size = ?,'
            +' last_sync_duration = ?,'
            +' last_sync_success = ?,'
            +' backend_refresh_time = ?,'
            +' source_attribs = ?,'
            +' schema = ?,'
            +' schema_version = ?,'
            +' associations = ?,'
            +' blob_attribs = ?'
            +' WHERE source_id = ?';
        var insertQuery = 'INSERT INTO sources ('
            +' name,'
            +' token,'
            +' sync_priority,'
            +' partition,'
            +' sync_type,'
            +' metadata,'
            +' last_updated,'
            +' last_inserted_size,'
            +' last_deleted_size,'
            +' last_sync_duration,'
            +' last_sync_success,'
            +' backend_refresh_time,'
            +' source_attribs,'
            +' schema,'
            +' schema_version,'
            +' associations,'
            +' blob_attribs,'
            +' source_id'
            +' ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)';
        return $.Deferred(function(dfr){
            _executeSql(isNew ? insertQuery : updateQuery, [
                source.name,
                source.token,
                source.sync_priority,
                source.partition,
                source.sync_type,
                source.metadata,
                source.last_updated,
                source.last_inserted_size,
                source.last_deleted_size,
                source.last_sync_duration,
                source.last_sync_success,
                source.backend_refresh_time,
                source.source_attribs,
                source.schema,
                source.schema_version,
                source.associations,
                source.blob_attribs,
                source.id], optionalTx).done(function(tx, rs) {
                dfr.resolve(tx, source);
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    function insertSource(source, optionalTx) {
        return storeSource(source, optionalTx, true);
    }

    function deleteSource(sourceOrId, optionalTx) {
        var id = ("object" == typeof sourceOrId) ? sourceOrId.id : sourceOrId;
        return $.Deferred(function(dfr){
            _executeSql('DELETE FROM sources WHERE source_id = ?', [id], optionalTx).done(function(tx, rs) {
                    dfr.resolve(tx, null);
            }).fail(function(obj, err) {
                dfr.reject(obj, err);
            });
        }).promise();
    }

    var attrManager = new AttrManager();

    function AttrManager() {
        this.blobAttrs = {};
        this.srcNames = {};

        this.initAttrManager = function() {
            return this.loadAttrs();
        };

        this.isBlobAttr = function(nSrcID, szAttr) {
            var mapAttr = this.blobAttrs[nSrcID];
            return mapAttr ? (szAttr in mapAttr) : false;
        };

        this.loadAttrs = function() {
            var that = this;
            return $.Deferred(function(dfr){

                that.blobAttrs = {};
                var strSql = "SELECT source_id,";
                strSql += "blob_attribs" + ",name from sources";

                rho.storage.executeSql(strSql).done(function(tx, rs){
                    for (var i=0; i<rs.rows.length; i++) {

                        var nSrcID = rs.rows.item(0)['source_id'];
                        var strAttribs = rs.rows.item(0)['blob_attribs'];
                        if (!strAttribs) return;

                        var mapAttr = new {};
                        var strAttr = "";

                        $.each(strAttribs.split(','), function(idx, tok){
                            if (!tok) return;
                            if (strAttr) {
                                mapAttr[strAttr] = +tok;
                                strAttr = "";
                            } else {
                                strAttr = tok;
                            }
                        });

                        that.blobAttrs[nSrcID] = mapAttr;
                        if ( that.srcNames != null )
                            that.srcNames[rs.rows.item(i)['name'].toUpperCase()] = nSrcID;
                    }
                    dfr.resolve();
                }).fail(function(obj, err){
                    dfr.reject(obj, err);
                });
            }).promise();
        }
    }

    $.extend(rho, {storage: publicInterface()});

})(jQuery);
