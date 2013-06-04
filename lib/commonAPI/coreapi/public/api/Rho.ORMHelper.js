(function ($,rho,rhoUtil) {
  'use strict';
  var moduleNS = 'Rho.ORMHelper';
  var dbPartitions = {};


  var createTable = function(source){
    //var sources = Opal.Rho._scope.RhoConfig.$sources();
    //var source  = sources['$[]'](modelName);
    var db = dbConnection(source);
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
      col_str += name + " " + sql_type(d_type) + ", ";
    });
    console.log("col str is: " + col_str);
    col_str += "\"object\" varchar(255) PRIMARY KEY";
    return col_str;
    //strCreate = "CREATE TABLE \"#{name}\" ( #{strCols} )";
  }

  function sql_type(d_type){
    var cols = "";
    console.log("dtype passed is: " + d_type.length);
    switch(d_type){
      case 'integer':
      case 'time':
        console.log("is integer!");
        cols = 'integer';
        break;
      case 'float':
        cols = 'float';
        break;
      default:
        console.log("dtype is: " + d_type + "but in default");
        cols = 'varchar';
        break;
    }
    console.log("cols is: " + cols);
    return cols;
    //cols += ";\r\n";
    //cols += processIndexes(schema_attr['index'], name, false);
    //cols += ";\r\n";
    //cols += processIndexes(schema_attr['unique_index'], name, true)
  }

  function processIndexes(index){

  }

  function loadModel(modelName,init_db){
    console.log("loading model name:" + modelName);
    if(Opal.Rho._scope.RhoConfig.$sources().map.hasOwnProperty(modelName) && !Opal.Rho._scope.RhoConfig.$sources()[modelName]['loaded']){
      console.log("inside of load modal ready to load");
    }
  }

  var getDbPartitions = function(){
    return dbPartitions;
  };

  function initSchemaSourcesPartition(uniq_sources, hash_migrate, partition, db){
    if(db.is_table_exist(source['name'])){
      call_migrate = true;
    }
  }

  function dbConnection(source){
    //look for db connection, if not found create one
    var filepath = Rho.Application.databaseFilePath(source['partition']);
    var partitions = getDbPartitions();
    var db;
    //db = (Opal.Object._scope.Rho)._scope.RHO.$get_src_db(source['partition']);
    
    if(partitions && partitions[source['partition']]){
      db = partitions[source['partition']];
    }
    else{
      db = Opal.Database.$new(filepath, source['partition']);
      partitions[source['partition']] = db;
    }
    return db;
    
    // var hash_migrate = {};
    // try{
    //   db.$start_transaction;
    //   var strCreate;
    //   strCreate = make_createsql_script(source['name'], source['schema'])
    //   initDbSource(db, source, source['partition'], hash_migrate)

    //   console.log("Migrate schema sources:" + hash_migrate);
      
    //   //initSchemaSourcesPartition(source, hash_migrate, source['partition'], db)
    //   //::Rho::RHO.check_sources_migration(uniq_sources)
      
    //   //::Rho::RHO.init_sync_source_properties(uniq_sources)
      
      
    //   //db.commit



    // }catch(e){
    //   console.error("error is:" + e.message);
    // }
  }

  function initDbSource(db, source, hash_migrate){
    //("sources", "metadata", __hash2({"name": src_name})
    //function(table, column, condition, params)
    //columns:'sync_priority,source_id,partition, sync_type, schema_version, associations, blob_attribs, name'
    var db_partition = source['partition'];
    db_sources = db.$select_from_table('sources',"*")
    console.log("db_sources is:" + db_sources);
    var start_id = get_start_id(db_sources,source['partition'])
    blob_attribs = process_blob_attribs(source, db)

    attribs = find_src_byname(db_sources,source['name'])
    //if(parseInt(source['sync_priority']) != )
  }

  function get_start_id(db_sources,db_partition){
    var start_id = 0;
    var src_id;
    for(var i=0;i<db_sources.length;i++){
      if(db_partition == db_sources[i]['partition']){
        src_id = db_sources[i]['source_id'];
        if(src_id > start_id)
          start_id = src_id;
      }
    }

    if(start_id < getSourceId(db_partition))
      start_id = getSourceId(db_partition) + 2;
    else
      start_id += 1;
    console.log("returned start_id:" + start_id);
    return start_id;
  }

  function process_blob_attribs(source,db){
    console.log("source is: " + JSON.stringify(source));
    if(source['str_blob_attribs'])
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
      console.log("str blob is: " + source['str_blob_attribs']);
      return source['str_blob_attribs'];
    }
  }

  function find_src_byname(db_sources,name){
  var res = null;
  for(var i=0;i<db_sources.length;i++){
    if(db_sources[i]['name'] == name)
      res = db_sources[i];
  }
  return res;
  }









  var ORMHelper = {
    loadAllSources: function(){
      loadAllSources();
    },
    getDbPartitions: function(){
      return getDbPartitions();
    },
    createTable: function(modelName){
      createTable(modelName);
    }
  };
  rhoUtil.namespace(moduleNS, ORMHelper, true);

})(jQuery, Rho, Rho.util);
