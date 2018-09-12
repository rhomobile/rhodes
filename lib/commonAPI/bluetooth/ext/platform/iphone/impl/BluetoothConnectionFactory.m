
#import "BluetoothConnectionFactory.h"
#import "BluetoothConnection.h"
#import "BluetoothConnectionSingleton.h"



@implementation BluetoothConnectionFactory


-(id<IBluetoothConnection>)createBluetoothConnectionByID:(NSString*)ID {
    BluetoothConnection* obj = [[BluetoothConnection alloc] init];
    obj.connectionID = ID;
    return obj;
}

@end
