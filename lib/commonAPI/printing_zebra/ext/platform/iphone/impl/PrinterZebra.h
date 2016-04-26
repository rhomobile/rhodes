
#import "api_generator/iphone/IMethodResult.h"
#import "IPrinterZebra.h"
#import "IPrinterZebraTech.h"
#import "PrinterZebraBase.h"

/////////////////////////////////////////////////////////////////////////

@interface PrinterZebra : PrinterZebraBase<IPrinterZebra, IPrinterCallbackHandler> {
    id<IMethodResult> mPrinterEventCallback;
    id<IPrinterZebraInstance> mPrinterInstance;
    NSMutableDictionary* mPrinterStatus;
}

+(NSString*) errorStringFromCode:(PrinterResult)code;

-(id) init;
-(void) applyPrinterTech:(id<IPrinterZebraInstance>)printerTech;

-(BOOL) isInitialized;

-(void) dealloc;

-(BOOL) handleEvent:(PrinterCommand)command event:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSDictionary*)info tag:(NSObject*)tag;

-(void) setPrinterEventCallback:(id<IMethodResult>)methodResult;

-(void) connect:(id<IMethodResult>)methodResult;

-(void) connectWithOptions:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;

-(void) requestState:(NSArray*)listOfParameters methodResult:(id<IMethodResult>)methodResult;
-(void) disconnect:(id<IMethodResult>)methodResult;

-(void) printFile:(NSString*)fileURI options:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;
-(void) printRawString:(NSString*)command options:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;
-(void) enumerateSupportedControlLanguages:(id<IMethodResult>)methodResult;
-(void) retrieveFileNames:(id<IMethodResult>)methodResult;
-(void) retrieveFileNamesWithExtensions:(NSArray*)extensions methodResult:(id<IMethodResult>)methodResult;
-(void) sendFileContents:(NSString*)filePath methodResult:(id<IMethodResult>)methodResult;
-(void) printStoredFormatWithHash:(NSString*)formatPathOnPrinter vars:(NSDictionary*)vars methodResult:(id<IMethodResult>)methodResult;
-(void) printStoredFormatWithArray:(NSString*)formatPathOnPrinter vars:(NSArray*)vars methodResult:(id<IMethodResult>)methodResult;
-(void) printImageFromFile:(NSString*)path x:(int)x y:(int)y options:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;

-(void) storeImage:(NSString*)printerDriveAndFileName imageFilePathOnDevice:(NSString*)imageFilePathOnDevice width:(int)width height:(int)height methodResult:(id<IMethodResult>)methodResult;

@end
