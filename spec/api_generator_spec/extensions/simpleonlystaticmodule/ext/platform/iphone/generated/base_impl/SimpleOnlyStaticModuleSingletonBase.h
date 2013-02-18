

#import "ISimpleOnlyStaticModule.h"

@interface SimpleOnlyStaticModuleSingletonBase : NSObject {
    NSString* mDefaultID;
}

-(id)init;

-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;

-(void)dealloc;

@end