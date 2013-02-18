
#import "SimpleOnlyStaticModuleFactoryBase.h"
#import "SimpleOnlyStaticModule.h"
#import "SimpleOnlyStaticModuleSingleton.h"

@implementation SimpleOnlyStaticModuleFactoryBase

-(id)init {
    self = [super init];
    mSimpleOnlyStaticModuleInstances = [NSMutableDictionary dictionary];
    mSimpleOnlyStaticModuleSingleton = nil;
    return self;
}

-(id<ISimpleOnlyStaticModule>) getSimpleOnlyStaticModuleByID:(NSString*)ID {
    SimpleOnlyStaticModule* obj = [mSimpleOnlyStaticModuleInstances objectForKey:ID];
    if (obj == nil) {
        obj = [self createSimpleOnlyStaticModuleByID:ID];
        [mSimpleOnlyStaticModuleInstances setObject:obj forKey:ID];
    }
    return obj;
}

-(void) destroyObjectByID:(NSString*)ID {
    SimpleOnlyStaticModule* obj = [mSimpleOnlyStaticModuleInstances objectForKey:ID];
    if (obj != nil) {
        [mSimpleOnlyStaticModuleInstances removeObjectForKey:ID];
    }
}


-(id<ISimpleOnlyStaticModule>)createSimpleOnlyStaticModuleByID:(NSString*)ID {
    SimpleOnlyStaticModule* obj = [[SimpleOnlyStaticModule alloc] init];
    [obj setProperty:@"ID" value:ID];
    return obj;
}


-(id<ISimpleOnlyStaticModuleSingleton>) getSimpleOnlyStaticModuleSingleton {
    if (mSimpleOnlyStaticModuleSingleton == nil) {
        mSimpleOnlyStaticModuleSingleton = [[SimpleOnlyStaticModuleSingleton alloc] init];
    }
    return mSimpleOnlyStaticModuleSingleton;
}

-(void)dealloc {
    [mSimpleOnlyStaticModuleInstances release];
    [super dealloc];
}


@end