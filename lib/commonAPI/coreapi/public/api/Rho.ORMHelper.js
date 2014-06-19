(function ($,rho,rhoUtil) {
  'use strict';
  var moduleNS = 'Rho.ORMHelper';
  var dbPartitions = {};

  var createTable = function(source){
    var db = dbConnection(source['partition']);
    var create_table_str = "";
    if(!db.$is_table_exist(source['name'])){
      var column_names = formatColNames(source['schema']['property']);
      create_table_str = "CREATE TABLE " + source['name'] + " (" + column_names + ")";
      db.$execute_sql(create_table_str);
      if(source['index'] !== undefined){
        $.each(source['index'],function(name,columns){
          db.$execute_sql("CREATE INDEX " + name + " ON " + source['name'] + " (" + columns.join(',') + ")" );
        });
      }
      if(source['unique_index'] !== undefined){
        $.each(source['unique_index'],function(name,columns){
          db.$execute_sql("CREATE UNIQUE INDEX " + name + " ON " + source['name'] + " (" + columns.join(',') + ")" );
        });
      }
    }
  };

  function formatColNames(props){
    var col_str = "";
    $.each(props,function(name,d_type){
      col_str += name + " " + sql_type(d_type[0]) + ", ";
    });
    col_str += "\"object\" varchar(255) PRIMARY KEY";
    return col_str;
  }

  function sql_type(d_type){
    var cols = "";
    switch(d_type){
      case 'integer':
      case 'time':
        cols = 'integer';
        break;
      case 'float':
        cols = 'float';
        break;
      default:
        cols = 'varchar';
        break;
    }
    return cols;
  }

  function loadModel(modelName,init_db){
    if(Opal.Rho._scope.RhoConfig.$sources().map.hasOwnProperty(modelName) && !Opal.Rho._scope.RhoConfig.$sources().map[modelName]['loaded']){
      console.log("inside of load modal ready to load");
    }
  }

  var getDbPartitions = function(){
    return dbPartitions;
  };

  function clearAllSources(){
    var sources = Opal.Rho._scope.RhoConfig.$sources().map;
    $.each(sources,function(index,value){
      delete sources[index];
    });
  }

  function initSchemaSourcesPartition(uniq_sources, hash_migrate, partition, db){
    if(db.is_table_exist(source['name'])){
      call_migrate = true;
    }
  }

  function dbConnection(partition){
    //look for db connection, if not found create one
    var filepath = Rho.Application.databaseFilePath(partition);
    var partitions = getDbPartitions();
    var db;

    if(partitions && partitions[partition]){
      db = partitions[partition];
    }
    else{
      db = Opal.Database.$new(filepath, partition);
      partitions[partition] = db;
    }
    return db;
  }

  var syncSource = function(source){
    var db = dbConnection(source['partition']);
    db.$start_transaction;
    initDbSource(db, source);
    db.$commit;

    // ::Rho::RHO.init_schema_sources(hash_migrate)
    // ::Rho::RHO.check_sources_migration(uniq_sources)   
    // ::Rho::RHO.init_sync_source_properties(uniq_sources)
  };

  function initDbSource(db, source){
    var sql = "select sync_priority,source_id,partition, sync_type, schema_version, associations, blob_attribs, name from sources where name ='" + source['name'] + "'";
    var db_source_raw = db.$execute_sql(sql);
    var db_source;
    
    if(db_source_raw.length > 0)
      db_source = db_source_raw[0].map;
    else
      db_source = null;

    var blob_attribs = processBlobAttribs(source);

    var start_id = getStartId(source);
    var hash_migrate = {};
    if(db_source !== null){
      updateSourceModel(db,source,db_source,hash_migrate);
    }
    else{
      createSourceModel(db,source,start_id,blob_attribs);
    }
  }

  function getStartId(source){
    var start_id = 0;
    var src_id = source['source_id'];
    var partition = source['partition'];

    if(src_id > start_id)
      start_id = src_id;

    if(start_id < Rho.ORM.getSourceId(partition))
      start_id = Rho.ORM.getSourceId(partition) + 2;
    else
      start_id += 1;
    return start_id;
  }

  function createSourceModel(db,source,start_id,blob_attribs){
    // Need to find out how bulk mode is set
    //if Rho::RhoConfig.use_bulk_model.to_s != 'true' && Rho::RhoConfig.use_bulk_model.to_s != '1'
    if(source['source_id'] === undefined || source['source_id'] === null){
        source['source_id'] = start_id;
        Opal.Rho._scope.RhoConfig.$sources().map[source['name']]['source_id'] = start_id;
        start_id += 1;
    }
    db.$insert_into_table('sources',__hash2(
        {"source_id":source['source_id'],"name":source['name'], "sync_priority":source['sync_priority'], "sync_type":source['sync_type'], "partition":source['partition'],
        "schema_version":source['schema_version'], 'associations':source['associations'],'blob_attribs':blob_attribs }));
  }

  function updateSourceModel(db,source,source_db,hash_migrate){
    var final_hash = {"name":source['name']};
    if(source_db['sync_priority'] !== source['sync_priority'])
        final_hash["sync_priority"] = source['sync_priority'];
    if(source_db['sync_type'] !== source['sync_type'])
      final_hash["sync_type"] = source['sync_type'];
    if(source_db['schema_version'] !== source['schema_version']){
      if(source_db['partition'] == source['partition'])
        hash_migrate[source['name']] = source_db['schema_version'];
      else
        final_hash["schema_version"] = source['schema_version'];
    }
    if(source_db['partition'] !== source['partition'])
      final_hash["partition"] = source['partition'];
    if(source_db['associations'] !== source['associations'])
      final_hash["associations"] = source['associations'];
    if(source_db['blob_attribs'] !== source['blob_attribs'])
      final_hash["blob_attribs"] = source['blob_attribs'];

    //console.log("final hash is: " + JSON.stringiffinal_hash);
    db.$update_into_table('sources', Opal.hash2(final_hash),Opal.hash2({"source_id":source["source_id"]}));
    if(source['source_id'] === undefined || source['source_id'] === null){
        source['source_id'] = source_db['source_id'];
        Opal.Rho._scope.RhoConfig.$sources().map[source['name']]['source_id'] = source_db['source_id'];
    }
  }

  function processBlobAttribs(source){
    if(source['str_blob_attribs'] !== undefined)
      return source['str_blob_attribs'];
    else{
      source['str_blob_attribs'] = "";
      var props;
      if(source['schema'])
        props = source['schema']['property'];
      else
        props = source['property'];
      if(props){
        var str = "";
        var s = "";
        for(var i in props){
          s = props[i];
          if(typeof props[i] == String)
            s = s.split(",");
          if(s && s[0] == 'blob'){
            if(str.length > 0)
              str += ',';
            str += i + ',';
            if(s[1] == 'overwrite')
              str += '1';
            else
              str += '0';
          }
        }
        source['str_blob_attribs'] = str;
      }
      //console.log("str blob is: " + source['str_blob_attribs']);
      return source['str_blob_attribs'];
    }
  }

  //RhomDbAdapter functions

  function getValueForSqlStmt(value, convertValueToString){
    convertValueToString = typeof convertValueToString !== 'undefined' ? convertValueToString : true;
    var res;
    if(value === undefined || value === null){
      res = null;
    }
    else if(typeof value === 'string'){
      res = value.replace(/'/g,"''");
    }
    else{
      if(convertValueToString)
        res = String(value).replace(/'/g,"''");
      else
        res = value;
    }
    return res;
  }

  function whereStr(condition){
    var where_str = "";
    if(condition !== undefined){
      where_str += string_from_key_vals(condition,"and");
      where_str = where_str.substring(0,where_str.length - 5);
    }
    return where_str;
  }

  function string_from_key_vals_set(values, delim){
    var vals = "";
    var op;
    $.each(values,function(key,value){
      op = '= ';
      vals += " \"#{key}\" #{op} #{get_value_for_sql_stmt(value)} #{delim}";
    });
    return vals;
  }


  var ORMHelper = {
    getAllSources: function(){
      return Opal.Rho._scope.RhoConfig.$sources().map;
    },
    clearAllSources: function(){
      clearAllSources();
    },
    getDbPartitions: function(){
      return getDbPartitions();
    },
    createTable: function(modelName){
      createTable(modelName);
    },
    syncSource: function(source){
      syncSource(source);
    },
    dbConnection: function(partition){
      return dbConnection(partition);
    },
    getValueForSqlStmt: function(value,convertValueToString){
      return getValueForSqlStmt(value);
    },
    whereStr: function(condition){
      return whereStr(condition);
    }
  };
  rhoUtil.namespace(moduleNS, ORMHelper, true);

})(Rho.jQuery, Rho, Rho.util);
