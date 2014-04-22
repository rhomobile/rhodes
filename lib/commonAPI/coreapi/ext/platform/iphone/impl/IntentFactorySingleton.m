
#import "IIntent.h"
#import "IntentFactoryBase.h"

static IntentFactoryBase* ourIntentFactory = nil;

@implementation IntentFactorySingleton

+(id<IIntentFactory>) getIntentFactoryInstance {
    if (ourIntentFactory == nil) {
        ourIntentFactory = [[IntentFactoryBase alloc] init];
    }
    return ourIntentFactory;
}

@end