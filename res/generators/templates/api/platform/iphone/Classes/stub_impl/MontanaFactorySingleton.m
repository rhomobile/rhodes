
#import "../api/I<%= $cur_module.name %>.h"
#import "<%= $cur_module.name %>FactoryBase.h"

static <%= $cur_module.name %>FactoryBase* our<%= $cur_module.name %>Factory = nil;

@implementation <%= $cur_module.name %>FactorySingleton

+(id<I<%= $cur_module.name %>Factory>) get<%= $cur_module.name %>FactoryInstance {
    if (our<%= $cur_module.name %>Factory == nil) {
        our<%= $cur_module.name %>Factory = [[<%= $cur_module.name %>FactoryBase alloc] init];
    }
    return our<%= $cur_module.name %>Factory;
}

@end