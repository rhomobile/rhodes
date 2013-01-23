
#import "Barcode1FactoryBase.h"
#import "../impl/Barcode1.h"
#import "../impl/Barcode1Singleton.h"

@implementation Barcode1FactoryBase

-(id)init {
    self = [super init];
    mBarcode1Instances = [NSMutableDictionary dictionary];
    mBarcode1Singleton = nil;
    return self;
}

-(id<IBarcode1>) getBarcode1ByID:(NSString*)ID {
    Barcode1* obj = [mBarcode1Instances objectForKey:ID];
    if (obj == nil) {
        obj = [self createBarcode1ByID:ID];
        [mBarcode1Instances setObject:obj forKey:ID];
    }
    return obj;
}

-(id<IBarcode1>)createBarcode1ByID:(NSString*)ID {
    Barcode1* obj = [[Barcode1 alloc] init];
    [obj setProperty:@"ID" value:ID];
    return obj;
}


-(id<IBarcode1Singleton>) getBarcode1Singleton {
    if (mBarcode1Singleton == nil) {
        mBarcode1Singleton = [[Barcode1Singleton alloc] init];
    }
    return mBarcode1Singleton;
}

-(void)dealloc {
    [mBarcode1Instances release];
    [super dealloc];
}


@end