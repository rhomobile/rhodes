
#import "IPrinter.h"
#import "PrinterFactoryBase.h"

static PrinterFactoryBase* ourPrinterFactory = nil;

@implementation PrinterFactorySingleton

+(id<IPrinterFactory>) getPrinterFactoryInstance {
    if (ourPrinterFactory == nil) {
        ourPrinterFactory = [[PrinterFactoryBase alloc] init];
    }
    return ourPrinterFactory;
}

@end