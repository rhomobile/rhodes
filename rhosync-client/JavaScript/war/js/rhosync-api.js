Rhomobile = function() {

	// === Storage ================================================

	var syncDbSchemaSQL = ''
		+'CREATE TABLE client_info ('
				+' "client_id" VARCHAR(255) default NULL,'
				+' "session" VARCHAR(255) default NULL,'
				+' "token" VARCHAR(255) default NULL,'
				+' "token_sent" BIGINT default 0,'
				+' "reset" BIGINT default 0,'
				+' "port" VARCHAR(10) default NULL,'
				+' "last_sync_success" VARCHAR(100) default NULL);'
		+'CREATE TABLE object_values ('
				+' "source_id" BIGINT default NULL,'
				+' "attrib" varchar(255) default NULL,'
				+' "object" varchar(255) default NULL,'
				+' "value" varchar default NULL);'
		+'CREATE TABLE changed_values ('
				+' "source_id" BIGINT default NULL,'
				+' "attrib" varchar(255) default NULL,'
				+' "object" varchar(255) default NULL,'
				+' "value" varchar default NULL,'
				+' "attrib_type" varchar(255) default NULL,'
				+' "update_type" varchar(255) default NULL,'
				+' "sent" BIGINT default 0);'
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
		+'CREATE INDEX by_src_id on object_values ("source_id");'
		+'CREATE UNIQUE INDEX by_src_object ON object_values ("object", "attrib", "source_id");'
		+'CREATE INDEX by_src_value ON object_values ("attrib", "source_id", "value");'
		;

	var _execInTx = function(db, sql, values, resultHdlr/*(tx,resultSet)*/, errHdlr/*(db|tx,err)*/) {
		db.transaction(function (tx) {
			tx.executeSql(sql, values, resultHdlr, errHdlr);
		}, function(err){
			if(errHdlr) errHdlr(db, err);
		});
	};

	var klass_webSqlStorage = function(dbName) {
		this.dbName = dbName;
	};

	klass_webSqlStorage.prototype.open = function(toExecute/*str|func*/, errHdlr/*(db|tx,err)*/)
	{
		try {
			var db = openDatabase(this.dbName, '1.0', 'Rhodes sync database', 2 * 1024 * 1024);
			if ("string" == typeof toExecute) {
					_execInTx(db, toExecute, null, null, errHdlr);
			} else if ("function" == typeof toExecute) {
				toExecute(db);
			}
		} catch(ex) {
			if(errHdlr) errHdlr(null, ex);
		}
	};
		
	klass_webSqlStorage.prototype.close = function()/*throws DBException*/{};

	klass_webSqlStorage.prototype.executeSQL = function(sql, values, resultHdlr/*(tx,resultSet)*/, errHdlr/*(db|tx,err)*/)
	{
		this.open(function(db){
			_execInTx(db, sql, values, resultHdlr, errHdlr);
		});
	};
		
	klass_webSqlStorage.prototype.executeBatchSQL = function(sql, errHdlr/*(db|tx,err)*/)
	{
		var statements = sql.split(";");
		for(var i in statements) {
			var stmt = statements[i];
			if(stmt) { // means: it is defined and not null and not empty string
				this.executeSQL(statements[i], null, null, errHdlr);
			}
		}
	};
		
	klass_webSqlStorage.prototype.initSchema = function(errHdlr/*(db|tx,err)*/)
	{
		this.executeBatchSQL(syncDbSchemaSQL, errHdlr);
	};
		
	klass_webSqlStorage.prototype.getAllTableNames = function(errHdlr/*(db|tx,err)*/)
	{
		var tableNames = [];
		this.executeSQL("SELECT name FROM sqlite_master WHERE type='table'", null, jQuery.proxy(function(tx,rs){
			for(var i=0; i<rs.rows.length; i++) {
				tableNames.push(rs.rows.item(i)['name']);
			}
		}, this), errHdlr);
		// there is an error: tableNames is empty at the moment of return!
		return tableNames;
	};

//			/*IDBResult*/ createResult: function(){},
//			/*void*/ deleteAllFiles: function(/*String*/ strPath)throws Exception{},
//			
//			/*void*/ startTransaction: function()/*throws DBException*/{},
//			/*void*/ commit: function()/*throws DBException*/{},
//			/*void*/ rollback: function()/*throws DBException*/{},
//			/*void*/ onBeforeCommit: function() /*throws DBException*/{},
//			
//			/*boolean*/ isTableExists: function(/*String*/ strName)/*throws DBException*/{},
//			/*boolean*/ isDbFileExists: function(/*String*/ strPath){},
//			
//			/*void*/ OnInsertObjectRecord(/*Object*/ NEW_source_id, /*Object*/ NEW_attrib ){},
//
//			/*void*/ createTriggers: function() /*throws DBException*/{},
//			/*void*/ setDbCallback: function(/*IDBCallback*/ callback){},

			
	// === Model ==================================================

	var klass_model = function(name, opts, props) {
		this.name = name;
		this.options = opts;
		this.properties = props;
	}
	
	// === Config ==================================================

	var klass_config = function() {
	}
	
	// === Engine ==================================================

	var klass_engine = function() {
	}
	
	// === API load support =======================================

	var _gwtClasses = ["com.rho.sync.SyncThread"];         	              	
	
	// Function to report the exact GWT class is ready to use.
	//
	// GWT class definitions are bound to load event and number
	// of them is unknown at the moment of this code runs.
	//
	// So we unable to rely on load event to have all GWT classes
	// defined completely.
	//
	// We use this function to report each class is ready, and
	// Rhomobile.event.apiReady event to fire an application
	// when all of GWT classes is ready to use.
	//
	var _gwtClassReady = function(className){
		for(var i in _gwtClasses) {
			if(className == _gwtClasses[i]) {
				_gwtClasses.splice(i, 1);
				if(0 == _gwtClasses.length) {
					jQuery(window).trigger(Rhomobile.event.apiReady);
				}
			}
		}
	};

	// === Public members =========================================

	return {
		internal: 	{
			gwtClassReady: _gwtClassReady
		},         	              	
		event: {
			apiReady: "Rhomobile.event.apiReady"
		},
		Config: klass_config,
		sync: {
			Model: klass_model,
			Engine: klass_engine
		},
		db: {
			DbStorage: klass_webSqlStorage
		}
	};
}();
