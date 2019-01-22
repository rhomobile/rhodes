
#import "IKeyboard.h"
#import "KeyboardFactoryBase.h"

static KeyboardFactoryBase* ourKeyboardFactory = nil;

@implementation KeyboardFactorySingleton

+(id<IKeyboardFactory>) getKeyboardFactoryInstance {
    if (ourKeyboardFactory == nil) {
        ourKeyboardFactory = [[KeyboardFactoryBase alloc] init];
    }
    return ourKeyboardFactory;
}

@end