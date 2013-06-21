
#import "IMediaplayer.h"
#import "MediaplayerFactoryBase.h"

static MediaplayerFactoryBase* ourMediaplayerFactory = nil;

@implementation MediaplayerFactorySingleton

+(id<IMediaplayerFactory>) getMediaplayerFactoryInstance {
    if (ourMediaplayerFactory == nil) {
        ourMediaplayerFactory = [[MediaplayerFactoryBase alloc] init];
    }
    return ourMediaplayerFactory;
}

@end