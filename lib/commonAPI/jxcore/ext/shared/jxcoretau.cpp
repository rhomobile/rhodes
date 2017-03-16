
#include "jxcoretau.h"
#include "jxcore/jx.h"
#include "jxcore/jx_result.h"



#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "common/ThreadQueue.h"

#include "common/RhodesApp.h"
#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "common/RhoConf.h"

#include "api_generator/js_helpers.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "jxCoreTAU"


//extern "C" void do_test_extension();

//extern rho::String js_jxCore_entry_point(const char* szJSON);


static bool our_is_http_server_started = false;

static void callback(JXValue *results, int argc) { }


static void call_js_entry_point(JXValue *results, int argc) {
    
    const char *data = JX_GetString(results+0);

    rho::String result = rho::apiGenerator::js_entry_point(data);
    
    
    
    
    int o;
    o = 9;
    
    JXValue eval_res;
    
    
    rho::String script = "global.RhomobileEngine.tau_js_entry_point_result="+result+";";
    
    bool is_ok = JX_Evaluate(script.c_str(), NULL, &eval_res);

    o = 10;
    
}


static void callServerStarted(JXValue *results, int argc) {
    our_is_http_server_started = true;
}

static void callJXcoreNative(JXValue *results, int argc) {
    // registered callJXcoreNative extension - JS code call to Native
    
    char *data = JX_GetString(results+0);
    
    
    int o;
    o = 9;
    
    /*
     
     if (argc < 2 || !JX_IsString(results + 0) || !JX_IsString(results+(argc-1))) {
     NSLog(@"Unknown call received to callJXcoreNative. First and last parameters have to be string\n");
     return;
     }
     
     NSMutableArray *params = [[NSMutableArray alloc] init];
     
     char *dt_name = JX_GetString(results + 0);
     NSString *name = [NSString stringWithUTF8String:dt_name];
     free(dt_name);
     
     dt_name = JX_GetString(results + (argc-1));
     NSString *callbackId = [NSString stringWithUTF8String:dt_name];
     free(dt_name);
     
     ConvertParams(results+1, argc-1, params);
     
     NSObject *obj = [natives valueForKey:name];
     
     if (obj != nil) {
     if ([obj isKindOfClass:[CPPWrapper class]])
     {
     CPPWrapper *cpp = [natives valueForKey:name];
     if (cpp != nil) {
     JXcoreNative callback = [cpp getCallback];
     callback(params, callbackId);
     }
     } else {
     void (^code_block)(NSArray*, NSString*);
     code_block = (void(^)(NSArray*, NSString*))obj;
     code_block(params, callbackId);
     }
     
     return;
     }
     
     NSLog(@"Native method %@ not found.", name);
     */
}


JXValue pingEvent;
JXValue startApp;
int fnc_count = 0;

static void defineEventCB(JXValue *params, int argc) {
    
    // registered defineEventCB extension for register some events from JS in Native for call events from Bative to JS !
    
    
    JXValue *fnc1 = params + 1;
    //JXValue *fnc2 = params + 2;
    
    
    char *data = JX_GetString(params+0);
    
    if (fnc_count == 0) {
        fnc_count++;
        JX_MakePersistent(fnc1);
        pingEvent = *fnc1;
        
    }
    else {
        JX_MakePersistent(fnc1);
        startApp = *fnc1;
    }
    
    
    int o;
    o = 9;
    /*
     if (argc < 2 || !JX_IsFunction(params+1) || !JX_IsString(params+0)) {
     NSLog(@"defineEventCB expects a function");
     return;
     }
     
     CPPWrapper *cpp = [[CPPWrapper alloc] init];
     JXValue *fnc = params + 1;
     JX_MakePersistent(fnc);
     [cpp setFunction:fnc];
     
     char *data = JX_GetString(params+0);
     NSString *name = [NSString stringWithUTF8String:data];
     free(data);
     
     [natives setObject:cpp forKey:name];
     */
}


void callEventCallback_with_JSON(const char* event_name, const char* json) {
    
    JXValue* fnc = &startApp;
    
    JXValue arr[2];
    
    const char* event_str = event_name;
    int ln_event_str = (int)strlen(event_name);
    JX_New(&arr[0]);
    JX_SetString(&arr[0], event_str, ln_event_str);
    
    if (!JX_CreateArrayObject(&arr[1])) {
        //NSLog(@"Wasnt able to create a new array object on JXcore callEventCallback. No memory left?");
        return;
    }
    
    JXValue value;
    JX_New(&value);
    
    //JX_SetJSON(&value, json, (int)strlen(json));
    JX_SetString(&value, json, (int)strlen(json));
    
    JX_SetIndexedProperty(&arr[1], 0, &value);
    //JX_Free(&value);
    
    
    JXValue ret_val;
    JX_CallFunction(fnc, &value, 1, &ret_val);
    
    JX_Free(&value);
    
    
    JX_Free(&ret_val);

    
    
}









void jxcoreLoop(int n) {
    int result = JX_LoopOnce();
}






class CjxCoreQueue : public rho::common::CThreadQueue
{
public:
    CjxCoreQueue() : CThreadQueue() {
        setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
    }

    virtual void run()
    {
        if(__rhoCurrentCategory.getName() == "NO_LOGGING")
            m_logThreadId = getThreadID();
        
        LOG(INFO) + "Starting main routine...";

        
        int result = 1;
        
        while( !isStopping() )
        {
            while (!m_bNoThreaded && !isStopping() && isNoCommands()) {
                int waitInterval = (result == 0 ? 0.05 : 0.001);
                if ( wait(waitInterval) == 1 ) {
                    onTimeout();
                }
                result = JX_LoopOnce();
            }
            if ( !m_bNoThreaded && !isStopping() ) {
                processCommands();
            }
        }
        LOG(INFO) + "Thread shutdown";
    }
};

class CjxCoreQueue_Command_startJXCore: public rho::common::CThreadQueue::IQueueCommand
{
public:
    CjxCoreQueue_Command_startJXCore( const rho::String& main_filename, const rho::String& xjcore_folderpath, const rho::String& user_folderpath ) : mMainFilepath(main_filename), mJxCoreFolderpath( xjcore_folderpath ), mUserFolderpath(user_folderpath){}
    virtual ~CjxCoreQueue_Command_startJXCore(){};
    virtual bool equals(const IQueueCommand& cmd)
    {
        return false;
    }
    virtual rho::String toString()
    {
        return "CjxCoreQueue_Command_startJXCore";
    }
    virtual void cancel(){}
    
    virtual void execute()
    {
        
        rho::common::CRhoFile main_file;
        bool isOpened = main_file.open(mMainFilepath.c_str(), rho::common::CRhoFile::EOpenModes::OpenReadOnly);
        rho::String mainContents;
        main_file.readString(mainContents);
        main_file.close();
        
        
        rho::String frm_str = "process.setPaths = function() {\n"
                                  "  process.userPath='%s'; "
                                  "  var node_module = require('module');\n"
                                  "  var pathModule = require('path');\n"
                                  "  // ugly patching\n"
                                  "  process.cwd = function () {\n"
                                  "    if (arguments.length) {\n"
                                  "      // or we should throw this as an exception ?\n"
                                  "      // Who knows how many node modules would break..\n"
                                  "      console.error('You are on iOS. This platform does not support setting cwd');\n"
                                  "    }\n"
                                  "    return '%s';\n"
                                  "  };\n"
                                  "  node_module.addGlobalPath(process.cwd());\n"
                                  "  node_module.addGlobalPath(process.userPath);\n"
                                  "};\n%s";
        
        int script_size = ((int)frm_str.size() + (int)mainContents.size() + (int)mJxCoreFolderpath.size() + (int)mUserFolderpath.size() ) * 2;
        char itializationScript[script_size];
        sprintf(itializationScript, frm_str.c_str(), mUserFolderpath.c_str(), mJxCoreFolderpath.c_str(), mainContents.c_str());
        
        JX_Initialize(mJxCoreFolderpath.c_str(), callback);
        JX_InitializeNewEngine();
        JX_DefineExtension("callJXcoreNative", callJXcoreNative);
        //JX_DefineExtension("defineEventCB", defineEventCB);
        JX_DefineExtension("js_entry_point", call_js_entry_point);
        JX_DefineExtension("httpServerStarted", callServerStarted);
        
        
        //do_test_extension();
        
        
        JX_DefineMainFile(itializationScript);
        JX_StartEngine();
        
        jxcoreLoop(0);
        
        
    }
private:
    rho::String mMainFilepath;
    rho::String mJxCoreFolderpath;
    rho::String mUserFolderpath;
};

class CjxCoreQueue_Command_startApp: public rho::common::CThreadQueue::IQueueCommand
{
public:
    CjxCoreQueue_Command_startApp( const rho::String& app_filename) : mAppFilepath(app_filename){}
    virtual ~CjxCoreQueue_Command_startApp(){};
    virtual bool equals(const IQueueCommand& cmd)
    {
        return false;
    }
    virtual rho::String toString()
    {
        return "CjxCoreQueue_Command_startApp";
    }
    virtual void cancel(){}
    
    virtual void execute()
    {
        //callEventCallback_with_JSON("StartApplication", mAppFilepath.c_str());
        rho::String js_code = "Mobile.loadMainFile('"+mAppFilepath+"');";
        
        JXValue eval_res;
        
        LOG(INFO) + "jxCore.executeJS with " + js_code;
        
        bool is_ok = JX_Evaluate(js_code.c_str(), NULL, &eval_res);
        
    }
private:
    rho::String mAppFilepath;
};

class CjxCoreQueue_Command_executeJS: public rho::common::CThreadQueue::IQueueCommand
{
public:
    CjxCoreQueue_Command_executeJS( const rho::String& js_code) : mJSCode(js_code){}
    virtual ~CjxCoreQueue_Command_executeJS(){};
    virtual bool equals(const IQueueCommand& cmd)
    {
        return false;
    }
    virtual rho::String toString()
    {
        return "CjxCoreQueue_Command_startApp";
    }
    virtual void cancel(){}
    
    virtual void execute()
    {
        JXValue eval_res;
        
        LOG(INFO) + "jxCore.executeJS with " + mJSCode;

        bool is_ok = JX_Evaluate(mJSCode.c_str(), NULL, &eval_res);
        
        
    }
private:
    rho::String mJSCode;
};




static CjxCoreQueue* ourJxCoreQueue = NULL;




extern "C" void jxcore_jvm_callback_provider(const char* js_code) {
    
    
    ourJxCoreQueue->addQueueCommand(new CjxCoreQueue_Command_executeJS(js_code));
    
}






extern "C" void jxcoretau_init() {
    setCustomJVMCallbackProvider("jxcore", jxcore_jvm_callback_provider);
    
    const char* user_path = rho_rhodesapp_getuserrootpath();
    rho::String root_path = rho_rhodesapp_getapprootpath();
    rho::String main_filename = root_path + "/nodejs/main.js";
    rho::String jxcore_path = root_path + "/nodejs/server/";
    
    jxcoretau_start(main_filename.c_str(), jxcore_path.c_str(), user_path);
    jxcoretau_start_app("app.js");
    
    while (!jxcoretau_is_http_server_started()) {
        usleep(100000);
        LOG(INFO) + "wait Node.js server ...";
    }
    
    
    
}


extern "C" void jxcoretau_start(const char* main_filename, const char* xjcore_folderpath, const char* user_folderpath) {
    ourJxCoreQueue = new CjxCoreQueue();
    ourJxCoreQueue->start(rho::common::CThreadQueue::epNormal);
    ourJxCoreQueue->addQueueCommand(new CjxCoreQueue_Command_startJXCore(main_filename, xjcore_folderpath, user_folderpath));
}

extern "C" void jxcoretau_start_app(const char* filename) {
    ourJxCoreQueue->addQueueCommand(new CjxCoreQueue_Command_startApp(filename));
}

extern "C" bool jxcoretau_is_http_server_started() {
    return our_is_http_server_started;
}


