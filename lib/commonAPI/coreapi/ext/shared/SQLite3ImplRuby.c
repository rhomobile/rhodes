#include "sqlite/sqlite3.h"
#include "ruby.h"

#include "statistic/RhoProfiler.h"
#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SQLite3"

void* rho_db_get_handle(void* pDB);
int rho_db_prepare_statement(void* pDB, const char* szSql, int nByte, sqlite3_stmt **ppStmt);
void rho_db_lock(void* pDB);
void rho_db_unlock(void* pDB);

unsigned long rb_c_impl_SQLite3_execute(int argc, VALUE *argv, void* pDB)
{
    sqlite3 * db = NULL;
	void **ppDB = &pDB;		
	sqlite3_stmt *statement = NULL;
	const char* sql = NULL;
	VALUE arRes = rb_ary_new();
    VALUE* colNames = NULL;
	int nRes = 0;
    char * szErrMsg = 0;
    int is_batch = 0;

	if ((argc < 2) || (argc > 3))
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 3)",argc);
	
	db = (sqlite3 *)rho_db_get_handle(*ppDB);
	sql = RSTRING_PTR(argv[0]);
    is_batch = argv[1] == Qtrue ? 1 : 0;

    RAWTRACE1("db_execute: %s", sql);

    PROF_START_CREATED("SQLITE");
    if ( is_batch )
    {
        PROF_START_CREATED("SQLITE_EXEC");

        rho_db_lock(*ppDB);
        nRes = sqlite3_exec(db, sql,  NULL, NULL, &szErrMsg);
        rho_db_unlock(*ppDB);

        PROF_STOP("SQLITE_EXEC");
    }
    else
    {
        rho_db_lock(*ppDB);
        PROF_START_CREATED("SQLITE_PREPARE");
        nRes = rho_db_prepare_statement(*ppDB, sql, -1, &statement);
        PROF_STOP("SQLITE_PREPARE");
        //nRes = sqlite3_prepare_v2(db, sql, -1, &statement, NULL);
        if ( nRes != SQLITE_OK)
        {
            szErrMsg = (char *)sqlite3_errmsg(db);
            rho_db_unlock(*ppDB);

            rb_raise(rb_eArgError, "could not prepare statement: %d; Message: %s",nRes, (szErrMsg?szErrMsg:""));
        }

        if ( (argc > 2) && (TYPE(argv[2]) == T_ARRAY) )
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
                    sqlite3_bind_text(statement, i+1, RSTRING_PTR(arg), RSTRING_LEN(arg), SQLITE_TRANSIENT);
                    break;
                case T_FLOAT:
                    sqlite3_bind_double(statement, i+1, NUM2DBL(arg));
                    break;
                case T_FIXNUM:
                case T_BIGNUM:
                    sqlite3_bind_int64(statement, i+1, NUM2LL(arg));
                    break;
                case T_DATA:
                    if (CLASS_OF(arg) == rb_cTime)
                    {
                        VALUE intVal = rb_funcall(arg, rb_intern("to_i"), 0);	
                        sqlite3_bind_int64(statement, i+1, NUM2LL(intVal));
                        break;
                    }

                default:
					{
						VALUE strVal = rb_funcall(arg, rb_intern("to_s"), 0);	
	                    sqlite3_bind_text(statement, i+1, RSTRING_PTR(strVal), -1, SQLITE_TRANSIENT);	
					}
					break;
                }
            }
        }

        PROF_START_CREATED("SQLITE_EXEC");
        nRes = sqlite3_step(statement);
        PROF_STOP("SQLITE_EXEC");

	    while( nRes== SQLITE_ROW ) {
		    int nCount = sqlite3_data_count(statement);
		    int nCol = 0;
		    VALUE hashRec = rb_hash_new();

            //if ( !colNames )
            //    colNames = getColNames(statement, nCount);

		    for(;nCol<nCount;nCol++){
			    int nColType = sqlite3_column_type(statement,nCol);
			    const char* szColName = sqlite3_column_name(statement,nCol);
                
			    //VALUE colName = rb_str_new2(szColName);
                VALUE colName = rb_utf8_str_new_cstr(szColName);

                VALUE colValue = Qnil;
    			
			    switch(nColType){
				    case SQLITE_NULL:
					    break;
                    case SQLITE_FLOAT:
                    {
                        double dVal = sqlite3_column_double(statement, nCol);
                        colValue = DBL2NUM(dVal);
                        break;
                    }
                    case SQLITE_INTEGER:
                    {
                        sqlite_int64 nVal = sqlite3_column_int64(statement, nCol);
                        colValue = LL2NUM(nVal);
                        break;
                    }
				    default:{
                        sqlite3_value * sqlValue = sqlite3_column_value(statement, nCol);
                        int nLen = sqlite3_value_bytes(sqlValue);
                        const char*  szValue = (const char *)sqlite3_value_text(sqlValue);
					    //char *text = (char *)sqlite3_column_text(statement, nCol);
					    
                        //colValue = rb_str_new(szValue, nLen);
                        colValue = rb_utf8_str_new(szValue, nLen);
					    break;
				    }
			    }
    			
			    rb_hash_aset(hashRec, colName/*colNames[nCol]*/, colValue);
		    }
    		
		    rb_ary_push(arRes, hashRec);

            PROF_START_CREATED("SQLITE_EXEC");
            nRes = sqlite3_step(statement);
            PROF_STOP("SQLITE_EXEC");

	    }

        rho_db_unlock(*ppDB);

    }

    if ( statement )
        //sqlite3_finalize(statement);
        sqlite3_reset(statement);

    if ( colNames )
        free(colNames);

    if ( nRes != SQLITE_OK && nRes != SQLITE_ROW && nRes != SQLITE_DONE )
    {
        if ( !szErrMsg )
            szErrMsg = (char*)sqlite3_errmsg(db);

        rb_raise(rb_eArgError, "could not execute statement: %d; Message: %s",nRes, (szErrMsg?szErrMsg:""));
    }

    PROF_STOP("SQLITE");

	return arRes;
}
