#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SQLite3"


rho::String js_SQLite3_open(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_close(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_startTransaction(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_commitTransaction(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_rollbackTransaction(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_lockDb(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_unlockDb(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_destroyTables(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_isTableExist(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_isUiWaitForDb(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_SQLite3_execute(rho::json::CJSONArray& argv, const rho::String& strObjID);





extern "C" void Init_JSAPI_SQLite3(void)
{

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:open", js_SQLite3_open);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:close", js_SQLite3_close);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:startTransaction", js_SQLite3_startTransaction);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:commitTransaction", js_SQLite3_commitTransaction);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:rollbackTransaction", js_SQLite3_rollbackTransaction);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:lockDb", js_SQLite3_lockDb);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:unlockDb", js_SQLite3_unlockDb);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:destroyTables", js_SQLite3_destroyTables);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:isTableExist", js_SQLite3_isTableExist);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:isUiWaitForDb", js_SQLite3_isUiWaitForDb);

    rho::apiGenerator::js_define_method("Rho:Database:SQLite3:execute", js_SQLite3_execute);


}

