
#import "I<%= namecamelcase %>.h"
#import "<%= namecamelcase %>FactoryBase.h"

static <%= namecamelcase %>FactoryBase* our<%= namecamelcase %>Factory = nil;

@implementation <%= namecamelcase %>FactorySingleton

+(id<I<%= namecamelcase %>Factory>) get<%= namecamelcase %>FactoryInstance {
    if (our<%= namecamelcase %>Factory == nil) {
        our<%= namecamelcase %>Factory = [[<%= namecamelcase %>FactoryBase alloc] init];
    }
    return our<%= namecamelcase %>Factory;
}

@end