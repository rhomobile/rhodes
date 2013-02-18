
#import "ISimpleOnlyStaticModule.h"

@interface SimpleOnlyStaticModuleFactoryBase : NSObject<ISimpleOnlyStaticModuleFactory> {
    NSMutableDictionary* mSimpleOnlyStaticModuleInstances;
    id<ISimpleOnlyStaticModuleSingleton> mSimpleOnlyStaticModuleSingleton;
}

-(id)init;

-(id<ISimpleOnlyStaticModuleSingleton>) getSimpleOnlyStaticModuleSingleton;
-(id<ISimpleOnlyStaticModule>) getSimpleOnlyStaticModuleByID:(NSString*)ID;

-(void) destroyObjectByID:(NSString*)ID;

-(id<ISimpleOnlyStaticModule>)createSimpleOnlyStaticModuleByID:(NSString*)ID;

-(void)dealloc;

@end