
#import "IMegamodule.h"

@interface MegamoduleFactoryBase : NSObject<IMegamoduleFactory> {
    NSMutableDictionary* mMegamoduleInstances;
    id<IMegamoduleSingleton> mMegamoduleSingleton;
}

-(id)init;

-(id<IMegamoduleSingleton>) getMegamoduleSingleton;
-(id<IMegamodule>) getMegamoduleByID:(NSString*)ID;

-(void) destroyObjectByID:(NSString*)ID;

-(id<IMegamodule>)createMegamoduleByID:(NSString*)ID;

-(void)dealloc;

@end