
#import "IEventsource.h"
#import "EventsourceSingletonBase.h"

@interface EventsourceSingleton : EventsourceSingletonBase<IEventsourceSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end