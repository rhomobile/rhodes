//
//  IPrinterZebraWorker.h
//  PrintingZebra

#import <Foundation/Foundation.h>
#import "../../../../../printing/ext/platform/iphone/impl/IPrinterTech.h"
#import <CoreGraphics/CoreGraphics.h>

#define LOCAL_CAST @"LOCAL_CAST"

/////////////////////////////////////////////////////////////////////////

@protocol IPrinterZebraInstance <IPrinterInstance>

-(void) retrieveFileNames:(id<ICallbackStruct>)callback;

-(void) retrieveFileNamesWithExtensions:(NSArray*)extensions callback:(id<ICallbackStruct>)callback;

-(void) sendFileContents:(NSString*)filePath callback:(id<ICallbackStruct>)callback;

-(void) retrieveFormatFromPrinterWithPath:(NSString*)formatPathOnPrinter callback:(id<ICallbackStruct>)callback;

-(void) printStoredFormat:(NSString*)formatPathOnPrinter withFormatData:(NSArray*)vars callback:(id<ICallbackStruct>)callback;

-(void) printStoredFormat:(NSString*)formatPathOnPrinter withDictionary:(NSDictionary*)vars callback:(id<ICallbackStruct>)callback;

-(void) printImage: (CGImageRef)image atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat callback:(id<ICallbackStruct>)callback;

-(void) storeImage:(NSString *)printerDriveAndFileName  withImage:(CGImageRef)image withWidth:(NSInteger)width andWithHeight:(NSInteger)height callback:(id<ICallbackStruct>)callback;

-(void) storeImage:(NSString *)printerDriveAndFileName withPathOnDevice:(NSString *)imageFilePathOnDevice withWidth:(NSInteger)width andWithHeight:(NSInteger)height callback:(id<ICallbackStruct>)callback;
@end

/////////////////////////////////////////////////////////////////////////

@protocol IPrinterZebraTech <IPrinterTech>

-(id<IPrinterZebraInstance>) getPrinterZebraByTag:(NSString*)instanceID;

@end
