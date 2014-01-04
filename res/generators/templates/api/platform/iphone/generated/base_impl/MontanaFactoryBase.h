
#import "I<%= $cur_module.name %>.h"

@interface <%= $cur_module.name %>FactoryBase : NSObject<I<%= $cur_module.name %>Factory> {
    NSMutableDictionary* m<%= $cur_module.name %>Instances;
    id<I<%= $cur_module.name %>Singleton> m<%= $cur_module.name %>Singleton;
}

-(id)init;

-(id<I<%= $cur_module.name %>Singleton>) get<%= $cur_module.name %>Singleton;
-(id<I<%= $cur_module.name %>>) get<%= $cur_module.name %>ByID:(NSString*)ID;

-(void) destroyObjectByID:(NSString*)ID;

-(id<I<%= $cur_module.name %>>)create<%= $cur_module.name %>ByID:(NSString*)ID;

-(NSArray*) enumerate<%= $cur_module.name %>Instances;

-(void)dealloc;

@end