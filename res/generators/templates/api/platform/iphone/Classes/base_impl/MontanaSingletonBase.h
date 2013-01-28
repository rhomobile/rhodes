

#import "../api/I<%= $cur_module.name %>.h"

@interface <%= $cur_module.name %>SingletonBase : NSObject {
    NSString* mDefaultID;
}

-(id)init;

-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;

-(void)dealloc;

@end