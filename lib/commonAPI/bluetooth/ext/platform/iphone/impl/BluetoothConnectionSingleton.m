
#import "BluetoothConnectionSingleton.h"
#import "Bluetooth.h"

@implementation BluetoothConnectionSingleton



-(void) getIsAvailable:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:YES]];
}

-(void) getDeviceName:(id<IMethodResult>)methodResult {
    [methodResult setResult:[RhoBluetoothManager sharedInstance].deviceName];
}

-(void) setDeviceName:(NSString*)deviceName methodResult:(id<IMethodResult>)methodResult {
    [RhoBluetoothManager sharedInstance].deviceName = [deviceName copy];
}

-(void) getLastError:(id<IMethodResult>)methodResult {
    [methodResult setResult:ERROR_NO_ERRORS];
}

-(void) enableBluetooth:(id<IMethodResult>)methodResult {
    //nothing
}

-(void) disableBluetooth:(id<IMethodResult>)methodResult {
    [[RhoBluetoothManager sharedInstance] doBluetoothOff];
}

-(void) getConnectionByID:(NSString*)connectionID methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:connectionID];
}

-(void) createConnection:(NSString*)role methodResult:(id<IMethodResult>)methodResult {
    [[RhoBluetoothManager sharedInstance] startConnect:methodResult];
}

-(void) createServerConnection:(id<IMethodResult>)methodResult {
    [[RhoBluetoothManager sharedInstance] startServerConnect:@"" callback:methodResult];
}

-(void) createClientConnection:(NSString*)serverName methodResult:(id<IMethodResult>)methodResult {
    [[RhoBluetoothManager sharedInstance] startClientConnect:[serverName copy] callback:methodResult];
}

-(void) stopCurrentConnectionProcess:(id<IMethodResult>)methodResult {
    [[RhoBluetoothManager sharedInstance] stop_current_connection];
}




@end
