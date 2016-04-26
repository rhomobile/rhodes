
#import "ISignalIndicators.h"
#import "SignalIndicatorsFactoryBase.h"

static SignalIndicatorsFactoryBase* ourSignalIndicatorsFactory = nil;

@implementation SignalIndicatorsFactorySingleton

+(id<ISignalIndicatorsFactory>) getSignalIndicatorsFactoryInstance {
    if (ourSignalIndicatorsFactory == nil) {
        ourSignalIndicatorsFactory = [[SignalIndicatorsFactoryBase alloc] init];
    }
    return ourSignalIndicatorsFactory;
}

@end