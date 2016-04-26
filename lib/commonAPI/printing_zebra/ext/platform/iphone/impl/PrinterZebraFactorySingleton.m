
#import "IPrinterZebra.h"
#import "PrinterZebraFactoryBase.h"

static PrinterZebraFactoryBase* ourPrinterZebraFactory = nil;

@implementation PrinterZebraFactorySingleton

+(id<IPrinterZebraFactory>) getPrinterZebraFactoryInstance {
    if (ourPrinterZebraFactory == nil) {
        ourPrinterZebraFactory = [[PrinterZebraFactoryBase alloc] init];
    }
    return ourPrinterZebraFactory;
}

@end
