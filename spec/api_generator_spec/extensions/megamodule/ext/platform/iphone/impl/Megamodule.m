
#import "Megamodule.h"
#import <UIKit/UIKit.h>

@implementation Megamodule

-(id)init {
    self = [super init];
    
    intProp = 0;
    
    
    return self;
}


-(void) getIntegerProperty:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:intProp]];
}
     
-(void) setIntegerProperty:(int)value {
    intProp = value;
}
     
-(void) typesTest:(NSString*)paramStr paramBool:(BOOL)paramBool paramInt:(int)paramInt paramFloat:(float)paramFloat paramArray:(NSArray*)paramArray paramHash:(NSDictionary*)paramHash methodResult:(id<IMethodResult>)methodResult {
    
    BOOL checked = YES;
    
    //(paramStr == "ABC") AND (paramBool == true) AND (paramInt == 555123) AND (paramFloat == 3.14) AND (paramArray == [1,2,3]) AND (paramHash == {:p1 => "abc", :p2 => "qwe"}
    
    if ([@"ABC" compare:paramStr] != NSOrderedSame) {
        checked = NO;
    }
    
    if (paramBool != YES) {
        checked = NO;
    }
    
    if (paramInt != 555123) {
        checked = NO;
    }
    
    if (paramFloat != 3.14f) {
        checked = NO;
    }
    
    if ([paramArray count] != 3) {
        checked = NO;
    }
    else {
        if (!([[paramArray objectAtIndex:0] isKindOfClass:[NSNumber class]]) ||
            !([[paramArray objectAtIndex:1] isKindOfClass:[NSNumber class]]) ||
            !([[paramArray objectAtIndex:2] isKindOfClass:[NSNumber class]]) ) {
            checked = NO;
        }
        else {
            if (( [((NSNumber*)[paramArray objectAtIndex:0]) intValue] != 1 ) ||
                ( [((NSNumber*)[paramArray objectAtIndex:1]) intValue] != 2 ) ||
                ( [((NSNumber*)[paramArray objectAtIndex:2]) intValue] != 3 ) ) {
                checked = NO;
            }
        }
    }
    
    NSArray* keys = [paramHash allKeys];
    if ([keys count] != 2) {
        checked = NO;
    }
    else {
        NSObject* value1 = [paramHash objectForKey:@"p1"];
        NSObject* value2 = [paramHash objectForKey:@"p2"];
        if (!([value1 isKindOfClass:[NSString class]]) || !([value2 isKindOfClass:[NSString class]])) {
            checked = NO;
        }
        else {
            if (([@"abc" compare:(NSString*)value1] != NSOrderedSame) ||
                ([@"qwe" compare:(NSString*)value2] != NSOrderedSame)) {
                checked = NO;
            }
        }
    }
    [methodResult setResult:[NSNumber numberWithBool:checked]];
}
     
-(void) produceArray:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:10];
    int i;
    for (i = 1; i <= 10; i++) {
        [ar addObject:[NSNumber numberWithInt:i]];
    }
    [methodResult setResult:ar];
}
     
-(void) showAlertFromUIThread:(NSString*)message {
    // this code crashed if run from non UI thread !
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Alert from UI Thread"
                                                    message:message
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles: nil];
    [alert show];
    [alert release];
}
     
-(void) setPeriodicallyCallback:(int)periodInMilliseconds methodResult:(id<IMethodResult>)methodResult {

}
     
-(void) isPeriodicallyCallbackSetted:(id<IMethodResult>)methodResult {

}
     
-(void) stopPeriodicallyCallback {

}

- (void)dealloc {
    
    
    [super dealloc];
}

@end