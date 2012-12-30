
#import "Barcode1.h"


@implementation Barcode1

- (id) init:(NSString*)_ID {
    self = [super init:_ID];
    
    //TODO: add implementation
    
    // BEGIN example code
    // this code prepared for example - this should not included to generated code !!!
    [self setProperty:@"display" value:@"LCD"];
    [self setProperty:@"sound" value:@"Dolby"];
    // END example code
    
    return self;
}


-(void) getProps:(id<IMethodResult>)methodResult {
    //TODO: add implementation
    
    // BEGIN example code
    // this code prepared for example - this should not included to generated code !!!
    [methodResult setResult:[self getAllProperties]];
    // END example code
}

-(void) getPropsWithString:(NSString*)param1 methodResult:(id<IMethodResult>)methodResult {
    //TODO: add implementation
    
    // BEGIN example code
    // this code prepared for example - this should not included to generated code !!!
    [methodResult setResult:[self getProperty:param1]];
    // END example code
}

-(void) getPropsWithArray:(NSArray*)param1 methodResult:(id<IMethodResult>)methodResult {
    //TODO: add implementation

    // BEGIN example code
    // this code prepared for example - this should not included to generated code !!!
    [methodResult setResult:[self getListOfProperties:param1]];
    // END example code
    
}

+(void) enumerate:(id<IMethodResult>)methodResult {
    //TODO: add implementation
    
    // BEGIN example code
    // this code prepared for example - this should not included to generated code !!!
    NSMutableArray* array = [NSMutableArray arrayWithCapacity:2];
    [array addObject:@"SC1"];
    [array addObject:@"SC2"];
    [methodResult setResult:array];
    // END example code
}


@end