// === ORM class definition ===
(function ($, rho, rhoUtil) {
  'use strict';

  var builder;
  var modelname;
  var unprocessedModels = {};

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

  var enable = function(name){
    builder.model.enable(name);
  };

  var property = function(name,type,option){
    builder.model.property(name,type,option);
  };

  var belongs_to = function(owner){
    builder.model.belongs_to(owner);
  };

  var addUniqueIndex = function(name,column){
    builder.model.addUniqueIndex(name,column);
  };

  var addIndex = function(name,column){
    builder.model.addIndex(name,column);
  };

  var set = function(name,value){
    builder.model.set(name,value);
  };

  var modelName = function(name){
    modelname = name;
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
      }else if(name === 'propertyBag'){
        fixedSchema = false;
      }else if(name === 'fixedSchema'){
        fixedSchema = true;
      } else {
        params[name] = value;
      }
      return model;
    };

    var model = {
      property: function(name, type, option) {
          if (type === undefined) {
              type = 'string';
          }
          properties[name] = [type, option];
          return model;
      },
      //addIndex: addIndex,
      set: function(name,value){
        return set(name,value);
      },
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
    // if (sources.map.hasOwnProperty(modelName)) {
    //   return sources.map[modelName];
    // }

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

  function processBelongsTo(source){
    var associationsSrc;
    var str_associations;
    $.each(source['belongs_to'],function(attrib,src_name){
        associationsSrc = Opal.Rho._scope.RhoConfig.$sources().map[src_name];
        if(associationsSrc === undefined || associationsSrc === null){
          console.error("Error: belongs_to "+source['name']+" : source name" +src_name+ "does not exist. will try to load later");
          if(unprocessedModels[src_name] !== undefined)
            unprocessedModels[src_name] += "," + source['name'];
          else
            unprocessedModels[src_name] = source['name'];
        }
        else{
          if(associationsSrc['str_associations'] !== undefined && associationsSrc['str_associations'].length > 0){
            str_associations = associationsSrc['str_associations'];
            str_associations += ',';
          }
          else{
            str_associations = "";
          }
          str_associations += source['name']; //+ ',' + attrib;
          associationsSrc['str_associations'] = str_associations;
        }
    });
  }

  function setup_late_assocation(source){
    if(source['str_associations'] !== undefined)
      source['str_associations'] = "," + unprocessedModels[source['name']];
    else
      source['str_associations'] = unprocessedModels[source['name']];
    delete unprocessedModels[source['name']];
  }

  var makeModel = function(modelName, builder) {
    Opal.Rhom._scope.RhomObjectFactory.$init_object(modelName);

    var source = addLoadedSource(modelName, builder.makeParams());

    //check for any belongs to that could not setup b/c model def not loaded
    if(unprocessedModels[source['name']] !== undefined)
      setup_late_assocation(source);


    console.log("source created is: " + JSON.stringify(source));
    if(source['schema'] !== undefined){
      Rho.ORMHelper.createTable(source);
    }
    if(source['sync_type'] !== undefined && source['sync_type'] !== 'none'){
      //TODO should belongs_to assocations with models not sync enabled be ignored?
      if(source['belongs_to'] !== undefined && source['belongs_to'] !== null)
        processBelongsTo(source);
      Rho.ORMHelper.syncSource(source);
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
        //Opal.Rho._scope.RhoConfig.$sources()['$[]='] = {};
    },
    addModel: function(modelDef) {
      modelname = null;
      builder = new makeModelBuilder();
      modelDef(Rho.ORM);
      models[modelname] = makeModel(modelname,builder);
      return models[modelname];
    },
    getModel: function(modelName) {
        return models[modelName];
    },
    getSourceId: function(partition){
      return getSourceId(partition);
    },
    accountSourceId: function(partition, sourceId){
      return accountSourceId(partition, sourceId);
    },
    enable: function(name){
      enable(name);
    },
    modelName: function(name){
      modelName(name);
    },
    addIndex: function(name,cols){
      addIndex(name,cols);
    },
    addUniqueIndex: function(name,cols){
      addUniqueIndex(name,cols);
    },
    property: function(name,type,option){
      property(name,type,option);
    },
    belongs_to: function(owner){
      belongs_to(owner);
    },
    set: function(name,value){
      set(name,value);
    }
  };
  //add Rho.ORM to public namespace
  var moduleNS = 'Rho.ORM';

  rhoUtil.namespace(moduleNS, ORM, true);

})(jQuery, Rho, Rho.util);