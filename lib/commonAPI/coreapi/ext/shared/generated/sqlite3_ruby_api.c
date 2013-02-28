
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_mSQLite3;

VALUE rb_SQLite3_open(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_close(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_startTransaction(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_commitTransaction(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_rollbackTransaction(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_lockDb(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_unlockDb(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_destroyTables(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_isTableExist(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_isUiWaitForDb(int argc, VALUE *argv, VALUE obj);
VALUE rb_SQLite3_execute(int argc, VALUE *argv, VALUE obj);




VALUE getRuby_SQLite3_Module(){ return rb_mSQLite3; }



static void _free_class_object(void *p)
{
    ruby_xfree(p);
}

static VALUE _allocate_class_object(VALUE klass)
{
    VALUE valObj = 0;
    char ** ppString = NULL;
    void* pData = ALLOC(void*);
    memset( pData, 0, sizeof(pData) );
    
	valObj = Data_Wrap_Struct(klass, 0, _free_class_object, pData);

    Data_Get_Struct(valObj, char *, ppString);
    *ppString = xmalloc(10);
    sprintf(*ppString, "%X", valObj);

    return valObj;
}

void Init_RubyAPI_SQLite3(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    
    tmpParent = rb_mParent;
    rb_mParent = rb_define_module_under(tmpParent, "Database");
    

	rb_mSQLite3 = rb_define_class_under(rb_mParent, "SQLite3", rb_cObject);

	rb_define_alloc_func(rb_mSQLite3, _allocate_class_object);
    //Constructor should be not available in case of static members
    //rb_undef_alloc_func(rb_mSQLite3);

    rb_define_method(rb_mSQLite3, "initialize", rb_SQLite3_open, -1);
    rb_define_method(rb_mSQLite3, "close", rb_SQLite3_close, -1);
    rb_define_method(rb_mSQLite3, "startTransaction", rb_SQLite3_startTransaction, -1);
    rb_define_method(rb_mSQLite3, "commitTransaction", rb_SQLite3_commitTransaction, -1);
    rb_define_method(rb_mSQLite3, "rollbackTransaction", rb_SQLite3_rollbackTransaction, -1);
    rb_define_method(rb_mSQLite3, "lockDb", rb_SQLite3_lockDb, -1);
    rb_define_method(rb_mSQLite3, "unlockDb", rb_SQLite3_unlockDb, -1);
    rb_define_method(rb_mSQLite3, "destroyTables", rb_SQLite3_destroyTables, -1);
    rb_define_method(rb_mSQLite3, "isTableExist", rb_SQLite3_isTableExist, -1);
    rb_define_method(rb_mSQLite3, "isUiWaitForDb", rb_SQLite3_isUiWaitForDb, -1);
    rb_define_method(rb_mSQLite3, "execute", rb_SQLite3_execute, -1);






    rb_define_alias(rb_mSQLite3, "start_transaction", "startTransaction");
    
    rb_define_alias(rb_mSQLite3, "commit", "commitTransaction");
    
    rb_define_alias(rb_mSQLite3, "rollback", "rollbackTransaction");
    
    rb_define_alias(rb_mSQLite3, "lock_db", "lockDb");
    
    rb_define_alias(rb_mSQLite3, "unlock_db", "unlockDb");
    
    rb_define_alias(rb_mSQLite3, "destroy_tables", "destroyTables");
    
    rb_define_alias(rb_mSQLite3, "table_exist?", "isTableExist");
    
    rb_define_alias(rb_mSQLite3, "is_ui_waitfordb", "isUiWaitForDb");
    
    rb_define_alias(rb_mSQLite3, "commit_transaction", "commitTransaction");
    
    rb_define_alias(rb_mSQLite3, "rollback_transaction", "rollbackTransaction");
    
    rb_define_alias(rb_mSQLite3, "is_table_exist", "isTableExist");
    
    rb_define_alias(rb_mSQLite3, "is_ui_wait_for_db", "isUiWaitForDb");
    


}

