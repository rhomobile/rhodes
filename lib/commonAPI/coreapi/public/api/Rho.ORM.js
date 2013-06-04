// === ORM class definition ===
(function ($, rho, rhoUtil) {
  'use strict';

  var freeSourceIds = {
    'user' :     1,
    'app'  : 20001,
    'local': 40001
  };
  var getSourceId = function(partition) {
    return freeSourceIds[partition];
  };
  var accountSourceId = function(partition, sourceId) {
    if (freeSourceIds[partition] <= sourceId) {
        freeSourceIds[partition] = sourceId + 1;
    }
  };


  var opalHash = function(values) {
    return Opal.hash2((values === undefined) ? {} : values);
  };

  var wrapOpalObject = function(opalObject) {
    return {
        object: function() {
            return opalObject.$vars().map['object'];
        },
        get: function(name) {
            return opalObject.$vars().map[name];
        },
        set: function(name, value) {
            opalObject.$method_missing(Opal.String(name + '='), value);
            return this;
        },
        has: function(name) {
            return opalObject.$vars().map.hasOwnProperty(name);
        },
        vars: function() {
            return opalObject.$vars().map;
        },
        save: function() {
            opalObject.$save();
        },
        updateAttributes: function(attributes) {
            opalObject.$update_attributes(opalHash(attributes));
        },
        destroy: function() {
            opalObject.$destroy();
        }
    };
  };

  var wrapOpalObjects = function(opalObjects) {
    var list = new Array(opalObjects.length);
    for (var i = 0; i < opalObjects.length; ++i) {
        list[i] = wrapOpalObject(opalObjects[i]);
    }
    return list;
  };

  var makeModelBuilder = function() {
    var fixedSchema = false;
    var params = {};
    var properties = {};

    var set = function(name, value) {
        if (name === 'sync') {
            if (value) {
                if (!params.hasOwnProperty('sync_type')) {
                    params['sync_type'] = 'incremental';
                }
            } else {
                delete params['sync_type'];
            }
        } else {
            params[name] = value;
        }
        return model;
    };

    var model = {
        propertyBag: function() {
            fixedSchema = false;
            return model;
        },
        fixedSchema: function() {
            fixedSchema = true;
            return model;
        },
        property: function(name, type, option) {
            if (type === undefined) {
                type = 'string';
            }
            properties[name] = [type, option];
            return model;
        },
        //addIndex: addIndex,
        set: set,
        enable: function(name) {
            return set(name, true);
        },
        belongs_to: function(owner) {
            if (!params.hasOwnProperty('belongs_to')) {
                params['belongs_to'] = [];
            }
            if (owner instanceof Array) {
                for (var i = 0; i < owner.length; ++i) {
                    params['belongs_to'].push(owner[i]);
                }
            } else {
                params['belongs_to'].push(owner);
            }
            return model;
        },
        addIndex: function(name,column) {
            if (!params.hasOwnProperty('index')) {
                params['index'] = {};
            }
            if (!params['index'].hasOwnProperty(name)){
                params['index'][name] = [];
            }
            if (column instanceof Array) {
                for (var i = 0; i < column.length; ++i) {
                    params['index'][name].push(column[i]);
                }
            } else {
                params['index'][name].push(column);
            }
            return model;
        },
        addUniqueIndex: function(name,column) {
            if (!params.hasOwnProperty('unique_index')) {
                params['unique_index'] = {};
            }
            if (!params['unique_index'].hasOwnProperty(name)){
                params['unique_index'][name] = [];
            }
            if (column instanceof Array) {
                for (var i = 0; i < column.length; ++i) {
                    params['unique_index'][name].push(column[i]);
                }
            } else {
                params['unique_index'][name].push(column);
            }
            return model;
        }
    };
    //addIndex('index');
    //addIndex('unique_index');

    var makeParams = function() {
        if (fixedSchema) {
            params['schema'] = {property: properties};
        } else {
            params['property'] = properties;
        }
        return params;
    };

    return {model: model, makeParams: makeParams};
  };

  var clone = function(original) {
    var clone = {};
    for (var key in original) {
        if (original.hasOwnProperty(key)) {
            clone[key] = original[key];
        }
    }
    return clone;
  };

  var addLoadedSource = function(modelName, newSource) {
    var sources = Opal.Rho._scope.RhoConfig.$sources();
    if (sources.map.hasOwnProperty(modelName)) {
      return;
    }

    var source = clone(newSource);

    var setDefault = function(key, value) {
        if (!source.hasOwnProperty(key)) {
            source[key] = value;
        }
    };

    source['loaded'] = true;
    source['name'] = modelName;
    setDefault('sync_priority', 1000);
    setDefault('sync_type', 'none');
    setDefault('partition', (source['sync_type'] !== 'none') ? 'user' : 'local');
    setDefault('source_id', getSourceId(source['partition']));

    accountSourceId(source['partition'], source['source_id']);

    sources['$[]='](modelName, source);
    return source;
  };

  function init_sync_source(source){
    //belongs to logic here
    //check for belongs to and append str_associations to that source 

    //get all db partitions and loop
    var db = Rho.ORMHelper.dbConnection(source);
    db.$start_transaction
    try{
      var hash_migrate = {};
      init_db_sources(db, source,hash_migrate);
    }
    catch(e){
      console.log("exception: " + e.message);
    }
  }


  var makeModel = function(modelName, builder) {
    Opal.Rhom._scope.RhomObjectFactory.$init_object(modelName);

    //var builder = makeModelBuilder(false);
    //if (initialize !== undefined) {
    //    initialize(builder.model);
    //} 

    var source = addLoadedSource(modelName, builder.makeParams());
    if(source['schema'] !== undefined){
      Rho.ORMHelper.createTable(source);
    }
    if(source['sync_type'] !== undefined && source['sync_type'] !== 'none'){
      //Rho.ORMHelper.syncSource(source);;
    }

    var opalModel = Opal.Object._scope[modelName];
    return {
        make: function(values) {
            return wrapOpalObject(opalModel.$new(opalHash(values)));
        },
        create: function(values) {
            return wrapOpalObject(opalModel.$create(opalHash(values)));
        },
        count: function() {
            return opalModel.$count();
        },
        find: function(what, options) {
            var found = opalModel.$find(what, options);
            switch (what) {
                case 'all'  : return wrapOpalObjects(found);
                case 'count': return                 found ;
                case 'first':
                default     : return wrapOpalObject (found);
            }
        },
        sync: function(callback, show_status_popup, query_params) {
            opalModel.$sync(callback, show_status_popup, query_params);
        },
        deleteAll: function() {
            opalModel.$delete_all();
        }
    };
  };

  var models = {};

  var ORM = {
    clear: function() {
        models = {};
    },
    initModel: function(){
      return makeModelBuilder();
    },
    addModel: function(modelName,builder) {
        models[modelName] = makeModel(modelName,builder);
        //Rho.ORMHelper.createTable(modelName);
        return models[modelName];
    },
    getModel: function(modelName) {
        return models[modelName];
    }
  };

  //add Rho.ORM to public namespace
  var moduleNS = 'Rho.ORM';

  rhoUtil.namespace(moduleNS, ORM, true);

})(jQuery, Rho, Rho.util);