
#import "IBluetoothConnection.h"
#import "BluetoothConnectionBase.h"

@interface BluetoothConnection : BluetoothConnectionBase<IBluetoothConnection> {
    NSString* connectionID;
}

@property(nonatomic, copy)     NSString     *connectionID;

-(void) getConnectionID:(id<IMethodResult>)methodResult;
-(void) getStatus:(id<IMethodResult>)methodResult;
-(void) setCallback:(id<IMethodResult>)methodResult;
-(void) writeData:(NSArray*)data methodResult:(id<IMethodResult>)methodResult;
-(void) readData:(id<IMethodResult>)methodResult;
-(void) writeString:(NSString*)string methodResult:(id<IMethodResult>)methodResult;
-(void) readString:(id<IMethodResult>)methodResult;
-(void) disconnect:(id<IMethodResult>)methodResult;

@end
