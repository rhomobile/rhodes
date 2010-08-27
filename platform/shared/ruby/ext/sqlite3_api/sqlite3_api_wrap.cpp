#include "sqlite/sqlite3.h"
#include "ruby.h"
#include "db/DBAdapter.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "DB"

using namespace rho::db;
extern "C" VALUE ruby_db_execute(int argc, VALUE *argv, VALUE self)
{
	//sqlite3 * db = NULL;
	void **ppDB = NULL;		
	//sqlite3_stmt *statement = NULL;
	const char* sql = NULL;
	VALUE arRes = rb_ary_new();
    //VALUE* colNames = NULL;
	//int nRes = 0;
    //char * szErrMsg = 0;
    int is_batch = 0;

	if ((argc < 2) || (argc > 3))
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 3)",argc);
	
	Data_Get_Struct(self, void *, ppDB);
	//db = (sqlite3 *)rho_db_get_handle(*ppDB);
	sql = RSTRING_PTR(argv[0]);
    is_batch = argv[1] == Qtrue ? 1 : 0;

    RAWTRACE1("db_execute: %s", sql);
	
	rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)(*ppDB));
	rho::db::CDBError dbError;
    if ( is_batch )
    {
		db.Lock();
		db.executeBatch(sql, dbError);
		db.Unlock();
	}else {

		db.Lock();
		
		DBResultPtr res = db.prepareStatement( sql );
		if ( !res->getDBError().isOK() )
		{
			db.Unlock();
            rb_raise(rb_eArgError, "could not prepare statement: %d; Message: %s",
					 res->getDBError().getCode(), res->getDBError().getError().c_str() );
		}
		
        if ( argc > 2 )
        {
			sqlite3_stmt *statement = res->getStatement();
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
		
		res = db.executeStatement(res);
		dbError = res->getDBError();
		if ( dbError.isSuccessfulCode() ) 
		{
			sqlite3_stmt *statement = res->getStatement();
		
			for ( ; !res->isEnd(); res->next() )
			{
				VALUE hashRec = rb_hash_new();
			
				for( int nCol = 0; nCol < res->getColCount(); nCol ++)
				{
					int nColType = sqlite3_column_type(statement,nCol);
					const char* szColName = sqlite3_column_name(statement,nCol);
					VALUE colName = rb_str_new2(szColName);
					VALUE colValue = Qnil;
			
					switch(nColType)
					{
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
			
					rb_hash_aset(hashRec, colName, colValue);
				}
		
				rb_ary_push(arRes, hashRec);
			}
		}
		
		db.Unlock();
	}

    if ( !dbError.isSuccessfulCode() ) 
    {
        rb_raise(rb_eArgError, "could not execute statement: %d; Message: %s",dbError.getCode(), dbError.getError().c_str());
    }
	
	return arRes;
}

