
#import "IBluetoothConnection.h"
#import "BluetoothConnectionSingletonBase.h"

@interface BluetoothConnectionSingleton : BluetoothConnectionSingletonBase<IBluetoothConnectionSingleton> {
}



-(void) getIsAvailable:(id<IMethodResult>)methodResult;
-(void) getDeviceName:(id<IMethodResult>)methodResult;
-(void) setDeviceName:(NSString*)deviceName methodResult:(id<IMethodResult>)methodResult;
-(void) getLastError:(id<IMethodResult>)methodResult;
-(void) enableBluetooth:(id<IMethodResult>)methodResult;
-(void) disableBluetooth:(id<IMethodResult>)methodResult;
-(void) getConnectionByID:(NSString*)connectionID methodResult:(id<IMethodResult>)methodResult;
-(void) createConnection:(NSString*)role methodResult:(id<IMethodResult>)methodResult;
-(void) createServerConnection:(id<IMethodResult>)methodResult;
-(void) createClientConnection:(NSString*)serverName methodResult:(id<IMethodResult>)methodResult;
-(void) stopCurrentConnectionProcess:(id<IMethodResult>)methodResult;




@end