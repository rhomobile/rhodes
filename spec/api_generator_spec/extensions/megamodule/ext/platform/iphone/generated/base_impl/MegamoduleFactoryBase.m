
#import "MegamoduleFactoryBase.h"
#import "Megamodule.h"
#import "MegamoduleSingleton.h"

@implementation MegamoduleFactoryBase

-(id)init {
    self = [super init];
    mMegamoduleInstances = [NSMutableDictionary dictionary];
    mMegamoduleSingleton = nil;
    return self;
}

-(id<IMegamodule>) getMegamoduleByID:(NSString*)ID {
    Megamodule* obj = [mMegamoduleInstances objectForKey:ID];
    if (obj == nil) {
        obj = [self createMegamoduleByID:ID];
        [mMegamoduleInstances setObject:obj forKey:ID];
    }
    return obj;
}

-(void) destroyObjectByID:(NSString*)ID {
    Megamodule* obj = [mMegamoduleInstances objectForKey:ID];
    if (obj != nil) {
        [mMegamoduleInstances removeObjectForKey:ID];
    }
}


-(id<IMegamodule>)createMegamoduleByID:(NSString*)ID {
    Megamodule* obj = [[Megamodule alloc] init];

    [obj setProperty:@"ID" propertyValue:ID];

    return obj;
}


-(id<IMegamoduleSingleton>) getMegamoduleSingleton {
    if (mMegamoduleSingleton == nil) {
        mMegamoduleSingleton = [[MegamoduleSingleton alloc] init];
    }
    return mMegamoduleSingleton;
}

-(void)dealloc {
    [mMegamoduleInstances release];
    [super dealloc];
}


@end