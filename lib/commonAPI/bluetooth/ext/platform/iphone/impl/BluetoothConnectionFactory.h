
#import "IBluetoothConnection.h"
#import "BluetoothConnectionFactoryBase.h"

@interface BluetoothConnectionFactory : BluetoothConnectionFactoryBase {
}

-(id<IBluetoothConnection>)createBluetoothConnectionByID:(NSString*)ID;

@end
