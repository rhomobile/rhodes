#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Megamodule"


rho::String js_Megamodule_getStringProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_getStringProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_getIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_getIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_setIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_setIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_typesTest(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_typesTest(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_produceArray(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_produceArray(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_produceHash(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_produceHash(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_produceComplicatedResult(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_produceComplicatedResult(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_Megamodule_getObjectsCount(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_Megamodule_getObjectByIndex(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_setPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_setPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_isPeriodicallyCallbackSetted(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_isPeriodicallyCallbackSetted(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_stopPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_stopPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_complicatedTypesTest1(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_complicatedTypesTest1(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Megamodule_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_def_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_Megamodule_enumerate(rho::json::CJSONArray& argv, const rho::String& strObjID);



rho::String js_s_Megamodule_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_Megamodule_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);



extern "C" void Init_JSAPI_Megamodule(void)
{

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:StringProperty", js_Megamodule_getStringProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_StringProperty", js_s_Megamodule_def_getStringProperty);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:IntegerProperty", js_Megamodule_getIntegerProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_IntegerProperty", js_s_Megamodule_def_getIntegerProperty);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:IntegerProperty=", js_Megamodule_setIntegerProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_IntegerProperty=", js_s_Megamodule_def_setIntegerProperty);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:typesTest", js_Megamodule_typesTest);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_typesTest", js_s_Megamodule_def_typesTest);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:produceArray", js_Megamodule_produceArray);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_produceArray", js_s_Megamodule_def_produceArray);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:produceHash", js_Megamodule_produceHash);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_produceHash", js_s_Megamodule_def_produceHash);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:produceComplicatedResult", js_Megamodule_produceComplicatedResult);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_produceComplicatedResult", js_s_Megamodule_def_produceComplicatedResult);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:getObjectsCount", js_s_Megamodule_getObjectsCount);

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:getObjectByIndex", js_s_Megamodule_getObjectByIndex);

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:showAlertFromUIThread", js_Megamodule_showAlertFromUIThread);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_showAlertFromUIThread", js_s_Megamodule_def_showAlertFromUIThread);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:setPeriodicallyCallback", js_Megamodule_setPeriodicallyCallback);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_setPeriodicallyCallback", js_s_Megamodule_def_setPeriodicallyCallback);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:isPeriodicallyCallbackSetted", js_Megamodule_isPeriodicallyCallbackSetted);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_isPeriodicallyCallbackSetted", js_s_Megamodule_def_isPeriodicallyCallbackSetted);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:stopPeriodicallyCallback", js_Megamodule_stopPeriodicallyCallback);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_stopPeriodicallyCallback", js_s_Megamodule_def_stopPeriodicallyCallback);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:complicatedTypesTest1", js_Megamodule_complicatedTypesTest1);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_complicatedTypesTest1", js_s_Megamodule_def_complicatedTypesTest1);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:getProperty", js_Megamodule_getProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_getProperty", js_s_Megamodule_def_getProperty);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:getProperties", js_Megamodule_getProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_getProperties", js_s_Megamodule_def_getProperties);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:getAllProperties", js_Megamodule_getAllProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_getAllProperties", js_s_Megamodule_def_getAllProperties);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:setProperty", js_Megamodule_setProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_setProperty", js_s_Megamodule_def_setProperty);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:setProperties", js_Megamodule_setProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_setProperties", js_s_Megamodule_def_setProperties);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:clearAllProperties", js_Megamodule_clearAllProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:def_clearAllProperties", js_s_Megamodule_def_clearAllProperties);;

    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:enumerate", js_s_Megamodule_enumerate);


    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:getDefaultID", js_s_Megamodule_getDefaultID);
    rho::apiGenerator::js_define_method("Rho:Examples:Megamodule:setDefaultID", js_s_Megamodule_setDefaultID);

}

