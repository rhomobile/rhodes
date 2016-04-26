
#import "IPrintingEpson.h"
#import "PrintingEpsonFactoryBase.h"

static PrintingEpsonFactoryBase* ourPrintingEpsonFactory = nil;

@implementation PrintingEpsonFactorySingleton

+(id<IPrintingEpsonFactory>) getPrintingEpsonFactoryInstance {
    if (ourPrintingEpsonFactory == nil) {
        ourPrintingEpsonFactory = [[PrintingEpsonFactoryBase alloc] init];
    }
    return ourPrintingEpsonFactory;
}

@end