#include "sqlite/sqlite3.h"
#include "ruby.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "DB"

static VALUE mSqlite3;
static VALUE mDatabase;

extern int rho_db_open(const char* szDBPath, const char* szDBPartition, void** ppDB);
//extern int rho_sync_closeDB();
extern int rho_db_startTransaction(void* pDB);
extern int rho_db_commitTransaction(void* pDB);
extern int rho_db_rollbackTransaction(void* pDB);
extern int rho_db_destroy_tables(void* pDB, unsigned long arInclude, unsigned long arExclude);
extern void* rho_db_get_handle(void* pDB);
extern int rho_db_is_ui_waitfordb(void* pDB);
extern int rho_db_prepare_statement(void* pDB, const char* szSql, int nByte, sqlite3_stmt **ppStmt);
extern void rho_db_lock(void* pDB);
extern void rho_db_unlock(void* pDB);
extern int  rho_db_is_table_exist(void* pDB, const char* szTableName);

static VALUE db_allocate(VALUE klass)
{
	//sqlite3 **db = malloc(sizeof(sqlite3 **));
    void* pDB = malloc(sizeof(void*));
	return Data_Wrap_Struct(klass, 0, 0, pDB);
}

static VALUE db_init(int argc, VALUE *argv, VALUE self)
{
	const char *szDbName = NULL;
    const char *szDbPartition = NULL;
	void **ppDB = NULL;
	int result;
	
	if ((argc < 2) || (argc > 2))
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 2)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	szDbName = STR2CSTR(argv[0]);
    szDbPartition = STR2CSTR(argv[1]);
	
	result = (int)rho_db_open(szDbName, szDbPartition, ppDB);//sqlite3_open(szDbName,ppDB);
	if ( result != SQLITE_OK )
		rb_raise(rb_eArgError, "could open database:%d",result);
	
	return self;
}

static VALUE db_close(int argc, VALUE *argv, VALUE self){
	//sqlite3 * db = NULL;
	void **ppDB = NULL;		
	int rc = 0;
	
	if (argc > 0)
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	
    //do not close sync db, close it at exit
	rc = 0;//rho_sync_closeDB();//sqlite3_close(db);
	
	return INT2NUM(rc);
}

static VALUE db_start_transaction(int argc, VALUE *argv, VALUE self){
	//sqlite3 * db = NULL;
	void **ppDB = NULL;		
	int rc = 0;
	
	if (argc > 0)
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	
	rc = rho_db_startTransaction(*ppDB);
	
	return INT2NUM(rc);
}

static VALUE db_commit(int argc, VALUE *argv, VALUE self){
	//sqlite3 * db = NULL;
	void **ppDB = NULL;		
	int rc = 0;
	
	if (argc > 0)
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	
	rc = rho_db_commitTransaction(*ppDB);
	
	return INT2NUM(rc);
}

static VALUE db_rollback(int argc, VALUE *argv, VALUE self){
	//sqlite3 * db = NULL;
	void **ppDB = NULL;		
	int rc = 0;
	
	if (argc > 0)
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	
	rc = rho_db_rollbackTransaction(*ppDB);
	
	return INT2NUM(rc);
}

static VALUE db_lock(int argc, VALUE *argv, VALUE self){
	//sqlite3 * db = NULL;
	void **ppDB = NULL;		
	int rc = 0;
	
	if (argc > 0)
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	
	rho_db_lock(*ppDB);
	
	return INT2NUM(rc);
}

static VALUE db_unlock(int argc, VALUE *argv, VALUE self){
	//sqlite3 * db = NULL;
	void **ppDB = NULL;		
	int rc = 0;
	
	if (argc > 0)
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	
	rho_db_unlock(*ppDB);
	
	return INT2NUM(rc);
}

static VALUE* getColNames(sqlite3_stmt* statement, int nCount)
{
    int nCol = 0;
    VALUE* res = malloc(sizeof(VALUE)*nCount);

	for(;nCol<nCount;nCol++)
    {
		const char* szColName = sqlite3_column_name(statement,nCol);
		res[nCol] = rb_str_new2(szColName);
    }

    return res;
}

static VALUE db_destroy_tables(int argc, VALUE *argv, VALUE self)
{
	void **ppDB = NULL;		
    int rc = 0;

	if ((argc < 2) || (argc > 2))
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 2)",argc);

	Data_Get_Struct(self, void *, ppDB);

    rc = rho_db_destroy_tables(*ppDB, argv[0], argv[1]);

    return INT2NUM(rc);
}

static VALUE db_is_table_exist(int argc, VALUE *argv, VALUE self)
{
	void **ppDB = NULL;		
    int rc = 0;
    const char * szTableName = 0;

	if ((argc < 1) || (argc > 1))
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 1)",argc);

	Data_Get_Struct(self, void *, ppDB);
    szTableName = RSTRING_PTR(argv[0]);

    rc = rho_db_is_table_exist(*ppDB, szTableName);

    return rc ? Qtrue : Qfalse;
}

static VALUE db_is_ui_waitfordb(int argc, VALUE *argv, VALUE self)
{
	void **ppDB = NULL;		
    int rc = 0;

	if (argc > 0)
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 0)",argc);

	Data_Get_Struct(self, void *, ppDB);

    rc = rho_db_is_ui_waitfordb(*ppDB);

    return rc == 0 ? Qfalse : Qtrue;
}

static VALUE db_execute(int argc, VALUE *argv, VALUE self)
{
	sqlite3 * db = NULL;
	void **ppDB = NULL;		
	sqlite3_stmt *statement = NULL;
	const char* sql = NULL;
	VALUE arRes = rb_ary_new();
    VALUE* colNames = NULL;
	int nRes = 0;
    char * szErrMsg = 0;
    int is_batch = 0;

	if ((argc < 2) || (argc > 3))
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 3)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	db = (sqlite3 *)rho_db_get_handle(*ppDB);
	sql = RSTRING_PTR(argv[0]);
    is_batch = argv[1] == Qtrue ? 1 : 0;

    RAWTRACE1("db_execute: %s", sql);

    if ( is_batch )
    {
        rho_db_lock(*ppDB);
        nRes = sqlite3_exec(db, sql,  NULL, NULL, &szErrMsg);
        rho_db_unlock(*ppDB);
    }
    else
    {
        //nRes = rho_db_prepare_statement(*ppDB, sql, -1, &statement);
        nRes = sqlite3_prepare_v2(db, sql, -1, &statement, NULL);
        if ( nRes != SQLITE_OK)
        {
            szErrMsg = (char *)sqlite3_errmsg(db);

            rb_raise(rb_eArgError, "could not prepare statement: %d; Message: %s",nRes, (szErrMsg?szErrMsg:""));
        }

        if ( argc > 2 )
        {
            int i = 0;
            VALUE args = argv[2];
            if ( RARRAY_LEN(args) > 0 && TYPE(RARRAY_PTR(args)[0]) == T_ARRAY )
                args = RARRAY_PTR(args)[0];

            for( ; i < RARRAY_LEN(args); i++ )
            {
                VALUE arg = RARRAY_PTR(args)[i];
                if (NIL_P(arg))
                {
                    sqlite3_bind_null(statement, i+1);
                    continue;
                }

                switch( TYPE(arg) )
                {
                case T_STRING:
                    sqlite3_bind_text(statement, i+1, RSTRING_PTR(arg), -1, SQLITE_TRANSIENT);
                    break;
                case T_FIXNUM:
                case T_FLOAT:
                case T_BIGNUM:
                    sqlite3_bind_int64(statement, i+1, NUM2LL(arg));
                    break;
                default:
					{
						VALUE strVal = rb_funcall(arg, rb_intern("to_s"), 0);	
	                    sqlite3_bind_text(statement, i+1, RSTRING_PTR(strVal), -1, SQLITE_TRANSIENT);	
					}
					break;
                }
            }
        }

        rho_db_lock(*ppDB);
	    while( (nRes=sqlite3_step(statement)) == SQLITE_ROW) {
		    int nCount = sqlite3_data_count(statement);
		    int nCol = 0;
		    VALUE hashRec = rb_hash_new();

            //if ( !colNames )
            //    colNames = getColNames(statement, nCount);

		    for(;nCol<nCount;nCol++){
			    int nColType = sqlite3_column_type(statement,nCol);
			    const char* szColName = sqlite3_column_name(statement,nCol);
			    VALUE colName = rb_str_new2(szColName);
			    VALUE colValue = Qnil;
    			
			    switch(nColType){
				    case SQLITE_NULL:
					    break;
                    case SQLITE_INTEGER:
                    {
                        sqlite_int64 nVal = sqlite3_column_int64(statement, nCol);
                        colValue = LL2NUM(nVal);
                        break;
                    }
				    default:{
					    char *text = (char *)sqlite3_column_text(statement, nCol);
					    colValue = rb_str_new2(text);
					    break;
				    }
			    }
    			
			    rb_hash_aset(hashRec, colName/*colNames[nCol]*/, colValue);
		    }
    		
		    rb_ary_push(arRes, hashRec);
	    }

        rho_db_unlock(*ppDB);
    }

    if ( statement )
        sqlite3_finalize(statement);
    if ( colNames )
        free(colNames);

    if ( nRes != SQLITE_OK && nRes != SQLITE_ROW && nRes != SQLITE_DONE )
    {
        if ( !szErrMsg )
            szErrMsg = (char*)sqlite3_errmsg(db);

        rb_raise(rb_eArgError, "could not execute statement: %d; Message: %s",nRes, (szErrMsg?szErrMsg:""));
    }

	return arRes;
}

void Init_sqlite3_api(void)
{
	mSqlite3 = rb_define_module("SQLite3");
	mDatabase = rb_define_class_under(mSqlite3, "Database", rb_cObject);
	
	rb_define_alloc_func(mDatabase, db_allocate);
	rb_define_method(mDatabase, "initialize", db_init, -1);
	rb_define_method(mDatabase, "close", db_close, -1);
	rb_define_method(mDatabase, "execute", db_execute, -1);	
	rb_define_method(mDatabase, "start_transaction", db_start_transaction, -1);	
	rb_define_method(mDatabase, "commit", db_commit, -1);	
    rb_define_method(mDatabase, "rollback", db_rollback, -1);	
    rb_define_method(mDatabase, "lock_db", db_lock, -1);	
    rb_define_method(mDatabase, "unlock_db", db_unlock, -1);	
    rb_define_method(mDatabase, "destroy_tables", db_destroy_tables, -1);	
    rb_define_method(mDatabase, "table_exist?", db_is_table_exist, -1);
    rb_define_method(mDatabase, "is_ui_waitfordb", db_is_ui_waitfordb, -1);
}

