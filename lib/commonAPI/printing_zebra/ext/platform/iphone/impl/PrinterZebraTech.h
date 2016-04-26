//
//  PrinterZebraWorker.h
//  PrintingZebra

#import <Foundation/Foundation.h>

#import "TcpPrinterConnection.h"
#import "MfiBtPrinterConnection.h"
#import "ZebraPrinterFactory.h"
#import "ZebraPrinter.h"
#import "ZebraErrorCode.h"
#import "PrinterStatus.h"


#import "../../../../../printing/ext/platform/iphone/impl/IPrinterTech.h"
#import "../../../../../printing/ext/platform/iphone/impl/PrinterTechBase.h"
#import "IPrinterZebraTech.h"

/////////////////////////////////////////////////////////////////////////

@interface ZebraConnectParams : ConnectParams {
}
-(void) setFromDictionary:(NSDictionary*) options;
-(NSDictionary*) getAsDictionary;
-(void) resetToDefaultValues;
@end

/////////////////////////////////////////////////////////////////////////

@interface PrinterZebraInstace : PrinterInstanceBase<IPrinterZebraInstance> {
    id<ZebraPrinterConnection, NSObject> mConnection;
}


-(id)init;

-(void)dealloc;

// IPrinterInstance
+(NSString*) getVendor;

-(BOOL) isConnected;

-(void) connect:(id<ICallbackStruct>)callback;

-(void) connect:(id<ICallbackStruct>)callback withParams:(id<IConnectParams>)params;

-(void) disconnect:(id<ICallbackStruct>)callback;

-(void) requestState:(NSArray*)listOfParameters callback:(id<ICallbackStruct>)callback;

-(void) printFile:(NSString*)fileURI options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback;

-(void) printRawString:(NSString*)command options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback;


// IPrinterZebraInstance
-(void) retrieveFileNames:(id<ICallbackStruct>)callback;

-(void) retrieveFileNamesWithExtensions:(NSArray*)extensions callback:(id<ICallbackStruct>)callback;

-(void) sendFileContents:(NSString*)filePath callback:(id<ICallbackStruct>)callback;

-(void) retrieveFormatFromPrinterWithPath:(NSString*)formatPathOnPrinter callback:(id<ICallbackStruct>)callback;

-(void) printStoredFormat:(NSString*)formatPathOnPrinter withFormatData:(NSArray*)vars callback:(id<ICallbackStruct>)callback;

-(void) printStoredFormat:(NSString*)formatPathOnPrinter withDictionary:(NSMutableDictionary*)vars callback:(id<ICallbackStruct>)callback;

-(void) printImageFromFile:(NSString*)path atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat callback:(id<ICallbackStruct>)callback;

-(void) printImage: (CGImageRef)image atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat callback:(id<ICallbackStruct>)callback;

-(void) storeImage:(NSString *)printerDriveAndFileName  withImage:(CGImageRef)image withWidth:(NSInteger)width andWithHeight:(NSInteger)height callback:(id<ICallbackStruct>)callback;

-(void) storeImage:(NSString *)printerDriveAndFileName withPathOnDevice:(NSString *)imageFilePathOnDevice withWidth:(NSInteger)width andWithHeight:(NSInteger)height callback:(id<ICallbackStruct>)callback;
@end

/////////////////////////////////////////////////////////////////////////

@interface PrinterZebraTech : PrinterTechBase< IPrinterZebraTech >

// IPrinterTech
-(PrintingDevice) getSupportedDeviceType;

-(void) searchPrinters:(ConnectionType)connectionType address:(NSString*)address port:(NSUInteger)port timeout:(NSInteger)timeout callback:(id<ICallbackStruct>)callback;

// IPrinterZebraTech
-(id<IPrinterZebraInstance>) getPrinterZebraByTag:(NSString*)instanceID;

@end

/////////////////////////////////////////////////////////////////////////

@interface PrinterZebraTechSingleton : NSObject {}

+(id<IPrinterZebraTech>) getPrinterZebraWorkerInstance;

@end
