

(function ($, rho, rhoUtil) {
    'use strict';

    var moduleNS = 'Rho.Database';
    var apiReq = rhoUtil.apiReqFor(moduleNS);

    var _execute_sql = function(db, sql, is_batch, args) {
        return (sql === undefined) ? [] : db.execute(sql, is_batch, args);
    }

    // === Database class definition ===

    function Database() {
        var id = null;
        this.getId = function () {return id;};

        if (1 == arguments.length && arguments[0][rhoUtil.rhoIdParam()]) {
            if (moduleNS != arguments[0][rhoUtil.rhoClassParam()]) {
                throw "Wrong class instantiation!";
            }
            id = arguments[0][rhoUtil.rhoIdParam()];
        } else {
            id = rhoUtil.nextId();
            // constructor methods are following:
            
                this.initialize.apply(this, arguments);
            
        }
    };

    // === Database instance members ===

    
        Database.prototype.initialize = function(/* const rho::String& */ dbPath, /* const rho::String& */ dbPartition, /* optional function */ oResult) {
            this.dbPath = dbPath;
            this.database = new Rho.Database.SQLite3(dbPath, dbPartition);
        };

    
        Database.prototype.close = function(/* optional function */ oResult) {
            if (this.database === null) {
                return false;
            }
            this.database.close();
            this.dbPath = null;
            this.database = null;
            return true;
        };

    
        Database.prototype.startTransaction = function(/* optional function */ oResult) {
            this.database.startTransaction();
        };

    
        Database.prototype.commitTransaction = function(/* optional function */ oResult) {
            this.database.commitTransaction();
        };

    
        Database.prototype.rollbackTransaction = function(/* optional function */ oResult) {
            this.database.rollbackTransaction();
        };

    
        Database.prototype.lockDb = function(/* optional function */ oResult) {
            this.database.lockDb();
        };

    
        Database.prototype.unlockDb = function(/* optional function */ oResult) {
            this.database.unlockDb();
        };

    
        Database.prototype.isUiWaitForDb = function(/* optional function */ oResult) {
            return this.database.isUiWaitForDb();
        };

    
        Database.prototype.executeSql = function(/* const rho::String& */ sqlStmt, /* const rho::Vector<rho::String>& */ args, /* optional function */ oResult) {
            return _execute_sql(this.database, sqlStmt, false, args);
        };

    
        Database.prototype.executeBatchSql = function(/* const rho::String& */ sqlStmt, /* const rho::Vector<rho::String>& */ args, /* optional function */ oResult) {
            return _execute_sql(this.database, sqlStmt, true, args);
        };

    
        Database.prototype.import = function(/* const rho::String& */ zipName, /* optional function */ oResult) {
            this.database.import(zipName);
        };

    
        Database.prototype.export = function(/* optional function */ oResult) {
            this.database.export();
        };

    
        Database.prototype.destroyTable = function(/* const rho::String& */ tableName, /* optional function */ oResult) {
            this.destroyTables({'include': tableName});
        };

    
        Database.prototype.destroyTables = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap, /* optional function */ oResult) {
            this.database.destroyTables(propertyMap['include'], propertyMap['exclude']);
        };

    
        Database.prototype.isTableExist = function(/* const rho::String& */ tableName, /* optional function */ oResult) {
            return this.database.isTableExist(tableName);
        };

    
        Database.prototype.setDoNotBackupAttribute = function(/* bool */ setFlag, /* optional function */ oResult) {
            setFlag = (setFlag === undefined) ? true : setFlag;
            if (Rho.System.getProperty('platform') === 'APPLE') {
                Rho.System.setDoNotBackupAttribute(this.dbpath             , setFlag);
                Rho.System.setDoNotBackupAttribute(this.dbpath + '.version', setFlag);
            }                         
        };

    

    // === Database static members ===

    

    

    // === Database default instance support ===

    

    rhoUtil.namespace(moduleNS, Database);

})(jQuery, Rho, Rho.util);
