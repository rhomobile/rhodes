//
//  PrinterTech.m
//  PrintingZebra

#import "PrinterTechBase.h"
#import "IPrinter.h"

#import "logging/RhoLog.h"

/////////////////////////////////////////////////////////////////////////

@implementation ConnectParams

@synthesize maxTimeoutForOpen = mMaxTimeoutForOpen, maxTimeoutForRead = mMaxTimeoutForRead, timeToWaitForMoreData =mTimeToWaitForMoreData, timeToWaitAfterReadInMilliseconds = mTimeToWaitAfterReadInMilliseconds, timeToWaitAfterWriteInMilliseconds = mTimeToWaitAfterWriteInMilliseconds;

-(id) init {
    self = [super init];
    if (self) {
        [self clear];
    }
    return self;
}

-(void) setFromParams:(id<IConnectParams>)params {
    for (int i = 0; i < PARAM_MAX; i++) {
        if ([params isInitialized:i]) {
            switch (i) {
                case PARAM_MAX_TIMEOUT_FOR_OPEN:
                    mMaxTimeoutForOpen = [params maxTimeoutForOpen];
                    break;

                case PARAM_MAX_TIMEOUT_FOR_READ:
                    mMaxTimeoutForRead = [params maxTimeoutForRead];
                    break;

                case (PARAM_TIME_TO_WAIT_FOR_MORE_DATA):
                    mTimeToWaitForMoreData = [params timeToWaitForMoreData];
                    break;

                case PARAM_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS:
                    mTimeToWaitAfterReadInMilliseconds = [params timeToWaitAfterReadInMilliseconds];
                    break;

                case PARAM_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS:
                    mTimeToWaitAfterWriteInMilliseconds = [params timeToWaitAfterWriteInMilliseconds];
                    break;
                    
                default:
                    break;
            }
        }
    }
}

-(void) setFromDictionary:(NSDictionary*) options {

}

-(NSDictionary*) getAsDictionary {
    return nil;
}

-(void) clear {
    mMaxTimeoutForOpen = -1;
    mMaxTimeoutForRead = -1;
    mTimeToWaitForMoreData = -1;
    mTimeToWaitAfterReadInMilliseconds = -1;
    mTimeToWaitAfterWriteInMilliseconds = -1;

    for (int i = 0; i < PARAM_MAX; i++) {
        mInitialized[i] = false;
    }
}

-(void) resetToDefaultValues {
    mMaxTimeoutForOpen = 1000;
    mMaxTimeoutForRead = 1000;
    mTimeToWaitForMoreData = 1000;
    mTimeToWaitAfterReadInMilliseconds = 10;
    mTimeToWaitAfterWriteInMilliseconds = 200;

    for (int i = 0; i < PARAM_MAX; i++) {
        mInitialized[i] = true;
    }
}

-(BOOL) isInitialized:(NSInteger)param {
    if ( 0 <= param && param < PARAM_MAX) {
        return mInitialized[param];
    } else {
        return false;
    }
}

-(BOOL) hasInitializedParameters {
    for (int i = 0; i < PARAM_MAX; i++) {
        if(mInitialized[i]) {
            return true;
        }
    }
    return false;
}

@end


/////////////////////////////////////////////////////////////////////////

@implementation PrinterCallback

-(id) init {
    self = [super init];
    if (self) {
        mCallbackHandler = nil;
        mTag = nil;
        mCommand = PRINTING_COMMAND_NONE;
        mEvent = PRINTING_EVENT_NONE;
        mResult = PRINTING_RESULT_NONE;
        mIsEnabled = YES;
    }
    return self;
}

-(void) dealloc {
    if (mCallbackHandler) {
        [mCallbackHandler release];
        mCallbackHandler = nil;
    }
    if (mTag) {
        [mTag release];
        mTag = nil;
    }
    mCommand = PRINTING_COMMAND_NONE;
    [super dealloc];
}

-(id)initWithTag:(NSObject*)tag andHandler:(id<IPrinterCallbackHandler>)handler {
    self = [self init];
    if (self) {
        mCallbackHandler = [handler retain];
        mTag = [tag retain];
    }
    return self;
}

-(id)initWithSelfTagAndHandler:(id<IPrinterCallbackHandler>)handler {
    self = [self init];
    if (self) {
        mCallbackHandler = [handler retain];
        mTag = self;
    }
    return self;
}

+(id<ICallbackStruct>) callbackWithHandler:(id<IPrinterCallbackHandler>)handler andTag:(NSObject*)tag {
    return [[PrinterCallback alloc] initWithTag:tag andHandler:handler];
}

+(id<ICallbackStruct>) callbackWithSelfTagAndHandler:(id<IPrinterCallbackHandler>)handler {
    return [[PrinterCallback alloc] initWithSelfTagAndHandler:handler];
}

-(void) disable {
    mIsEnabled = NO;
}

-(BOOL) isEnabled {
    return mIsEnabled;
}

-(void) setCommand:(PrinterCommand)command {
    mCommand = command;
}

-(PrinterCommand) getCommand {
    return mCommand;
}

-(NSObject*) getTag {
    return mTag;
}

-(PrinterEvent) getLastEvent {
    return mEvent;
}

-(PrinterResult) getLastResult {
    return mResult;
}

-(BOOL) isInvokedWithError {
    return ((mEvent == PRINTING_EVENT_FAILURE) && (mResult != PRINTING_RESULT_NONE) && (mResult != PRINTING_RESULT_SUCCESS));
}


-(void) invokeWithEvent:(PrinterEvent)event result:(PrinterResult)resultCode {
    mEvent = event; mResult = resultCode;
    [mCallbackHandler handleEvent:mCommand event:event result:resultCode info:nil tag:mTag];
}

-(void) invokeWithEvent:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSObject*)info {
    mEvent = event; mResult = resultCode;
    [mCallbackHandler handleEvent:mCommand event:event result:resultCode info:info tag:mTag];
}

@end

/////////////////////////////////////////////////////////////////////////

@implementation TaskShedulerBase

-(id) init {
    self = [super init];
    if( self )
    {
        mCanSheduleTasks = NO;

        mOperationQueue = [NSOperationQueue new];
        [mOperationQueue setMaxConcurrentOperationCount:1];

        mPendingOperations = [NSMutableSet new];

        taskIdCounter = 0;

        mCanSheduleTasks = YES;
    }
    return self;
}

-(void) dealloc {
    mCanSheduleTasks = NO;

    @synchronized(self) {
        [mOperationQueue waitUntilAllOperationsAreFinished];
        [mOperationQueue release];
        mOperationQueue = nil;
    }

    [mPendingOperations release];
    mPendingOperations = nil;

    [super dealloc];
}

-(void) stopSheduler {
    mCanSheduleTasks = NO;
}

-(NSUInteger) getNumberOfTasks {
    return [mOperationQueue operationCount];
}

-(NSUInteger) sheduleTask:(void (^)(void))task {
    NSUInteger result = 0;
    if (!mCanSheduleTasks) {
        return 0;
    }
    @synchronized(mOperationQueue) {
        if (mOperationQueue) {
            result = ++taskIdCounter;
            [mOperationQueue addOperationWithBlock:^{
                NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
                @try {
                    NSLog(@"sheduleTask block started");
                    task();
                }
                @finally {
                    NSLog(@"sheduleTask block finished");
                    [pool drain];
                }
            }];
        }
    }
    return result;

}

-(NSUInteger) sheduleOnInstance:(id<IPrinterInstance>)instance callback:(id<ICallbackStruct>)callback printerTask:(PrinterResult (^)(void))task {

    [callback retain];

    if (!mCanSheduleTasks) {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
        return 0;
    }

    NSUInteger result = 0;
    @synchronized(mOperationQueue) {

        if (mOperationQueue) {
            result = ++taskIdCounter;
            
            [mOperationQueue addOperationWithBlock:^{
                NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
                @try {
                    NSLog(@"sheduleTask block started");
                    if (![instance isConnected]) {
                        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_NOT_CONNECTED];
                        return;
                    } else {
                        [callback invokeWithEvent:PRINTING_EVENT_STARTING result:PRINTING_RESULT_NONE];

                        PrinterResult task_result = PRINTING_RESULT_ERROR;

                        @try {
                            task_result = task();
                        }
                        @catch (NSException* exception) {
                            NSString* excDescr = [NSString stringWithFormat:@"Uncaught exception: %@", exception.description];
                            NSString* stack = [NSString stringWithFormat:@"Stack trace: %@", [exception callStackSymbols]];
                            RAWLOGC_ERROR2("Printing","%s\r\n%s",[excDescr UTF8String],[stack UTF8String]);
                        }

                        if (task_result != PRINTING_RESULT_SUCCESS && task_result != PRINTING_RESULT_NONE) {
                            [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:task_result];
                        }
                    }
                }
                @finally {
                    NSLog(@"sheduleTask block finished");
                    [pool drain];
                }
                [callback release];
            }];
        }
    }
    return result;

}

-(void) stopCommandWithTag:(NSObject*)tag {

}

-(void) stopCommandWithIndex:(NSUInteger)index {
    
}

@end

/////////////////////////////////////////////////////////////////////////

@implementation PrinterInstanceBase

-(id) init {
    self = [super init];
    if( self )
    {
        mInstanceID = 0;
        mSheduler = nil;
        mInitialized = NO;
        mConnectionType = CONNECTION_NONE;
        mConnectionAddress = nil;
        mPrinterLanguages = nil;
        mParams = [[ConnectParams alloc] init];
        [mParams resetToDefaultValues];
        mPort = 0;
    }
    return self;
}

-(void) dealloc {
    if (mSheduler) {
        [mSheduler release];
        mSheduler = nil;
    }
    if (mConnectionAddress) {
        [mConnectionAddress release];
        mConnectionAddress = nil;
    }
    if (mPrinterLanguages) {
        [mPrinterLanguages release];
        mPrinterLanguages = nil;
    }
    [super dealloc];
}

-(void) setSupportedLanguages:(NSArray*)langs {
    if (mPrinterLanguages) {
        [mPrinterLanguages release];
        mPrinterLanguages = nil;
    }
    mPrinterLanguages = [langs retain];
}

-(BOOL) isInitialized {
    return mInitialized;
}

-(id)initWithInstanceId:(NSUInteger)instanceID andSheduler:(id<ITaskSheduler>)sheduler withTCPAdress:(NSString*)address andPort:(NSUInteger)port {
    self = [self init];
    if (self) {
        mInstanceID = instanceID;
        mSheduler = [sheduler retain];
        mConnectionType = CONNECTION_TCP;
        mConnectionAddress = [[NSString stringWithString:address] retain];
        mPort = port;
        mInitialized = YES;
    }
    return self;
}

-(id)initWithInstanceId:(NSUInteger)instanceID andSheduler:(id<ITaskSheduler>)sheduler withBTAdress:(NSString*)address {
    self = [self init];
    if (self) {
        mInstanceID = instanceID;
        mSheduler = [sheduler retain];
        mConnectionType = CONNECTION_BLUETOOTH;
        mConnectionAddress = [[NSString stringWithString:address] retain];
        mPort = 0;
        mInitialized = YES;
    }
    return self;
}

-(BOOL) updateConnectParams:(id<IConnectParams>)params {
    [mParams setFromParams:params];
    return true;
}

-(id<IConnectParams>) getConnectParams {
    return mParams;
}

+(NSString*) getPrinterType {
    return PRINTER_TYPE_NATIVE;
}

+(NSString*) getVendor {
    return @"NONE";
}

+(NSString*) makeInstanceTag:(ConnectionType)connectionType withAdress:(NSString*)address {
    NSString* result = nil;
    switch (connectionType) {
        case CONNECTION_TCP:
            result = [NSString stringWithFormat:@"%@-tcp-%@", [self getVendor], address];
            break;

        case CONNECTION_BLUETOOTH:
            result = [NSString stringWithFormat:@"%@-bt-%@", [self getVendor], address];
            break;

        case CONNECTION_USB:
            result = [NSString stringWithFormat:@"%@-usb-%@", [self getVendor], address];
            break;

        default:
            break;
    }
    return result;
}

-(ConnectionType) getConnectionType {
    return mConnectionType;
}

-(NSString*) getAddress {
    return mConnectionAddress;
}

-(NSUInteger) getPort {
    return mPort;
}

-(NSUInteger) getInstanceID {
    return mInstanceID;
}

-(NSString*) getTag {
    return [[self class] makeInstanceTag:mConnectionType withAdress:mConnectionAddress];
}

-(BOOL) isConnected {
    return false;
}

-(NSDictionary*) getCurrentStatus {
    return mPrinterStatus;
}

-(NSArray*) getSupportedPrinterLanguages {
    return [NSArray arrayWithArray:mPrinterLanguages];
}

-(void) connect:(id<ICallbackStruct>)callback {
    [callback setCommand:PRINTING_COMMAND_CONNECT];
    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
}

-(void) connect:(id<ICallbackStruct>)callback withParams:(id<IConnectParams>)params; {
    [callback setCommand:PRINTING_COMMAND_CONNECT];
    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
}

-(void) disconnect:(id<ICallbackStruct>)callback {
    [callback setCommand:PRINTING_COMMAND_DISCONNECT];
    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
}

-(void) requestState:(NSArray*)listOfParameters callback:(id<ICallbackStruct>)callback; {
    [callback setCommand:PRINTING_COMMAND_UPDATE_STATUS];
    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
}

-(void) printFile:(NSString*)fileURI options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback {
    [callback setCommand:PRINTING_COMMAND_PRINT];
    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
}

-(void) printImageFromFile:(NSString*)path atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat callback:(id<ICallbackStruct>)callback {
    [callback setCommand:PRINTING_COMMAND_PRINT];
    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
}

-(void) printRawString:(NSString*)command options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback {
    [callback setCommand:PRINTING_COMMAND_PRINT];
    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
}

@end

/////////////////////////////////////////////////////////////////////////

@implementation PrinterTechBase

-(id) init {
    self = [super init];
    if( self )
    {
        mSearchCallbacks = [[NSMutableArray alloc] initWithCapacity:10];

        mDiscoveredPrinters = [NSMutableDictionary new];

        printerIdCounter = 0;
    }
    return self;
}

-(void) dealloc {
    [self stopSheduler];

    [mOperationQueue waitUntilAllOperationsAreFinished];

    if (mSearchCallbacks) {
        [mSearchCallbacks release];
        mSearchCallbacks = nil;
    }

    if (mDiscoveredPrinters) {
        [mDiscoveredPrinters release];
        mDiscoveredPrinters = nil;
    }

    [super dealloc];
}

-(PrintingDevice) getSupportedDeviceType {
    return PRINTING_DEVICE_NATIVE;
}

-(void) searchPrinters:(ConnectionType)connectionType address:(NSString*)address port:(NSUInteger)port timeout:(NSInteger)timeout  callback:(id<ICallbackStruct>)callback {
    [callback setCommand:PRINTING_COMMAND_DISCOVER];
    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
}

-(void) stopSearch:(id<ICallbackStruct>)callback {
    for (id<ICallbackStruct> cbk in mSearchCallbacks) {
        [cbk disable];
    }
}

-(id<IPrinterInstance>) getPrinterByTag:(NSString*)instanceTag {
    id<IPrinterInstance> printer = nil;
    @synchronized(mDiscoveredPrinters) {
        printer = [mDiscoveredPrinters objectForKey:instanceTag];
    }
    return printer;
}

-(NSArray*) getDiscoveredPrinters {
    NSArray *printerKeys = nil;
    @synchronized(mDiscoveredPrinters) {
        printerKeys = [mDiscoveredPrinters allKeys];
    }
    return printerKeys;

}

@end

/////////////////////////////////////////////////////////////////////////

@implementation PrinterTechManagerSingleton

static PrinterTechManagerSingleton *gInstance = NULL;

+(id<IPrinterTechManagerSingleton>) sharedInstance {

	@synchronized(self)
    {
		if (gInstance == NULL)
			gInstance = [NSAllocateObject([self class], 0, NULL) init];
    }
	return(gInstance);
}

-(id) init {
    self = [super init];
    if (self) {
        mTechSet = [[[NSMutableSet alloc] initWithCapacity:5] retain];
    }
    return self;
}

+ (id) allocWithZone:(NSZone *)zone
{
    return [[self sharedInstance] retain];
}

- (id) copyWithZone:(NSZone*)zone
{
    return self;
}

- (id) retain
{
    return self;
}

- (NSUInteger) retainCount
{
    return NSUIntegerMax;
}

- (oneway void) release { }

- (id) autorelease
{
    return self;
}

-(BOOL) registerTech:(id<IPrinterTech>)tech {
    id obj = [mTechSet member:tech];
    if (obj == nil ) {
        [mTechSet addObject:tech];
    }
    return obj == nil;
}

-(BOOL) unregiserTech:(id<IPrinterTech>)tech {

    id obj = [mTechSet member:tech];
    if ( obj != nil ) {
        [mTechSet removeObject:tech];
    }
    return obj != nil;
}

-(NSArray*) getRegisteredTech {
    return [mTechSet allObjects];
}

@end
