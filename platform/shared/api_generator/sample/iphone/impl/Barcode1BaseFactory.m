
#import "Barcode1BaseFactory.h"
#import "Barcode1.h"

@implementation Barcode1BaseFactory

-(id)init {
    self = [super init];
    mBarcode1Instances = [NSMutableDictionary dictionary];
    mDefaultID= nil;
    return self;
}

-(id<IBarcode1>) getBarcode1ByID:(NSString*)ID {
    Barcode1* obj = [mBarcode1Instances objectForKey:ID];
    if (obj == nil) {
        obj = [[Barcode1 alloc] init:ID];
        [mBarcode1Instances setObject:obj forKey:ID];
    }
    return obj;
}

-(NSString*) getDefaultID {
    if (mDefaultID == nil) {
        mDefaultID = [[self getInitialDefaultID] retain];
    }
    return mDefaultID;
}

-(void) setDefaultID:(NSString*)defaultID {
    mDefaultID = [defaultID retain];
}

-(NSString*)getInitialDefaultID {
    return nil;
}

-(Class) getBarcode1ImplementationClass {
    return [Barcode1 class];
}

-(void)dealloc {
    [mBarcode1Instances release];
    [mDefaultID release];
    [super dealloc];
}


@end