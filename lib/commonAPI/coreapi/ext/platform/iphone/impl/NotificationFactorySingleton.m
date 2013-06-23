
#import "INotification.h"
#import "NotificationFactoryBase.h"

static NotificationFactoryBase* ourNotificationFactory = nil;

@implementation NotificationFactorySingleton

+(id<INotificationFactory>) getNotificationFactoryInstance {
    if (ourNotificationFactory == nil) {
        ourNotificationFactory = [[NotificationFactoryBase alloc] init];
    }
    return ourNotificationFactory;
}

@end