
#import "IBluetoothConnection.h"
#import "BluetoothConnectionFactory.h"

static BluetoothConnectionFactory* ourBluetoothConnectionFactory = nil;

@implementation BluetoothConnectionFactorySingleton

+(id<IBluetoothConnectionFactory>) getBluetoothConnectionFactoryInstance {
    if (ourBluetoothConnectionFactory == nil) {
        ourBluetoothConnectionFactory = [[BluetoothConnectionFactory alloc] init];
    }
    return ourBluetoothConnectionFactory;
}

@end
