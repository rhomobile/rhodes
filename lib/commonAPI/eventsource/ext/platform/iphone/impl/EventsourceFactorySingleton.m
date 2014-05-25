
#import "IEventsource.h"
#import "EventsourceFactoryBase.h"

static EventsourceFactoryBase* ourEventsourceFactory = nil;

@implementation EventsourceFactorySingleton

+(id<IEventsourceFactory>) getEventsourceFactoryInstance {
    if (ourEventsourceFactory == nil) {
        ourEventsourceFactory = [[EventsourceFactoryBase alloc] init];
    }
    return ourEventsourceFactory;
}

@end