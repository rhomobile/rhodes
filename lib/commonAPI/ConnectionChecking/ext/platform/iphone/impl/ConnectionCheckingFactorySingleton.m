
#import "IConnectionChecking.h"
#import "ConnectionCheckingFactoryBase.h"

static ConnectionCheckingFactoryBase* ourConnectionCheckingFactory = nil;

@implementation ConnectionCheckingFactorySingleton

+(id<IConnectionCheckingFactory>) getConnectionCheckingFactoryInstance {
    if (ourConnectionCheckingFactory == nil) {
        ourConnectionCheckingFactory = [[ConnectionCheckingFactoryBase alloc] init];
    }
    return ourConnectionCheckingFactory;
}

@end