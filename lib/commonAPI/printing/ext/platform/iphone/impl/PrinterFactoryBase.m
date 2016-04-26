
#import "PrinterFactoryBase.h"
#import "PrinterImpl.h"
#import "PrinterSingleton.h"



@implementation PrinterFactoryBase

-(id)init {
    self = [super init];
    mPrinterInstances = [[NSMutableDictionary dictionaryWithCapacity:4] retain];
    mPrinterSingleton = nil;
    return self;
}

-(id<IPrinter>) getPrinterByID:(NSString*)ID {
    id<IPrinter> obj = [mPrinterInstances objectForKey:ID];
    if (obj == nil) {
        obj = [self createPrinterByID:ID];
        [mPrinterInstances setObject:obj forKey:ID];
    }
    return obj;
}

-(void) destroyObjectByID:(NSString*)ID {
    PrinterImpl* obj = [mPrinterInstances objectForKey:ID];
    if (obj != nil) {
        [mPrinterInstances removeObjectForKey:ID];
    }
}


-(id<IPrinter>)createPrinterByID:(NSString*)ID {
    PrinterImpl* obj = [[PrinterImpl alloc] init];

    [obj setProperty:@"ID" propertyValue:ID methodResult:nil];

    return obj;
}


-(id<IPrinterSingleton>) getPrinterSingleton {
    if (mPrinterSingleton == nil) {
        mPrinterSingleton = [[PrinterSingleton alloc] init];
    }
    return mPrinterSingleton;
}

-(NSArray*) enumeratePrinterInstances {
    return [mPrinterInstances allKeys];
}

-(void)dealloc {
    [mPrinterInstances release];
    [super dealloc];
}


@end