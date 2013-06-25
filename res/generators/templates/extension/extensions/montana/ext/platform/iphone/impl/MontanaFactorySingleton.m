
#import "I<%= name.camel_case %>.h"
#import "<%= name.camel_case %>FactoryBase.h"

static <%= name.camel_case %>FactoryBase* our<%= name.camel_case %>Factory = nil;

@implementation <%= name.camel_case %>FactorySingleton

+(id<I<%= name.camel_case %>Factory>) get<%= name.camel_case %>FactoryInstance {
    if (our<%= name.camel_case %>Factory == nil) {
        our<%= name.camel_case %>Factory = [[<%= name.camel_case %>FactoryBase alloc] init];
    }
    return our<%= name.camel_case %>Factory;
}

@end