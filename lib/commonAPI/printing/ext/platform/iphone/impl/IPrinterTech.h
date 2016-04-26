//
//  IPrinterWorker.h
//  PrintingZebra

#import <Foundation/Foundation.h>

typedef enum {
    CONNECTION_NONE,
    CONNECTION_TCP,
    CONNECTION_BLUETOOTH,
    CONNECTION_USB,
    CONNECTION_ANY
} ConnectionType;

typedef enum {
    PRINTING_COMMAND_NONE = 0,
    PRINTING_COMMAND_CONNECT,
    PRINTING_COMMAND_DISCONNECT,
    PRINTING_COMMAND_DISCOVER,
    PRINTING_COMMAND_RECEIVE_FILE_LIST,
    PRINTING_COMMAND_RECEIVE_CONTENT,
    PRINTING_COMMAND_RECEIVE_STORED_FORMAT,
    PRINTING_COMMAND_STORE_FILE,
    PRINTING_COMMAND_SEND_CONTENT,
    PRINTING_COMMAND_UPDATE_STATUS,
    PRINTING_COMMAND_PRINT,
    PRINTING_COMMAND_RAW_COMMAND
} PrinterCommand;

typedef enum {
    PRINTING_EVENT_NONE = 0,
    PRINTING_EVENT_STARTING,
    PRINTING_EVENT_PROCESSING,
    PRINTING_EVENT_NEW_DATA,
    PRINTING_EVENT_FINISHED,   // callback should be removed
    PRINTING_EVENT_FAILURE     // callback should be removed
} PrinterEvent;


typedef enum {
    PRINTING_RESULT_NONE = 0,
    PRINTING_RESULT_CANCELLED,
    PRINTING_RESULT_SUCCESS,
    PRINTING_RESULT_ERROR,
    PRINTING_RESULT_ERR_IO,
    PRINTING_RESULT_ERR_MEMORY,
    PRINTING_RESULT_ERR_NETWORK,
    PRINTING_RESULT_ERR_NOT_CONNECTED,
    PRINTING_RESULT_ERR_NOT_FOUND,
    PRINTING_RESULT_ERR_PARAM,
    PRINTING_RESULT_ERR_PROCESSING,
    PRINTING_RESULT_ERR_RESPONSE,
    PRINTING_RESULT_ERR_TIMEOUT,
    PRINTING_RESULT_ERR_UNSUPPORTED,
    PRINTING_RESULT_ERR_UNINITIALIZED
} PrinterResult;


typedef enum {
    PRINTING_DEVICE_NONE,
    PRINTING_DEVICE_APD,
    PRINTING_DEVICE_EPSON,
    PRINTING_DEVICE_NATIVE,
    PRINTING_DEVICE_ZEBRA,
    PRINTING_DEVICE_ANY,
    PRINTING_DEVICE_COUNT
} PrintingDevice;

/* Printer control language CPCL */
#define PRINTER_LANGUAGE_CPCL_CONST @"PRINTER_LANGUAGE_CPCL"

/* Printer control language EPS */
#define PRINTER_LANGUAGE_EPS_CONST @"PRINTER_LANGUAGE_EPS"

/* Printer control language ZPL */
#define PRINTER_LANGUAGE_ZPL_CONST @"PRINTER_LANGUAGE_ZPL"

enum EConnectParam {
    PARAM_NONE = 0,
    // pre-connect
    PARAM_MAX_TIMEOUT_FOR_OPEN,
    // connect
    PARAM_MAX_TIMEOUT_FOR_READ,
    PARAM_TIME_TO_WAIT_FOR_MORE_DATA,
    // bt
    PARAM_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS,
    PARAM_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS,
    //
    PARAM_MAX
};

@protocol IConnectParams<NSObject>

-(void) setFromParams:(id<IConnectParams>)params;
-(void) setFromDictionary:(NSDictionary*) options;
-(NSDictionary*) getAsDictionary;

-(void) clear;
-(void) resetToDefaultValues;

-(BOOL) isInitialized:(NSInteger)param;
-(BOOL) hasInitializedParameters;

-(void) setMaxTimeoutForOpen:(int)value;
-(int) maxTimeoutForOpen;
-(void) setMaxTimeoutForRead:(int)value;
-(int) maxTimeoutForRead;
-(void) setTimeToWaitForMoreData:(int)value;
-(int) timeToWaitForMoreData;
-(void) setTimeToWaitAfterReadInMilliseconds:(int)value;
-(int) timeToWaitAfterReadInMilliseconds;
-(void) setTimeToWaitAfterWriteInMilliseconds:(int)value;
-(int) timeToWaitAfterWriteInMilliseconds;
@end

/////////////////////////////////////////////////////////////////////////

@protocol IPrinterCallbackHandler<NSObject>

-(BOOL) handleEvent:(PrinterCommand)command event:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSObject*)info tag:(NSObject*)tag;

@end

/////////////////////////////////////////////////////////////////////////
// Sturct for storing callback handler, tag for cb info and command code

// Callback is called at least twice
// On command start
// On command failure/success

@protocol ICallbackStruct<NSObject>

-(id)initWithTag:(NSObject*)tag andHandler:(id<IPrinterCallbackHandler>)handler;

-(id)initWithSelfTagAndHandler:(id<IPrinterCallbackHandler>)handler;

+(id<ICallbackStruct>) callbackWithHandler:(id<IPrinterCallbackHandler>)handler andTag:(NSObject*)tag;

-(void) disable;

-(BOOL) isEnabled;

-(void) setCommand:(PrinterCommand)command;

-(PrinterCommand) getCommand;

-(NSObject*) getTag;

-(PrinterEvent) getLastEvent;

-(PrinterResult) getLastResult;

-(BOOL) isInvokedWithError;

-(void) invokeWithEvent:(PrinterEvent)event result:(PrinterResult)resultCode;

-(void) invokeWithEvent:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSObject*)info;

@end

/////////////////////////////////////////////////////////////////////////
@protocol IPrinterInstance;

/////////////////////////////////////////////////////////////////////////
// async task sheduler and executor
@protocol ITaskSheduler<NSObject>

-(NSUInteger) getNumberOfTasks;

-(NSUInteger) sheduleTask:(void (^)(void))task;

-(NSUInteger) sheduleOnInstance:(id<IPrinterInstance>)instance callback:(id<ICallbackStruct>)callback printerTask:(PrinterResult (^)(void))task;

-(void) stopCommandWithTag:(NSObject*)tag;

-(void) stopCommandWithIndex:(NSUInteger)index;

@end

/////////////////////////////////////////////////////////////////////////
@protocol IPrinterInstance<NSObject>

-(BOOL) isInitialized;

-(id)initWithInstanceId:(NSUInteger)instanceID andSheduler:(id<ITaskSheduler>)sheduler withTCPAdress:(NSString*)address andPort:(NSUInteger)port;

-(id)initWithInstanceId:(NSUInteger)instanceID andSheduler:(id<ITaskSheduler>)sheduler withBTAdress:(NSString*)address;

-(BOOL) updateConnectParams:(id<IConnectParams>)params;

-(id<IConnectParams>) getConnectParams;

+(NSString*) getPrinterType;

+(NSString*) getVendor;

+(NSString*) makeInstanceTag:(ConnectionType)connectionType withAdress:(NSString*)address;

-(ConnectionType) getConnectionType;

-(NSString*) getAddress;

-(NSUInteger) getPort;

-(NSUInteger) getInstanceID;

-(NSString*) getTag;

-(BOOL) isConnected;

-(NSDictionary*) getCurrentStatus;

-(NSArray*) getSupportedPrinterLanguages;

-(void) connect:(id<ICallbackStruct>)callback;

-(void) connect:(id<ICallbackStruct>)callback withParams:(id<IConnectParams>)params;

-(void) disconnect:(id<ICallbackStruct>)callback;

-(void) requestState:(NSArray*)listOfParameters callback:(id<ICallbackStruct>)callback;

-(void) printFile:(NSString*)path options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback;

-(void) printImageFromFile:(NSString*)path atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat callback:(id<ICallbackStruct>)callback;

-(void) printRawString:(NSString*)command options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback;

@end

/////////////////////////////////////////////////////////////////////////

@protocol IPrinterTech<ITaskSheduler>

-(PrintingDevice) getSupportedDeviceType;

-(void) searchPrinters:(ConnectionType)connectionType address:(NSString*)address port:(NSUInteger)port timeout:(NSInteger)timeout callback:(id<ICallbackStruct>)callback;

-(void) stopSearch:(id<ICallbackStruct>)callback;

-(id<IPrinterInstance>) getPrinterByTag:(NSString*)instanceTag;

-(NSArray*) getDiscoveredPrinters;

@end


@protocol IPrinterTechManagerSingleton<NSObject>

+(id<IPrinterTechManagerSingleton>) sharedInstance;

-(BOOL) registerTech:(id<IPrinterTech>)tech;
-(BOOL) unregiserTech:(id<IPrinterTech>)tech;
-(NSArray*) getRegisteredTech;

@end
