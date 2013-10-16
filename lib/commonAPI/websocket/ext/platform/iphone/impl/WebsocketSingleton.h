
#import "IWebsocket.h"
#import "WebsocketSingletonBase.h"

@interface WebsocketSingleton : WebsocketSingletonBase<IWebsocketSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end