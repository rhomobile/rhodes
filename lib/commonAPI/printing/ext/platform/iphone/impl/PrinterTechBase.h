//
//  PrinterTech.h
//  PrintingZebra

#import <Foundation/Foundation.h>
#import "IPrinterTech.h"

/////////////////////////////////////////////////////////////////////////

@interface ConnectParams : NSObject<IConnectParams> {
    int mMaxTimeoutForOpen;
    int mMaxTimeoutForRead;
    int mTimeToWaitForMoreData;
    int mTimeToWaitAfterReadInMilliseconds;
    int mTimeToWaitAfterWriteInMilliseconds;
    BOOL mInitialized[PARAM_MAX+1];
}

-(id) init;

-(void) setFromParams:(id<IConnectParams>)params;
-(void) setFromDictionary:(NSDictionary*) options;
-(NSDictionary*) getAsDictionary;

-(void) clear;
-(void) resetToDefaultValues;

-(BOOL) isInitialized:(NSInteger)param;
-(BOOL) hasInitializedParameters;

@property (assign) int maxTimeoutForOpen;
@property (assign) int maxTimeoutForRead;
@property (assign) int timeToWaitForMoreData;

@property (assign) int timeToWaitAfterReadInMilliseconds;
@property (assign) int timeToWaitAfterWriteInMilliseconds;

@end


/////////////////////////////////////////////////////////////////////////

@interface PrinterCallback : NSObject <ICallbackStruct> {
    id<IPrinterCallbackHandler> mCallbackHandler;
    NSObject* mTag;
    PrinterCommand mCommand;
    PrinterEvent mEvent;
    PrinterResult mResult;
    BOOL mIsEnabled;
}

-(id) init;

-(void) dealloc;

-(id)initWithTag:(NSObject*)tag andHandler:(id<IPrinterCallbackHandler>)handler;

-(id)initWithSelfTagAndHandler:(id<IPrinterCallbackHandler>)handler;

+(id<ICallbackStruct>) callbackWithHandler:(id<IPrinterCallbackHandler>)handler andTag:(NSObject*)tag;

+(id<ICallbackStruct>) callbackWithSelfTagAndHandler:(id<IPrinterCallbackHandler>)handler;

-(void) disable;

-(BOOL) isEnabled;

-(void) setCommand:(PrinterCommand)command;

-(PrinterCommand) getCommand;

-(NSObject*) getTag;

-(PrinterEvent) getLastEvent;

-(PrinterResult) getLastResult;

-(BOOL) isInvokedWithError;

-(void) invokeWithEvent:(PrinterEvent)event result:(PrinterResult)resultCode;

-(void) invokeWithEvent:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSDictionary*)info;

@end

/////////////////////////////////////////////////////////////////////////

@interface TaskShedulerBase : NSObject<ITaskSheduler> {
    NSOperationQueue *mOperationQueue;
    NSUInteger taskIdCounter;
    NSMutableSet *mPendingOperations;
    BOOL mCanSheduleTasks;
}

-(id) init;

-(void) dealloc;

-(void) stopSheduler;

-(NSUInteger) getNumberOfTasks;

-(NSUInteger) sheduleTask:(void (^)(void))task;

-(NSUInteger) sheduleOnInstance:(id<IPrinterInstance>)instance callback:(id<ICallbackStruct>)callback printerTask:(PrinterResult (^)(void))task;

-(void) stopCommandWithTag:(NSObject*)tag;

-(void) stopCommandWithIndex:(NSUInteger)index;

@end

/////////////////////////////////////////////////////////////////////////

@interface PrinterInstanceBase : NSObject<IPrinterInstance> {
    NSUInteger mInstanceID;
    id<ITaskSheduler> mSheduler;
    BOOL mInitialized;
    ConnectionType mConnectionType;
    ConnectParams* mParams;
    NSString* mConnectionAddress;
    NSDictionary* mPrinterStatus;
    NSArray* mPrinterLanguages;
    int mPort;
}


// IPrinterInstance

-(id) init;

-(void) dealloc;

-(BOOL) isInitialized;

-(id)initWithInstanceId:(NSUInteger)instanceID andSheduler:(id<ITaskSheduler>)sheduler withTCPAdress:(NSString*)address andPort:(NSUInteger)port;

-(id)initWithInstanceId:(NSUInteger)instanceID andSheduler:(id<ITaskSheduler>)sheduler withBTAdress:(NSString*)address;

-(BOOL) updateConnectParams:(id<IConnectParams>)params;

-(id<IConnectParams>) getConnectParams;

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

-(void) printFile:(NSString*)fileURI options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback;

-(void) printImageFromFile:(NSString*)path atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat callback:(id<ICallbackStruct>)callback;

-(void) printRawString:(NSString*)command options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback;

// etc
-(void) setSupportedLanguages:(NSArray*)langs;

@end

/////////////////////////////////////////////////////////////////////////

@interface PrinterTechBase : TaskShedulerBase<IPrinterTech> {
    NSMutableArray* mSearchCallbacks;
    NSMutableDictionary *mDiscoveredPrinters;
    NSUInteger printerIdCounter;
}

-(id) init;

-(void) dealloc;

-(PrintingDevice) getSupportedDeviceType;

-(void) searchPrinters:(ConnectionType)connectionType address:(NSString*)address port:(NSUInteger)port timeout:(NSInteger)timeout  callback:(id<ICallbackStruct>)callback;

-(void) stopSearch:(id<ICallbackStruct>)callback;

-(id<IPrinterInstance>) getPrinterByTag:(NSString*)instanceTag;

-(NSArray*) getDiscoveredPrinters;

@end

/////////////////////////////////////////////////////////////////////////

@interface PrinterTechManagerSingleton : NSObject<IPrinterTechManagerSingleton> {
    NSMutableSet *mTechSet;
}

+(id<IPrinterTechManagerSingleton>) sharedInstance;

-(id) init;

-(BOOL) registerTech:(id<IPrinterTech>)tech;
-(BOOL) unregiserTech:(id<IPrinterTech>)tech;
-(NSArray*) getRegisteredTech;

@end
