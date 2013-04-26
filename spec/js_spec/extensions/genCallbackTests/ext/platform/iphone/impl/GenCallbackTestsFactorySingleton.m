
#import "IGenCallbackTests.h"
#import "GenCallbackTestsFactoryBase.h"

static GenCallbackTestsFactoryBase* ourGenCallbackTestsFactory = nil;

@implementation GenCallbackTestsFactorySingleton

+(id<IGenCallbackTestsFactory>) getGenCallbackTestsFactoryInstance {
    if (ourGenCallbackTestsFactory == nil) {
        ourGenCallbackTestsFactory = [[GenCallbackTestsFactoryBase alloc] init];
    }
    return ourGenCallbackTestsFactory;
}

@end