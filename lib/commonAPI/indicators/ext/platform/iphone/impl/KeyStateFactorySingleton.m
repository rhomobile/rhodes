
#import "IKeyState.h"
#import "KeyStateFactoryBase.h"

static KeyStateFactoryBase* ourKeyStateFactory = nil;

@implementation KeyStateFactorySingleton

+(id<IKeyStateFactory>) getKeyStateFactoryInstance {
    if (ourKeyStateFactory == nil) {
        ourKeyStateFactory = [[KeyStateFactoryBase alloc] init];
    }
    return ourKeyStateFactory;
}

@end