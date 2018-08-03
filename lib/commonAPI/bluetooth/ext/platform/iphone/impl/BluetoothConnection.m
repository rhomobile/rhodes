
#import "BluetoothConnection.h"
#import "Bluetooth.h"

@implementation BluetoothConnection

@synthesize connectionID;


-(void) getConnectionID:(id<IMethodResult>)methodResult {
    [methodResult setResult:self.connectionID];
}

-(void) getStatus:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:[[RhoBluetoothManager sharedInstance] getPacketsSize]]];
}

-(void) setCallback:(id<IMethodResult>)methodResult {
    [RhoBluetoothManager sharedInstance].sessionCallback = methodResult;
}

-(void) writeData:(NSArray*)data methodResult:(id<IMethodResult>)methodResult {
    int size = (int)[data count];
    if (size <= 0) {
        return;
    }
    unsigned char* buf = malloc(size);
    int i;
    for (i = 0; i < size; i++) {
        buf[i] = (unsigned char)[[data objectAtIndex:i] intValue];
    }
    [[RhoBluetoothManager sharedInstance] sendData:buf length:size];
    free(buf);
}

-(void) readData:(id<IMethodResult>)methodResult {
    int size = [[RhoBluetoothManager sharedInstance] getPacketsSize];
    unsigned char* buf = malloc(size+1);
    [[RhoBluetoothManager sharedInstance] readFromPackets:buf length:size];
    //VALUE val = rho_ruby_create_byte_array(buf, size);
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:size];
    int i;
    for (i = 0; i < size; i++) {
        [ar addObject:[NSNumber numberWithInt:(int)buf[i]]];
    }
    free(buf);
    [methodResult setResult:ar];
}

-(void) writeString:(NSString*)string methodResult:(id<IMethodResult>)methodResult {
    [[RhoBluetoothManager sharedInstance] sendString:string];
}

-(void) readString:(id<IMethodResult>)methodResult {
    NSString* s = [[RhoBluetoothManager sharedInstance] readString];
    [methodResult setResult:s];
}

-(void) disconnect:(id<IMethodResult>)methodResult {
    [[RhoBluetoothManager sharedInstance] doDisconnect];
}


@end
