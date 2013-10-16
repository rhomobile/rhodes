
#import "IWebsocket.h"
#import "WebsocketFactoryBase.h"

static WebsocketFactoryBase* ourWebsocketFactory = nil;

@implementation WebsocketFactorySingleton

+(id<IWebsocketFactory>) getWebsocketFactoryInstance {
    if (ourWebsocketFactory == nil) {
        ourWebsocketFactory = [[WebsocketFactoryBase alloc] init];
    }
    return ourWebsocketFactory;
}

@end