(function($, Ext) {

    var baseTempId = null;

    /**
     * @author DmitryP@rhomobile.com
     * @class Ext.data.RhosyncStorageProxy
     * @extends Ext.data.ClientProxy
     *
     * <p>The RhosyncStorageProxy uses the new HTML5 WebSQL API to save {@link Ext.data.Model Model} data locally on
     * the client browser in the database instance with Rhosync schema.</p>
     *
     * @constructor
     * Creates the proxy, throws an error if Rhosync WebSQL database is not available
     * @param {Object} config Optional config object
     */
    Ext.data.RhosyncStorageProxy = Ext.extend(Ext.data.ClientProxy, {

        LOG: new RhoSync.rho.Logger('Rhosync.ext.js'),

        /**
         * @cfg {String} dbName The Rhosync database instance name to store all record data
         */
        dbName: undefined,

        /**
         * @ignore
         */
        constructor: function(config) {
            Ext.data.RhosyncStorageProxy.superclass.constructor.call(this, config);

            //ensures that the reader has been instantiated properly
            this.setReader(this.reader);

            if (this.getStorageObject() == undefined) {
                throw "Rhosync Storage is not available, please ensure you have rhosync.js scripts properly loaded";
            }

            //if an dbName is not given, try to use the store's id instead
            this.dbName = this.dbName || (this.store ? this.store.storeId : undefined);

            if (this.dbName == undefined) {
                throw "No database name was provided to the rhosync storage proxy. " +
                        "See Ext.data.RhosyncStorageProxy documentation for details";
            }

            this.initialize();
        },

        //inherit docs
        create: function(operation, callback, scope) {
            var that = this;
            var records = operation.records;
            var id;

            operation.setStarted();

            var dfrMap = RhoSync.rho.deferredMapOn(records);

            $.each(records, function(i, record){
                if (record.phantom) {
                    record.phantom = false;
                    id = that.getNextId();
                } else {
                    id = record.getId();
                }
                that.setRecord(record, id).done(function(record){
                    dfrMap.resolve(i, [record]);
                }).fail(function(obj, err){
                    dfrMap.reject(i, [obj, err]);
                });
            });

            dfrMap.when().done(function(){
                operation.setSuccessful();
                _localAfterCreate();
            }).fail(function(){
                _localAfterCreate();
                that.LOG.error('update() object update error');
            });

            function _localAfterCreate() {
                operation.setCompleted();

                if (typeof callback == 'function') {
                    callback.call(scope || this, operation);
                }
            }
        },

        //inherit docs
        read: function(operation, callback, scope) {
            //TODO: respect sorters, filters, start and limit options on the Operation
            var that = this;
            var modelName = that.model.modelName;
            var reader = that.getReader();
            var records = [];

            //read a single record
            if (operation.id) {
                that.getRecord(operation.id).done(function(record){
                    if (record) {
                        records.push(record);
                        operation.setSuccessful();
                    }
                    _localAfterRead();
                }).fail(function(obj, err){
                    that.LOG.error('read() single object read error: ' +err);
                    _localAfterRead();
                });
            } else {
                that.findAll(modelName).done(function(recs){
                    records = recs;
                    operation.setSuccessful();
                    _localAfterRead();
                }).fail(function(obj, err){
                    that.LOG.error('read() all objects read error: ' +err);
                    _localAfterRead();
                });
            }

            function _localAfterRead() {
                var result = null;

                if (that.root) {
                    var rooted = {};
                    rooted.name = modelName;
                    rooted[that.root] = records;
                    result = reader.read(rooted);
                } else {
                    result = reader.read(records);
                }

                Ext.apply(operation, {
                    resultSet: result
                });

                operation.setCompleted();

                if (typeof callback == 'function') {
                    callback.call(scope || that, operation);
                }
            }
        },

        //inherit docs
        update: function(operation, callback, scope) {
            var that = this;
            var records = operation.records;

            operation.setStarted();

            var dfrMap = RhoSync.rho.deferredMapOn(records);

            $.each(records, function(i, record){
                that.setRecord(record).done(function(record){
                    dfrMap.resolve(i, [record]);
                }).fail(function(obj, err){
                    dfrMap.reject(i, [obj, err]);
                });
            });

            dfrMap.when().done(function(){
                operation.setSuccessful();
                _localAfterUpdate();
            }).fail(function(){
                _localAfterUpdate();
                that.LOG.error('update() object update error');
            });

            function _localAfterUpdate() {
                operation.setCompleted();

                if (typeof callback == 'function') {
                    callback.call(scope || this, operation);
                }
            }
        },

        //inherit
        destroy: function(operation, callback, scope) {
            var that = this;
            var records = operation.records;

            operation.setStarted();

            var dfrMap = RhoSync.rho.deferredMapOn(records);

            $.each(records, function(i, record){
                that.removeRecord(record).done(function(record){
                    dfrMap.resolve(i, [record]);
                }).fail(function(obj, err){
                    dfrMap.reject(i, [obj, err]);
                });
            });

            dfrMap.when().done(function(){
                operation.setSuccessful();
                _localAfterDelete();
            }).fail(function(){
                _localAfterDelete();
                that.LOG.error('update() object update error');
            });

            function _localAfterDelete() {
                operation.setCompleted();

                if (typeof callback == 'function') {
                    callback.call(scope || this, operation);
                }
            }

        },

        findAll: function(srcName) {
            var that = this;
            var storage = that.getStorageObject();

            function _setupObject(map, id, attrib, value) {
                if (!map[id]) map[id] = {};
                map[id][attrib] = value;
            }

            function _buildRecord(id, objAttrs) {
                var data    = {},
                    Model   = that.model,
                    fields  = Model.prototype.fields.items,
                    length  = fields.length,
                    i, field, name, record;
                for (i = 0; i < length; i++) {
                    field = fields[i];
                    name  = field.name;

                    if (typeof field.decode == 'function') {
                        data[name] = field.decode(objAttrs[name]);
                    } else {
                        data[name] = objAttrs[name];
                    }
                }
                data.id = id;
                //data.leaf = true;
                return data;
    //            record = new Model(data, id);
    //            record.phantom = false;
    //            return record;
            }

            return $.Deferred(function(dfr){
                storage.executeSql("SELECT source_id FROM sources WHERE name=? LIMIT 1 OFFSET 0",
                        [srcName]).done(function(tx, rs){
                    var srcId = null;
                    if (rs.rows && rs.rows.length && rs.rows.length > 0) {
                        srcId = rs.rows.item(0)['source_id'];
                    }
                    _localSrcIdFound(srcId);
                }).fail(function(obj, err){
                    that.LOG.error('findAll() source_id select error: ' +err);
                    dfr.reject(obj, err);
                });

                function _localSrcIdFound(srcId) {
                    storage.executeSql("SELECT * FROM object_values WHERE source_id=?",
                            [srcId]).done(function(tx, rs){
                        var objects = {};
                        for(var i=0; i<rs.rows.length; i++) {
                            var objId = rs.rows.item(i)['object'];
                            var attrib = rs.rows.item(i)['attrib'];
                            var value = rs.rows.item(i)['value'];
                            _setupObject(objects, objId, attrib, value);
                        }
                        _localObjectsAreRead(objects);
                    }).fail(function(obj, err){
                        that.LOG.error('findAll() select all objects for source error: ' +err);
                        dfr.reject(obj, err);
                    });

                }

                function _localObjectsAreRead(objects) {
                    var records = [];
                    $.each(objects, function(id, object){
                        records.push(_buildRecord(id, object));
                    });

                    dfr.resolve(records);
                }
            }).promise();
        },


        /**
         * @private
         * Fetches a model instance from the Proxy by ID. Runs each field's decode function (if present) to decode the data
         * @param {String} id The record's unique ID
         * @return The deferred object to resolve with {Ext.data.Model} model instance or to reject with error code and
         * error message
         */
        getRecord: function(id) {
            var that = this;
            var storage = that.getStorageObject();

            function _buildRecord(rs) {
                var data    = {},
                    Model   = that.model,
                    fields  = Model.prototype.fields.items,
                    length  = fields.length,
                    i, field, name, record;
                var rawData = {};
                for(i=0; i<rs.rows.length; i++) {
                    var attrName = rs.rows.item(i)['attrib'];
                    rawData[attrName] = rs.rows.item(i)['value'];
                }
                for (i = 0; i < length; i++) {
                    field = fields[i];
                    name  = field.name;

                    if (typeof field.decode == 'function') {
                        data[name] = field.decode(rawData[name]);
                    } else {
                        data[name] = rawData[name];
                    }
                }
                data.id = id;
                //data.leaf = true;
                return data;
    //            record = new Model(data, id);
    //            record.phantom = false;
    //            return record;
            }

            return $.Deferred(function(dfr){
                storage.executeSql("SELECT * FROM object_values WHERE object=?", [id.toString()]).done(function(tx, rs){
                    dfr.resolve(_buildRecord(rs));
                }).fail(function(obj, err){
                    that.LOG.error('getRecord() error: ' +err);
                    dfr.reject(obj, err);
                });
            }).promise();
        },

        /**
         * Saves the given record in the Proxy. Runs each field's encode function (if present) to encode the data
         * @param {Ext.data.Model} record The model instance
         * @param {String} id The id to save the record under (defaults to the value of the record's getId() function)
         */
        setRecord: function(record, id) {
            var that = this;
            var storage = that.getStorageObject();
            var srcName = that.model.modelName;
            var srcId = null;
            var isNew = false;

            if (id) {
                record.setId(id);
                isNew = true;
            } else {
                id = record.getId();
            }

            var rawData = record.data,
                data    = {},
                model   = that.model,
                fields  = model.prototype.fields.items;

            return $.Deferred(function(dfr){

                if(undefined != record.dirty && !record.dirty) {
                    dfr.resolve(id);
                    return;
                }

                // Read source_id for stored object (object should be already stored)
                storage.executeSql("SELECT source_id FROM sources WHERE name=?",
                        [srcName]).done(function(tx, rs){
                    if (rs.rows && rs.rows.length && rs.rows.length > 0) {
                        srcId = rs.rows.item(0)['source_id'];
                    }
                    _localWithSrcId();
                }).fail(function(obj, err){
                    that.LOG.error('setRecord() read source_id error: ' +err);
                    dfr.reject(obj, err);
                });

                // to select UPDATE/INSERT query we firstly need to know which attributes already present for the object
                var attrsToUpdate = {};
                function _localWithSrcId() {
                    storage.executeSql("SELECT attrib FROM object_values WHERE object=?",
                            [id.toString()]).done(function(tx, rs){
                        for (var i=0; i< rs.rows.length; i++) {
                            var attrName = rs.rows.item(i)['attrib'];
                            if (attrName) attrsToUpdate[attrName] = true;
                        }
                        _localWithAttrsToUpdate();
                    }).fail(function(obj, err){
                        that.LOG.error('setRecord() read attr names error: ' +err);
                        dfr.reject(obj, err);
                    });
                }

                function _localWithAttrsToUpdate() {
                    var updateQuery = 'UPDATE object_values SET'
                            +' value=?'
                            +' WHERE source_id=? and object=? and attrib=?';

                    var insertQuery = 'INSERT INTO object_values ('
                            +' value,'
                            +' source_id,'
                            +' object,'
                            +' attrib'
                            +' ) VALUES (?, ?, ?, ?)';

                    var insertChangedQuery = 'INSERT INTO changed_values ('
                            +' value,'
                            +' source_id,'
                            +' object,'
                            +' attrib,'
                            +' update_type'
                            +' ) VALUES (?, ?, ?, ?, ?)';

                    //var dfrMap = RhoSync.rho.deferredMapOn(fields);
                    storage.rwTx().ready(function(db, tx){
                        $.each(fields, function(i, field) {
                            var name = field.name;

                            if (typeof field.encode == 'function') {
                                data[name] = field.encode(rawData[name], record);
                            } else {
                                data[name] = rawData[name];
                            }

                            var query = attrsToUpdate[name] ? updateQuery : insertQuery;
                            var value = data[name];
                            if (name != 'id') {
                                storage.executeSql(query, [value, srcId.toString(), id.toString(), name], tx)/*.done(function(tx, rs){
                                    //dfrMap.resolve(i, []);
                                    that.LOG.warning('OK: setRecord() update/insert object_values ok');
                                    //that.LOG.warning('  "' +query +'", [' +value +', ' +srcId +', ' +id +', ' +name +']');
                                    //that.LOG.warning('  rs.rowsaffected: ' +rs.rowsAffected);
                                }).fail(function(obj, err){
                                    //dfrMap.reject(i, [obj, err]);
                                    that.LOG.warning('ERR: setRecord() update/insert object_values error: ' +err);
                                })*/;
                                if (!isNew) {
                                    storage.executeSql(insertChangedQuery,
                                            [value, srcId.toString(), id.toString(), name, 'update'], tx)/*.done(function(tx, rs){
                                        //dfrMap.resolve(i, []);
                                        that.LOG.warning('OK: setRecord() update/insert changed_values ok');
                                        that.LOG.warning('  "' +query +'", [' +value +', ' +srcId +', ' +id +', ' +name +']');
                                        that.LOG.warning('  rs.rowsaffected: ' +rs.rowsAffected);
                                    }).fail(function(obj, err){
                                        //dfrMap.reject(i, [obj, err]);
                                        that.LOG.warning('ERR: setRecord() update/insert changed_values error: ' +err);
                                    })*/;
                                }
                            }
                        });
                        if (isNew) {
                            storage.executeSql(insertChangedQuery,
                                    [null/*id.toString()*/, srcId.toString(), id.toString(), 'object', 'create'], tx)/*.done(function(tx, rs){
                                //dfrMap.resolve(i, []);
                                that.LOG.warning('OK: setRecord() update/insert changed_values ok');
                                that.LOG.warning('  "' +query +'", [' +value +', ' +srcId +', ' +id +', ' +name +']');
                                that.LOG.warning('  rs.rowsaffected: ' +rs.rowsAffected);
                            }).fail(function(obj, err){
                                //dfrMap.reject(i, [obj, err]);
                                that.LOG.warning('ERR: setRecord() update/insert changed_values error: ' +err);
                            })*/;
                        }
                    }).done(function(db){
                        record.dirty = false;
                        dfr.resolve(id);
                    }).fail(function(obj, err){
                        dfr.reject(null, 'setRecord() update/insert attr error');
                    });
                }
            }).promise();
        },

        /**
         * Physically removes a given record from the rhosync storage. Used internally by {@link #destroy}.
         * @param {Ext.data.Model} record The model instance
         * @param {String} id The id of record to remove
         */
        removeRecord: function(record) {
            var that = this;
            var storage = that.getStorageObject();
            var srcName = that.model.modelName;
            var srcId = null;
            var id = null;

            if (record && record.data && record.data.id) {
                id = record.data.id;
            }

            return $.Deferred(function(dfr){

                // Read source_id for stored object (object should be already stored)
                storage.executeSql("SELECT source_id FROM sources WHERE name=?",
                        [srcName]).done(function(tx, rs){
                    if (rs.rows && rs.rows.length && rs.rows.length > 0) {
                        srcId = rs.rows.item(0)['source_id'];
                    }
                    _localWithSrcId();
                }).fail(function(obj, err){
                    that.LOG.error('setRecord() read source_id error: ' +err);
                    dfr.reject(obj, err);
                });

                function _localWithSrcId() {
                    storage.rwTx().ready(function(db, tx) {

                        var attrsToDelete = {};
                        storage.executeSql("SELECT * FROM object_values WHERE object=? AND source_id=?", [id.toString(), srcId.toString()], tx).done(function(tx, rs){
                            for (var i=0; i< rs.rows.length; i++) {
                                var attrName = rs.rows.item(i)['attrib'];
                                var attrValue = rs.rows.item(i)['value'];
                                if (attrName) attrsToDelete[attrName] = attrValue;
                            }
                            storage.executeSql("DELETE FROM object_values WHERE object=? AND source_id=?", [id.toString(), srcId.toString()], tx).done(function(tx, rs){
                                _localWithObjValsDeleted();
                            });
                        });

                        function _localWithObjValsDeleted() {

                            var updateType = 'delete';
                            storage.executeSql("SELECT update_type FROM changed_values WHERE object=? AND update_type=? AND sent=?", [id.toString(), 'create', 0], tx).done(function(tx, rs){
                                if (0 < rs.rows.length) {
                                    updateType = null;
                                }
                                storage.executeSql("DELETE FROM changed_values WHERE object=? AND source_id=? AND sent=?", [id.toString(), srcId.toString(), 0], tx).done(function(tx, rs){

                                    var doInsert = false;
                                    $.each(attrsToDelete, function(name, value) {
                                        if (updateType) doInsert = true;
                                    });

                                    if (doInsert) {
                                        _localDoInsertDelete();
                                    }
                                });
                            });

                            function _localDoInsertDelete() {
                                var insertChangedQuery = 'INSERT INTO changed_values ('
                                        +' value,'
                                        +' source_id,'
                                        +' object,'
                                        +' attrib,'
                                        +' update_type'
                                        +' ) VALUES (?, ?, ?, ?, ?)';
                                $.each(attrsToDelete, function(name, value) {
                                    storage.executeSql(insertChangedQuery, [value, srcId.toString(), id.toString(), name, updateType], tx)/*.done(function(tx, rs){
                                    //dfrMap.resolve(i, []);
                                        that.LOG.warning('OK: setRecord() update/insert changed_values ok');
                                        that.LOG.warning('  "' +query +'", [' +value +', ' +srcId +', ' +id +', ' +name +']');
                                        that.LOG.warning('  rs.rowsaffected: ' +rs.rowsAffected);
                                        }).fail(function(obj, err){
                                    //dfrMap.reject(i, [obj, err]);
                                        that.LOG.warning('ERR: setRecord() update/insert changed_values error: ' +err);
                                    })*/;
                                });
                            }
                        }
                    }).done(function(db) {
                        record.dirty = false;
                        dfr.resolve(id);
                    }).fail(function(obj, err) {
                        dfr.reject(null, 'setRecord() remove attr error');
                    });
                }
            }).promise();
        },

        /**
         * @private
         * Physically removes a given record from the rhosync storage. Used internally by {@link #destroy}, which you should
         * use instead because it updates the list of currently-stored record ids
         * @param {String|Number|Ext.data.Model} id The id of the record to remove, or an Ext.data.Model instance
         */
    /*
        removeRecord: function(id, updateIds) {
            if (id instanceof Ext.data.Model) {
                id = id.getId();
            }

            if (updateIds !== false) {
                var ids = this.getIds();
                ids.remove(id);
                this.setIds(ids);
            }

            this.getStorageObject().removeItem(this.getRecordKey(id));
        },
    */


        /**
         * @private
         */
        initialize: function() {
        },

        /**
         * Destroys all records stored in the proxy and removes values used to support the proxy from the storage object
         */
        clear: Ext.emptyFn,
    /*
        clear: function() {
            // unsure we need it
            var storage = this.getStorageObject();

            storage.executeSql('DELETE FROM object_values').done(function(){
            }).fail(function(errCode, err){
                that.LOG.error('clear() error: ' +(err || errCode));
            });
        },
    */

        /**
         * @private
         * @return {Object} The storage object
         */
        getStorageObject: function() {
            return RhoSync.rho.storage;
        },

        getNextId: function() {
            baseTempId = baseTempId || (Date.now() - (new Date(2009, 1, 1)).getTime());
            baseTempId = baseTempId + 1;
            return baseTempId;
        }
    });

    Ext.data.ProxyMgr.registerType('rhosync', Ext.data.RhosyncStorageProxy);

})(jQuery, Ext);
